#include "spectowidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QTimer>
#include <iostream>
#include <algorithm>

SpectoWidget::SpectoWidget(QWidget *parent, spectools_ng::SpectoolsRaw *stream, std::string device_name)
    : QGLWidget(parent)
{
    elapsed = 0;
    setAutoFillBackground(false);
    QGLFormat temp_format = QGLFormat();
    temp_format.defaultFormat();
    temp_format.setAlpha(true);
    setFormat(temp_format);
    background = QBrush(QColor(0,0,0));
    scan_buffer = &(stream->getBuffer(device_name));
}

void SpectoWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}


void SpectoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    QRect render_rect = event->rect();
    painter.fillRect(render_rect, background);
    painter.setOpacity(1);
    double render_width = (double)render_rect.width();
    double render_height = (double)render_rect.height();
    IntRange horz_out_range = {1.05*render_width, 0};
    IntRange horz_in_range;
    spectools_ng::scan_vector view = scan_buffer->getBufferView();
    int row = render_height;
    for (auto sample = view.rbegin(); sample < view.rend(); sample++){
        horz_in_range = {(*sample)->size(), 0};
        QVector<QPoint> points;
        for (unsigned int i = 0; (i < (*sample)->size()) and (row > 0); i++) {
            points.push_back(QPoint(scaleByRange(horz_in_range, horz_out_range, i), row));
        }
        drawPoints(painter, points, *(*sample), render_width);
        row-=2;
    }
    painter.end();
}

void SpectoWidget::drawPoints(QPainter &painter, QVector<QPoint> &points, spectools_ng::Scan &sample, double render_width) {
    QPen pen;
    auto sample_ptr = sample.begin();
    pen.setWidth(2);
    for (int i = 1; i < points.size(); sample_ptr++, i++) {
        pen.setColor(colourFromValue(*sample_ptr));
        painter.setPen(pen);
        painter.drawPoint(points[i]);
    }
}

int SpectoWidget::scaleByRange(IntRange &in_range, IntRange &out_range, int n){
    int out = out_range.second +((n - in_range.second) * (out_range.first - out_range.second)) / (in_range.first - in_range.second);
    return out;
}

QColor SpectoWidget::colourFromValue(int value){
    int out = ((value - min_value) * 255) / (max_value - min_value);
    QColor colour = QColor(out, out, out);
    return colour;
}

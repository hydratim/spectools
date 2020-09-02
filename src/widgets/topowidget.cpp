#include "./topowidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QTimer>
#include <iostream>
#include <algorithm>

TopoWidget::TopoWidget(QWidget *parent, spectools_ng::SpectoolsRaw *stream, std::string device_name)
    : QGLWidget(parent)
{
    elapsed = 0;
    setAutoFillBackground(false);
    QGLFormat temp_format = QGLFormat();
    temp_format.defaultFormat();
    temp_format.setAlpha(true);
    setFormat(temp_format);
    //background = QBrush(QColor(4,4,30));
    background = QBrush(QColor(0,0,0));
    scan_buffer = &(stream->getBuffer(device_name));
}

void TopoWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}


void TopoWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    QRect render_rect = event->rect();
    painter.fillRect(render_rect, background);
    painter.setOpacity(0.05);
    double render_width = (double)render_rect.width();
    double render_height = (double)render_rect.height();
    IntRange vert_in_range = {-100,-20};
    IntRange vert_out_range = {render_height, 0};
    IntRange horz_out_range = {1.05*render_width, 0};
    IntRange horz_in_range;
    spectools_ng::scan_vector view = scan_buffer->getBufferView();
    QPen pen;
    for (std::shared_ptr<spectools_ng::Scan> sample: view){
        pen.setColor(qRgb(255,255,255));
        pen.setWidth(2);
        painter.setPen(pen);
        horz_in_range = {sample->size(), 0};
        QVector<QPoint> points;
        auto sample_ptr = sample->begin();
        for (unsigned int i = 0; i < sample->size(); sample_ptr++, i++) {
            points.push_back(QPoint(scaleByRange(horz_in_range, horz_out_range, i), scaleByRange(vert_in_range, vert_out_range, *sample_ptr)));
        }
        drawPoints(painter, points);
    }
    painter.end();
}

void TopoWidget::drawPoints(QPainter &painter, QVector<QPoint> &points) {
    for (int i = 1; i < points.size(); i++) {
        painter.drawPoint(points[i]);
    }
}

int TopoWidget::scaleByRange(IntRange &in_range, IntRange &out_range, int n){
    int out = out_range.second +((n - in_range.second) * (out_range.first - out_range.second)) / (in_range.first - in_range.second);
    return out;
}

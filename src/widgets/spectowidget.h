#ifndef SPECTOWIDGET_H
#define SPECTOWIDGET_H

#include <QtOpenGL/QGLWidget>
#include <QColor>

#include "../spectool_wrapper.h"

typedef std::pair<int,int> IntRange;

class SpectoWidget : public QGLWidget
{
    Q_OBJECT
public:
    SpectoWidget(QWidget *parent, spectools_ng::SpectoolsRaw *stream, std::string device_name);
public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int elapsed;
    int max_value = -20;
    int min_value = -100;
    QBrush background;
    void drawLine(QPainter &painter, spectools_ng::Scan &samples, int max_x, int y_index);
    void calculateScale();
    QColor colourFromValue(int value);
    spectools_ng::ScanBuffer *scan_buffer;
    void drawPoints(QPainter &painter, QVector<QPoint> &points, spectools_ng::Scan &sample, double render_width);
    int scaleByRange(IntRange &in_range, IntRange &out_range, int n);
};

#endif // SPECTOWIDGET_H

#ifndef TOPOWIDGET_H
#define TOPOWIDGET_H

#include <QtOpenGL/QGLWidget>

#include "../spectool_wrapper.h"

typedef std::pair<int,int> IntRange;

class TopoWidget : public QGLWidget
{
    Q_OBJECT
public:
    TopoWidget(QWidget *parent, spectools_ng::SpectoolsRaw *stream, std::string device_name);
public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int elapsed;
    QBrush background;
    void drawPoints(QPainter &painter, QVector<QPoint> &points);
    int scaleByRange(IntRange &in_range, IntRange &out_range, int n);
    spectools_ng::ScanBuffer *scan_buffer;
};

#endif // TOPOWIDGET_H

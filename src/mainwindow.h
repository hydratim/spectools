#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QPainter>
#include <QImage>
#include <QScopedPointer>
#include "spectool_wrapper.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    bool save();
    bool saveAs();
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    bool setWiFiView(bool enabled);
    bool openDevice(std::string device_name);
private:
    spectools_ng::SpectoolsRaw *stream;
    Ui::MainWindow *ui;
    QImage image;
    QRgb value;
    QString curFile;
    QTabWidget *tabwidget;
    QGridLayout *layout;
    bool saveFile(const QString &fileName);
    void initializeGL();
};
#endif // MAINWINDOW_H

#include <iostream>
#include <QtWidgets>
#include <QTimer>
#include "widgets/topowidget.h"
#include "widgets/spectowidget.h"
#include "mainwindow.h"
#include "spectool_wrapper.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //tabwidget = new QTabWidget(this);
    layout = new QGridLayout();
    ui->centralwidget->setLayout(layout);
    //layout->addWidget(tabwidget, 0, 0);
    stream = new spectools_ng::SpectoolsRaw();
    sleep(2);
    std::vector<std::string> devices = stream->getKeys();
    std::cout << devices[0] << std::endl;
    openDevice(devices[0]);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stream;
}

bool MainWindow::setWiFiView(bool enabled) {
    return enabled;
}

bool MainWindow::openDevice(std::string device_name) {
    //QGridLayout *layout = new QGridLayout();
    //QWidget *tab = new QWidget(this);
    //tab->setLayout(layout);
    TopoWidget *topowidget = new TopoWidget(this, stream, device_name);
    layout->addWidget(topowidget, 1, 0);
    QTimer *topotimer = new QTimer(this);
    connect(topotimer, &QTimer::timeout, topowidget, &TopoWidget::animate);
    topotimer->start(16);
    std::cout << "started topo" <<std::endl;
    SpectoWidget *spectowidget = new SpectoWidget(this, stream, device_name);
    layout->addWidget(spectowidget, 0, 0);
    QTimer *spectotimer = new QTimer(this);
    connect(spectotimer, &QTimer::timeout, spectowidget, &SpectoWidget::animate);
    spectotimer->start(16);
    std::cout << "started specto" <<std::endl;
    //this->tabwidget->addTab(tab, QString(device_name.c_str()));
    return true;
}

bool MainWindow::save() {
    if (curFile.isEmpty()) {
            return saveAs();
        } else {
            return saveFile(curFile);
        }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}

bool MainWindow::saveFile(const QString &fileName)
{
    QString errorMessage;

    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << "Hello World! \n"; // TODO: REPLACE WITH CSV WRITE
        if (!file.commit()) {
            errorMessage = tr("Cannot write file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(fileName), file.errorString());
        }
    } else {
        errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                       .arg(QDir::toNativeSeparators(fileName), file.errorString());
    }
    QGuiApplication::restoreOverrideCursor();

    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, tr("Application"), errorMessage);
        return false;
    }

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::maybeSave()
{
    // TODO: Detect if buffer.
//    if (!textEdit->document()->isModified())
//        return true;
//    const QMessageBox::StandardButton ret
//        = QMessageBox::warning(this, tr("Application"),
//                               tr("The document has been modified.\n"
//                                  "Do you want to save your changes?"),
//                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//    switch (ret) {
//    case QMessageBox::Save:
//        return save();
//    case QMessageBox::Cancel:
//        return false;
//    default:
//        break;
//    }
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}



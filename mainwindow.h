#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void portSettings(QSerialPort *);
    void createMSG();
    int lrcCount(QByteArray);
    void reverse();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QByteArray msg;
    int chastota=0;

  //  QSerialPort *port;

};
#endif // MAINWINDOW_H

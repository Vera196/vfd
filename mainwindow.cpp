#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->spinBox->setRange(0, 599);
    ui->spinBox->setSingleStep(5);
    ui->spinBox->setSuffix(" Hz");


   // QSerialPort *port = new QSerialPort ("COM3");
    portSettings();

    port->open(QIODevice::ReadWrite);

    connect(port, &QSerialPort::readyRead, this, &MainWindow::readAnswer );    //potenzyalnoe mesto kosiaka

//: slaveadress kodfunc dannye lrc \r\n


//slaveadress -- my vsegda connect k 01(skoree vsego)

//kodfunc -- 03-chtenie
//kodfunc -- 06-zapis v 1 registr
//kodfunc -- 10-zapis v neskolko registrov

//dannye -- po situazii
// 02.12 chastota po comportu ???

//lrc -- 0x100 - 0x(summa vseh ascii simvolov)




    //upravlenie s rs-485(prioritet)
//    msg=":010602010003\r\n"; //nado li ono
//    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());
//    qDebug()<< msg;
//    port->write(msg);

//    port->waitForBytesWritten(19);

}

MainWindow::~MainWindow()
{
    delete ui;
    port->close();
}

void MainWindow::portSettings()
{
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data7);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::TwoStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
}

int MainWindow::lrcCount(QByteArray arr)
{
    int sum=0;
    bool ok;
    arr=arr.mid(1,arr.size()-3);
    while(arr.size()!=0)
    {
        sum+= arr.left(2).toInt(&ok, 16);
        arr.remove(0,2);
    }

    sum=256-sum;
    if (sum<0)                                 //mogut byt` problemki
            sum = sum+256;                   //iz-za "dopolnitelnogo koda"

    return sum;
}

void MainWindow::readAnswer()
{
    QByteArray buffer = port->readAll();
    ui->lineEdit->setText(buffer);
    qDebug()<<"prischlo  "<<buffer;
}



void MainWindow::forward()
{
    msg=":010620000100\r\n";

    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());

    port->write(msg);
}

void MainWindow::reverse()
{
    msg=":010620000200\r\n";

    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());

    port->write(msg);
}

void MainWindow::run()
{
    msg=":010620000002D7\r\n";
    port->write(msg);
}

void MainWindow::stop()
{
    msg=":010620000001D8\r\n";
    port->write(msg);
}



void MainWindow::on_pushButton_clicked()  //zapros po chastote
{
    chastota=ui->spinBox->value();

    msg=":";

    //zapisali slave, func i adres
    msg.push_back("01062001");

    QByteArray data(QByteArray::number(chastota, 16).toUpper());
    //QByteArray data(QByteArray::number(chastota, 10));

    while(data.size()%4!=0)
        data.insert(0, "0");
    msg.push_back(data);

    msg.push_back("\r\n");   

    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());
    port->write(msg);

    qDebug()<< msg;
    qDebug()<<msg.size();



    //    msg=":010620000002\r\n";
    //  port->write(":010620000002D7\r\n");
  //port->waitForBytesWritten(1000);

}


void MainWindow::on_pushButton_2_clicked()
{
//    msg=":010620000001D8\r\n";
//    port->write(msg);
   stop();
}


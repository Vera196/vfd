#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->spinBox->setRange(0, 599);
    ui->spinBox->setSingleStep(50);
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
    msg=":010602010003\r\n"; //nado li ono
    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());

    qDebug()<< msg;
    qDebug()<<msg.size();


    //port->write(msg, msg.length());
    port->write(msg);

//    port->waitForBytesWritten(19);
//    port->flush();                        //scho eto voobze
//    port->waitForReadyRead(19);




    //port->close();

}

MainWindow::~MainWindow()
{
    delete ui;
    port->close();
}

void MainWindow::portSettings()
{
    port->setBaudRate(QSerialPort::Baud9600);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);
    port->setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::createMSG()
{
    msg=":\r\n";
// //    msg.resize(3);
// //    msg[0] = 58;  //:
// //    msg[1] = 13;  //\r
// //    msg[2] = 10;  //\n

// //  :010600380002C0\r\n

//    QByteArray slave("01");
//    msg.insert(1, slave);

//    QByteArray func("03");
//    msg.insert(3, func);


//    QByteArray data("04010001");
// //    while(data.size()%4!=0)                       vazno!
// //        data.insert(0, "0");              vazno
//    msg.insert(5, data);

//    QByteArray lrc;
//   // lrc.resize(2);
//    lrc.insert(0, QByteArray::number(lrcCount(msg), 16).toUpper());
//    //msg.insert(5+data.size(), lrc);
//    msg.insert(msg.size()-2, lrc);
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


void MainWindow::reverse()
{
    msg=":";

    // 04.06 - 04.08 ????
    // data = 21(v 10-richnoi)???

    //zapisali slave, func i adres
    msg.push_back("01060212");                       //pomeniat adres

//    QByteArray data(QByteArray::number(chastota, 16).toUpper());
//    while(data.size()%4!=0)
//        data.insert(0, "0");
//    //msg.insert(msg.size()-2, data);
//    msg.push_back(data);

    msg.push_back("\r\n");

    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());
   // msg.push_back(QByteArray::number(lrcCount(msg), 16).toUpper());


        qDebug()<< msg;
        qDebug()<<msg.size();


}

void MainWindow::readAnswer()
{
    QByteArray buffer = port->readAll();
    ui->lineEdit->setText(buffer);
}



void MainWindow::on_pushButton_clicked()  //zapros po chastote
{
    chastota=ui->spinBox->value();

    msg=":";

// 02.12 chastota po comportu ???
// ili 04.06 - 04.08 ????

    //zapisali slave, func i adres
    msg.push_back("01060212");

    QByteArray data(QByteArray::number(chastota, 16).toUpper());
    while(data.size()%4!=0)
        data.insert(0, "0");
    msg.push_back(data);

    msg.push_back("\r\n");

    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());
   // msg.push_back(QByteArray::number(lrcCount(msg), 16).toUpper());


        qDebug()<< msg;
        qDebug()<<msg.size();
        //ui->lineEdit->setText(msg);

    port->write(msg);

}


void MainWindow::on_pushButton_2_clicked()
{
    msg=":010602120000\r\n";

    msg.insert(msg.size()-2, QByteArray::number(lrcCount(msg), 16).toUpper());

    qDebug()<< msg;
    qDebug()<<msg.size();


    port->write(msg);
}


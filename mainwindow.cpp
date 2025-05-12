#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

void em_init_plot(QCustomPlot *p)
{
    QPen pen_x(QColor(0, 114, 189));
    QPen pen_y(QColor(217, 83, 25));
    QPen pen_z(QColor(237, 177, 32));
    QPen pen_w(QColor(126, 47, 142));
    pen_x.setWidth(2);
    pen_y.setWidth(2);
    pen_z.setWidth(2);
    pen_w.setWidth(2);

    p->xAxis->setLabel("Time [s]");
    p->yAxis->setLabel("Current [mA]");
    p->xAxis->setLabelFont(QFont("Courier New", 12));
    p->yAxis->setLabelFont(QFont("Courier New", 12));
    p->xAxis->setLabelColor(Qt::blue);
    p->yAxis->setLabelColor(Qt::blue);
    p->addGraph();
    p->addGraph();
    p->addGraph();
    p->addGraph();
    p->graph(0)->setName("EM-0");
    p->graph(1)->setName("EM-1");
    p->graph(2)->setName("EM-2");
    p->graph(3)->setName("EM-3");
    p->legend->setVisible(true);
    p->graph(0)->setPen(pen_x);
    p->graph(1)->setPen(pen_y);
    p->graph(2)->setPen(pen_z);
    p->graph(3)->setPen(pen_w);
    p->replot();

    p->legend->setBrush(Qt::NoBrush);
    p->setBackground(Qt::transparent);
    p->axisRect()->setBackground(Qt::transparent);
    p->setAttribute(Qt::WA_TranslucentBackground);
    p->setStyleSheet("background: transparent;");
    p->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , udp_socket(new QUdpSocket(this))
{
    ui->setupUi(this);

    ui->textEdit_em0->setText(QString::number(1000));
    ui->textEdit_em1->setText(QString::number(1000));
    ui->textEdit_em2->setText(QString::number(1000));
    ui->textEdit_em3->setText(QString::number(1000));
    ui->textEdit_em_kp->setText(QString::number(0.065));
    ui->textEdit_em_ki->setText(QString::number(0.3));
    ui->textEdit_em_fc->setText(QString::number(0.0));
    ui->textEdit_em_fs->setText(QString::number(0.0));
    ui->textEdit_udp_ip->setText("192.168.0.100");
    ui->textEdit_udp_port->setText("8080");

    qDebug() << "Hello World\n",
    em_init_plot(ui->widget_em_plot);

    //udp_socket->bind(QHostAddress::AnyIPv4, 8081);
    //qDebug() << "Bound to:" << udp_socket->localAddress().toString() << udp_socket->localPort();
    //connect(udp_socket, &QUdpSocket::readyRead, this, &MainWindow::receiveMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::sendMessage()
{
    // QString message = ui->lineEdit_input->text();
    // QByteArray data = message.toUtf8();
    // udp_socket->writeDatagram(data, serverAddress, serverPort);
}

void MainWindow::receiveMessage()
{
    while (udp_socket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(int(udp_socket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udp_socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        qDebug() << "Received from" << sender.toString() << ":" << senderPort << "->"
                 << QString::fromUtf8(buffer);
    }
}

void MainWindow::on_pushButton_em0_toggled(bool checked)
{
    QByteArray data = "Hello from Qt";
    udp_socket->writeDatagram(data, QHostAddress(udp_server_ip), udp_server_port);

    if (checked)
    {
       ui->pushButton_em0->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        ui->pushButton_em0->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}


void MainWindow::on_pushButton_em1_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButton_em1->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        ui->pushButton_em1->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}


void MainWindow::on_pushButton_em2_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButton_em2->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        ui->pushButton_em2->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}


void MainWindow::on_pushButton_em3_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButton_em3->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        ui->pushButton_em3->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}

void MainWindow::on_pushButton_em_enable_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButton_em_enable->setIcon(QIcon(":/assets/em_on.png"));
    }
    else
    {
        ui->pushButton_em_enable->setIcon(QIcon(":/assets/em_off.png"));
    }
}

void MainWindow::on_pushButton_udp_connect_toggled(bool checked)
{
    QString ip_str = ui->textEdit_udp_ip->toPlainText();
    QString port_str = ui->textEdit_udp_port->toPlainText();

    // Check if IP is valid and matches the expected server IP (192.168.0.101)
    if (ip_str != "192.168.0.101") {
        qDebug() << "Invalid server IP. Connection will not be made.";
        QPixmap pix(":/assets/wifi_off.png");
        ui->label->setPixmap(pix);
        ui->pushButton_udp_connect->setChecked(false); // Reset the button
        return; // Exit the function early if the IP is wrong
    }

    // Proceed with connection if IP matches the expected server IP
    if (ip_str.isEmpty() || port_str.isEmpty()) {
        qDebug() << "Please enter a valid IP and port.";
        return;
    }

    QHostAddress serverIp(ip_str);
    quint16 serverPort = port_str.toUInt();

    if (!serverIp.isNull() && serverPort != 0) {
        udp_server_ip = serverIp;
        udp_server_port = serverPort;
        qDebug() << "Configured server:" << udp_server_ip.toString() << udp_server_port;

        if (checked) {
            // Start the UDP connection and begin receiving
            if (udp_socket->bind(QHostAddress::AnyIPv4, 8081)) {
                connect(udp_socket, &QUdpSocket::readyRead, this, &MainWindow::receiveMessage);
                qDebug() << "UDP Enabled. Receiving from:" << udp_socket->localAddress().toString() << udp_socket->localPort();
                QPixmap pix(":/assets/wifi_on.png");
                ui->label->setPixmap(pix);
            } else {
                qDebug() << "Failed to bind UDP socket.";
                QPixmap pix(":/assets/wifi_off.png");
                ui->label->setPixmap(pix);
                ui->pushButton_udp_connect->setChecked(false);
            }
        } else {
            // Disable UDP connection when the button is toggled off
            disconnect(udp_socket, &QUdpSocket::readyRead, this, &MainWindow::receiveMessage);
            udp_socket->close(); // unbinds socket
            qDebug() << "UDP Disabled.";
            QPixmap pix(":/assets/wifi_off.png");
            ui->label->setPixmap(pix);
        }
    } else {
        qDebug() << "Invalid IP or port.";
        QPixmap pix(":/assets/wifi_off.png");
        ui->label->setPixmap(pix);
        ui->pushButton_udp_connect->setChecked(false);
    }
}



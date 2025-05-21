#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"

uint16_t crc16_ccitt(const QByteArray &data)
{
    uint16_t crc = 0xFFFF;
    for (char byte : data) {
        crc ^= static_cast<uint8_t>(byte) << 8;
        for (int i = 0; i < 8; ++i) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

bool parse_telemetry(const QString &rx, telemetry_t &t)
{
    if (!rx.startsWith('$') || !rx.endsWith('#')){
        qDebug() << "Invalid frame format";
        return false;
    }

    QString payload = rx.mid(1, rx.length() - 1);
    const QStringList components = payload.split(',');

    // Iterate over each comma separated components
    for (const QString &component : components)
    {
        // Split by colon to separate type and values
        QStringList parts = component.split(':');
        if (parts.size() != 2)
        {
            //qDebug() << "Invalid component:" << component;
            return false;
        }

        QString type = parts[0];
        QStringList values = parts[1].split('x');

        if (type == "d") // Distance
        {
            for (int i = 0; i < 4; ++i)
            {
                t.d[i] = values[i].toFloat();
            }
        }
        else if (type == "c") // current
        {
            for (int i = 0; i < 4; ++i) {
                t.c[i] = values[i].toFloat();
            }
        }
        else if (type == "r") // CRC
        {
            t.crc = values[0].toUInt();
        }
        else
        {
            qDebug() << "Unknown data type:" << type;
            return false;
        }
    }

    return true;
}

static double count = 0;

void MainWindow::populate_telemetry(const telemetry_t &t)
{
    tms.append(count);
    count += 0.055;

    for (uint8_t i = 0; i < 4; i++)
    {
        d[i].append(t.d[i]);
        c[i].append(t.c[i]);
    }


    if (tms.size() > 250)
    {
        tms.removeFirst();

        for (uint8_t i = 0; i < 4; i++)
        {
            d[i].removeFirst();
            c[i].removeFirst();
        }
    }
}

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

    p->xAxis->setLabel("t [s]");
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

void tof_init_plot(QCustomPlot *p)
{
    QPen pen_x(QColor(0, 114, 189));
    QPen pen_y(QColor(217, 83, 25));
    QPen pen_z(QColor(237, 177, 32));
    QPen pen_w(QColor(126, 47, 142));

    pen_x.setWidth(2);
    pen_y.setWidth(2);
    pen_z.setWidth(2);
    pen_w.setWidth(2);

    p->xAxis->setLabel("t [s]");
    p->yAxis->setLabel("Current [mA]");
    p->xAxis->setLabelFont(QFont("Courier New", 12));
    p->yAxis->setLabelFont(QFont("Courier New", 12));
    p->xAxis->setLabelColor(Qt::blue);
    p->yAxis->setLabelColor(Qt::blue);
    p->addGraph();
    p->addGraph();
    p->addGraph();
    p->addGraph();
    p->graph(0)->setName("TOF-0");
    p->graph(1)->setName("TOF-1");
    p->graph(2)->setName("TOF-2");
    p->graph(3)->setName("TOF-3");
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
    tof_init_plot(ui->widget_tof_plot);

    timer_plot_mag = new QTimer(this);

    connect(timer_plot_mag, &QTimer::timeout, this, [this]()
    {
        for (int i = 0; i < 4; ++i)
        {
            ui->widget_em_plot->graph(i)->setData(tms, c[i]);
            ui->widget_tof_plot->graph(i)->setData(tms, d[i]);
        }

        ui->widget_em_plot->rescaleAxes();
        ui->widget_em_plot->replot();

        ui->widget_tof_plot->rescaleAxes();
        ui->widget_tof_plot->replot();
    });

    timer_plot_mag->start(70);

    connect(udp_socket, &QUdpSocket::bytesWritten,
            this, &MainWindow::handleBytesWritten);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
    isSending = false;

    // Add small delay if needed (10ms)
    QTimer::singleShot(10, this, &MainWindow::processMessageQueue);
}

void MainWindow::processMessageQueue()
{
    if (!messageQueue.isEmpty() && !isSending) {
        isSending = true;
        auto message = messageQueue.dequeue();

        QString formatted = QString("$%1:%2#")
                                .arg(message.first)
                                .arg(message.second, 0, 'f', 3);

        udp_socket->writeDatagram(formatted.toUtf8(), udp_server_ip, udp_server_port);
    }
}

void MainWindow::sendMessage(tcmd_idx_t idx, double data)
{
    QString message = QString("$%1:%2#").arg(idx).arg(data, 0, 'f', 3);
    qDebug() << message;
    QByteArray byteData = message.toUtf8();
    udp_socket->writeDatagram(byteData, udp_server_ip, udp_server_port);
}

void MainWindow::receiveMessage()
{
    while (udp_socket->hasPendingDatagrams())
    {
        QByteArray buffer;
        buffer.resize(int(udp_socket->pendingDatagramSize()));

        QHostAddress tpi_ip;
        quint16 tpi_port;
        udp_socket->readDatagram(buffer.data(), buffer.size(), &tpi_ip, &tpi_port);

        //qDebug() << "Received from" << tpi_ip.toString() << ":" << tpi_port << "->" << QString::fromUtf8(buffer);
        telemetry_t t;

        if(parse_telemetry(QString::fromUtf8(buffer), t))
        {
            populate_telemetry(t);
        }
    }
}

void MainWindow::on_pushButton_em0_toggled(bool checked)
{
    if (checked)
    {
       sendMessage(TCMD_EM0, ui->textEdit_em0->toPlainText().toDouble());
        qDebug() << ui->textEdit_em0->toPlainText().toDouble();

       ui->pushButton_em0->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        sendMessage(TCMD_EM0_STOP, 0.0);
        ui->pushButton_em0->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}


void MainWindow::on_pushButton_em1_toggled(bool checked)
{
    if (checked)
    {
        sendMessage(TCMD_EM1, ui->textEdit_em1->toPlainText().toDouble());
        ui->pushButton_em1->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        sendMessage(TCMD_EM1_STOP, 0.0);
        ui->pushButton_em1->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}


void MainWindow::on_pushButton_em2_toggled(bool checked)
{
    if (checked)
    {
        sendMessage(TCMD_EM2, ui->textEdit_em2->toPlainText().toDouble());
        ui->pushButton_em2->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        sendMessage(TCMD_EM2_STOP, 0.0);
        ui->pushButton_em2->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}


void MainWindow::on_pushButton_em3_toggled(bool checked)
{
    if (checked)
    {
        sendMessage(TCMD_EM3, ui->textEdit_em3->toPlainText().toDouble());
        ui->pushButton_em3->setIcon(QIcon(":/assets/toggle_on.png"));
    }
    else
    {
        sendMessage(TCMD_EM3_STOP, 0.0);
        ui->pushButton_em3->setIcon(QIcon(":/assets/toggle_off.png"));
    }
}

void MainWindow::on_pushButton_em_enable_toggled(bool checked)
{
    if (checked)
    {
        sendMessage(TCMD_EM_ENABLE, 0.0);
        ui->pushButton_em_enable->setIcon(QIcon(":/assets/em_on.png"));
    }
    else
    {
        sendMessage(TCMD_EM_STOP_ALL, 0.0);
        ui->pushButton_em_enable->setIcon(QIcon(":/assets/em_off.png"));
    }
}

void MainWindow::on_pushButton_udp_connect_toggled(bool checked)
{
    QString ip_str = ui->textEdit_udp_ip->toPlainText();
    QString port_str = ui->textEdit_udp_port->toPlainText();
/*
    if (ip_str != "192.168.0.100") {
        qDebug() << "Invalid server IP. Connection will not be made.";
        QPixmap pix(":/assets/wifi_off.png");
        ui->label->setPixmap(pix);
        ui->pushButton_udp_connect->setChecked(false); // Reset the button
        return; // Exit the function early if the IP is wrong
    }
*/

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

                QByteArray data = "Hello from Qt";
                udp_socket->writeDatagram(data, QHostAddress(udp_server_ip), udp_server_port);

                QPixmap pix(":/assets/wifi_on.png");
                ui->label->setPixmap(pix);
            } else {
                qDebug() << "Failed to bind UDP socket.";
                QPixmap pix(":/assets/wifi_off.png");
                ui->label->setPixmap(pix);
                ui->pushButton_udp_connect->setChecked(false);
            }
        } else {
            QByteArray data = "Bye from Qt";
            udp_socket->writeDatagram(data, QHostAddress(udp_server_ip), udp_server_port);

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



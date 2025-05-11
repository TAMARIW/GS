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

    em_init_plot(ui->widget_em_plot);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_em0_toggled(bool checked)
{
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


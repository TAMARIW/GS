#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_em0_toggled(bool checked);

    void on_pushButton_em1_toggled(bool checked);

    void on_pushButton_em2_toggled(bool checked);

    void on_pushButton_em3_toggled(bool checked);

    void on_pushButton_em_enable_toggled(bool checked);

    void sendMessage();

    void receiveMessage();


private:
    Ui::MainWindow *ui;
    QUdpSocket *udp_socket;
    QHostAddress udp_server_ip;
    quint16 udp_server_port;
};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QQueue>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFile>
#include <QUrl>

typedef struct
{
    float d[4]; // ToF measurements [mm]
    float c[4]; // Electromagnet current feedback [mA]
    float kf_d[4]; // Kalman Filter distance estimates
    float kf_v[4]; // Kalman Filter velocity estimates
    uint16_t crc;
} telemetry_t;

typedef enum
{
    EM_ON,
    EM_OFF,
    EM_STANDBY
} em_state_t;

typedef enum
{
    // PID gains
    TCMD_EM_KP,
    TCMD_EM_KI,
    TCMD_EM0,
    TCMD_EM1,
    TCMD_EM2,
    TCMD_EM3,
    TCMD_EM0_STOP,
    TCMD_EM1_STOP,
    TCMD_EM2_STOP,
    TCMD_EM3_STOP,
    TCMD_EM_ENABLE,
    TCMD_EM_STOP_ALL,
    TCMD_KF_Q00,
    TCMD_KF_Q11,
    TCMD_KF_R,

    // Do not remove!
    TCMD_LENGTH
} tcmd_idx_t;

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

    void sendMessage(tcmd_idx_t idx, double data);

    void receiveMessage();

    void on_pushButton_udp_connect_toggled(bool checked);

    void handleBytesWritten(qint64 bytes);

    void processMessageQueue();

    void on_pushButton_em3_toggled(bool checked);

    void on_pushButton_em_gain_clicked();

    void on_pushButton_em_pow_clicked(bool checked);

    void on_pushButton_browse_clicked();

    void on_pushButton_flash_clicked();

    void on_pushButton_flash_2_clicked();

    void on_pushButton_em_gain_2_clicked();

private:
    void populate_telemetry(const telemetry_t &t);

    QVector<double> tms, d[4], c[4], kf_d[4], kf_v[4];
    em_state_t em_state[4] = {EM_OFF, EM_OFF, EM_OFF, EM_OFF};

    QString hexFilePath;
    QNetworkAccessManager *manager;

    Ui::MainWindow *ui;
    QUdpSocket *udp_socket;
    QHostAddress udp_server_ip;
    quint16 udp_server_port;
    QTimer *timer_plot_mag;
    QQueue<QPair<tcmd_idx_t, double>> messageQueue;
    bool isSending = false;
};
#endif // MAINWINDOW_H

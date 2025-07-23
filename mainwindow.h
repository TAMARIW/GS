#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QQueue>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QFile>
#include <QUrl>

// Satellite docking states.
// Please make sure it is identical to the one on embedded firmware.
enum dock_state
{
    DOCK_STATE_START,   // Indicates the start of docking sequence (received from third party)
    DOCK_STATE_IDLE,    // Do nothing at all
    DOCK_STATE_CAPTURE, // Passive coil actuation to bring satellites together
    DOCK_STATE_CONTROL, // Soft docking control with position and velocity feedback
    DOCK_STATE_LATCH,   // Extra push to overcome latch friction
    DOCK_STATE_UNLATCH, // Repel latched satellites
    DOCK_STATE_ABORT    // Abort the docking sequence under unsafe conditions
};

typedef struct
{
    float d[4];    // ToF measurements [mm]
    float c[4];    // Electromagnet current feedback [mA]
    float dt[5];    // Thread periods [ms]
    float kf_d[4]; // Kalman Filter distance estimates
    float kf_v[4]; // Kalman Filter velocity estimates
    enum dock_state state; // Current docking state
    uint16_t crc;
} telemetry_t;

typedef enum
{
    EM_ON,
    EM_OFF,
    EM_STANDBY
} em_state_t;

// Add/remove new/obsolete telecommands as enum elements.
// Please make sure it is identical to the one on embedded firmware.
enum tcmd_idx
{
    // Coil PI controller gains
    TCMD_EM_KP,
    TCMD_EM_KI,

    // Coil control set-points
    TCMD_EM0,
    TCMD_EM1,
    TCMD_EM2,
    TCMD_EM3,

    // Coil enable/disable flags
    TCMD_EM0_STOP,
    TCMD_EM1_STOP,
    TCMD_EM2_STOP,
    TCMD_EM3_STOP,
    TCMD_EM_STOP_ALL,

    // KF noise covariances
    TCMD_KF_R,
    TCMD_KF_Q00,
    TCMD_KF_Q11,

    // Docking configurable parameters
    TCMD_DOCK_KP,
    TCMD_DOCK_KI,
    TCMD_DOCK_KD,
    TCMD_DOCK_KF,
    TCMD_DOCK_LATCH_CURRENT,
    TCMD_DOCK_UNLATCH_CURRENT,
    TCMD_DOCK_VELOCITY_SP,
    TCMD_DOCK_DISTANCE_SP,

    // Docking states
    TCMD_DOCK_STATE_START,
    TCMD_DOCK_STATE_IDLE,
    TCMD_DOCK_STATE_LATCH,
    TCMD_DOCK_STATE_ABORT,
    TCMD_DOCK_STATE_CAPTURE,
    TCMD_DOCK_STATE_CONTROL,
    TCMD_DOCK_STATE_UNLATCH,

    // Number of enumerators
    TCMD_LENGTH
};

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

    void sendMessage(enum tcmd_idx idx, double data);

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

    void on_pushButton_dock_clicked();

    void on_pushButton_dock_state_idle_clicked();

    void on_pushButton_dock_state_capture_clicked();

    void on_pushButton_dock_state_control_clicked();

    void on_pushButton_dock_state_latch_clicked();

    void on_pushButton_dock_state_unlatch_clicked();

    void on_pushButton_state_abort_clicked();

    void on_pushButton_send_latch_current_clicked();

    void on_pushButton_send_disp_sp_clicked();

    void on_pushButton_send_vel_sp_clicked();

    void on_pushButton_send_unlatch_current_clicked();

    void on_pushButton_send_dock_kp_clicked();

    void on_pushButton_send_dock_ki_clicked();

    void on_pushButton_send_dock_kd_clicked();

    void on_pushButton_send_dock_kf_clicked();

    void on_pushButton_goto_kf_dist_plot_clicked();

    void on_pushButton_goto_kf_vel_plot_clicked();

    void on_pushButton_goto_current_plot_clicked();

    void on_pushButton_send_dist_sp_1_clicked();

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
    QQueue<QPair<enum tcmd_idx, double>> messageQueue;
    bool isSending = false;
};
#endif // MAINWINDOW_H

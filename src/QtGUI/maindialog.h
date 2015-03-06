#ifndef _MAINDIALOG_
#define _MAINDIALOG_
#include <QDialog>
#include "interface.h"
class QPushButton;
class QLabel;
class QTimer;
class UpdateThread;
class QCheckBox;
class MainDialog:public QDialog
{
    Q_OBJECT
public:
    MainDialog(QWidget * parent = 0);

private:
    QLabel * vidWnd;
    QLabel * tempLabel;
    QLabel * HumiLabel;
    QLabel * luxLabel;
    QLabel * alarmLabel;
    QLabel * rfidLabel;
    QLabel * rfidNameLabel;
    QLabel * tileLabel;
    QCheckBox *autoLed1;
    QCheckBox *autoLed2;
    QPushButton *led1Button;
    QPushButton *led2Button;
    QPushButton *motorDir1Button;
    QPushButton *motorDir2Button;
    QPushButton *motorStopButton;
    QTimer *timer;
    API_HANDLE * api_handle;
    int led1enable;
    int led2enable;
    UpdateThread *thread;
public:
    void getTemp();
    void getHumi();
    void getLux();
    void getAlarmStatus();
    void getRFIDCard();
    void updateGUI();
private slots:
    void setled1();
    void setled2();
    void setMotDir1();
    void setMotDir2();
    void setMotStop();
};


#endif

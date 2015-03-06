#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include "maindialog.h"
#include "updateThread.h"
#include <msg.h>
#include <qwt.h>
MainDialog::MainDialog(QWidget * parent)
    :QDialog(parent)
{
    unsigned short gd[]={0x2103,0};
    QString dc = QString::fromUtf16(gd);
    QString str = QString(tr("Temperature:0.00 "));
    str.append(dc);
    led1enable = 0;
    vidWnd = new QLabel(this);
    vidWnd->setGeometry(50,50,400,300);
    vidWnd->setStyleSheet("background-color:rgb(0,255,0)");
    api_handle = new(API_HANDLE);
    if(api_handle == NULL)
    {
//        printf("api_handle is NULL!\n");
    }
    api_init(api_handle, MSG_TYPE_GUI);
    led1Button = new QPushButton(tr("LED1"),this);
    led1Button->setGeometry(50,380,150,30);
    
    led2Button = new QPushButton(tr("LED2"),this);
    led2Button->setGeometry(230,380,150,30);

    autoLed1 = new QCheckBox(tr("Auto LED1"),this);
    autoLed1->setGeometry(410,380,150,30);

    autoLed2 = new QCheckBox("Auto LED2",this);
    autoLed2->setGeometry(590,380,150,30);

    motorDir1Button = new QPushButton(tr("Motor Direction1"),this);
    motorDir1Button->setGeometry(50,430,200,30);
    
    motorDir2Button = new QPushButton(tr("Motor Direction2"),this);
    motorDir2Button->setGeometry(280,430,200,30);
    
    motorStopButton = new QPushButton(tr("Motor Stop"),this);
    motorStopButton->setGeometry(510,430,200,30);
    
    tileLabel = new QLabel(tr("CloudSail SmartHome System"),this);
    tileLabel->setGeometry(200, 10, 400, 30);
    
    tempLabel = new QLabel(str,this);
    tempLabel->setGeometry(470, 50, 250, 30);
   
    HumiLabel = new QLabel(tr("Humidity      :0.00 %rh"),this);
    HumiLabel->setGeometry(470, 100, 330, 30);
    
    luxLabel = new QLabel(tr("Illuminance  :0.00 lux"),this);
    luxLabel->setGeometry(470, 150, 330, 30);
   
    alarmLabel = new QLabel(tr("Alarm Status:Normal"),this);
    alarmLabel->setGeometry(470, 200, 330, 30);
    
    rfidLabel = new QLabel(tr("RFID ID         :No Card"),this);
    rfidLabel->setGeometry(470, 250, 330, 30);

    rfidNameLabel = new QLabel(tr(""),this);
    rfidNameLabel->setGeometry(470, 300, 260, 30);
     
    thread = new UpdateThread(this);
    thread->start();

    connect(led1Button, SIGNAL(clicked()),this, SLOT(setled1()));    
    connect(led2Button, SIGNAL(clicked()),this, SLOT(setled2()));    
    connect(motorDir1Button, SIGNAL(clicked()),this, SLOT(setMotDir1()));    
    connect(motorDir2Button, SIGNAL(clicked()),this, SLOT(setMotDir2()));    
    connect(motorStopButton, SIGNAL(clicked()),this, SLOT(setMotStop()));    
}
void MainDialog::setled1()
{
    if(!autoLed1->isChecked())
    {
        led1enable = !led1enable;
        SetLed1Enable(api_handle, led1enable); 
    }
}
void MainDialog::setled2()
{
    if(!autoLed2->isChecked())
    {
        led2enable = !led2enable;
        SetLed2Enable(api_handle, led2enable); 
    }
}
void MainDialog::getTemp()
{
    float val;
    unsigned short gd[]={0x2103,0};
    QString dc = QString::fromUtf16(gd);
    GetTempData(api_handle, &val);
    QString str = QString("Temperature:%1 ").arg(val, 0, 'f', 2);
    str.append(dc); 
    tempLabel->setText(str);
}
void MainDialog::getHumi()
{
    float val;
    GetHumiData(api_handle, &val);
    QString str = QString("Humidity      :%1 %rh").arg(val, 0,'f', 2);
    HumiLabel->setText(str);
}
void MainDialog::getLux()
{
    float val;
    GetLuxData(api_handle, &val);
    QString str = QString("Illuminance  :%1 lux").arg(val, 0, 'f', 2);
    luxLabel->setText(str);
    if(autoLed1->isChecked())
    {
        if(val < 10)
        {
            SetLed1Enable(api_handle, 1);
        }
        else
        {
            SetLed1Enable(api_handle, 0);
        }
    }
}
void MainDialog::getAlarmStatus()
{
    int val;
    static int enable = 1;
    QString str;
    GetAlarmStatus(api_handle, &val);
    if(val == 0)
        str = QString("Alarm Status:Normal");
    else if(val == 1)
        str = QString("Alarm Status:Audio Warning");
    else
        str = QString("Alarm Status:Warning");
    if(autoLed2->isChecked())
    {
        if(val == 1)
        {
            enable = !enable;
            SetLed2Enable(api_handle,enable);
        }else
            SetLed2Enable(api_handle,0);
    }
    alarmLabel->setText(str);
}
void MainDialog::getRFIDCard()
{
    unsigned char id[5] = {0xff};
    QString str;
    QString name;
    GetRFIDData(api_handle, id);
    
    if((id[0]!=0x00)&&(id[1] != 0x00))
    {
        str = str.sprintf("RFID ID         :0x%x%x%x%x",id[3],id[2],id[1],id[0]);
        rfidLabel->setText(str);
        if((id[3] == 0x43)&&(id[2] == 0x91) && (id[1]==0xfa) && id[0] == 0x34)
        {
            name = QString("Hello,Tong Xingyu");
            rfidNameLabel->setText(name);
            SetBeeperEnable(api_handle, 1);
            usleep(200000);
            SetBeeperEnable(api_handle, 0);
            SetMotorDir(api_handle,0);
            usleep(500000);
            SetMotorStop(api_handle);
        }else
        {
            name = QString("Unauthorized user!");
            rfidNameLabel->setText(name);
            SetBeeperEnable(api_handle, 1);
            usleep(100000);
            SetBeeperEnable(api_handle, 0);
            usleep(100000);
            SetBeeperEnable(api_handle, 1);
            usleep(100000);
            SetBeeperEnable(api_handle, 0);
        } 
    }
    else
    {
        str = QString("RFID ID         :No Card");
        rfidLabel->setText(str);
        rfidNameLabel->setText(tr(""));
    }
}
void MainDialog::setMotDir1()
{
    SetMotorDir(api_handle, 0);
}
void MainDialog::setMotDir2()
{
    SetMotorDir(api_handle, 1);
}
void MainDialog::setMotStop()
{
    SetMotorStop(api_handle);
}
void MainDialog::updateGUI()
{
    getTemp();
    getHumi();
    getLux();
    getAlarmStatus();
    getRFIDCard();
}

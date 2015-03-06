#ifndef _UPDATETHREAD_
#define _UPDATETHREAD_
#include <QDialog>
#include <QThread>
class MainDialog;
class UpdateThread : public QThread
{
    Q_OBJECT
public:
    UpdateThread(MainDialog * parent);
    void stop();
protected:
    void run();
private:
    volatile bool stopped;
    MainDialog * parentDlg;
};

#endif

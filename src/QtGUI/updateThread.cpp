#include "maindialog.h"
#include "updateThread.h"

UpdateThread::UpdateThread(MainDialog * parent)
{
    stopped = false;
    parentDlg = parent; 

}
void UpdateThread::run()
{
    while(!stopped)
    {
        parentDlg->updateGUI();
        usleep(3000);
    }
    stopped = false;
}
void UpdateThread::stop()
{
    stopped = true;
}

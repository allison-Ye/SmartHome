#include <QApplication>

#include "maindialog.h"

int main(int argc,char ** argv)
{
    QApplication app(argc, argv);
    MainDialog * mainDialog = new MainDialog();
    mainDialog->resize(800,480);
    mainDialog->setWindowOpacity(1);  
    mainDialog->setWindowFlags(Qt::FramelessWindowHint);      
    mainDialog->show();
    
    return app.exec();
}

#include <QCoreApplication>
#include <QDebug>
#include <QObject>
#include "controleur.h"
#include <signal.h>



void SigInt_Handler(int)
{
    qDebug() << "Interrupt received";
    qApp->quit();
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    controleur *mycontroleur=new controleur(&a);
    signal(SIGINT, &SigInt_Handler);

    return a.exec();

}

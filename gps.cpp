#include "gps.h"
#include <QDebug>

gps::gps(QObject *parent) : QObject(parent)
{
    com_gps = new QSerialPort;
    connect(com_gps,SIGNAL(readyRead()),this,SLOT(lecture_trame()));
}

bool gps::init(QString comx)
{
    bool com_etat;
    com_gps->setPortName(comx);
    com_gps->setReadBufferSize(1);
    com_gps->setFlowControl(QSerialPort::NoFlowControl);
    com_gps->setParity(QSerialPort::NoParity);
    com_gps->setDataBits(QSerialPort::Data8);
    com_gps->setStopBits(QSerialPort::OneStop);
    com_gps->setBaudRate(QSerialPort::Baud9600);
    if (com_gps->open( QIODevice::ReadWrite))
        {
          com_etat=true;
        }
        else
        {
          com_etat=false;
        }
return com_etat;
}

bool gps::close()
{
  qDebug()<<"fermeture gps";
    com_gps->close();
  return true;
}

void gps::lecture_trame()
{
   static bool debut_trame=false;
   static int debut=-1;
   static int fin=-1;
   donnees+=(com_gps->readAll());
   if (debut_trame==false)
   {
         debut=donnees.indexOf('$');

         if(debut>=0)
          {
           debut_trame=true;
           donnees=donnees.remove(0,debut);
           debut=-1;
          }
    }
   else
    {
       fin=donnees.indexOf('\n');
       if(fin>=0)
       {
           debut_trame=false;
           data_recv.clear();
           donnees=donnees.remove(fin,donnees.size());
           data_recv.append(donnees);

           fin=-1;
           //qDebug()<<donnees;
           donnees.clear();
           emit trame_recue(data_recv);
       }

    }

}
gps::~gps()
{
    if(com_gps->isOpen())
    {
        qDebug()<<"fermeture gps";
        com_gps->close();
        delete com_gps;
    }

}

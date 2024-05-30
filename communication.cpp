#include "communication.h"

communication::communication(QObject *parent) : QObject(parent)
{

    com_serie = new QSerialPort;
    connect(com_serie,SIGNAL(readyRead()),this,SLOT(lecture_trame()));
    connect(com_serie,SIGNAL(bytesWritten(qint64)),this,SLOT(check_ecriture_trame(qint64)));
    transmis=true;
}


bool communication::init(QString comx)
{
    bool com_etat;
    com_serie->setPortName(comx);
    com_serie->setReadBufferSize(1);
    com_serie->setFlowControl(QSerialPort::NoFlowControl);
    com_serie->setParity(QSerialPort::NoParity);
    com_serie->setDataBits(QSerialPort::Data8);
    com_serie->setStopBits(QSerialPort::OneStop);
    com_serie->setBaudRate(QSerialPort::Baud9600);
    if (com_serie->open( QIODevice::ReadWrite))
        {
          com_etat=true;
        }
        else
        {
          com_etat=false;
        }
return com_etat;
}

bool communication::close()
{
  com_serie->close();
  return true;
}

void communication::lecture_trame()
{
   static bool debut_trame=false;
   static int debut=-1;
   static int fin=-1;
   donnees+=(com_serie->readAll());
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

void communication::envoyer(QString trame)
{
   transmis=false;
   QByteArray new_trame =trame.toUtf8();
   taille_trame_tx= new_trame.length();
   com_serie->write(new_trame) ;
   //if (com_serie->waitForBytesWritten(100)==true)
  // {
     //transmis=true;
   //}
}

void communication::check_ecriture_trame(qint64 nbr_octets)
{
  if (nbr_octets==taille_trame_tx)
  {
  transmis=true;
  }
}

bool communication::check_transmis()
{
  return transmis;
}


communication::~communication()
{
    if(com_serie->isOpen())
    {
    qDebug()<<"fermeture zigbee";
    com_serie->close();
    delete com_serie;
    }
}

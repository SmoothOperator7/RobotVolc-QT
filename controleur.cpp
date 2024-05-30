#include "controleur.h"
#include <QTime>

controleur::controleur(QObject *parent) : QObject(parent)
{
    zigbee=new communication(this);
     if(zigbee->init(module_zigbee))
     {
       qDebug()<<"ok zigbee" ;
       QObject::connect(zigbee,SIGNAL(trame_recue(QString)),this,SLOT(lireTrameZigbee(QString)));
     }
     recept_gps=new gps(this);
     if(recept_gps->init(module_gps))
     {
         qDebug()<<"ok gps";
         QObject::connect(recept_gps,SIGNAL(trame_recue(QString)),this,SLOT(lireTrameGPS(QString)));
     }
     cde_mouvements = new mouvements(this);
     if (cde_mouvements->etat_connexion()==true)
     {
         qDebug()<<"ok module moteurs";
     }


}

void controleur::lireTrameGPS(QString trame)
{

    QStringList champs = trame.split(',');
        //qDebug()<<champs[0];
               if( champs[0] == "$GPRMC")
               {
                 heure=champs[1];
                 latitude = champs[3];
                 longitude = champs[5];
                 vitesse_gps = champs[7];
                 //altitude = champs[9] ;

                 QString trame_loc=QString("$GPRMC,%1,%2,%3,%4,\r\n").arg(latitude).arg(longitude).arg(heure).arg(vitesse_gps);
                 qDebug()<<trame_loc;
                //problème si false plus d'envoi,; prevoir un timeout
                if(zigbee->check_transmis()==true)
                 {
                    zigbee->envoyer(trame_loc);

                 }


               }
}

void controleur::lireTrameZigbee(QString trame)
{
   // qDebug()<<trame;
    QStringList champs = trame.split(',');
               if( champs[0] == "$PILOT")
               {
               vitesse=champs[1].toInt();
               sens=champs[2][0];
               direction=champs[3][0];
               mode=champs[4][0];
               bras=champs[5].toInt();
               pince=champs[6].toInt();

               cde_mouvements->deplacer(vitesse,sens,direction);
               cde_mouvements->prelever(pince,bras);

                }

}

controleur::~controleur()
{
   //a ce moment les objets gps et zigbee ne sont détruits s'ils ne sont pas enfants de QObject
qDebug()<<"fermeture des taches";

//QTime dieTime= QTime::currentTime().addSecs(1);
   // while (QTime::currentTime() < dieTime);
}




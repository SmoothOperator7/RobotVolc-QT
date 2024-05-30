#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include "gps.h"
#include "communication.h"
#include "mouvements.h"

#define module_zigbee "/dev/serial/by-id/usb-FTDI_FT231X_USB_UART_DN01IW81-if00-port0"
#define module_gps "/dev/ttyAMA0"

class controleur : public QObject
{
    Q_OBJECT
public:
    explicit controleur(QObject *parent = nullptr);
    ~controleur();
    QString latitude;
    QString longitude;
    QString altitude;
    QString heure;
    QString vitesse_gps;
    int vitesse;
    QChar sens;
    QChar direction;
    QChar mode;
    int bras;
    int pince;

private:
    gps *recept_gps;
    communication *zigbee;
    mouvements *cde_mouvements;

signals:

private slots:
    void lireTrameGPS(QString);
    void lireTrameZigbee(QString);
};



#endif // CONTROLEUR_H

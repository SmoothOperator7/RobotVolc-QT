#ifndef MOUVEMENTS_H
#define MOUVEMENTS_H

#include <QDebug>
#include <unistd.h>
#include <bcm2835.h>
#include <QObject>
#include <QTimer>

#define MODULE_MD22 0x58  //(0xB0 sur 8 bits)
#define MODULE_SRF02_DROIT 0x70 //(0xE0 sur 8 bits)
#define MODULE_SRF02_GAUCHE 0x71
#define REG_MODE 0
#define REG_SPEED1 1  //moteurs de droite
#define REG_SPEED2 2  //moteurs de gauche
#define REG_ACCEL 3
#define REG_VERSION 7



#define PAS_OBSTACLE 0
#define OBSTACLE_DEVANT 1
#define OBSTACLE_DROITE 2
#define OBSTACLE_GAUCHE 3
#define PIN_PWM_PINCE 12
#define PIN_PWM_BRAS 13
#define PWM_PINCE 0
#define PWM_BRAS 1

class mouvements : public QObject
{
    Q_OBJECT
public:
    explicit mouvements(QObject *parent = nullptr);
    bool etat_connexion();
    void deplacer(int vitesse,QChar sens,QChar direction);
    void prelever(int pince,int bras);
    int getObstacle()
    {
     return obstacle;
    }
    ~mouvements();



private:
    bool etat=false;
    int fd;
    int obstacle;
    QTimer timer;
    void init();
	
private slots:
    void onTime();

};

#endif // MOUVEMENTS_H

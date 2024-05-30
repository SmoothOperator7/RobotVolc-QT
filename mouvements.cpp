#include "mouvements.h"

mouvements::mouvements(QObject *parent) : QObject(parent)
{
init();
    if (etat==true)
    {
    
     QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(onTime()));
     timer.start(100);
  
    }
}

void mouvements::init()
{


       if (bcm2835_init())
        {
            qDebug()<<"initialisation pince";
            bcm2835_gpio_fsel(PIN_PWM_PINCE,BCM2835_GPIO_FSEL_ALT0);  //PWM0
            bcm2835_gpio_fsel(PIN_PWM_BRAS,BCM2835_GPIO_FSEL_ALT0);  //PWM1
            bcm2835_pwm_set_mode(0,1,1);
            bcm2835_pwm_set_mode(1,1,1);
            bcm2835_pwm_set_range(0,1024);
            bcm2835_pwm_set_range(1,1024);
             bcm2835_pwm_set_clock(375); // mettre en dernier

            bcm2835_pwm_set_data(PWM_BRAS,130);//bras en position haute 130
            bcm2835_pwm_set_data(PWM_PINCE,110); //position initiale ouiverte 110
            char commande[2]={0,0};
            bcm2835_i2c_begin();                //Start I2C operations.
            bcm2835_i2c_setSlaveAddress(MODULE_MD22);  //I2C address
            bcm2835_i2c_write(commande,2);
            bcm2835_i2c_end();
          etat=true;
        }
}

bool mouvements::etat_connexion()
{
    return etat;
}

void mouvements::prelever(int pince, int bras)
{

      bcm2835_pwm_set_data(PWM_PINCE,pince);
      bcm2835_pwm_set_data(PWM_BRAS,bras);
}


void mouvements::deplacer(int vitesse,QChar sens,QChar direction)
{

    int obstacle = getObstacle();
          // qDebug()<<obstacle;
          unsigned char vit=(unsigned char)vitesse;
           char cde_mot1[2]={REG_SPEED1,128};
           char cde_mot2[2]={REG_SPEED2,128};

              if(sens=='N')
                {//pas de mouvement
                  bcm2835_i2c_begin();
                  bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                  bcm2835_i2c_write(cde_mot1,2);
                  bcm2835_i2c_end();
                  bcm2835_i2c_begin();
                  bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                  bcm2835_i2c_write(cde_mot2,2);
                  bcm2835_i2c_end();
                return;
                }
                if(sens=='A')  //On demande d'avancer
                {
                  if (direction=='F')  //avancer tout droit
                  {
                      if(obstacle==OBSTACLE_DEVANT)
                      {
                        
                            bcm2835_i2c_begin();
                            bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                            bcm2835_i2c_write(cde_mot1,2);
                            bcm2835_i2c_end();
                            bcm2835_i2c_begin();
                            bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                            bcm2835_i2c_write(cde_mot2,2);
                            bcm2835_i2c_end();
                      }
                      else
                      {
                        
                            cde_mot1[1]=128+vit;
                            cde_mot2[1]=128+vit;
                            bcm2835_i2c_begin();
                            bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                            bcm2835_i2c_write(cde_mot1,2);
                            bcm2835_i2c_end();
                            bcm2835_i2c_begin();
                            bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                            bcm2835_i2c_write(cde_mot2,2);
                            bcm2835_i2c_end();
                      }
                     return;
                  }
                  if (direction=='G') //avancer vers la gauche
                  {
                      if(obstacle==OBSTACLE_GAUCHE)
                      {
                      
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot1,2);
                          bcm2835_i2c_end();
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot2,2);
                          bcm2835_i2c_end();
                      }
                      else
                      {
                     	//on augmente la vitesse des moteurs droits
                          cde_mot1[1]=148+vit;
                          cde_mot2[1]=128+vit;
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot1,2);
                          bcm2835_i2c_end();
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot2,2);
                          bcm2835_i2c_end();
                      }
                      return;
                  }

                  if (direction=='D') //avancer vers la gauche
                  {
                      if(obstacle==OBSTACLE_DROITE)
                      {
                        
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot1,2);
                          bcm2835_i2c_end();
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot2,2);
                          bcm2835_i2c_end();
                      }
                      else
                      {
                         //on augmente la vitesse des moteurs gauches
                         
                          cde_mot1[1]=128+vit;
                          cde_mot2[1]=148+vit;
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot1,2);
                          bcm2835_i2c_end();
                          bcm2835_i2c_begin();
                          bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                          bcm2835_i2c_write(cde_mot2,2);
                          bcm2835_i2c_end();
                      }
                       return;
                   }
                 }
                if (sens=='R')//On demande de reculer
                {
                  if (direction=='F')  //reculer tout droit
                  {
                 
                      cde_mot1[1]=128-vit;
                      cde_mot2[1]=128-vit;
                      bcm2835_i2c_begin();
                      bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                      bcm2835_i2c_write(cde_mot1,2);
                      bcm2835_i2c_end();
                      bcm2835_i2c_begin();
                      bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                      bcm2835_i2c_write(cde_mot2,2);
                      bcm2835_i2c_end();

                  }
                  if (direction=='G') //reculer vers la gauche
                  {
            
                 	//on augmente la vitesse des moteurs droits
                      cde_mot1[1]=108-vit;
                      cde_mot2[1]=128-vit;
                      bcm2835_i2c_begin();
                      bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                      bcm2835_i2c_write(cde_mot1,2);
                      bcm2835_i2c_end();
                      bcm2835_i2c_begin();
                      bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                      bcm2835_i2c_write(cde_mot2,2);
                      bcm2835_i2c_end();
                  }

                  if (direction=='D') //reculer vers la droite
                  {
                  //on augmente la vitesse des moteurs gauches
                 
                      cde_mot1[1]=128-vit;
                      cde_mot2[1]=108-vit;
                      bcm2835_i2c_begin();
                      bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                      bcm2835_i2c_write(cde_mot1,2);
                      bcm2835_i2c_end();
                      bcm2835_i2c_begin();
                      bcm2835_i2c_setSlaveAddress(MODULE_MD22);
                      bcm2835_i2c_write(cde_mot2,2);
                      bcm2835_i2c_end();
                  }
                  return;
                }



}
    void mouvements::onTime()
    {
        //Lecture du capteur droit
       char buf[4];
         char commande[2]={0,0X51};
         bcm2835_i2c_begin();                //Start I2C operations.
         bcm2835_i2c_setSlaveAddress(MODULE_SRF02_DROIT);  //I2C address
         bcm2835_i2c_write(commande,1);
         bcm2835_i2c_read(buf,4);
         unsigned char highByte = buf[2];
         unsigned char lowByte = buf[3];
         int distance_droit=(highByte*256+lowByte);
         //qDebug()<<"droit:"<<distance_droit;
         bcm2835_i2c_end();

          //Lecture du capteur gauche
         bcm2835_i2c_begin();
         bcm2835_i2c_setSlaveAddress(MODULE_SRF02_GAUCHE);  //I2C address
         bcm2835_i2c_write(commande,1);
         bcm2835_i2c_read(buf,4);

           highByte = buf[2];
           lowByte = buf[3];
           int distance_gauche=(highByte*256+lowByte);
           //qDebug()<<"gauche:"<<distance_gauche;

                 if (distance_droit>12 && distance_droit<=40 && distance_gauche>12 && distance_gauche<=40)
                  {
                   obstacle=OBSTACLE_DEVANT;
                   qDebug()<<"devant";
                  }

                 else if (distance_droit<40 && distance_gauche>50)
                  {
                    obstacle=OBSTACLE_DROITE;
                    qDebug()<<"droite";
                  }
                  else if (distance_gauche<40 && distance_droit>50)
                  {
                     obstacle=OBSTACLE_GAUCHE ;
                       qDebug()<<"gauche";

                  }
                  else
                  {
                     obstacle=PAS_OBSTACLE;
                     qDebug()<<"rien";
                  }
           bcm2835_i2c_write(commande,2);
           bcm2835_i2c_setSlaveAddress(MODULE_SRF02_DROIT);  //I2C address
           bcm2835_i2c_write(commande,2);
           bcm2835_i2c_end();
    }

    mouvements::~mouvements()
    {
        qDebug()<<"fermeture module moteur";
       
        bcm2835_close();
    }


#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>

class communication:public QObject
{
    Q_OBJECT
public:


    communication(QObject *parent);
    bool init(QString);
    bool close();
    void envoyer(QString);
    bool check_transmis();

    QString data_recv ;
    //bool com_etat;

    ~communication();

protected:

private:
    QString port;
    QSerialPort *com_serie;
    qint64 taille_trame_tx;
    bool transmis;
    QByteArray donnees;


private slots:
    void lecture_trame();
    void check_ecriture_trame(qint64);

signals:
    void trame_recue(QString trames);
    void trame_emise();

};


#endif // COMMUNICATION_H

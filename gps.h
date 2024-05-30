#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QSerialPort>

class gps : public QObject
{
    Q_OBJECT
public:
    explicit gps(QObject *parent = nullptr);  
    bool init(QString);
    bool close();

    QString data_recv ;

    ~gps();

private:
    QString port;
    QSerialPort *com_gps;
    QByteArray donnees;


private slots:
    void lecture_trame();

signals:
    void trame_recue(QString trames_gps);

};

#endif // GPS_H

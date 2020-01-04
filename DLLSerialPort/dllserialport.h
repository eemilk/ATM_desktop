#ifndef DLLSERIALPORT_H
#define DLLSERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QObject>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include "dllserialport_global.h"

class DLLSERIALPORTSHARED_EXPORT DLLSerialPort : public QObject
{
    Q_OBJECT

public:
    DLLSerialPort();
    void DLLSERIALPORTSHARED_EXPORT returnCardNumber();
    QString returnValue;
    QString availablePorts();
    QString ports;

private:
   const quint16 serialPort_Vendor = 5562;
   const quint16 serialPort_Product = 22;

   QSerialPort * serial;
   QString cardSerialNumber;

signals:
   void DLLSERIALPORTSHARED_EXPORT layerInterfaceSignal();

public slots:
   void DLLSERIALPORTSHARED_EXPORT emitInterfaceSignalFunction();
   QString DLLSERIALPORTSHARED_EXPORT printCardInfo();
};

#endif // DLLSERIALPORT_H

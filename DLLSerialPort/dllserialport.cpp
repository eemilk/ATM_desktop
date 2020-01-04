#include "dllserialport.h"


DLLSerialPort::DLLSerialPort()
{
}

void DLLSerialPort::returnCardNumber()
{
    serial = new QSerialPort(this);
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        if (port.vendorIdentifier() == serialPort_Vendor && port.productIdentifier() == serialPort_Product)
        {
            //Setting the portname to associate with the known RFID card reader regardless of the serial port used on the computer
            serial->setPortName(port.portName());
        }
    }
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::HardwareControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << serial->portName();
    }

    else
    {
        qDebug() << "Unable to open the serial port.";
    }

    connect(serial, SIGNAL(readyRead()), this, SLOT(printCardInfo()));

    qDebug() << "Hello!";
}

void DLLSerialPort::emitInterfaceSignalFunction()
{
    emit layerInterfaceSignal();
    qDebug() << "Testing signal emitting.";
}

QString DLLSerialPort::printCardInfo()
{

    char readData[20];
    int bytesRead;
    bytesRead = serial->read(readData, 20);
    readData[bytesRead] = '\0';

    if (bytesRead > 10)
    {
        for (int i = 0; i <= 9; i++)
        {
            cardSerialNumber = readData;
        }

        cardSerialNumber.remove(0, 3);

        qDebug() << "The number of the card is:" << cardSerialNumber;
        returnValue = cardSerialNumber;
    }

       else
        {
            qDebug() << "Error while reading the card!";
        }

        emitInterfaceSignalFunction();
        return returnValue;
}

QString DLLSerialPort::availablePorts()
{
    QString ports;
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        {
            //Debugging to see the names and values to be assigned to vendor and product variables
            qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier()
                     << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy();
            ports += port.portName();
        }
    }
    return ports;
}

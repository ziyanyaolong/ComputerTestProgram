#include "COM_Port.h"

COM_Port::COM_Port(QObject* parent) :
	QObject(parent)
{
	serialPort = new QSerialPort();
	serialPortInfo = new QSerialPortInfo();
	thread = new QThread(this);
	serialPort->moveToThread(thread);
	thread->start();
}

COM_Port::~COM_Port()
{
	if (thread)
	{
		thread->exit();
	}

	if (serialPort)
	{
		serialPort->close();
		delete serialPort;
	}

	if (thread)
	{
		while (thread->isRunning()) thread->exit();
		delete thread;
	}
}

void COM_Port::updateSerialPort()
{
	foreach(*serialPortInfo, QSerialPortInfo::availablePorts())
	{
		COM_Name.push_back(serialPortInfo->portName());
	}
	emit haveUpdateSerialPortData(&COM_Name);
}

void COM_Port::connectSerialPort(const QString serialPortName)
{
    serialPort->close();
    serialPort->setPortName(serialPortName);
    //���ò�����
    serialPort->setBaudRate(QSerialPort::Baud9600);
    //��������λ��
    serialPort->setDataBits(QSerialPort::Data8);
    //������żУ��
    serialPort->setParity(QSerialPort::NoParity);
    //����ֹͣλ
    serialPort->setStopBits(QSerialPort::OneStop);
    //����������
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if (!serialPort->open(QIODevice::ReadWrite))
    {
        emit finishConnectSerialPort(false);
		emit serialPortConditionVariation(false);
		emit errorConnect(1);
    }
    else
    {
        emit finishConnectSerialPort(true);
		emit serialPortConditionVariation(true);
    }
}

void COM_Port::closeSerialPort()
{
	serialPort->close();
	emit serialPortConditionVariation(false);
}

void COM_Port::sendData(const QByteArray &byteArray)
{
	if (!serialPort->isOpen())
	{
		return;
	}
	serialPort->write(byteArray);
}
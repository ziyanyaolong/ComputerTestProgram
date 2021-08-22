#pragma once

#include <qserialportinfo.h>
#include <qstringlist.h>
#include <qserialport.h>
#include <qthread.h>
#include <qstring.h>

class COM_Port : public QObject
{
	Q_OBJECT

public:
	COM_Port(QObject* parent = Q_NULLPTR);
	~COM_Port();

private:
	QSerialPort* serialPort;
	QSerialPortInfo* serialPortInfo;
	QThread* thread;

	QStringList COM_Name;

signals:
	void haveUpdateSerialPortData(const QStringList*);
	void finishConnectSerialPort(bool);
	void serialPortConditionVariation(bool);
	void errorConnect(int);

public slots:
	void updateSerialPort();
	void connectSerialPort(const QString);
	void closeSerialPort();
	void sendData(const QByteArray& byteArray);
};


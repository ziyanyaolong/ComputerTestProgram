#include "Performeter.h"

Performeter::Performeter(QObject *parent) :
	QObject(parent)
{
	localDataUpdateThread = new QThread(this);
	OLD = new CPULocalData();
	MLD = new MemoryLocalData();
	dataSynchronization = new QTimer(this);
	
	dataTable = new DataTable;

	dataSynchronization->start(1000);

	OLD->moveToThread(localDataUpdateThread);
	MLD->moveToThread(localDataUpdateThread);
	localDataUpdateThread->start();

	emit OLD->getCpuUsage();
	emit MLD->getWinMemUsage();

	connect(OLD, &CPULocalData::haveNewData, this, [=](double cpuUsage) {
		haveNewUpdateData = true;
		dataTable->cpuUsage = cpuUsage;
		emit OLD->getCpuUsage();
		}, Qt::QueuedConnection);

	connect(MLD, &MemoryLocalData::haveNewData, this, [=](unsigned int memoryUsage) {
		haveNewUpdateData = true;
		dataTable->memoryUsage = memoryUsage;
		emit this->haveNewData(*dataTable);
		emit MLD->getWinMemUsage();
		}, Qt::QueuedConnection);

	connect(dataSynchronization, &QTimer::timeout, this, [=]() {
		if (haveNewUpdateData)
		{
			haveNewUpdateData = false;
			emit this->haveNewData(*dataTable);
		}
		}, Qt::QueuedConnection);
}

Performeter::~Performeter()
{
	if (localDataUpdateThread)
	{
		localDataUpdateThread->exit(0);
	}

	if (localDataUpdateThread)
	{
		while (localDataUpdateThread->isRunning()) localDataUpdateThread->exit(0);
		delete localDataUpdateThread;
	}

	if (dataSynchronization)
	{
		dataSynchronization->stop();
		delete dataSynchronization;
	}

	if (OLD)
		delete OLD;
	if (MLD)
		delete MLD;
	if (dataTable)
		delete dataTable;
}

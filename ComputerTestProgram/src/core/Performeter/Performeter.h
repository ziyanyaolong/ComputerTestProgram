#pragma once

#include <qobject.h>
#include <qthread.h>
#include <qtimer.h>

#include "core/ObtainingLocalData/CPULocalData/CPULocalData.h"
#include "core/ObtainingLocalData/MemoryLocalData/MemoryLocalData.h"
#include "core/DataTable/DataTable.h"

class Performeter : public QObject
{
	Q_OBJECT

public:
	Performeter(QObject *parent = Q_NULLPTR);
	~Performeter();

private:
	DataTable* dataTable;
	CPULocalData* OLD;
	MemoryLocalData* MLD;
	QThread* localDataUpdateThread;
	QTimer* dataSynchronization;

	bool haveNewUpdateData = false;

signals:
	void haveNewData(const DataTable&);
};


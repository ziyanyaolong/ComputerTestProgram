#pragma once

#include <Windows.h>
#include <QObject>
#include <QTime>
#include <QTimer>

#include "core/ObtainingLocalData/ObtainingLocalData.h"

class CPULocalData : public QObject
{
	Q_OBJECT

public:
	CPULocalData(QObject* parent = Q_NULLPTR);
	~CPULocalData();
private:
	QTimer* timer = nullptr;
	FILETIME preIdleTime;
	FILETIME preKernelTime;
	FILETIME preUserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	bool isRunCalculate = false;

	__int64 CompareFileTime2(const FILETIME& preTime, const FILETIME& nowTime);
	__int64 Filetime2Int64(const FILETIME& ftime);
	void GetSystemTimes1() { GetSystemTimes(&preIdleTime, &preKernelTime, &preUserTime); }
	void GetSystemTimes2() { GetSystemTimes(&idleTime, &kernelTime, &userTime); }
	void calculateCpuUsage();

signals:
	void haveNewData(double);
	void getCpuUsage();
};


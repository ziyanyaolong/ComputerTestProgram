#include "MemoryLocalData.h"

MemoryLocalData::MemoryLocalData(QObject* parent) :
    QObject(parent)
{
    updateMemoryUsage = new QTimer(this);

    connect(updateMemoryUsage, &QTimer::timeout, this, [=]() {
        updateMemoryUsage->stop();
        isRunCalculate = false;
        if (haveNewEvent)
        {
            haveNewEvent = false;
            calculateCpuUsage();
        }
        });

    connect(this, &MemoryLocalData::getWinMemUsage, this, &MemoryLocalData::calculateCpuUsage);

}

void MemoryLocalData::calculateCpuUsage()
{
    haveNewEvent = true;
    if (isRunCalculate)
    {
        return;
    }
    haveNewEvent = false;
    isRunCalculate = true;
    updateMemoryUsage->stop();
    MEMORYSTATUSEX ms;
    GlobalMemoryStatus((LPMEMORYSTATUS)&ms);
    emit haveNewData(ms.dwMemoryLoad); 
    updateMemoryUsage->start(ObtainingLocalData::updateTime);
}

MemoryLocalData::~MemoryLocalData()
{
    if (updateMemoryUsage)
    {
        updateMemoryUsage->stop();
        delete updateMemoryUsage;
    }
}

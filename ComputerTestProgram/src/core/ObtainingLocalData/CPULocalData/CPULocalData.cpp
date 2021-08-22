#include "CPULocalData.h"

__int64 CPULocalData::Filetime2Int64(const FILETIME& ftime)
{
    LARGE_INTEGER li;
    li.LowPart = ftime.dwLowDateTime;
    li.HighPart = ftime.dwHighDateTime;
    return li.QuadPart;
}

__int64 CPULocalData::CompareFileTime2(const FILETIME& preTime, const FILETIME& nowTime)
{
    return Filetime2Int64(nowTime) - Filetime2Int64(preTime);
}

void CPULocalData::calculateCpuUsage()
{
    auto idle = CompareFileTime2(preIdleTime, idleTime);
    auto kernel = CompareFileTime2(preKernelTime, kernelTime);
    auto user = CompareFileTime2(preUserTime, userTime);

    if (kernel + user == 0)
        emit this->haveNewData(0);

    emit this->haveNewData(100.0 * (kernel + user - idle) / (kernel + user));
}

CPULocalData::CPULocalData(QObject* parent) :
    QObject(parent)
{
    timer = new QTimer(this);

    connect(this, &CPULocalData::getCpuUsage, this, [=]() {
        if (isRunCalculate)
        {
            return;
        }
        isRunCalculate = true;
        GetSystemTimes1();
        timer->stop();
        timer->start(ObtainingLocalData::updateTime);
        });

    connect(timer, &QTimer::timeout, this, [=]() {
        GetSystemTimes2();
        isRunCalculate = false;
        calculateCpuUsage();
        });
}

CPULocalData::~CPULocalData()
{
    if (timer)
    {
        timer->stop();
        delete timer;
    }
}

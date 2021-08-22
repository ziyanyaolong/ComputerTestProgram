#pragma once

#include <Windows.h>
#include <QObject>
#include <QTimer>

#include "core/ObtainingLocalData/ObtainingLocalData.h"

class MemoryLocalData : public QObject
{
    Q_OBJECT

public:
    MemoryLocalData(QObject* parent = Q_NULLPTR);
    ~MemoryLocalData();

private:
    QTimer* updateMemoryUsage;
    bool isRunCalculate = false;
    bool haveNewEvent = false;
    void calculateCpuUsage();

signals:
    void haveNewData(unsigned int);
    void getWinMemUsage();
};


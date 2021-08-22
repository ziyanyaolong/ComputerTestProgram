#pragma once

#include <QObject>

#include <Halcon.h>
#include <HalconCpp.h>
#include <HDevThread.h>

class HalconVidicon : public QObject
{
	Q_OBJECT

public:
	HalconVidicon(QObject *parent = Q_NULLPTR);
	~HalconVidicon();
	HalconCpp::HObject  ho_Image;
	HalconCpp::HTuple  hv_AcqHandle;

signals:
	void havePicture(const HalconCpp::HObject&);
	void finishPicture(const HalconCpp::HObject&);

public slots:
	void connectVidicon();
	void useVidicon();
	void closeVidicon();
	void testPicture();

private:
	bool isUsing = false;
	bool isOpen = false;
};

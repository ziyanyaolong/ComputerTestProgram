#pragma once

#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#define USE_VIDICON

#include <QtWidgets/qmainwindow.h>
#include <qstring.h>
#include <qtimer.h>
#include <QByteArray>
#include <QPainter>
#include <QResizeEvent>
#include <QImage>

#include "ui_GUI.h"
#include "core/Performeter/Performeter.h"
#include "common/Interface/COM_Port/COM_Port.h"
#include "core/Halcon/HalconVidicon/HalconVidicon.h"
#include "core/DataTable/DataTable.h"

class GUI : public QMainWindow
{
	Q_OBJECT

public:
	GUI(QMainWindow*parent = Q_NULLPTR);
	~GUI();

private:
	virtual void paintEvent(QPaintEvent* event) override;
	virtual void resizeEvent(QResizeEvent* event) override;

	HalconVidicon* halconVidicon;
	Performeter* p;
	COM_Port* serialPort;
	QTimer* updataVidicon;

	Ui::MainWindow *ui;

	HalconCpp::HTuple hv_WindowHandle, hv_WindowHandle2;

	void(QComboBox::* pComboBox)(int);

	bool saveVidicon = false;
	bool testVidicon = false;
	QImage* background;

	const unsigned int updateVidiconTimer = 1;
signals:

};

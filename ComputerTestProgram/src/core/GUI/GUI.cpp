#include "GUI.h"

GUI::GUI(QMainWindow*parent)
	: QMainWindow(parent)
{
	ui = new Ui::MainWindow;
	ui->setupUi(this);
	halconVidicon = new HalconVidicon(this);
	updataVidicon = new QTimer(this);
	p = new Performeter(this);
	background = new QImage(":/res/res/background.jpg");
	serialPort = new COM_Port(this);

	ui->lcdNumber->setDigitCount(4);
	ui->lcdNumber->setSmallDecimalPoint(true);
	ui->pushButton_2->setEnabled(false);

	pComboBox = &QComboBox::currentIndexChanged;
#ifdef USE_VIDICON

	connect(updataVidicon, &QTimer::timeout, halconVidicon, &HalconVidicon::useVidicon, Qt::QueuedConnection);;

	connect(ui->pushButton_4, &QPushButton::clicked, this, [=]() {
		testVidicon = true;
		}, Qt::QueuedConnection);

	connect(ui->pushButton_5, &QPushButton::clicked, this, [=]() {
		if (ui->pushButton_5->text() == "摄像机启用")
		{
			ui->pushButton_5->setText("摄像机关闭");
			saveVidicon = false;
			testVidicon = false;
			emit halconVidicon->connectVidicon();
			emit halconVidicon->useVidicon();
			if (HalconCpp::HDevWindowStack::IsOpen())
				HalconCpp::CloseWindow(HalconCpp::HDevWindowStack::Pop());
			HalconCpp::OpenWindow(0, 0, ui->label_3->width(), ui->label_3->height(), (Hlong)ui->label_3->winId(), "visible", "", &hv_WindowHandle);
			HalconCpp::OpenWindow(0, 0, ui->label_4->width(), ui->label_4->height(), (Hlong)ui->label_4->winId(), "visible", "", &hv_WindowHandle2);
			HalconCpp::HDevWindowStack::Push(hv_WindowHandle);
			HalconCpp::HDevWindowStack::Push(hv_WindowHandle2);
			HalconCpp::HDevWindowStack::SetActive(hv_WindowHandle);
			updataVidicon->start(updateVidiconTimer);
		}
		else if (ui->pushButton_5->text() == "摄像机关闭")
		{
			updataVidicon->stop();
			ui->pushButton_5->setText("摄像机启用");
			emit halconVidicon->closeVidicon();
		}
		}, Qt::QueuedConnection);

	connect(halconVidicon, &HalconVidicon::havePicture, this, [=](const HalconCpp::HObject& ho_Image) {
		if (saveVidicon)
		{
			srand((unsigned int)time(nullptr));
			QString temp = ui->lineEdit->text();
			if (temp != "")
			{
				saveVidicon = false;
				HalconCpp::WriteImage(ho_Image, "bmp", 0, QString(temp + "/" + QString::number(rand() % 100 + 1)).toStdString().c_str());
				return;
			}
		}
		else if (testVidicon)
		{
			emit halconVidicon->testPicture();
			testVidicon = false;
			return;
		}
		if (HalconCpp::HDevWindowStack::IsOpen())
		{
			HalconCpp::ClearWindow(HalconCpp::HDevWindowStack::GetActive());
			HalconCpp::DispObj(ho_Image, HalconCpp::HDevWindowStack::GetActive());
		}
		});

	connect(halconVidicon, &HalconVidicon::finishPicture, this, [=](const HalconCpp::HObject& ho_Image) {
		if (HalconCpp::HDevWindowStack::IsOpen())
		{
			HalconCpp::ClearWindow(hv_WindowHandle2);
			HalconCpp::DispObj(ho_Image, hv_WindowHandle2);
		}
		});
#endif

	connect(p, &Performeter::haveNewData, this, [=](const DataTable& dataTavle) {
		ui->lcdNumber->display(dataTavle.cpuUsage);
		ui->lcdNumber_2->display(dataTavle.memoryUsage);
		QByteArray byteArray;
		byteArray.resize(1);
		byteArray[0] = 0x01;
		emit serialPort->sendData(byteArray);
		});

	connect(ui->pushButton, &QPushButton::clicked, serialPort, &COM_Port::updateSerialPort, Qt::QueuedConnection);

	connect(ui->pushButton_3, &QPushButton::clicked, this, [=]() {
		saveVidicon = true;
		});

	connect(ui->pushButton_2, &QPushButton::clicked, this, [=]() {
		if (ui->pushButton_2->text() == "打开串口")
		{
			ui->pushButton_2->setText("关闭串口");
			emit serialPort->connectSerialPort(ui->comboBox->currentText());
		}
		else if (ui->pushButton_2->text() == "关闭串口")
		{
			ui->pushButton_2->setText("打开串口");
			emit serialPort->closeSerialPort();
		}
		}, Qt::QueuedConnection);

	connect(ui->comboBox, pComboBox, this, [=]() {
		ui->pushButton_2->setText("打开串口");
		emit serialPort->closeSerialPort();
		}, Qt::QueuedConnection);

	connect(serialPort, &COM_Port::haveUpdateSerialPortData, this, [=](const QStringList* stringList) {
		if (stringList == nullptr)
			return;
		foreach (QString serialPortName, *stringList)
		{
			ui->comboBox->addItem(serialPortName);
		}
		ui->pushButton_2->setEnabled(true);
		}, Qt::QueuedConnection);

	connect(serialPort, &COM_Port::serialPortConditionVariation, this, [=](bool finish) {
		if (!finish)
		{
			return;
		}
		ui->pushButton_2->setText("关闭串口");
		}, Qt::QueuedConnection);

}

void GUI::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, ui->centralwidget->width(), ui->centralwidget->height()), *background);
	this->QMainWindow::paintEvent(event);
}

void GUI::resizeEvent(QResizeEvent* event)
{
	if (HalconCpp::HDevWindowStack::IsOpen())
	{
		HalconCpp::CloseWindow(HalconCpp::HDevWindowStack::Pop());
		HalconCpp::OpenWindow(0, 0, ui->label_3->width(), ui->label_3->height(), (Hlong)ui->label_3->winId(), "visible", "", &hv_WindowHandle);
		HalconCpp::OpenWindow(0, 0, ui->label_4->width(), ui->label_4->height(), (Hlong)ui->label_4->winId(), "visible", "", &hv_WindowHandle2);
		HalconCpp::HDevWindowStack::Push(hv_WindowHandle);
		HalconCpp::HDevWindowStack::Push(hv_WindowHandle2);
		HalconCpp::HDevWindowStack::SetActive(hv_WindowHandle);
	}

	this->QMainWindow::resizeEvent(event);
}

GUI::~GUI()
{
	if (p)
		delete p;
	if (serialPort)
		delete serialPort;
	if (halconVidicon)
		delete halconVidicon;
	if (background)
		delete background;
	if (ui)
		delete ui;
}
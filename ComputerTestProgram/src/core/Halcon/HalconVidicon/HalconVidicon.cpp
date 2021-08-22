#include "HalconVidicon.h"

HalconVidicon::HalconVidicon(QObject *parent)
	: QObject(parent)
{
}

HalconVidicon::~HalconVidicon()
{
	closeVidicon();
}

void HalconVidicon::connectVidicon()
{
	closeVidicon();
	HalconCpp::OpenFramegrabber("DirectShow", 1, 1, 0, 0, 0, 0, "default", 8, "rgb", -1, "false",
		"default", "[0] DroidCam Source 3", 0, -1, &hv_AcqHandle);
	GrabImageStart(hv_AcqHandle, -1);
	isOpen = true;
}

void HalconVidicon::useVidicon()
{
	if (isUsing || !isOpen)
	{
		return;
	}
	isUsing = true;
	HalconCpp::GrabImage(&ho_Image, hv_AcqHandle);
	emit havePicture(ho_Image);
	isUsing = false;
}

void HalconVidicon::closeVidicon()
{
	isOpen = false;
	HalconCpp::CloseFramegrabber(hv_AcqHandle);
}

void HalconVidicon::testPicture()
{
	// Local iconic variables
	HalconCpp::HObject  ho_GrayImage, ho_Region, ho_RegionTrans;
	HalconCpp::HObject  ho_ImageReduced, ho_ImageMean, ho_Region1, ho_RegionTrans1, ho_ImageReduced1;
	HalconCpp::HObject  ho_RegionDynThresh, ho_RegionFillUp, ho_ConnectedRegions;
	HalconCpp::HObject  ho_SelectedRegions, ho_SelectedRegions2, ho_ImageReduced2;

	HalconCpp::Rgb1ToGray(ho_Image, &ho_GrayImage);
	HalconCpp::Threshold(ho_GrayImage, &ho_Region, 10, 255);
	HalconCpp::ShapeTrans(ho_Region, &ho_RegionTrans, "rectangle1");
	HalconCpp::ReduceDomain(ho_GrayImage, ho_RegionTrans, &ho_ImageReduced);

	//动态域值分割
	//mean_image (ImageReduced, ImageMean, 20, 20)
	//dyn_threshold (ImageReduced, ImageMean, RegionDynThresh, 5, 'light')

	HalconCpp::Threshold(ho_ImageReduced, &ho_Region1, 10, 12);
	HalconCpp::ShapeTrans(ho_Region1, &ho_RegionTrans1, "rectangle1");
	HalconCpp::ReduceDomain(ho_ImageReduced, ho_RegionTrans1, &ho_ImageReduced1);
	
	//动态域值分割
	HalconCpp::MeanImage(ho_ImageReduced1, &ho_ImageMean, 20, 20);
	HalconCpp::DynThreshold(ho_ImageReduced1, ho_ImageMean, &ho_RegionDynThresh, 3, "light");
	
	HalconCpp::FillUp(ho_RegionDynThresh, &ho_RegionFillUp);
	HalconCpp::Connection(ho_RegionFillUp, &ho_ConnectedRegions);
	HalconCpp::OpeningRectangle1(ho_ConnectedRegions, &ho_SelectedRegions, 180, 180);
	SelectShape(ho_SelectedRegions, &ho_SelectedRegions2, "area", "and", 10000, 99999);
	HalconCpp::ReduceDomain(ho_ImageReduced1, ho_SelectedRegions2, &ho_ImageReduced2);

	HalconCpp::HTuple hNum;
	HalconCpp::CountObj(ho_ImageReduced2, &hNum);
	if (hNum != 0)
	{
		emit finishPicture(ho_ImageReduced2);
	}
}

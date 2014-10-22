// -*- C++ -*-
/*!
 * @file  CapturePresentation.cpp
 * @brief CapturePresentation Component
 * @date $Date$
 *
 * $Id$
 */

#include "CapturePresentation.h"
#include "ImageDataCom.h"

// Module specification
// <rtc-template block="module_spec">
static const char* capturepresentation_spec[] =
  {
    "implementation_id", "CapturePresentation",
    "type_name",         "CapturePresentation",
    "description",       "CapturePresentation Component",
    "version",           "0.1.0",
    "vendor",            "Miyamoto Nobuhiko",
    "category",          "TESTR",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    "conf.default.windowtype", "Desktop",
	"conf.default.scale", "2",
    "conf.__widget__.windowtype", "radio",
	"conf.__widget__.scale", "spin",
    "conf.__constraints__.windowtype", "(Desktop, ActiveWindow)",
	"conf.__constraints__.scale", "1<=x<=100",
	// Configuration variables
	"conf.default.string_encode", "off",
	"conf.default.int_encode_quality", "75",
    // Widget
	"conf.__widget__.string_encode", "radio",
	"conf.__widget__.int_encode_quality", "spin",
    // Constraints
	"conf.__constraints__.string_encode", "(off,jpeg,png)",
	"conf.__constraints__.int_encode_quality", "1<=x<=100",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
CapturePresentation::CapturePresentation(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_imageOut("image", m_image)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
CapturePresentation::~CapturePresentation()
{
}



RTC::ReturnCode_t CapturePresentation::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("image", m_imageOut);
  
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  bindParameter("windowtype", windowtype, "Desktop");
  bindParameter("scale", scale, "2");
  bindParameter("string_encode", m_string_encode, "off");
  bindParameter("int_encode_quality", m_int_encode_quality, "75");
  

  

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CapturePresentation::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CapturePresentation::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CapturePresentation::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

bool CapturePresentation::InitWindowCapture()
{
	if(windowtype == "Desktop")
		c_window = GetDesktopWindow();
	else
		c_window = GetForegroundWindow();

	RECT rect;

	GetWindowRect(c_window, &rect);

	width = rect.right;

	height = rect.bottom;

	

 

	

	iplimage = cvCreateImageHeader(cvSize(width, height), IPL_DEPTH_8U, 3);

	flipimage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);

	resizeimage = cvCreateImage(cvSize(width/scale, height/scale), IPL_DEPTH_8U, 3);

	

 

	

	BITMAPINFO bmpInfo;

	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	bmpInfo.bmiHeader.biWidth = width;

	bmpInfo.bmiHeader.biHeight = height;

	bmpInfo.bmiHeader.biPlanes = 1;

	bmpInfo.bmiHeader.biBitCount = 24;

	bmpInfo.bmiHeader.biCompression = BI_RGB;

	

 

	

	LPDWORD lpPixel;

	hDC = GetDC(c_window);

	hBitmap = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&lpPixel, NULL, 0);

	hMemDC = CreateCompatibleDC(hDC);

	SelectObject(hMemDC, hBitmap);

	ReleaseDC(c_window, hDC);

	


	

	iplimage->imageData = (char *)lpPixel;

	return true;
}


bool CapturePresentation::ExitWindowCapture()
{
	cvReleaseImageHeader(&iplimage);

	cvReleaseImage(&flipimage);
	
	cvReleaseImage(&resizeimage);



	

	

	DeleteDC(hMemDC);

	DeleteObject(hBitmap);
	
	return true;
}




RTC::ReturnCode_t CapturePresentation::onActivated(RTC::UniqueId ec_id)
{
	InitWindowCapture();



	
	
	
	
	
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CapturePresentation::onDeactivated(RTC::UniqueId ec_id)
{
	ExitWindowCapture();


	

  return RTC::RTC_OK;
}


RTC::ReturnCode_t CapturePresentation::onExecute(RTC::UniqueId ec_id)
{
	
	hDC = GetDC(c_window);
	BitBlt(hMemDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
	ReleaseDC(c_window,hDC);



	

	

	cvFlip(iplimage, flipimage);
	cvResize(flipimage, resizeimage);

	SetCameraImage(&m_image, resizeimage, m_string_encode, m_int_encode_quality);
	

	
	m_imageOut.write();
  
  


	

	
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CapturePresentation::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CapturePresentation::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CapturePresentation::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CapturePresentation::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CapturePresentation::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/




extern "C"
{
 
  void CapturePresentationInit(RTC::Manager* manager)
  {
    coil::Properties profile(capturepresentation_spec);
    manager->registerFactory(profile,
                             RTC::Create<CapturePresentation>,
                             RTC::Delete<CapturePresentation>);
  }
  
};



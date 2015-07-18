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
    "category",          "Presentation",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
	// Configuration variables
	"conf.default.string_encode", "off",
	"conf.default.int_encode_quality", "75",
	"conf.default.windowtype", "Desktop",
	"conf.default.scale", "2",
	"conf.default.path", "localhost\\CapturePresentation0",
	"conf.default.name", "Presentation1",
	"conf.default.screen_x", "0",
	"conf.default.screen_y", "0",
	"conf.default.screen_width", "1000",
	"conf.default.screen_height", "500",
	"conf.default.setRect", "0",
    // Widget
	"conf.__widget__.string_encode", "radio",
	"conf.__widget__.int_encode_quality", "spin",
	"conf.__widget__.windowtype", "radio",
	"conf.__widget__.scale", "slider",
	"conf.__widget__.screen_x", "text",
	"conf.__widget__.screen_y", "text",
	"conf.__widget__.screen_width", "text",
	"conf.__widget__.screen_height", "text",
	"conf.__widget__.setRect", "radio",
    // Constraints
	"conf.__constraints__.string_encode", "(off,jpeg,png)",
	"conf.__constraints__.int_encode_quality", "1<=x<=100",
	"conf.__constraints__.setRect", "(0,1)",
	"conf.__constraints__.windowtype", "(Desktop, ActiveWindow)",
	"conf.__constraints__.scale", "1<=x<=200",
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
    m_imageOut("image", m_image),
	m_DataBasePort("DataBase")
    // </rtc-template>
{
	iplimage = NULL;
	flipimage = NULL;
	resizeimage = NULL;
	roiimage = NULL;
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
  m_DataBasePort.registerConsumer("database", "DataBase::mDataBase", m_database);
  addPort(m_DataBasePort);
  
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  bindParameter("windowtype", windowtype, "Desktop");
  bindParameter("scale", scale, "2");
  bindParameter("string_encode", m_string_encode, "off");
  bindParameter("int_encode_quality", m_int_encode_quality, "75");
  bindParameter("path", m_path, "localhost\\CapturePresentation0");
  bindParameter("name", m_name, "Presentation1");
  bindParameter("screen_x", m_screen_x, "0");
  bindParameter("screen_y", m_screen_y, "0");
  bindParameter("screen_width", m_screen_width, "1000");
  bindParameter("screen_height", m_screen_height, "500");
  bindParameter("setRect", m_setRect, "0");

  

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

	//roiimage = cvCreateImage(cvSize(m_screen_width, m_screen_height), IPL_DEPTH_8U, 3);

 

	

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

	//cvReleaseImage(&roiimage);



	

	

	DeleteDC(hMemDC);

	DeleteObject(hBitmap);
	
	return true;
}




RTC::ReturnCode_t CapturePresentation::onActivated(RTC::UniqueId ec_id)
{
	InitWindowCapture();



	try
	{
		
		m_database._ptr()->setConnection("Presentation","","");
		
		std::string sql = "INSERT INTO LIST(NAME, PATH, CAMERAIMAGE) VALUES(";
		sql += "'";
		sql += m_name;
		sql += "', '";
		sql += m_path;
		sql += "', '";
		sql += "image";
		sql += "'";
		sql += ")";

		
		
		m_database._ptr()->executeUpdate("Presentation", sql.c_str());
		
	}
	catch(...)
	{

	}
	
	
	
	
	
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CapturePresentation::onDeactivated(RTC::UniqueId ec_id)
{
	ExitWindowCapture();


	try
	{
		
		std::string sql = "DELETE FROM LIST WHERE NAME = ";
		sql += "'";
		sql += m_name;
		sql += "'";
		
		m_database._ptr()->executeUpdate("Presentation", sql.c_str());
		
	}
	catch(...)
	{

	}

	

  return RTC::RTC_OK;
}


RTC::ReturnCode_t CapturePresentation::onExecute(RTC::UniqueId ec_id)
{
	
	hDC = GetDC(c_window);
	BitBlt(hMemDC, 0, 0, width, height, hDC, 0, 0, SRCCOPY);
	ReleaseDC(c_window,hDC);



	

	

	cvFlip(iplimage, flipimage);
	cvResize(flipimage, resizeimage);
	


	if(m_setRect == 1)
	{
		int t_x = m_screen_x;
		if(resizeimage->width < t_x)
			t_x = resizeimage->width;

		int t_y = m_screen_y;
		if(resizeimage->height < t_y)
			t_y = resizeimage->height;

		int t_width = m_screen_width;
		if(resizeimage->width < t_x+t_width)
			t_width = resizeimage->width - t_x;

		int t_height = m_screen_height;
		if(resizeimage->height < t_y+t_height)
			t_height = resizeimage->height - t_y;

		//cvSetImageROI(resizeimage, cvRect(m_screen_x, m_screen_y, m_screen_width, m_screen_height));
		//cvSetImageROI(resizeimage, cvRect(100, 150, 100, 100));
		
		//roiimage = cvCloneImage(resizeimage);
		
		cv::Mat bigImage = cv::cvarrToMat(resizeimage);
		cv::Mat smallImage = cv::Mat(bigImage, cv::Rect(t_x,t_y,t_width,t_height));
		IplImage m_iplImage = smallImage;
		roiimage = cvCreateImage(cvSize(t_width, t_height), IPL_DEPTH_8U, 3);
		cvCopy( &m_iplImage, roiimage);
		
		SetCameraImage(&m_image, roiimage, m_string_encode, m_int_encode_quality);
		
		cvReleaseImage(&roiimage);
		//cvResetImageROI(resizeimage);
	}
	else
	{
		SetCameraImage(&m_image, resizeimage, m_string_encode, m_int_encode_quality);
	}
	
	
	//std::cout << m_imageOut.getName() << std::endl;
	//m_imageOut.connectors()[0]->write(&m_image);
	//coil::TimeValue t1(coil::gettimeofday());
	
	m_imageOut.write();
	//coil::TimeValue t2(coil::gettimeofday());
	//std::cout << t2 - t1 << std::endl;
  
	
		

	

	
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



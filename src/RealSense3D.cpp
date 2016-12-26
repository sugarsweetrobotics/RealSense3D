// -*- C++ -*-
/*!
 * @file  RealSense3D.cpp
 * @brief Intel Real Sense 3D Component
 * @date $Date$
 *
 * @author Sugar Swee Robotics
 *
 * GPL
 *
 * $Id$
 */

#include "RealSense3D.h"

// Module specification
// <rtc-template block="module_spec">
static const char* realsense3d_spec[] =
  {
    "implementation_id", "RealSense3D",
    "type_name",         "RealSense3D",
    "description",       "Intel Real Sense 3D Component",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Sensor",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "1",
	"conf.default.width", "640",
	"conf.default.height", "480",
    // Widget
    "conf.__widget__.debug", "text",
	"conf.__widget__.width", "text",
	"conf.__widget__.height", "text",
	// Constraints

    "conf.__type__.debug", "int",
	"conf.__type__.width", "int",
	"conf.__type__.height", "int",

    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RealSense3D::RealSense3D(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rgbdCameraImageOut("rgbdCameraImage", m_rgbdCameraImage)

    // </rtc-template>
{
	m_PXCSenseManager = NULL;
}

/*!
 * @brief destructor
 */
RealSense3D::~RealSense3D()
{
}



RTC::ReturnCode_t RealSense3D::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  addOutPort("rgbdCameraImage", m_rgbdCameraImageOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "1");
  bindParameter("width", m_width, "640");
  bindParameter("height", m_height, "480");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RealSense3D::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealSense3D::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealSense3D::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RealSense3D::onActivated(RTC::UniqueId ec_id)
{
	// create the PXCSenseManager  
    // See https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/index.html?manuals_raw_stream_capture_and_process.html
	// 
	m_PXCSenseManager = PXCSenseManager::CreateInstance();
	if (!m_PXCSenseManager) {          
		std::cout << "[RealSense3D] Unable to create the PXCSenseManager" << std::endl;
		return RTC::RTC_ERROR;       
	}  


	// select the color stream of size 640x480 and depth stream of size 640x480  
	m_PXCSenseManager->EnableStream(PXCCapture::STREAM_TYPE_COLOR, m_width, m_height);
	m_PXCSenseManager->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, m_width, m_height); 
	

	// initialize the PXCSenseManager
	if( m_PXCSenseManager->Init() != PXC_STATUS_NO_ERROR) {
		std::cout << "[RealSense3D] Unable to Init the PXCSenseManager" << std::endl;
		return RTC::RTC_ERROR;
	} 

	m_rgbdCameraImage.data.cameraImage.image.format = Img::CF_RGB;
	m_rgbdCameraImage.data.cameraImage.image.width = m_width;
	m_rgbdCameraImage.data.cameraImage.image.height = m_height;
	m_rgbdCameraImage.data.cameraImage.image.raw_data.length(m_width*m_height * 3);

	m_rgbdCameraImage.data.depthImage.width = m_width;
	m_rgbdCameraImage.data.depthImage.height = m_height;
	m_rgbdCameraImage.data.depthImage.raw_data.length(m_width*m_height);

	std::cout << "[RealSense3D] Successfully Activated." << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealSense3D::onDeactivated(RTC::UniqueId ec_id)
{
	if (m_PXCSenseManager) {
		m_PXCSenseManager->Release();
	}
	m_PXCSenseManager = NULL;

	std::cout << "[RealSense3D] Successfully Deactivated." << std::endl;
	return RTC::RTC_OK;
}


RTC::ReturnCode_t RealSense3D::onExecute(RTC::UniqueId ec_id)
{

	PXCImage *colorIm, *depthIm;


	if (m_PXCSenseManager->AcquireFrame(true) < PXC_STATUS_NO_ERROR) {
		std::cout << "[RealSense3D] Unable AcquireFrame" << std::endl;
		return RTC::RTC_ERROR;
	}

	// retrieve all available image samples   
	PXCCapture::Sample *sample = m_PXCSenseManager->QuerySample(); 

	// retrieve the image or frame by type from the sample  
	colorIm = sample->color;  
	depthIm = sample->depth; 

	// Accessing Raw Data 
	// See https://software.intel.com/sites/landingpage/realsense/camera-sdk/v2016r3/documentation/html/index.html?doc_essential_image_data.html

	Intel::RealSense::ImageData imdata;
	colorIm->AcquireAccess(Intel::RealSense::ImageAccess::ACCESS_READ, Intel::RealSense::PixelFormat::PIXEL_FORMAT_RGB24, &imdata);
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			int index = i * m_width + j * 3;
			m_rgbdCameraImage.data.cameraImage.image.raw_data[index + 0] = (imdata.planes[0])[index + 2];
			m_rgbdCameraImage.data.cameraImage.image.raw_data[index + 1] = (imdata.planes[0])[index + 1];
			m_rgbdCameraImage.data.cameraImage.image.raw_data[index + 2] = (imdata.planes[0])[index + 0];
		}
	}
	// Color Format must be BGR, so the code below does not work.
	// memcpy(&(m_rgbdCameraImage.data.cameraImage.image.raw_data[0]), imdata.planes, m_width*m_height * 3 * sizeof(uint8_t));
	colorIm->ReleaseAccess(&imdata);

	Intel::RealSense::ImageData ddata;
	depthIm->AcquireAccess(Intel::RealSense::ImageAccess::ACCESS_READ, Intel::RealSense::PixelFormat::PIXEL_FORMAT_DEPTH, &ddata);
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			int index = i * m_width + j;
			m_rgbdCameraImage.data.depthImage.raw_data[index] = ((uint16_t*)imdata.planes[0])[index];
		}
	}
	//memcpy(&(m_rgbdCameraImage.data.cameraImage.image.raw_data[0]), ddata.planes, m_width*m_height * 3 * sizeof(uint8_t));
	depthIm->ReleaseAccess(&ddata);

	// release or unlock the current frame to fetch the next frame
	m_PXCSenseManager->ReleaseFrame(); 

	::setTimestamp<RGBDCamera::TimedRGBDCameraImage>(m_rgbdCameraImage);
	m_rgbdCameraImageOut.write();
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RealSense3D::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealSense3D::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealSense3D::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealSense3D::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RealSense3D::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RealSense3DInit(RTC::Manager* manager)
  {
    coil::Properties profile(realsense3d_spec);
    manager->registerFactory(profile,
                             RTC::Create<RealSense3D>,
                             RTC::Delete<RealSense3D>);
  }
  
};



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
#include <sstream>
// Module specification
// <rtc-template block="module_spec">
static const char* realsense3d_spec[] = {
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
  "conf.default.depthWidth", "480",
  "conf.default.depthHeight", "360",
  "conf.default.deviceID", "0",
  "conf.default.frameRate", "60",
  // Widget
  "conf.__widget__.debug", "text",
  "conf.__widget__.width", "text",
  "conf.__widget__.height", "text",
  // Constraints
  "conf.__type__.debug", "int",
  "conf.__type__.width", "int",
  "conf.__type__.height", "int",
  "conf.__type__.depthWidth", "int",
  "conf.__type__.depthHeight", "int",
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
  bindParameter("depthWidth", m_depthWidth, "480");
  bindParameter("depthHeight", m_depthHeight, "360");
  bindParameter("deviceID", m_deviceID, "0");
  bindParameter("frameRate", m_frameRate, "60");
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

  try {
    m_pContext = new rs::context();
    if (m_pContext->get_device_count() == 0) {
      RTC_ERROR(("No RealSense Device detected"));
      std::cout << "No RealSense Device detected." << std::endl;
      return RTC::RTC_ERROR;
    }
  } catch ( std::exception& ex) {
    RTC_ERROR(("Exception: when RealSense Context Created."));
    std::stringstream ss;
    ss << "Exception: " << ex.what();
    RTC_ERROR((ss.str().c_str()));
    std::cout << "Exception: when RealSense Context Created." << std::endl;
    std::cout << "Exception: " << ex.what() << std::endl;
    return RTC::RTC_ERROR;
  }

  //  
  // rs2::context ctx;
  // auto list = ctx.query_devices(); // Get a snapshot of currently connected devices
  // if(list.size() == 0)
  //   throw std::runtime_error("No device detected. Is it plugged in?");
  // rs2::device dev = list.front();

	// create the PXCSenseManager  
    // See https://software.intel.com/sites/landingpage/realsense/camera-sdk/v1.1/documentation/html/index.html?manuals_raw_stream_capture_and_process.html
	// 
	//m_PXCSenseManager = PXCSenseManager::CreateInstance();

  RTC_DEBUG(("RealSense Device Preparing...."));
  rs::device* dev = NULL;
  try {
    dev = m_pContext->get_device(m_deviceID);
    dev->enable_stream(rs::stream::color, m_width, m_height, rs::format::rgb8, m_frameRate, rs::output_buffer_format::continous);
    dev->enable_stream(rs::stream::depth, m_depthWidth, m_depthHeight, rs::format::z16, m_frameRate, rs::output_buffer_format::continous);
    //dev->enable_stream(rs::stream::color, rs::preset::best_quality);
    //dev->enable_stream(rs::stream::depth, rs::preset::best_quality);
    dev->start();
  } catch (std::exception &ex) {
    RTC_ERROR(("Exception: when RealSense Device Preparing."));
    std::stringstream ss;
    ss << "Exception: " << ex.what();
    RTC_ERROR((ss.str().c_str()));
    std::cout << "Exception: when RealSense Device Preparing." << std::endl;
    std::cout << "Exception: " << ex.what() << std::endl;
    return RTC::RTC_ERROR;


  }
  RTC_DEBUG(("RealSense pipe.start() is called"));
  
	// if (!m_PXCSenseManager) {          
	// 	std::cout << "[RealSense3D] Unable to create the PXCSenseManager" << std::endl;
	// 	return RTC::RTC_ERROR;       
	// }  


	// // select the color stream of size 640x480 and depth stream of size 640x480  
	// pipe ->EnableStream(PXCCapture::STREAM_TYPE_COLOR, m_width, m_height);
	// m_PXCSenseManager->EnableStream(PXCCapture::STREAM_TYPE_DEPTH, m_depthWidth, m_depthHeight); 
	

	// // initialize the PXCSenseManager
	// if( m_PXCSenseManager->Init() != PXC_STATUS_NO_ERROR) {
	// 	std::cout << "[RealSense3D] Unable to Init the PXCSenseManager" << std::endl;
	// 	return RTC::RTC_ERROR;
	// } 

  auto color_intrinsic = dev->get_stream_intrinsics(rs::stream::color);
  std::cout << "Color Stream:" << std::endl;
  std::cout << "  FORMAT   : " << dev->get_stream_format(rs::stream::color) << std::endl;
  std::cout << "  FRAMERATE: " << dev->get_stream_framerate(rs::stream::color) << std::endl;
  std::cout << "  WIDTH    : " << dev->get_stream_width(rs::stream::color) << std::endl;
  std::cout << "  HEIGHT   : " << dev->get_stream_height(rs::stream::color) << std::endl;
  std::cout << "  H-FOV    : " << color_intrinsic.hfov() << std::endl;
  std::cout << "  V-FOV    : " << color_intrinsic.vfov() << std::endl;


	m_rgbdCameraImage.data.cameraImage.image.format = Img::CF_RGB;
	m_rgbdCameraImage.data.cameraImage.image.width = m_width;
	m_rgbdCameraImage.data.cameraImage.image.height = m_height;
	m_rgbdCameraImage.data.cameraImage.image.raw_data.length(m_width*m_height * 3);
	m_rgbdCameraImage.data.cameraImage.intrinsic.distortion_coefficient.length(1);
	m_rgbdCameraImage.data.cameraImage.intrinsic.distortion_coefficient[0] = 0;
	m_rgbdCameraImage.data.cameraImage.intrinsic.matrix_element[0] = 0;
	m_rgbdCameraImage.data.cameraImage.intrinsic.matrix_element[1] = 0;
	m_rgbdCameraImage.data.cameraImage.intrinsic.matrix_element[2] = 0;
	m_rgbdCameraImage.data.cameraImage.intrinsic.matrix_element[3] = 0;
	m_rgbdCameraImage.data.cameraImage.intrinsic.matrix_element[4] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[0][0] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[0][1] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[0][2] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[0][3] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[1][0] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[1][1] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[1][2] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[1][3] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[2][0] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[2][1] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[2][2] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[2][3] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[3][0] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[3][1] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[3][2] = 0;
	m_rgbdCameraImage.data.cameraImage.extrinsic[3][3] = 0;
	m_rgbdCameraImage.data.cameraImage.captured_time.sec = 0;
	m_rgbdCameraImage.data.cameraImage.captured_time.nsec = 0;

    auto depth_intrinsic = dev->get_stream_intrinsics(rs::stream::depth);
    std::cout << "Depth Stream:" << std::endl;
    std::cout << "  FORMAT   : " << dev->get_stream_format(rs::stream::depth) << std::endl;
    std::cout << "  FRAMERATE: " << dev->get_stream_framerate(rs::stream::depth) << std::endl;
    std::cout << "  WIDTH    : " << dev->get_stream_width(rs::stream::depth) << std::endl;
    std::cout << "  HEIGHT   : " << dev->get_stream_height(rs::stream::depth) << std::endl;
    std::cout << "  H-FOV    : " << depth_intrinsic.hfov() << std::endl;
    std::cout << "  V-FOV    : " << depth_intrinsic.vfov() << std::endl;
      
	m_rgbdCameraImage.data.depthImage.width = m_depthWidth;
	m_rgbdCameraImage.data.depthImage.height = m_depthHeight;
	m_rgbdCameraImage.data.depthImage.verticalFOV = depth_intrinsic.vfov();
	m_rgbdCameraImage.data.depthImage.horizontalFOV = depth_intrinsic.hfov();
	m_rgbdCameraImage.data.depthImage.raw_data.length(m_depthWidth*m_depthHeight);


    
	m_rgbdCameraImage.data.geometry.pose.position.x = 0;
	m_rgbdCameraImage.data.geometry.pose.position.y = 0;
	m_rgbdCameraImage.data.geometry.pose.position.z = 0;
	m_rgbdCameraImage.data.geometry.pose.orientation.p = 0;
	m_rgbdCameraImage.data.geometry.pose.orientation.r = 0;
	m_rgbdCameraImage.data.geometry.pose.orientation.y = 0;
	std::cout << "[RealSense3D] Successfully Activated." << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealSense3D::onDeactivated(RTC::UniqueId ec_id)
{
  try {
    rs::device* dev = m_pContext->get_device(m_deviceID);
    dev->stop();
    delete m_pContext;
  } catch (std::exception &ex) {
    RTC_ERROR(("Exception: when RealSense Context Released."));
    std::stringstream ss;
    ss << "Exception: " << ex.what();
    RTC_ERROR((ss.str().c_str()));
    std::cout << "Exception: when RealSense Context Released." << std::endl;
    std::cout << "Exception: " << ex.what() << std::endl;
    return RTC::RTC_ERROR;


  }

  std::cout << "[RealSense3D] Successfully Deactivated." << std::endl;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealSense3D::onExecute(RTC::UniqueId ec_id)
{
  rs::device* dev = m_pContext->get_device(m_deviceID);
  
  dev->wait_for_frames();
  const uint8_t *color_frame = static_cast<const uint8_t*>(dev->get_frame_data(rs::stream::color));
  const uint16_t *depth_frame = static_cast<const uint16_t*>(dev->get_frame_data(rs::stream::depth));
        
  if (color_frame && depth_frame) {
    for (int i = 0; i < m_height; i++) {
      for (int j = 0; j < m_width; j++) {
        int index = (i * m_width + j) * 3;
        m_rgbdCameraImage.data.cameraImage.image.raw_data[index + 0] = (color_frame[index + 0]);        
        m_rgbdCameraImage.data.cameraImage.image.raw_data[index + 1] = (color_frame[index + 1]);
        m_rgbdCameraImage.data.cameraImage.image.raw_data[index + 2] = (color_frame[index + 2]);
      }
    }
  }

  float scale = dev->get_depth_scale();
  for(int i = 0;i < m_depthHeight;i++) {
    for(int j = 0;j < m_depthWidth;j++) {
      int index = (i * m_depthWidth + j);
      m_rgbdCameraImage.data.depthImage.raw_data[index] = depth_frame[index] * scale;
    }
  }        

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



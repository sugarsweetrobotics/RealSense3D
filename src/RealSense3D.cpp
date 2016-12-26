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

    // Widget
    "conf.__widget__.debug", "text",
    // Constraints

    "conf.__type__.debug", "int",

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
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealSense3D::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RealSense3D::onExecute(RTC::UniqueId ec_id)
{
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



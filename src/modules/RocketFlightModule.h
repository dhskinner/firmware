#pragma once
#ifndef _ROCKETFLIGHT_MODULE_H_
#define _ROCKETFLIGHT_MODULE_H_

#include "../configuration.h"

#if !MESHTASTIC_EXCLUDE_GPS && defined(ROCKETFLIGHT_EDITION)

#include <cmath>
#include "../detect/ScanI2C.h"
#include "../gps/RTC.h"
#include "../mesh/MeshService.h"
#include "../mesh/Throttle.h"
#include "../mesh/TypeConversions.h"
#include "../modules/PositionModule.h"
#include "../modules/Telemetry/Sensor/BMP3XXSensor.h"
#include "../motion/ICM20948Sensor.h"
#include "../sleep.h"

// From main.cpp
extern std::pair<uint8_t, TwoWire *> nodeTelemetrySensorsMap[];
extern ScanI2C::DeviceAddress accelerometer_found;
extern ScanI2C::DeviceAddress barometer_found;

// From MeshService.cpp
extern MeshService *service;

// RocketFlightModule replaces PositionModule to perform asynchronous (low rate) tasks
class RocketFlightModule : public PositionModule
{
protected:
  // Altimeter
  BMP3XXSingleton *altimeter = nullptr;

  // Store the latest sent as a position message
  int32_t lastBaroAltitude = 0;

  // Milliseconds between successive position broadcasts, when using smart position mode
  uint32_t maximumTimeThreshold =
    Default::getConfiguredOrDefaultMs(config.position.position_broadcast_secs, 60)*1000;

public:

  // Constructor
  RocketFlightModule();

  // Initialise the sensor
  bool init();

  // Log position, and then send it into the mesh
  virtual void handleNewPosition() override;

protected:

  // Prepare a position report to send - same functionality as PositionModule
  // except using barometric altitude AMSL (GPS altitude is only used as a fallback)
  virtual meshtastic_MeshPacket *allocReply() override;

  // Does our periodic broadcast
  virtual int32_t runOnce() override;

  // Helper to tell if a sensor is present
  virtual bool hasSensor(_meshtastic_TelemetrySensorType sensorType);

  // Helper to calculate if distance and altitude travelled exceeds the 'smart' position threshold
  virtual bool hasMoved();

  // Helper to put the unit inti deep sleep if needed
  virtual void deepSleep();

  // Read barometric altitude if it is available (metres AMSL)
  int32_t getBaroAltitude();
};

#endif

#endif
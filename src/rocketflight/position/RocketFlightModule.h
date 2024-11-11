#pragma once
#ifndef _ROCKETFLIGHT_POSITION_H_
#define _ROCKETFLIGHT_POSITION_H_

#include "../configuration.h"

#ifdef ROCKETFLIGHT_POSITION

#include <cmath>
#include <vector>
#include <limits>
#include "Channels.h"
#include "graphics/ScreenFonts.h"
#include "graphics/images.h"
#include "RTC.h"
#include "MeshService.h"
#include "TypeConversions.h"
#include "modules/PositionModule.h"
#include "sleep.h"
#include "../motion/ICM20948Sensor.h"

#if !MESHTASTIC_EXCLUDE_GPS
#include "GPS.h"
#endif

// From main.cpp
extern std::pair<uint8_t, TwoWire *> nodeTelemetrySensorsMap[];
extern ScanI2C::FoundDevice accelerometer_found;
extern ScanI2C::FoundDevice altimeter_found;
extern meshtastic::PowerStatus *powerStatus;
extern meshtastic::GPSStatus *gpsStatus;
extern meshtastic::NodeStatus *nodeStatus;

// From MeshService.cpp
extern MeshService *service;

// From EnvironmentTelemetry.cpp
#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR
#include "../modules/Telemetry/Sensor/Altimeter.h"
#include "../modules/Telemetry/Sensor/BMP085Sensor.h"
#include "../modules/Telemetry/Sensor/BMP280Sensor.h"
#include "../modules/Telemetry/Sensor/BME280Sensor.h"
#include "../modules/Telemetry/Sensor/BME680Sensor.h"
#include "../modules/Telemetry/Sensor/LPS22HBSensor.h"
#include "../modules/Telemetry/Sensor/BMP3XXSensor.h"
extern BMP085Sensor bmp085Sensor;
extern BMP280Sensor bmp280Sensor;
extern BME280Sensor bme280Sensor;
extern BME680Sensor bme680Sensor;
extern BMP3XXSensor bmp3xxSensor;
extern LPS22HBSensor lps22hbSensor;
#endif

// RocketFlightModule replaces PositionModule to perform asynchronous (low rate) tasks
class RocketFlightModule : public PositionModule
{
protected:

  // Altimeter
  Altimeter *altimeter = nullptr;

  // Store the last altitude sent as a position message
  int32_t lastAltitudeSent = 0;
  
  // double currentAltitude = INVALID_ALTITUDE;
  // meshtastic_Position_AltSource currentAltitudeSource =  meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_UNSET;

  // Milliseconds between successive position broadcasts, when using smart position mode
  uint32_t maximumTimeThreshold =
      Default::getConfiguredOrDefaultMs(config.position.position_broadcast_secs, 60) * 1000;

  // Protects internal state.
  concurrency::Lock lock;

public:
  // Constructor
  RocketFlightModule();

  // Initialise the sensor
  bool init();

  // Log position, and then send it into the mesh
  virtual void handleNewPosition() override;

  virtual bool wantUIFrame() override;
#if !HAS_SCREEN
  void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
#else
  virtual void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y) override;
#endif

  // Get the reference for ground level, from when the unit was first initialised
  double getReferenceAltitude();

  // Get the current altitude, based on the reference altitude above
  double getCurrentAltitude();

protected:
  // Internal heartbeat status
  bool heartbeat = false;

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

  // Draw power bars or a charging indicator on an image of a battery, determined by battery charge voltage or percentage.
  static void drawBattery(OLEDDisplay *display, int16_t x, int16_t y, const meshtastic::PowerStatus *powerStatus);

  // Draw node status
  static void drawNodes(OLEDDisplay *display, int16_t x, int16_t y, const meshtastic::NodeStatus *nodeStatus);

  // Draw aligned text
  static void drawText(OLEDDisplay *display, int16_t x, int16_t y, OLEDDISPLAY_TEXT_ALIGNMENT textAlignment, const String &text);

#if HAS_GPS

  // GeoCoord object for the screen
  static GeoCoord geoCoord;

  // Draw status when GPS is disabled or not present
  static void drawGPS(OLEDDisplay *display, int16_t x, int16_t y, const meshtastic::GPSStatus *gps);

  // Draw the current altitude
  static void drawAltitude(OLEDDisplay *display, int16_t x, int16_t y, double altitude);

#endif
};

#endif

#endif
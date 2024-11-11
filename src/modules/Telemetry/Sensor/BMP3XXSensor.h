#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#ifndef _BMP3XX_SENSOR_H
#define _BMP3XX_SENSOR_H

#include "TelemetrySensor.h"
#include "Altimeter.h"
#include <Adafruit_BMP3XX.h>
#include <typeinfo>

extern std::pair<uint8_t, TwoWire *> nodeTelemetrySensorsMap[];

// Singleton wrapper for the Adafruit_BMP3XX class
class BMP3XXSingleton : public Adafruit_BMP3XX
{
private:
  static BMP3XXSingleton *pinstance;

protected:
  BMP3XXSingleton();
  ~BMP3XXSingleton();
  bool status = false;

public:
  // Create a singleton instance (not thread safe)
  static BMP3XXSingleton *GetInstance();

  // Singletons should not be cloneable.
  BMP3XXSingleton(BMP3XXSingleton &other) = delete;

  // Singletons should not be assignable.
  void operator=(const BMP3XXSingleton &) = delete;

  // Initialise with common settings for an altimeter/barometer
  bool init();

  // Performs a full reading of all sensors in the BMP3XX. Assigns
  // the internal temperature, pressure and altitudeAmsl variables
  bool performReading();

  // Return true if the sensor is ok
  inline bool isValid() { return status && altitudeAmslMetres != INVALID_ALTITUDE; };

  // Altitude in metres above mean sea level, assigned after calling performReading()
  double altitudeAmslMetres = INVALID_ALTITUDE;
};

class BMP3XXSensor : public TelemetrySensor, public Altimeter
{
protected:
  BMP3XXSingleton *bmp3xx = nullptr;
  virtual void setup() override;

public:
  BMP3XXSensor();
  ~BMP3XXSensor();
  int32_t runOnce() override;
  bool getMetrics(meshtastic_Telemetry *measurement) override;
  double getAltitude() override;
  bool isValid() override;
};

#endif

#endif
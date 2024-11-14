#pragma once
#ifndef _ROCKETFLIGHT_POSITION_
#define _ROCKETFLIGHT_POSITION_

#include "configuration.h"
#if !MESHTASTIC_EXCLUDE_GPS && defined(ROCKETFLIGHT_POSITION)

#include "GPS.h"
#include "RTC.h"
#include "meshUtils.h"
#include "modules/Telemetry/Sensor/Altimeter.h"

/**
 * A gps class that reads position and keeps the gps powered up (does not sleep)
 *
 * When new data is available it will notify observers.
 */
class RocketFlightGPS : private GPS, public Altimeter
{
  public:
    RocketFlightGPS();
    static GPS *createGps();
    void toggleGpsMode();
    using GPS::enable;
    using GPS::p;

    // Return altitude in metres above MSL, or INVALID_ALTITUDE if unavailable
    double getAltitude() override;

    // Return true if a valid altitude is available
    bool isValid() override;

    // Handle conversion from base class to Altimeter
    inline Altimeter *asAltimeter() override { return this; }

  private:
    virtual int32_t runOnce() override;
    unsigned long lastReceivedPositionMillis = 0;
};

extern GPS *gps;

#endif // Exclude GPS

#endif
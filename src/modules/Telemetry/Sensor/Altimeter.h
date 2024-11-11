#pragma once
#ifndef _ALTIMETER_H_
#define _ALTIMETER_H_

#include "configuration.h"

#include <cmath>
#include <limits>
#include <Wire.h>
#include "meshtastic/localonly.pb.h"
#include "meshtastic/telemetry.pb.h"
#include "detect/ScanI2C.h"

#define SEA_LEVEL_HPA 1013.2
#define INVALID_ALTITUDE std::numeric_limits<double>::min()

extern ScanI2C::FoundDevice altimeter_found;
extern meshtastic_LocalConfig config;

// Base class for altimeters
class Altimeter
{
public:
    explicit Altimeter();
    explicit Altimeter(meshtastic_TelemetrySensorType sensor, const char *sensorName, meshtastic_Position_AltSource source, double alt = INVALID_ALTITUDE);

    // Return true if a valid altitude reading is available
    virtual bool isValid();

    // Read altitude in metres above MSL (returns INVALID_ALTITUDE if no altimeter is present or a reading is unavailable)
    virtual double getAltitude();

    // Calculate and return the reference altitude, based on when the unit weas first initialised
    virtual double getReferenceAltitude();

    // Ger the source for altitude measurements
    virtual meshtastic_Position_AltSource getAltitudeSource();

    // Convert pressure in hectopascals to altitude in metres
    static double hectopascalsToMetres(double pressureHpa, double qfe = SEA_LEVEL_HPA);

    // Return the sensor name
    virtual inline const char * name() { return label; };

protected:
    const char *label;
    double referenceAltitude = INVALID_ALTITUDE;
    double manualAltitude = INVALID_ALTITUDE;
    std::vector<double> referenceAltitudeSamples;
    unsigned long referenceAltitudeNextSample = 0;
    meshtastic_TelemetrySensorType sensor = meshtastic_TelemetrySensorType_SENSOR_UNSET;
    meshtastic_Position_AltSource altitudeSource = meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_UNSET;
};

#endif


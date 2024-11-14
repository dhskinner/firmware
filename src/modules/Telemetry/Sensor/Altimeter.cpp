#include "Altimeter.h"

Altimeter::Altimeter() {}

Altimeter::Altimeter(meshtastic_TelemetrySensorType type, const char *name, meshtastic_Position_AltSource source, double alt)
{
    sensor = type;
    label = name;
    altitudeSource = source;
    manualAltitude = alt;
}

double Altimeter::getAltitude()
{
    // this should be overridden in child class
    return manualAltitude;
};

bool Altimeter::isValid()
{
    // this should be overridden in child class
    return (getAltitude() != INVALID_ALTITUDE);
}

double Altimeter::hectopascalsToMetres(double pressureHpa, double qfe)
{
    return 44330.0 * (1.0 - pow(pressureHpa / qfe, 0.1903));
};

meshtastic_Position_AltSource Altimeter::getAltitudeSource()
{
    return altitudeSource;
}

double Altimeter::getReferenceAltitude()
{
    // reference altitude is only set once, based on the first n altitude readings after startup
    if (referenceAltitude != INVALID_ALTITUDE)
        return referenceAltitude;

    // if config is a fixed position then don't bother
    if (config.position.fixed_position) {
        referenceAltitude = 0.0;
        LOG_INFO("Altimeter reference is fixed");
        return referenceAltitude;
    }

    double alt = getAltitude();
    if (millis() > referenceAltitudeNextSample) {

        // first take n samples a few milliseconds apart to remove noisy readings from the altitude sensor
        referenceAltitudeNextSample = millis() + ROCKETFLIGHT_REFERENCE_ALTITUDE_SAMPLE_MILLIS;
        referenceAltitudeSamples.push_back(alt);

        // when enough samples are available, set the reference altitude
        if (referenceAltitudeSamples.size() >= ROCKETFLIGHT_REFERENCE_ALTITUDE_SAMPLE_SIZE) {

            // sort by value
            std::sort(referenceAltitudeSamples.begin(), referenceAltitudeSamples.end());

            // use the median as the reference altitude
            referenceAltitude = referenceAltitudeSamples[static_cast<int>(referenceAltitudeSamples.size() / 2)];
            LOG_INFO("Altimeter reference altitude set to %.1f", referenceAltitude);

            // release memory from the samples vector
            std::vector<double>().swap(referenceAltitudeSamples);
        }
    }

    // return the reference altitude
    return referenceAltitude == INVALID_ALTITUDE ? 0.0 : referenceAltitude;
}
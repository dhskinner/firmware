#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "BMP3XXSensor.h"

BMP3XXSensor::BMP3XXSensor()
    : TelemetrySensor(meshtastic_TelemetrySensorType_BMP3XX, "BMP3XX"),
    Altimeter(meshtastic_TelemetrySensorType_BMP3XX, "BMP3XX", meshtastic_Position_AltSource ::meshtastic_Position_AltSource_ALT_BAROMETRIC){};

BMP3XXSensor::~BMP3XXSensor() {};

void BMP3XXSensor::setup() {};

int32_t BMP3XXSensor::runOnce()
{
    LOG_INFO("Init sensor: %s", sensorName);
    if (!hasSensor())
        return DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS;

    // Get a singleton instance and initialise the bmp3xx
    if (bmp3xx == nullptr)
        bmp3xx = BMP3XXSingleton::GetInstance();
    status = bmp3xx->init();

    return initI2CSensor();
}

bool BMP3XXSensor::isValid()
{
        if (bmp3xx == nullptr)
        return false;

        return bmp3xx->isValid();
}

bool BMP3XXSensor::getMetrics(meshtastic_Telemetry *measurement)
{
    if (bmp3xx == nullptr)
        return false;

    if ((int)measurement->which_variant == meshtastic_Telemetry_environment_metrics_tag)
    {
        if (!bmp3xx->performReading())
            return false;

        measurement->variant.environment_metrics.has_temperature = true;
        measurement->variant.environment_metrics.has_barometric_pressure = true;
        measurement->variant.environment_metrics.has_relative_humidity = false;

        measurement->variant.environment_metrics.temperature = static_cast<float>(bmp3xx->temperature);
        measurement->variant.environment_metrics.barometric_pressure = static_cast<float>(bmp3xx->pressure) / 100.0F;
        measurement->variant.environment_metrics.relative_humidity = 0.0f;

        LOG_DEBUG("BMP3XXSensor::getMetrics id: %i temp: %.1f press %.1f", measurement->which_variant,
                  measurement->variant.environment_metrics.temperature,
                  measurement->variant.environment_metrics.barometric_pressure);
    }
    else
    {
        LOG_DEBUG("BMP3XXSensor::getMetrics id: %i", measurement->which_variant);
    }
    return true;
}

double BMP3XXSensor::getAltitude()
{
    if (bmp3xx == nullptr)
        return INVALID_ALTITUDE;

    if (!bmp3xx->performReading())
        return INVALID_ALTITUDE;

    return bmp3xx->altitudeAmslMetres;
}

// Get a singleton wrapper for an Adafruit_bmp3xx
BMP3XXSingleton *BMP3XXSingleton::GetInstance()
{
    if (pinstance == nullptr)
        pinstance = new BMP3XXSingleton();

    return pinstance;
}

BMP3XXSingleton::BMP3XXSingleton() {};

BMP3XXSingleton::~BMP3XXSingleton() {};

BMP3XXSingleton *BMP3XXSingleton::pinstance{nullptr};

bool BMP3XXSingleton::init()
{
    // is there a sensor present?
    auto sensor = nodeTelemetrySensorsMap[meshtastic_TelemetrySensorType_BMP3XX];
    if (sensor.first == 0)
        return false;

    // initialise i2c comms
    status = Adafruit_BMP3XX::begin_I2C(sensor.first, sensor.second);
    if (!status)
        return false;

    // set up oversampling and filter initialization
    Adafruit_BMP3XX::setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
    Adafruit_BMP3XX::setPressureOversampling(BMP3_OVERSAMPLING_8X);
    Adafruit_BMP3XX::setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
    Adafruit_BMP3XX::setOutputDataRate(BMP3_ODR_25_HZ);

    // take a couple of initial readings to settle the sensor filters
    for (int i = 0; i < 3; i++)
    {
        if (!Adafruit_BMP3XX::performReading())
            return false;
    }
    return true;
}

bool BMP3XXSingleton::performReading()
{
    if (Adafruit_BMP3XX::performReading())
    {
        altitudeAmslMetres = 44330.0 * (1.0 - pow((this->pressure / 100.0) / SEA_LEVEL_HPA, 0.1903));
        return true;
    }
    altitudeAmslMetres = INVALID_ALTITUDE;
    return false;
}

#endif
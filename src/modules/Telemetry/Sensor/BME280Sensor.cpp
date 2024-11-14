#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "BME280Sensor.h"
#include "TelemetrySensor.h"
#include <Adafruit_BME280.h>
#include <typeinfo>

BME280Sensor::BME280Sensor()
    : TelemetrySensor(meshtastic_TelemetrySensorType_BME280, "BME280"),
      Altimeter(meshtastic_TelemetrySensorType_BME280, "BME280",
                meshtastic_Position_AltSource ::meshtastic_Position_AltSource_ALT_BAROMETRIC)
{
}

BME280Sensor::~BME280Sensor(){};

int32_t BME280Sensor::runOnce()
{
    LOG_INFO("Init sensor: %s", sensorName);
    if (!hasSensor()) {
        return DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS;
    }
    status = bme280.begin(nodeTelemetrySensorsMap[sensorType].first, nodeTelemetrySensorsMap[sensorType].second);

    bme280.setSampling(Adafruit_BME280::MODE_FORCED,
                       Adafruit_BME280::SAMPLING_X1, // Temp. oversampling
                       Adafruit_BME280::SAMPLING_X1, // Pressure oversampling
                       Adafruit_BME280::SAMPLING_X1, // Humidity oversampling
                       Adafruit_BME280::FILTER_OFF, Adafruit_BME280::STANDBY_MS_1000);

    return initI2CSensor();
}

void BME280Sensor::setup() {}

bool BME280Sensor::getMetrics(meshtastic_Telemetry *measurement)
{
    measurement->variant.environment_metrics.has_temperature = true;
    measurement->variant.environment_metrics.has_relative_humidity = true;
    measurement->variant.environment_metrics.has_barometric_pressure = true;

    LOG_DEBUG("BME280 getMetrics");
    bme280.takeForcedMeasurement();
    measurement->variant.environment_metrics.temperature = bme280.readTemperature();
    measurement->variant.environment_metrics.relative_humidity = bme280.readHumidity();
    measurement->variant.environment_metrics.barometric_pressure = bme280.readPressure() / 100.0F;

    return true;
}

double BME280Sensor::getAltitude()
{
    if (!isInitialized())
        return INVALID_ALTITUDE;

    if (!bme280.takeForcedMeasurement())
        return INVALID_ALTITUDE;

    return Altimeter::hectopascalsToMetres(bme280.readPressure() / 100.0F);
}
#endif
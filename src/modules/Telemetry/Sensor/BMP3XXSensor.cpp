#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "BMP3XXSensor.h"

BMP3XXSensor::BMP3XXSensor()
    : TelemetrySensor(meshtastic_TelemetrySensorType_BMP3XX, "BMP3XX"),
      Altimeter(meshtastic_TelemetrySensorType_BMP3XX, "BMP3XX",
                meshtastic_Position_AltSource ::meshtastic_Position_AltSource_ALT_BAROMETRIC){};

BMP3XXSensor::~BMP3XXSensor(){};

void BMP3XXSensor::setup(){};

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

    if ((int)measurement->which_variant == meshtastic_Telemetry_environment_metrics_tag) {
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
    } else {
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

// --------------------------------------------------------------------------------

// Get a singleton wrapper for a BMP388 sensor
BMP3XXSingleton *BMP3XXSingleton::GetInstance()
{
    if (pinstance == nullptr)
        pinstance = new BMP3XXSingleton();

    return pinstance;
}

BMP3XXSingleton *BMP3XXSingleton::pinstance{nullptr};

#if defined(BMP3XX_INTERRUPT_PIN) && defined(ARCH_ESP32)

// --------------------------------------------------------------------------------
// Use interrupts from the DFRobot library

BMP3XXSingleton::BMP3XXSingleton() : DFRobot_BMP388_I2C(&Wire, eSDOPinMode_t::eSDOGND){};

BMP3XXSingleton::~BMP3XXSingleton(){};

// Flag when an interrupt has been detected
volatile static bool BMP3XX_IRQ = false;

// Interrupt service routine
void BMP3XXSetInterrupt()
{
    BMP3XX_IRQ = true;
}

bool BMP3XXSingleton::init()
{
    // is there a sensor present?
    auto sensor = nodeTelemetrySensorsMap[meshtastic_TelemetrySensorType_BMP3XX];
    if (sensor.first == 0)
        return false;

    // initialise comms
    int result;
    while (ERR_OK != (result = DFRobot_BMP388_I2C::begin())) {
        if (ERR_DATA_BUS == result) {
            LOG_WARN("BMP3XXSensor::begin data bus error");
            return false;
        } else if (ERR_IC_VERSION == result) {
            LOG_WARN("BMP3XXSensor::begin chip versions do not match");
            return false;
        }
    }
    status = true;
    LOG_INFO("BMP3XXSensor::begin ok");

    /**
     * Interrupt configuration
     *   The interrupt mode needs to set. The following modes add up to mode:
     *      Interrupt pin output mode: eINTPinPP: Push pull, eINTPinOD: Open drain
     *      Interrupt pin active level: eINTPinActiveLevelLow: Active low, eINTPinActiveLevelHigh: Active high
     *      Register interrupt latch: eINTLatchDIS: Disable, eINTLatchEN: Enable
     *      FIFO water level reached interrupt: eIntFWtmDis: Disable, eIntFWtmEn: Enable
     *      FIFO full interrupt: eINTFFullDIS: Disable, eINTFFullEN: Enable
     *      Interrupt pin initial (invalid, non-interrupt) level: eINTInitialLevelLOW: Low, eINTInitialLevelHIGH: High
     *      Temperature/pressure data ready interrupt: eINTDataDrdyDIS: Disable, eINTDataDrdyEN: Enable
     * Notice: In non-latching mode (eINTLatchDIS), interrupt signal is 2.5 ms pulse signal
     * Note: When using eINTPinActiveLevelLow (Active low interrupt pin), you need to use eINTInitialLevelHIGH (Initial
     *       level of interrupt pin is high). Please use “FALLING” to trigger the following interrupt.
     *       When using eINTPinActiveLevelHigh (Active low interrupt pin), you need to use eINTInitialLevelLOW (Initial
     *       level of interrupt pin is high). Please use “RISING” to trigger the following interrupt.
     */
    DFRobot_BMP388_I2C::setINTMode(eINTPinPP | eINTPinActiveLevelHigh | eINTLatchDIS | eIntFWtmDis | eINTFFullDIS |
                                   eINTInitialLevelLOW | eINTDataDrdyEN);

    // Set up an interrupt pin with an internal pullup for active high
    pinMode(BMP3XX_INTERRUPT_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(BMP3XX_INTERRUPT_PIN), BMP3XXSetInterrupt, RISING);

    /**
     * 6 commonly used sampling modes that allows users to configure easily, mode:
     *      eUltraLowPrecision, Ultra-low precision, suitable for monitoring weather (lowest power consumption), the power is
     * mandatory mode. eLowPrecision, Low precision, suitable for random detection, power is normal mode eNormalPrecision1, Normal
     * precision 1, suitable for dynamic detection on handheld devices (e.g on mobile phones), power is normal mode.
     *      eNormalPrecision2, Normal precision 2, suitable for drones, power is normal mode.
     *      eHighPrecision, High precision, suitable for low-power handled devices (e.g mobile phones), power is in normal mode.
     *      eUltraPrecision, Ultra-high precision, suitable for indoor navigation, its acquisition rate will be extremely low, and
     * the acquisition cycle is 1000 ms.
     */
    if (!DFRobot_BMP388_I2C::setSamplingMode(eNormalPrecision2)) {
        LOG_WARN("BMP3XXSensor::begin set samping mode failed");
        return false;
    }

    // take a couple of initial readings to settle the sensor filters
    for (int i = 0; i < 3; i++) {
        performReading();
    }
    return true;
}

bool BMP3XXSingleton::performReading()
{
    // check if there are new readings
    if (!BMP3XX_IRQ) {
        return true;
    }

    // Reset the interrupt flag
    BMP3XX_IRQ = false;

    // Read temperature, unit: °C
    temperature = DFRobot_BMP388_I2C::readTempC();

    // Read altitude, unit: m
    altitudeAmslMetres = DFRobot_BMP388_I2C::readAltitudeM();
    return true;
}

#else

// --------------------------------------------------------------------------------
// Use forced readings from the Adafruit library

BMP3XXSingleton::BMP3XXSingleton(){};

BMP3XXSingleton::~BMP3XXSingleton(){};

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
    for (int i = 0; i < 3; i++) {
        if (!performReading())
            return false;
    }
    return true;
}

bool BMP3XXSingleton::performReading()
{
    if (Adafruit_BMP3XX::performReading()) {
        altitudeAmslMetres = 44330.0 * (1.0 - pow((this->pressure / 100.0) / SEA_LEVEL_HPA, 0.1903));
        return true;
    }
    altitudeAmslMetres = INVALID_ALTITUDE;
    return false;
}

#endif

#endif
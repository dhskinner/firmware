#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_GPS && defined(ROCKETFLIGHT_GPS)

#include "RocketFlightGPS.h"

RocketFlightGPS::RocketFlightGPS() 
: GPS(), Altimeter(meshtastic_TelemetrySensorType::meshtastic_TelemetrySensorType_CUSTOM_SENSOR, "GPS", meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_INTERNAL) {}   

int32_t RocketFlightGPS::runOnce()
{
    // Initialise using the base class 
    if (!GPSInitFinished) {

        // initialise
        return GPS::runOnce();

        // wake the GPS (always)
        if (!config.position.fixed_position && powerState != GPS_ACTIVE)
        up();
    }

    // Check for NMEA sentences
    if (whileActive()) {

        // if we have received valid NMEA claim we are connected 
        setConnected();

        // if we have a new position, publish it immediately
        bool gotLoc = lookForLocation();
        if (gotLoc)
        {
            if (!hasValidLocation) 
            {
                LOG_INFO("got GPS lock");
                hasValidLocation = true;
            }   
            lastReceivedPositionMillis = millis();
            shouldPublish = true;  

        } else if (millis() - lastReceivedPositionMillis > GPS_LOST_LOCK_MILLIS && hasValidLocation) {
            LOG_WARN("lost GPS lock");
            p = meshtastic_Position_init_default;
            hasValidLocation = false;
            shouldPublish = true;
        }
    }

    // If we've already set time from the GPS, no need to ask the GPS
    bool gotTime = (getRTCQuality() >= RTCQualityGPS);
    if (!gotTime && lookForTime()) { // Note: we count on this && short-circuiting and not resetting the RTC time
        gotTime = true;
        shouldPublish = true;
    }

    // If state has changed do a publish
    publishUpdate();

    return GPS_THREAD_INTERVAL;
}

bool RocketFlightGPS::isValid(){
    return (hasLock() && p.has_altitude);
}

double RocketFlightGPS::getAltitude()
{
    if(isValid())
        return p.altitude;
    else
        return INVALID_ALTITUDE;
};

void RocketFlightGPS::toggleGpsMode()
{
   // disable this just in case 
} 

GPS *RocketFlightGPS::createGps()
{
    int8_t _rx_gpio = config.position.rx_gpio;
    int8_t _tx_gpio = config.position.tx_gpio;
    int8_t _en_gpio = config.position.gps_en_gpio;
#if HAS_GPS && !defined(ARCH_ESP32)
    _rx_gpio = 1; // We only specify GPS serial ports on ESP32. Otherwise, these are just flags.
    _tx_gpio = 1;
#endif
#if defined(GPS_RX_PIN)
    if (!_rx_gpio)
        _rx_gpio = GPS_RX_PIN;
#endif
#if defined(GPS_TX_PIN)
    if (!_tx_gpio)
        _tx_gpio = GPS_TX_PIN;
#endif
#if defined(PIN_GPS_EN)
    if (!_en_gpio)
        _en_gpio = PIN_GPS_EN;
#endif
#ifdef ARCH_PORTDUINO
    if (!settingsMap[has_gps])
        return nullptr;
#endif
    if (!_rx_gpio || !_serial_gps) // Configured to have no GPS at all
        return nullptr;

    RocketFlightGPS *new_gps = new RocketFlightGPS;
    new_gps->rx_gpio = _rx_gpio;
    new_gps->tx_gpio = _tx_gpio;

    GpioVirtPin *virtPin = new GpioVirtPin();
    new_gps->enablePin = virtPin; // Always at least populate a virtual pin
    if (_en_gpio) {
        GpioPin *p = new GpioHwPin(_en_gpio);

        if (!GPS_EN_ACTIVE) { // Need to invert the pin before hardware
            new GpioNotTransformer(
                virtPin, p); // We just leave this created object on the heap so it can stay watching virtPin and driving en_gpio
        } else {
            new GpioUnaryTransformer(
                virtPin, p); // We just leave this created object on the heap so it can stay watching virtPin and driving en_gpio
        }
    }

#ifdef PIN_GPS_PPS
    // pulse per second
    pinMode(PIN_GPS_PPS, INPUT);
#endif

// Currently disabled per issue #525 (TinyGPS++ crash bug)
// when fixed upstream, can be un-disabled to enable 3D FixType and PDOP
#ifndef TINYGPS_OPTION_NO_CUSTOM_FIELDS
    // see NMEAGPS.h
    gsafixtype.begin(reader, NMEA_MSG_GXGSA, 2);
    gsapdop.begin(reader, NMEA_MSG_GXGSA, 15);
    LOG_DEBUG("Using " NMEA_MSG_GXGSA " for 3DFIX and PDOP");
#endif

    // Make sure the GPS is awake before performing any init.
    new_gps->up();

#ifdef PIN_GPS_RESET
    pinMode(PIN_GPS_RESET, OUTPUT);
    digitalWrite(PIN_GPS_RESET, GPS_RESET_MODE); // assert for 10ms
    delay(10);
    digitalWrite(PIN_GPS_RESET, !GPS_RESET_MODE);
#endif

    if (_serial_gps) {
#ifdef ARCH_ESP32
        // In esp32 framework, setRxBufferSize needs to be initialized before Serial
        _serial_gps->setRxBufferSize(SERIAL_BUFFER_SIZE); // the default is 256
#endif

//  ESP32 has a special set of parameters vs other arduino ports
#if defined(ARCH_ESP32)
        LOG_DEBUG("Using GPIO%d for GPS RX", new_gps->rx_gpio);
        LOG_DEBUG("Using GPIO%d for GPS TX", new_gps->tx_gpio);
        _serial_gps->begin(GPS_BAUDRATE, SERIAL_8N1, new_gps->rx_gpio, new_gps->tx_gpio);
#elif defined(ARCH_RP2040)
        _serial_gps->setFIFOSize(256);
        _serial_gps->begin(GPS_BAUDRATE);
#else
        _serial_gps->begin(GPS_BAUDRATE);
#endif
    }
    return new_gps;
}

#endif // Exclude GPS
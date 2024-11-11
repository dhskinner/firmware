#include "RocketFlightModule.h"

#ifdef ROCKETFLIGHT_MODULE

RocketFlightModule::RocketFlightModule() : PositionModule::PositionModule()
{
    // set up this module for rocketry
    init();
}

bool RocketFlightModule::init()
{
    LOG_INFO("Initializing");
    OSThread::ThreadName = "RocketFlightModule";

    // set a short timeframe between successive position broadcasts (milliseconds)
    minimumTimeThreshold = config.position.broadcast_smart_minimum_interval_secs * 1000;
    maximumTimeThreshold = config.position.position_broadcast_secs * 1000;

    // get an altimeter if one is available
#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR
    switch (altimeter_found.type)
    {
    case ScanI2C::DeviceType::BME_680:
        if (bme680Sensor.hasSensor())
        {
            if (!bme680Sensor.isInitialized())
                bme680Sensor.runOnce();
            if (bme680Sensor.isInitialized())
                altimeter = &bme680Sensor;
        }
        break;
    case ScanI2C::DeviceType::BME_280:
        if (bme280Sensor.hasSensor())
        {
            if (!bme280Sensor.isInitialized())
                bme280Sensor.runOnce();
            if (bme280Sensor.isInitialized())
                altimeter = &bme280Sensor;
        }
        break;
    case ScanI2C::DeviceType::BMP_280:
        if (bmp280Sensor.hasSensor())
        {
            altimeter = &bmp280Sensor;
            if (!bmp280Sensor.isInitialized())
                bmp280Sensor.runOnce();
            if (bmp280Sensor.isInitialized())
                altimeter = &bmp280Sensor;
        }
        break;
    case ScanI2C::DeviceType::BMP_085:
        if (bmp085Sensor.hasSensor())
        {

            if (!bmp085Sensor.isInitialized())
                bmp085Sensor.runOnce();
            if (bmp085Sensor.isInitialized())
                altimeter = &bmp085Sensor;
        }
        break;
    case ScanI2C::DeviceType::BMP_3XX:
        if (bmp3xxSensor.hasSensor())
        {
            if (!bmp3xxSensor.isInitialized())
                bmp3xxSensor.runOnce();
            if (bmp3xxSensor.isInitialized())
                altimeter = &bmp3xxSensor;
        }
        break;
    case ScanI2C::DeviceType::LPS22HB:
        if (lps22hbSensor.hasSensor())
        {
            if (!lps22hbSensor.isInitialized())
                lps22hbSensor.runOnce();
            if (lps22hbSensor.isInitialized())
                altimeter = &lps22hbSensor;
        }
        break;
    default:
        // is there a manual position?
        if (config.position.fixed_position)
        {
            altimeter = new Altimeter(meshtastic_TelemetrySensorType::meshtastic_TelemetrySensorType_CUSTOM_SENSOR, "Fixed", meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_MANUAL, static_cast<double>(localPosition.altitude));
            break;
        }
#ifdef ROCKETFLIGHT_GPS
        // is Rocketflight GPS available?
        if (gps != nullptr)
        {
            altimeter = gps->asAltimeter();
            break;
        }
#endif
        // fallback to nothing
        altimeter = new Altimeter(meshtastic_TelemetrySensorType::meshtastic_TelemetrySensorType_SENSOR_UNSET, "None", meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_UNSET);
        break;
    }
    LOG_INFO("Altimeter type: %s status: %s", altimeter->name(), altimeter->isValid() ? "ok" : "failed");

    // // get the motion processor if one is available
    // switch (accelerometer_found.type)
    // {
    //     case ScanI2C::DeviceType::ICM20948:
    //         mpu = ICM20948Singleton::GetInstance();
    //         bool result = mpu->init();
    //         LOG_DEBUG("RocketFlightModule::init: motion processor %s", result ? "ok" : "fail");
    //         result = mpu->setWakeOnMotion();
    //         LOG_DEBUG("RocketFlightModule::init: wake on motion %s", result ? "ok" : "fail");
    //     default:

    // }

    // start sending positions immediately
    setIntervalFromNow(ROCKETFLIGHT_MODULE_INTERVAL);

    return true;
}

int32_t RocketFlightModule::runOnce()
{
    // Handle deep sleep mode if required
    if (sleepOnNextExecution == true)
        deepSleep();

    // Get info for own node
    meshtastic_NodeInfoLite *node = nodeDB->getMeshNode(nodeDB->getNodeNum());
    if (node == nullptr)
    {
        LOG_DEBUG("RunOnce could not get own node");
        return ROCKETFLIGHT_MODULE_INTERVAL;
    }

    // Only send packets if the channel util. is less than 25% utilized or we're a tracker with less than 40% utilized.
    if (!airTime->isTxAllowedChannelUtil(config.device.role != meshtastic_Config_DeviceConfig_Role_TRACKER &&
                                         config.device.role != meshtastic_Config_DeviceConfig_Role_TAK_TRACKER))
    {
        LOG_DEBUG("Skipped sending due to channel congestion");
        return ROCKETFLIGHT_MODULE_INTERVAL;
    }

    // currentAltitude = altimeter->getAltitude();
    // currentAltitudeSource = altimeter->g;
    // if (altimeter != nullptr)
    // {
    //     currentAltitude = static_cast<double>(localPosition.altitude);
    //     currentAltitudeSource = meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_MANUAL;
    // }
    // else if (altimeter != nullptr)
    // {
    //     currentAltitude = altimeter->getAltitude();
    //     currentAltitudeSource = altimeter->getAltitudeSource();
    // }
    // else if (gpsStatus->getHasLock())
    // {
    //     currentAltitude = static_cast<double>(localPosition.altitude);
    //     currentAltitudeSource = meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_INTERNAL;
    // }
    // else
    // {
    //     currentAltitude = INVALID_ALTITUDE;
    //     currentAltitudeSource = meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_UNSET;
    // }
    // // Apply an offset from reference altitude
    // if (currentAltitude != INVALID_ALTITUDE)
    //     currentAltitude -= referenceAltitude;

    // If not in smart position mode, only send every n milliseconds
    uint32_t now = millis();
    if ((now - lastGpsSend) < minimumTimeThreshold)
    {
        return ROCKETFLIGHT_MODULE_INTERVAL;
    }

    // If in smart position mode, send every n metres up to a maximum of n milliseconds
    if (config.position.position_broadcast_smart_enabled 
    && ((now - lastGpsSend) < maximumTimeThreshold) && !hasMoved())
    {
        return ROCKETFLIGHT_MODULE_INTERVAL;
    }

    // Send it
    sendOurPosition();
    if (config.device.role == meshtastic_Config_DeviceConfig_Role_LOST_AND_FOUND)
    {
        sendLostAndFoundText();
    }

    LOG_DEBUG("Will run again in %d msec", ROCKETFLIGHT_MODULE_INTERVAL);
    return ROCKETFLIGHT_MODULE_INTERVAL;
}

void RocketFlightModule::handleNewPosition()
{
    runOnce();

    if (altimeter != nullptr)
        altimeter->getReferenceAltitude();
}

// Prepare a position report to send - same functionality as PositionModule
// except using barometric altitude (GPS altitude is only used as a fallback)
meshtastic_MeshPacket *RocketFlightModule::allocReply()
{
    // should guarantee there is now a position
    meshtastic_NodeInfoLite *node = service->refreshLocalMeshNode();
    // assert(node->has_position);

    // populate a position struct with ONLY the requested fields
    uint32_t pos_flags = config.position.position_flags;
    meshtastic_Position p = meshtastic_Position_init_default;

    // if localPosition is totally empty, put our last saved position (lite) in there
    if (localPosition.latitude_i == 0 && localPosition.longitude_i == 0)
    {
        nodeDB->setLocalPosition(TypeConversions::ConvertToPosition(node->position));
    }
    localPosition.seq_number++;

    // lat/lon are included if available
    if (localPosition.latitude_i == 0 && localPosition.longitude_i == 0)
    {
        LOG_WARN("Position is unknown - lat/lon are zero");
    }
    else
    {
        LOG_DEBUG("Sending location with precision %i", precision);
        if (precision < 32 && precision > 0)
        {
            p.latitude_i = localPosition.latitude_i & (UINT32_MAX << (32 - precision));
            p.longitude_i = localPosition.longitude_i & (UINT32_MAX << (32 - precision));

            // We want the imprecise position to be the middle of the possible location, not quantized at the edges
            p.latitude_i += (1 << (31 - precision));
            p.longitude_i += (1 << (31 - precision));
        }
        else
        {
            p.latitude_i = localPosition.latitude_i;
            p.longitude_i = localPosition.longitude_i;
        }
        p.precision_bits = precision;
        p.has_latitude_i = true;
        p.has_longitude_i = true;
        p.time = getValidTime(RTCQualityNTP) > 0 ? getValidTime(RTCQualityNTP) : localPosition.time;

        if (config.position.fixed_position)
        {
            p.location_source = meshtastic_Position_LocSource_LOC_MANUAL;
        }
    }

    // altitude is included if available (rocketflight module ignores )
    if ((pos_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE) && (altimeter != nullptr))
    {
        if (altimeter->isValid())
        {
            if(pos_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL)
                // Add AMSL altitude
                p.altitude = (int32_t)std::round(altimeter->getAltitude() );
            else
                // Usually rocketry people want AGL altitude, which is offset from a ground reference
                p.altitude = (int32_t)std::round(altimeter->getAltitude() - altimeter->getReferenceAltitude());

            p.altitude_source = altimeter->getAltitudeSource();
            p.has_altitude = true;

            if (p.altitude_source == meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_INTERNAL 
                || p.altitude_source == meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_EXTERNAL)
            {
                // add GPS HAE and geoidal separation info if we are using GPS
                // if (!pos_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL)
                // {
                //     p.altitude_hae = localPosition.altitude_hae;
                //     p.has_altitude_hae = p.altitude_hae != 0;
                // }
                if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION)
                {
                    p.altitude_geoidal_separation = localPosition.altitude_geoidal_separation;
                    p.has_altitude_geoidal_separation = p.altitude_geoidal_separation != 0;
                }
            }
        }
    }
    if (p.has_altitude)
    {
        lastAltitudeSent = p.altitude;
    }

    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_DOP)
    {
        if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_HVDOP)
        {
            p.HDOP = localPosition.HDOP;
            p.VDOP = localPosition.VDOP;
        }
        else
            p.PDOP = localPosition.PDOP;
    }

    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_SATINVIEW)
        p.sats_in_view = localPosition.sats_in_view;

    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP)
        p.timestamp = localPosition.timestamp;

    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO)
        p.seq_number = localPosition.seq_number;

    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_HEADING)
    {
        p.ground_track = localPosition.ground_track;
        p.has_ground_track = true;
    }
    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_SPEED)
    {
        p.ground_speed = localPosition.ground_speed;
        p.has_ground_speed = true;
    }

    // Strip out any time information before sending packets to other nodes - to keep the wire size small (and because other
    // nodes shouldn't trust it anyways) Note: we allow a device with a local GPS or NTP to include the time, so that devices
    // without can get time.
    if (getRTCQuality() < RTCQualityNTP)
    {
        LOG_DEBUG("Stripping time %u from position send", p.time);
        p.time = 0;
    }
    else
    {
        p.time = getValidTime(RTCQualityNTP);
        LOG_DEBUG("Providing time to mesh %u", p.time);
    }

    LOG_INFO("Position: time=%i lat=%i lon=%i alt=%i", p.time, p.latitude_i, p.longitude_i, p.altitude);

    // Set the current coords as our last ones, after we've compared distance with current and decided to send
    if (p.latitude_i != 0 && p.longitude_i != 0)
    {
        lastGpsLatitude = node->position.latitude_i;
        lastGpsLongitude = node->position.longitude_i;
    }
    lastGpsSend = millis();

    // TAK Tracker devices should send their position in a TAK packet over the ATAK port
    if (config.device.role == meshtastic_Config_DeviceConfig_Role_TAK_TRACKER)
        return allocAtakPli();

    return allocDataProtobuf(p);
}

bool RocketFlightModule::hasSensor(_meshtastic_TelemetrySensorType sensorType)
{
    return nodeTelemetrySensorsMap[sensorType].first > 0;
}

bool RocketFlightModule::hasMoved()
{
    // Should guarantee there is now a position
    const meshtastic_NodeInfoLite *node = service->refreshLocalMeshNode();

    // Determine the distance in meters between two points on the globe
    uint32_t distanceTraveled = 0;
    if (hasValidPosition(node))
    {
        meshtastic_PositionLite currentPosition = node->position;
        distanceTraveled = abs(std::round(GeoCoord::latLongToMeter(
            lastGpsLatitude * 1e-7, lastGpsLongitude * 1e-7, currentPosition.latitude_i * 1e-7, currentPosition.longitude_i * 1e-7)));
    }

    // Get the altitude travelled
    uint32_t altitudeTravelled = 0;
    uint32_t elapsed = millis() - lastGpsSend;
    if (altimeter->isValid())
        altitudeTravelled = abs((int32_t)std::round(altimeter->getAltitude()) - lastAltitudeSent);
    
    // Has position moved?
    bool moved = distanceTraveled > config.position.broadcast_smart_minimum_distance || altitudeTravelled > config.position.broadcast_smart_minimum_distance;

    LOG_DEBUG("Moved: %s horiz: %dm vert: %dm threshold: %dm elapsed: %dms ",
              moved ? "yes" : "no",
              distanceTraveled,
              altitudeTravelled,
              config.position.broadcast_smart_minimum_distance,
              elapsed);
    return moved;
}

void RocketFlightModule::deepSleep()
{
    sleepOnNextExecution = false;
    uint32_t nightyNightMs = Default::getConfiguredOrDefaultMs(config.position.position_broadcast_secs);
    LOG_DEBUG("Deep sleeping for %ims, then awaking to send position again.", nightyNightMs);
    doDeepSleep(nightyNightMs, false);
}

#endif

#endif
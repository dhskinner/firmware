#include "RocketFlightModule.h"

#if !MESHTASTIC_EXCLUDE_GPS && defined(ROCKETFLIGHT_EDITION)

RocketFlightModule::RocketFlightModule() : PositionModule::PositionModule()
{
    // set up this module for rocketry
    init();
}

bool RocketFlightModule::init()
{
    LOG_INFO("Initializing\n");
    OSThread::ThreadName = "RocketFlightModule";

    // set the highest position precision
    precision = default_position_precision;

    // set a short timeframe between successive position broadcasts (milliseconds)
    minimumTimeThreshold = config.position.broadcast_smart_minimum_interval_secs * 1000;
    maximumTimeThreshold = config.position.position_broadcast_secs * 1000;

    // get the altimeter if it is available
    if (hasSensor(meshtastic_TelemetrySensorType_BMP3XX))
    {
        altimeter = BMP3XXSingleton::GetInstance();
        uint32_t result = altimeter->init();
        LOG_DEBUG("::init: altimeter %s\n", result ? "ok" : "fail");
    }

    // // get the motion processor if it is available
    // if (hasSensor(meshtastic_TelemetrySensorType_ICM20948))
    // {
    //     mpu = ICM20948Singleton::GetInstance();
    //     bool result = mpu->init();
    //     LOG_DEBUG("RocketFlightModule::init: motion processor %s\n", result ? "ok" : "fail");
    //     result = mpu->setWakeOnMotion();
    //     LOG_DEBUG("RocketFlightModule::init: wake on motion %s\n", result ? "ok" : "fail");
    // }

    // start sending positions immediately
    setIntervalFromNow(default_rocketflight_module_interval);

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
        LOG_DEBUG("::runOnce could not get own node\n");
        return default_rocketflight_module_interval;
    }

    // Only send packets if the channel util. is less than 25% utilized or we're a tracker with less than 40% utilized.
    if (!airTime->isTxAllowedChannelUtil(config.device.role != meshtastic_Config_DeviceConfig_Role_TRACKER &&
                                         config.device.role != meshtastic_Config_DeviceConfig_Role_TAK_TRACKER))
    {
        LOG_DEBUG("::runOnce skipped sending due to channel congestion\n");
        return default_rocketflight_module_interval;
    }

    // If not in smart position mode, only send every n milliseconds
    uint32_t now = millis();
    if (!config.position.position_broadcast_smart_enabled && ((now - lastGpsSend) < minimumTimeThreshold))
    {
        return default_rocketflight_module_interval;
    }

    // If in smart position mode, send every n metres up to a maximum of n milliseconds
    if (config.position.position_broadcast_smart_enabled && ((now - lastGpsSend) < maximumTimeThreshold) && !hasMoved())
    {
        return default_rocketflight_module_interval;
    }

    // Send it
    sendOurPosition();
    if (config.device.role == meshtastic_Config_DeviceConfig_Role_LOST_AND_FOUND)
    {
        sendLostAndFoundText();
    }

    LOG_DEBUG("::runOnce will run again in %d msec\n", default_rocketflight_module_interval);
    return default_rocketflight_module_interval;
}

void RocketFlightModule::handleNewPosition()
{
    runOnce();
}

// Prepare a position report to send - same functionality as PositionModule
// except using barometric altitude AMSL (GPS altitude is only used as a fallback)
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

    // lat/lon are unconditionally included IF AVAILABLE!
    if (localPosition.latitude_i == 0 && localPosition.longitude_i == 0)
    {
        LOG_WARN("::allocReply position is unknown - lat/lon are zero\n");
    }
    else
    {
        LOG_DEBUG("::allocReply sending location with precision %i\n", precision);
        if (precision < 32 && precision > 0)
        {
            p.latitude_i = localPosition.latitude_i & (UINT32_MAX << (32 - precision));
            p.longitude_i = localPosition.longitude_i & (UINT32_MAX << (32 - precision));

            // We want the imprecise position to be the middle of the possible location, not
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

    // altitude is unconditionally included IF AVAILABLE!
    if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE)
    {
        if (altimeter != nullptr)
        {
            p.altitude = getBaroAltitude();
            p.altitude_source = meshtastic_Position_AltSource_ALT_BAROMETRIC;
            p.has_altitude = p.altitude != 0;
            lastBaroAltitude = p.altitude;
        }
        else
        {
            if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL)
            {
                p.altitude = localPosition.altitude;
                p.has_altitude = true;
            }
            else
            {
                p.altitude_hae = localPosition.altitude_hae;
                p.has_altitude_hae = true;
            }
            if (pos_flags & meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION)
            {
                p.altitude_geoidal_separation = localPosition.altitude_geoidal_separation;
                p.has_altitude_geoidal_separation = true;
            }
            p.altitude_source = meshtastic_Position_AltSource_ALT_INTERNAL;
        }
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
        LOG_DEBUG("::allocReply stripping time %u from position send\n", p.time);
        p.time = 0;
    }
    else
    {
        p.time = getValidTime(RTCQualityNTP);
        LOG_DEBUG("::allocReply providing time to mesh %u\n", p.time);
    }

    LOG_INFO("Position reply: time=%i lat=%i lon=%i alt=%i press=%.2f\n", p.time, p.latitude_i, p.longitude_i, p.altitude, altimeter->pressure);

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

int32_t RocketFlightModule::getBaroAltitude()
{
    if (altimeter == nullptr)
        return 0;

    if (altimeter->performReading())
    {
        return (int32_t)std::round(altimeter->altitudeAmslMetres);
    }
    return 0;
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
    uint32_t altitudeTravelled = abs(getBaroAltitude() - lastBaroAltitude);
    bool moved = (distanceTraveled > config.position.broadcast_smart_minimum_distance || altitudeTravelled > config.position.broadcast_smart_minimum_distance);
    uint32_t elapsed = millis() - lastGpsSend;

    // Has position moved?
    bool hasMoved = distanceTraveled > config.position.broadcast_smart_minimum_distance || altitudeTravelled > config.position.broadcast_smart_minimum_distance;
    
    LOG_DEBUG("::hasMoved gps: %s distance: %dm altitude: %dm timeElapsed: %dms threshold: %dm moved: %s\n",
              hasValidPosition(node) ? "ok" : "--",
              distanceTraveled,
              altitudeTravelled,
              config.position.broadcast_smart_minimum_distance,
              elapsed,
              moved ? "yes" : "no");
    return moved;
}

void RocketFlightModule::deepSleep()
{
    sleepOnNextExecution = false;
    uint32_t nightyNightMs = Default::getConfiguredOrDefaultMs(config.position.position_broadcast_secs);
    LOG_DEBUG("::deepSleep sleeping for %ims, then awaking to send position again.\n", nightyNightMs);
    doDeepSleep(nightyNightMs, false);
}

#endif
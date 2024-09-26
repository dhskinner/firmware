#include "RocketFlightDB.h"
#include <ErriezCRC32.h>

#ifdef ROCKETFLIGHT_EDITION

RocketFlightDB::RocketFlightDB() : NodeDB::NodeDB()
{
    // first the default NodeDB::NodeDB() constructor loads all the settings from file -
    // this will always overwrite all changes from the app, for the settings lited below.
    // Ideally this produce a working tracker, out-of-the-box on first boot after a factory reset
    LOG_INFO("Initializing RocketFlightDB\n");

    // next selectively overwrite with suitable configs for rocketry (this is currently hardcoded)
    config.device.role = default_device_role;
    config.device.node_info_broadcast_secs = default_node_info_broadcast_secs;
    config.power.is_power_saving = default_power_is_power_saving;
    config.position.broadcast_smart_minimum_distance = default_broadcast_smart_minimum_distance;
    config.position.broadcast_smart_minimum_interval_secs = default_broadcast_smart_minimum_interval_secs;
    config.position.position_broadcast_secs = default_broadcast_interval_secs;
    config.position.position_broadcast_smart_enabled = default_position_broadcast_smart_enabled;
    config.position.position_flags = config.position.position_flags | default_position_flags;
    owner.role = config.device.role;
}

void RocketFlightDB::storeSettings()
{
    int saveWhat = 0;
    uint32_t devicestateCRC = crc32Buffer(&devicestate, sizeof(devicestate));
    uint32_t configCRC = crc32Buffer(&config, sizeof(config));
    uint32_t channelFileCRC = crc32Buffer(&channelFile, sizeof(channelFile));
    if (devicestateCRC != crc32Buffer(&devicestate, sizeof(devicestate)))
        saveWhat |= SEGMENT_DEVICESTATE;
    if (configCRC != crc32Buffer(&config, sizeof(config)))
        saveWhat |= SEGMENT_CONFIG;
    if (channelFileCRC != crc32Buffer(&channelFile, sizeof(channelFile)))
        saveWhat |= SEGMENT_CHANNELS;
    saveToDisk(saveWhat);
}

#endif
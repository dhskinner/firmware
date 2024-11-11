#include "RocketFlightDB.h"
#include <ErriezCRC32.h>
#include <map>
#include <string>
#include "Channels.h"

#ifdef ROCKETFLIGHT_CONFIG

extern meshtastic_LocalConfig config;
extern meshtastic_LocalModuleConfig moduleConfig;
extern meshtastic_ChannelFile channelFile;
extern meshtastic_User &owner;

void NodeDB::loadFromJson()
{
    // TODO TODO
    // LOG_INFO("RocketFlightDB printing config\n");
    // RocketFlightDB::printJson();

    LOG_INFO("RocketFlight is checking config - all settings will be reset except for username\n");

    uint32_t ownerCRC = crc32Buffer(&owner, sizeof(owner));
    uint32_t configCRC = crc32Buffer(&config, sizeof(config));
    uint32_t moduleCRC = crc32Buffer(&moduleConfig, sizeof(moduleConfig));
    uint32_t channelFileCRC = crc32Buffer(&channelFile, sizeof(channelFile));
    
    JsonDocument doc = RocketFlightDB::loadJson();
    RocketFlightDB::deserialiseOwnerConfig(doc, owner);
    RocketFlightDB::deserialiseLocalConfig(doc, config);
    RocketFlightDB::deserialiseModuleConfig(doc, moduleConfig);
    RocketFlightDB::deserialiseChannelConfig(doc, channelFile);

    int saveWhat = 0;
    if (ownerCRC != crc32Buffer(&owner, sizeof(owner)))
        saveWhat |= SEGMENT_DEVICESTATE;
    if (configCRC != crc32Buffer(&config, sizeof(config)))
        saveWhat |= SEGMENT_CONFIG;
    if (moduleCRC != crc32Buffer(&moduleConfig, sizeof(moduleConfig)))
        saveWhat |= SEGMENT_MODULECONFIG;
    if (channelFileCRC != crc32Buffer(&channelFile, sizeof(channelFile)))
        saveWhat |= SEGMENT_CHANNELS;
    if (saveWhat) {
        LOG_WARN("RocketFlight found config changes - saving new config files\n");
        RocketFlightDB::printJson();
        saveToDisk(saveWhat);
    }
}

void RocketFlightDB::printJson()
{
    // Allocate the JSON document
    JsonDocument doc;

    // Serialise the meshtastic config that was loaded from disk
    RocketFlightDB::serialiseOwnerConfig(doc, owner);
    RocketFlightDB::serialiseLocalConfig(doc, config);
    RocketFlightDB::serialiseModuleConfig(doc, moduleConfig);
    RocketFlightDB::serialiseChannelConfig(doc, channelFile);

    // Generate the JSON printout
    DEBUG_PORT.println("--------------------------------------------------------------------------------");
    serializeJsonPretty(doc, DEBUG_PORT);
    //serializeJson(doc, DEBUG_PORT);
    DEBUG_PORT.println("\n--------------------------------------------------------------------------------");

}

#endif
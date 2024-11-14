#pragma once
#ifndef _ROCKETFLIGHT_DB_H_
#define _ROCKETFLIGHT_DB_H_

#include "Channels.h"
#include "Default.h"
#include "NodeDB.h"
#include <ErriezCRC32.h>
#include <map>
#include <string>

#ifdef ROCKETFLIGHT_CONFIG

#include <ArduinoJson.h>

class RocketFlightDB
{
  public:
    static void printJson();
    static JsonDocument loadJson();
    static const std::string userPrefJson;

    static void deserialiseOwnerConfig(JsonDocument &doc, meshtastic_User &out);
    static void deserialiseLocalConfig(JsonDocument &doc, meshtastic_LocalConfig &out);
    static void deserialiseChannelConfig(JsonDocument &doc, meshtastic_ChannelFile &out);
    static void deserialiseModuleConfig(JsonDocument &doc, meshtastic_LocalModuleConfig &out);

    static pb_size_t deserialiseByteArray(JsonArray &arr, uint8_t *out, size_t outSize);

    static void serialiseOwnerConfig(JsonDocument &doc, meshtastic_User &in);
    static void serialiseLocalConfig(JsonDocument &doc, meshtastic_LocalConfig &in);
    static void serialiseChannelConfig(JsonDocument &doc, meshtastic_ChannelFile &in);
    static void serialiseModuleConfig(JsonDocument &doc, meshtastic_LocalModuleConfig &in);

    template <typename T> static void serialiseByteArray(JsonObject &obj, std::string key, T &in);

    static meshtastic_Config_DeviceConfig_RebroadcastMode getRebroadcastMode(std::string key);
    static meshtastic_Config_DeviceConfig_Role getDeviceRole(std::string key);
    static meshtastic_Config_PositionConfig_PositionFlags getPositionFlag(std::string key);
    static meshtastic_Config_PositionConfig_GpsMode getGpsMode(std::string key);
    static meshtastic_Config_NetworkConfig_AddressMode getAddressMode(std::string key);
    static meshtastic_Config_DisplayConfig_GpsCoordinateFormat getGpsFormat(std::string key);
    static meshtastic_Config_DisplayConfig_DisplayUnits getDisplayUnits(std::string key);
    static meshtastic_Config_DisplayConfig_OledType getOledType(std::string key);
    static meshtastic_Config_DisplayConfig_DisplayMode getDisplayMode(std::string key);
    static meshtastic_Config_DisplayConfig_CompassOrientation getCompassOrientation(std::string key);
    static meshtastic_Config_LoRaConfig_RegionCode getRegionCode(std::string key);
    static meshtastic_Config_LoRaConfig_ModemPreset getModemPreset(std::string key);
    static meshtastic_Config_BluetoothConfig_PairingMode getPairingMode(std::string key);
    static meshtastic_Channel_Role getChannelRole(std::string key);
    // TODO TODO
    // static meshtastic_ScreenFonts getScreenFont(std::string key);

    static std::string getRebroadcastMode(meshtastic_Config_DeviceConfig_RebroadcastMode key);
    static std::string getDeviceRole(meshtastic_Config_DeviceConfig_Role key);
    static std::string getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags key);
    static std::string getGpsMode(meshtastic_Config_PositionConfig_GpsMode key);
    static std::string getAddressMode(meshtastic_Config_NetworkConfig_AddressMode key);
    static std::string getGpsFormat(meshtastic_Config_DisplayConfig_GpsCoordinateFormat key);
    static std::string getDisplayUnits(meshtastic_Config_DisplayConfig_DisplayUnits key);
    static std::string getOledType(meshtastic_Config_DisplayConfig_OledType key);
    static std::string getDisplayMode(meshtastic_Config_DisplayConfig_DisplayMode key);
    static std::string getCompassOrientation(meshtastic_Config_DisplayConfig_CompassOrientation key);
    static std::string getRegionCode(meshtastic_Config_LoRaConfig_RegionCode key);
    static std::string getModemPreset(meshtastic_Config_LoRaConfig_ModemPreset key);
    static std::string getPairingMode(meshtastic_Config_BluetoothConfig_PairingMode key);
    static std::string getChannelRole(meshtastic_Channel_Role key);
    // TODO TODO
    // static std::string getScreenFont(meshtastic_ScreenFonts key);
    static std::string getHardwareModel(meshtastic_HardwareModel key);
};

#endif

#endif
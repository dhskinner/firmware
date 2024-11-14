#include "Channels.h"
#include "RocketFlightDB.h"
#include <ErriezCRC32.h>
#include <map>
#include <string>

#ifdef ROCKETFLIGHT_CONFIG

static bool IsNotNull(std::string key)
{
    return (key.size() > 0 && key.compare("null") != 0);
}

meshtastic_Config_DeviceConfig_RebroadcastMode RocketFlightDB::getRebroadcastMode(std::string key)
{
    static std::map<std::string, meshtastic_Config_DeviceConfig_RebroadcastMode> values = {
        {"ALL", meshtastic_Config_DeviceConfig_RebroadcastMode_ALL},
        {"ALL_SKIP_DECODING", meshtastic_Config_DeviceConfig_RebroadcastMode_ALL_SKIP_DECODING},
        {"LOCAL_ONLY", meshtastic_Config_DeviceConfig_RebroadcastMode_LOCAL_ONLY},
        {"KNOWN_ONLY", meshtastic_Config_DeviceConfig_RebroadcastMode_KNOWN_ONLY}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config setting for RebroadcastMode is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DeviceConfig_RebroadcastMode_ALL;
}

meshtastic_Config_DeviceConfig_Role RocketFlightDB::getDeviceRole(std::string key)
{
    static std::map<std::string, meshtastic_Config_DeviceConfig_Role> values = {
        {"CLIENT", meshtastic_Config_DeviceConfig_Role_CLIENT},
        {"CLIENT_MUTE", meshtastic_Config_DeviceConfig_Role_CLIENT_MUTE},
        {"ROUTER", meshtastic_Config_DeviceConfig_Role_ROUTER},
        {"ROUTER_CLIENT", meshtastic_Config_DeviceConfig_Role_ROUTER_CLIENT},
        {"REPEATER", meshtastic_Config_DeviceConfig_Role_REPEATER},
        {"TRACKER", meshtastic_Config_DeviceConfig_Role_TRACKER},
        {"SENSOR", meshtastic_Config_DeviceConfig_Role_SENSOR},
        {"TAK", meshtastic_Config_DeviceConfig_Role_TAK},
        {"CLIENT_HIDDEN", meshtastic_Config_DeviceConfig_Role_CLIENT_HIDDEN},
        {"LOST_AND_FOUND", meshtastic_Config_DeviceConfig_Role_LOST_AND_FOUND},
        {"TAK_TRACKER", meshtastic_Config_DeviceConfig_Role_TAK_TRACKER}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for Device Role is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DeviceConfig_Role_CLIENT;
}

meshtastic_Config_PositionConfig_PositionFlags RocketFlightDB::getPositionFlag(std::string key)
{
    static std::map<std::string, meshtastic_Config_PositionConfig_PositionFlags> values = {
        {"UNSET", meshtastic_Config_PositionConfig_PositionFlags_UNSET},
        {"ALTITUDE", meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE},
        {"ALTITUDE_MSL", meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL},
        {"GEOIDAL_SEPARATION", meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION},
        {"DOP", meshtastic_Config_PositionConfig_PositionFlags_DOP},
        {"HVDOP", meshtastic_Config_PositionConfig_PositionFlags_HVDOP},
        {"SATINVIEW", meshtastic_Config_PositionConfig_PositionFlags_SATINVIEW},
        {"SEQ_NO", meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO},
        {"TIMESTAMP", meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP},
        {"HEADING", meshtastic_Config_PositionConfig_PositionFlags_HEADING},
        {"SPEED", meshtastic_Config_PositionConfig_PositionFlags_SPEED}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for position flags is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_PositionConfig_PositionFlags_UNSET;
}

meshtastic_Config_PositionConfig_GpsMode RocketFlightDB::getGpsMode(std::string key)
{
    static std::map<std::string, meshtastic_Config_PositionConfig_GpsMode> values = {
        {"DISABLED", meshtastic_Config_PositionConfig_GpsMode_DISABLED},
        {"ENABLED", meshtastic_Config_PositionConfig_GpsMode_ENABLED},
        {"NOT_PRESENT", meshtastic_Config_PositionConfig_GpsMode_NOT_PRESENT}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for GPS mode is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_PositionConfig_GpsMode_ENABLED;
}

meshtastic_Config_NetworkConfig_AddressMode RocketFlightDB::getAddressMode(std::string key)
{
    static std::map<std::string, meshtastic_Config_NetworkConfig_AddressMode> values = {
        {"DHCP", meshtastic_Config_NetworkConfig_AddressMode_DHCP},
        {"STATIC", meshtastic_Config_NetworkConfig_AddressMode_STATIC}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for network mode is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_NetworkConfig_AddressMode_DHCP;
}

meshtastic_Config_DisplayConfig_GpsCoordinateFormat RocketFlightDB::getGpsFormat(std::string key)
{
    static std::map<std::string, meshtastic_Config_DisplayConfig_GpsCoordinateFormat> values = {
        {"DEC", meshtastic_Config_DisplayConfig_GpsCoordinateFormat_DEC},
        {"DMS", meshtastic_Config_DisplayConfig_GpsCoordinateFormat_DMS},
        {"UTM", meshtastic_Config_DisplayConfig_GpsCoordinateFormat_UTM},
        {"MGRS", meshtastic_Config_DisplayConfig_GpsCoordinateFormat_MGRS},
        {"OLC", meshtastic_Config_DisplayConfig_GpsCoordinateFormat_OLC},
        {"OSGR", meshtastic_Config_DisplayConfig_GpsCoordinateFormat_OSGR}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for GPS coord format is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DisplayConfig_GpsCoordinateFormat_DEC;
}

meshtastic_Config_DisplayConfig_DisplayUnits RocketFlightDB::getDisplayUnits(std::string key)
{
    static std::map<std::string, meshtastic_Config_DisplayConfig_DisplayUnits> values = {
        {"METRIC", meshtastic_Config_DisplayConfig_DisplayUnits_METRIC},
        {"IMPERIAL", meshtastic_Config_DisplayConfig_DisplayUnits_IMPERIAL}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for display units is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DisplayConfig_DisplayUnits_METRIC;
}

meshtastic_Config_DisplayConfig_OledType RocketFlightDB::getOledType(std::string key)
{
    static std::map<std::string, meshtastic_Config_DisplayConfig_OledType> values = {
        {"OLED_AUTO", meshtastic_Config_DisplayConfig_OledType_OLED_AUTO},
        {"OLED_SSD1306", meshtastic_Config_DisplayConfig_OledType_OLED_SSD1306},
        {"OLED_SH1106", meshtastic_Config_DisplayConfig_OledType_OLED_SH1106},
        {"OLED_SH1107", meshtastic_Config_DisplayConfig_OledType_OLED_SH1107}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for OLED type is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DisplayConfig_OledType_OLED_AUTO;
}

meshtastic_Config_DisplayConfig_DisplayMode RocketFlightDB::getDisplayMode(std::string key)
{
    static std::map<std::string, meshtastic_Config_DisplayConfig_DisplayMode> values = {
        {"DEFAULT", meshtastic_Config_DisplayConfig_DisplayMode_DEFAULT},
        {"TWOCOLOR", meshtastic_Config_DisplayConfig_DisplayMode_TWOCOLOR},
        {"INVERTED", meshtastic_Config_DisplayConfig_DisplayMode_INVERTED},
        {"COLOR", meshtastic_Config_DisplayConfig_DisplayMode_COLOR}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for OLED display mode is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DisplayConfig_DisplayMode_DEFAULT;
}

meshtastic_Config_DisplayConfig_CompassOrientation RocketFlightDB::getCompassOrientation(std::string key)
{
    static std::map<std::string, meshtastic_Config_DisplayConfig_CompassOrientation> values = {
        {"DEGREES_0", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_0},
        {"DEGREES_90", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_90},
        {"DEGREES_180", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_180},
        {"DEGREES_270", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_270},
        {"DEGREES_0_INVERTED", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_0_INVERTED},
        {"DEGREES_90_INVERTED", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_90_INVERTED},
        {"DEGREES_180_INVERTED", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_180_INVERTED},
        {"DEGREES_270_INVERTED", meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_270_INVERTED}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for compass orientation is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_0;
}

meshtastic_Config_LoRaConfig_RegionCode RocketFlightDB::getRegionCode(std::string key)
{
    static std::map<std::string, meshtastic_Config_LoRaConfig_RegionCode> values = {
        {"UNSET", meshtastic_Config_LoRaConfig_RegionCode_UNSET},   {"US", meshtastic_Config_LoRaConfig_RegionCode_US},
        {"EU_433", meshtastic_Config_LoRaConfig_RegionCode_EU_433}, {"EU_868", meshtastic_Config_LoRaConfig_RegionCode_EU_868},
        {"CN", meshtastic_Config_LoRaConfig_RegionCode_CN},         {"JP", meshtastic_Config_LoRaConfig_RegionCode_JP},
        {"ANZ", meshtastic_Config_LoRaConfig_RegionCode_ANZ},       {"KR", meshtastic_Config_LoRaConfig_RegionCode_KR},
        {"TW", meshtastic_Config_LoRaConfig_RegionCode_TW},         {"RU", meshtastic_Config_LoRaConfig_RegionCode_RU},
        {"IN", meshtastic_Config_LoRaConfig_RegionCode_IN},         {"NZ_865", meshtastic_Config_LoRaConfig_RegionCode_NZ_865},
        {"TH", meshtastic_Config_LoRaConfig_RegionCode_TH},         {"LORA_24", meshtastic_Config_LoRaConfig_RegionCode_LORA_24},
        {"UA_433", meshtastic_Config_LoRaConfig_RegionCode_UA_433}, {"UA_868", meshtastic_Config_LoRaConfig_RegionCode_UA_868},
        {"MY_433", meshtastic_Config_LoRaConfig_RegionCode_MY_433}, {"MY_919", meshtastic_Config_LoRaConfig_RegionCode_MY_919},
        {"SG_923", meshtastic_Config_LoRaConfig_RegionCode_SG_923}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for region code is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_LoRaConfig_RegionCode_UNSET;
}

meshtastic_Config_LoRaConfig_ModemPreset RocketFlightDB::getModemPreset(std::string key)
{
    static std::map<std::string, meshtastic_Config_LoRaConfig_ModemPreset> values = {
        {"LONG_FAST", meshtastic_Config_LoRaConfig_ModemPreset_LONG_FAST},
        {"LONG_SLOW", meshtastic_Config_LoRaConfig_ModemPreset_LONG_SLOW},
        {"VERY_LONG_SLOW", meshtastic_Config_LoRaConfig_ModemPreset_VERY_LONG_SLOW},
        {"MEDIUM_SLOW", meshtastic_Config_LoRaConfig_ModemPreset_MEDIUM_SLOW},
        {"MEDIUM_FAST", meshtastic_Config_LoRaConfig_ModemPreset_MEDIUM_FAST},
        {"SHORT_SLOW", meshtastic_Config_LoRaConfig_ModemPreset_SHORT_SLOW},
        {"SHORT_FAST", meshtastic_Config_LoRaConfig_ModemPreset_SHORT_FAST},
        {"LONG_MODERATE", meshtastic_Config_LoRaConfig_ModemPreset_LONG_MODERATE},
        {"SHORT_TURBO", meshtastic_Config_LoRaConfig_ModemPreset_SHORT_TURBO}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for modem preset is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_LoRaConfig_ModemPreset_LONG_FAST;
}

meshtastic_Config_BluetoothConfig_PairingMode RocketFlightDB::getPairingMode(std::string key)
{
    static std::map<std::string, meshtastic_Config_BluetoothConfig_PairingMode> values = {
        {"RANDOM_PIN", meshtastic_Config_BluetoothConfig_PairingMode_RANDOM_PIN},
        {"FIXED_PIN", meshtastic_Config_BluetoothConfig_PairingMode_FIXED_PIN},
        {"NO_PIN", meshtastic_Config_BluetoothConfig_PairingMode_NO_PIN}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for bluetooth pairing is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Config_BluetoothConfig_PairingMode_RANDOM_PIN;
}

meshtastic_Channel_Role RocketFlightDB::getChannelRole(std::string key)
{
    static std::map<std::string, meshtastic_Channel_Role> values = {{"DISABLED", meshtastic_Channel_Role_DISABLED},
                                                                    {"PRIMARY", meshtastic_Channel_Role_PRIMARY},
                                                                    {"SECONDARY", meshtastic_Channel_Role_SECONDARY}};

    auto value = values.find(key);
    if (value != values.end())
        return value->second;

    if (IsNotNull(key)) {
        LOG_ERROR("Config for channel role is unknown - could not find '%s'", key.c_str());
    }
    return meshtastic_Channel_Role_DISABLED;
}

// TODO TODO
// meshtastic_ScreenFonts RocketFlightDB::getScreenFont(std::string key)
// {
//     static std::map<std::string, meshtastic_ScreenFonts> values = {
//         {"FONT_SMALL", meshtastic_ScreenFonts_FONT_SMALL},
//         {"FONT_MEDIUM", meshtastic_ScreenFonts_FONT_MEDIUM},
//         {"FONT_LARGE", meshtastic_ScreenFonts_FONT_LARGE}};

//     auto value = values.find(key);
//     if (value != values.end())
//         return value->second;

//     if (IsNotNull(key))
//     {
//         LOG_ERROR("Config for font size is unknown - could not find '%s'", key.c_str());
//     }
//     return meshtastic_ScreenFonts_FONT_SMALL;
// }

// --------------------------------------------------------------------------------

std::string RocketFlightDB::getRebroadcastMode(meshtastic_Config_DeviceConfig_RebroadcastMode key)
{
    switch (key) {
    case meshtastic_Config_DeviceConfig_RebroadcastMode_ALL:
        return "ALL";
    case meshtastic_Config_DeviceConfig_RebroadcastMode_ALL_SKIP_DECODING:
        return "ALL_SKIP_DECODING";
    case meshtastic_Config_DeviceConfig_RebroadcastMode_LOCAL_ONLY:
        return "LOCAL_ONLY";
    case meshtastic_Config_DeviceConfig_RebroadcastMode_KNOWN_ONLY:
        return "KNOWN_ONLY";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getDeviceRole(meshtastic_Config_DeviceConfig_Role key)
{
    switch (key) {
    case meshtastic_Config_DeviceConfig_Role_CLIENT:
        return "CLIENT";
    case meshtastic_Config_DeviceConfig_Role_CLIENT_MUTE:
        return "CLIENT_MUTE";
    case meshtastic_Config_DeviceConfig_Role_ROUTER:
        return "ROUTER";
    case meshtastic_Config_DeviceConfig_Role_ROUTER_CLIENT:
        return "ROUTER_CLIENT";
    case meshtastic_Config_DeviceConfig_Role_REPEATER:
        return "REPEATER";
    case meshtastic_Config_DeviceConfig_Role_TRACKER:
        return "TRACKER";
    case meshtastic_Config_DeviceConfig_Role_SENSOR:
        return "SENSOR";
    case meshtastic_Config_DeviceConfig_Role_TAK:
        return "TAK";
    case meshtastic_Config_DeviceConfig_Role_CLIENT_HIDDEN:
        return "CLIENT_HIDDEN";
    case meshtastic_Config_DeviceConfig_Role_LOST_AND_FOUND:
        return "LOST_AND_FOUND";
    case meshtastic_Config_DeviceConfig_Role_TAK_TRACKER:
        return "TAK_TRACKER";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags key)
{
    switch (key) {
    case meshtastic_Config_PositionConfig_PositionFlags_UNSET:
        return "UNSET";
    case meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE:
        return "ALTITUDE";
    case meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL:
        return "ALTITUDE_MSL";
    case meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION:
        return "GEOIDAL_SEPARATION";
    case meshtastic_Config_PositionConfig_PositionFlags_DOP:
        return "DOP";
    case meshtastic_Config_PositionConfig_PositionFlags_HVDOP:
        return "HVDOP";
    case meshtastic_Config_PositionConfig_PositionFlags_SATINVIEW:
        return "SATINVIEW";
    case meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO:
        return "SEQ_NO";
    case meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP:
        return "TIMESTAMP";
    case meshtastic_Config_PositionConfig_PositionFlags_HEADING:
        return "HEADING";
    case meshtastic_Config_PositionConfig_PositionFlags_SPEED:
        return "SPEED";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getGpsMode(meshtastic_Config_PositionConfig_GpsMode key)
{
    switch (key) {
    case meshtastic_Config_PositionConfig_GpsMode_DISABLED:
        return "DISABLED";
    case meshtastic_Config_PositionConfig_GpsMode_ENABLED:
        return "ENABLED";
    case meshtastic_Config_PositionConfig_GpsMode_NOT_PRESENT:
        return "NOT_PRESENT";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getAddressMode(meshtastic_Config_NetworkConfig_AddressMode key)
{
    switch (key) {
    case meshtastic_Config_NetworkConfig_AddressMode_DHCP:
        return "DHCP";
    case meshtastic_Config_NetworkConfig_AddressMode_STATIC:
        return "STATIC";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getGpsFormat(meshtastic_Config_DisplayConfig_GpsCoordinateFormat key)
{
    switch (key) {
    case meshtastic_Config_DisplayConfig_GpsCoordinateFormat_DEC:
        return "DEC";
    case meshtastic_Config_DisplayConfig_GpsCoordinateFormat_DMS:
        return "DMS";
    case meshtastic_Config_DisplayConfig_GpsCoordinateFormat_UTM:
        return "UTM";
    case meshtastic_Config_DisplayConfig_GpsCoordinateFormat_MGRS:
        return "MGRS";
    case meshtastic_Config_DisplayConfig_GpsCoordinateFormat_OLC:
        return "OLC";
    case meshtastic_Config_DisplayConfig_GpsCoordinateFormat_OSGR:
        return "OSGR";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getDisplayUnits(meshtastic_Config_DisplayConfig_DisplayUnits key)
{
    switch (key) {
    case meshtastic_Config_DisplayConfig_DisplayUnits_METRIC:
        return "METRIC";
    case meshtastic_Config_DisplayConfig_DisplayUnits_IMPERIAL:
        return "IMPERIAL";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getOledType(meshtastic_Config_DisplayConfig_OledType key)
{
    switch (key) {
    case meshtastic_Config_DisplayConfig_OledType_OLED_AUTO:
        return "OLED_AUTO";
    case meshtastic_Config_DisplayConfig_OledType_OLED_SSD1306:
        return "OLED_SSD1306";
    case meshtastic_Config_DisplayConfig_OledType_OLED_SH1106:
        return "OLED_SH1106";
    case meshtastic_Config_DisplayConfig_OledType_OLED_SH1107:
        return "OLED_SH1107";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getDisplayMode(meshtastic_Config_DisplayConfig_DisplayMode key)
{
    switch (key) {
    case meshtastic_Config_DisplayConfig_DisplayMode_DEFAULT:
        return "DEFAULT";
    case meshtastic_Config_DisplayConfig_DisplayMode_TWOCOLOR:
        return "TWOCOLOR";
    case meshtastic_Config_DisplayConfig_DisplayMode_INVERTED:
        return "INVERTED";
    case meshtastic_Config_DisplayConfig_DisplayMode_COLOR:
        return "COLOR";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getCompassOrientation(meshtastic_Config_DisplayConfig_CompassOrientation key)
{
    switch (key) {
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_0:
        return "DEGREES_0";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_90:
        return "DEGREES_90";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_180:
        return "DEGREES_180";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_270:
        return "DEGREES_270";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_0_INVERTED:
        return "DEGREES_0_INVERTED";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_90_INVERTED:
        return "DEGREES_90_INVERTED";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_180_INVERTED:
        return "DEGREES_180_INVERTED";
    case meshtastic_Config_DisplayConfig_CompassOrientation_DEGREES_270_INVERTED:
        return "DEGREES_270_INVERTED";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getRegionCode(meshtastic_Config_LoRaConfig_RegionCode key)
{
    switch (key) {
    case meshtastic_Config_LoRaConfig_RegionCode_UNSET:
        return "UNSET";
    case meshtastic_Config_LoRaConfig_RegionCode_US:
        return "US";
    case meshtastic_Config_LoRaConfig_RegionCode_EU_433:
        return "EU_433";
    case meshtastic_Config_LoRaConfig_RegionCode_EU_868:
        return "EU_868";
    case meshtastic_Config_LoRaConfig_RegionCode_CN:
        return "CN";
    case meshtastic_Config_LoRaConfig_RegionCode_JP:
        return "JP";
    case meshtastic_Config_LoRaConfig_RegionCode_ANZ:
        return "ANZ";
    case meshtastic_Config_LoRaConfig_RegionCode_KR:
        return "KR";
    case meshtastic_Config_LoRaConfig_RegionCode_TW:
        return "TW";
    case meshtastic_Config_LoRaConfig_RegionCode_RU:
        return "RU";
    case meshtastic_Config_LoRaConfig_RegionCode_IN:
        return "IN";
    case meshtastic_Config_LoRaConfig_RegionCode_NZ_865:
        return "NZ_865";
    case meshtastic_Config_LoRaConfig_RegionCode_TH:
        return "TH";
    case meshtastic_Config_LoRaConfig_RegionCode_LORA_24:
        return "LORA_24";
    case meshtastic_Config_LoRaConfig_RegionCode_UA_433:
        return "UA_433";
    case meshtastic_Config_LoRaConfig_RegionCode_UA_868:
        return "UA_868";
    case meshtastic_Config_LoRaConfig_RegionCode_MY_433:
        return "MY_433";
    case meshtastic_Config_LoRaConfig_RegionCode_MY_919:
        return "MY_919";
    case meshtastic_Config_LoRaConfig_RegionCode_SG_923:
        return "SG_923";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getModemPreset(meshtastic_Config_LoRaConfig_ModemPreset key)
{
    switch (key) {
    case meshtastic_Config_LoRaConfig_ModemPreset_LONG_FAST:
        return "LONG_FAST";
    case meshtastic_Config_LoRaConfig_ModemPreset_LONG_SLOW:
        return "LONG_SLOW";
    case meshtastic_Config_LoRaConfig_ModemPreset_VERY_LONG_SLOW:
        return "VERY_LONG_SLOW";
    case meshtastic_Config_LoRaConfig_ModemPreset_MEDIUM_SLOW:
        return "MEDIUM_SLOW";
    case meshtastic_Config_LoRaConfig_ModemPreset_MEDIUM_FAST:
        return "MEDIUM_FAST";
    case meshtastic_Config_LoRaConfig_ModemPreset_SHORT_SLOW:
        return "SHORT_SLOW";
    case meshtastic_Config_LoRaConfig_ModemPreset_SHORT_FAST:
        return "SHORT_FAST";
    case meshtastic_Config_LoRaConfig_ModemPreset_LONG_MODERATE:
        return "LONG_MODERATE";
    case meshtastic_Config_LoRaConfig_ModemPreset_SHORT_TURBO:
        return "SHORT_TURBO";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getPairingMode(meshtastic_Config_BluetoothConfig_PairingMode key)
{
    switch (key) {
    case meshtastic_Config_BluetoothConfig_PairingMode_RANDOM_PIN:
        return "RANDOM_PIN";
    case meshtastic_Config_BluetoothConfig_PairingMode_FIXED_PIN:
        return "FIXED_PIN";
    case meshtastic_Config_BluetoothConfig_PairingMode_NO_PIN:
        return "NO_PIN";
    default:
        return "UNKNOWN";
    }
}

std::string RocketFlightDB::getChannelRole(meshtastic_Channel_Role key)
{
    switch (key) {
    case meshtastic_Channel_Role_DISABLED:
        return "DISABLED";
    case meshtastic_Channel_Role_PRIMARY:
        return "PRIMARY";
    case meshtastic_Channel_Role_SECONDARY:
        return "SECONDARY";
    default:
        return "UNKNOWN";
    }
}

// TODO TODO
// std::string RocketFlightDB::getScreenFont(meshtastic_ScreenFonts key)
// {
//     switch (key)
//     {
//     case meshtastic_ScreenFonts_FONT_SMALL:
//         return "FONT_SMALL";
//     case meshtastic_ScreenFonts_FONT_MEDIUM:
//         return "FONT_MEDIUM";
//     case meshtastic_ScreenFonts_FONT_LARGE:
//         return "FONT_LARGE";
//     default:
//         return "UNKNOWN";
//     }
// }

std::string RocketFlightDB::getHardwareModel(meshtastic_HardwareModel key)
{
    switch (key) {
    case meshtastic_HardwareModel_UNSET:
        return "UNSET";
    case meshtastic_HardwareModel_TLORA_V2:
        return "TLORA_V2";
    case meshtastic_HardwareModel_HELTEC_V2_1:
        return "HELTEC_V2_1";
    case meshtastic_HardwareModel_HELTEC_V1:
        return "HELTEC_V1";
    case meshtastic_HardwareModel_LILYGO_TBEAM_S3_CORE:
        return "LILYGO_TBEAM_S3_CORE";
    case meshtastic_HardwareModel_RAK11200:
        return "RAK11200";
    case meshtastic_HardwareModel_NANO_G1:
        return "NANO_G1";
    case meshtastic_HardwareModel_TLORA_V2_1_1P8:
        return "TLORA_V2_1_1P8";
    case meshtastic_HardwareModel_TLORA_T3_S3:
        return "TLORA_T3_S3";
    case meshtastic_HardwareModel_NANO_G1_EXPLORER:
        return "NANO_G1_EXPLORER";
    case meshtastic_HardwareModel_NANO_G2_ULTRA:
        return "NANO_G2_ULTRA";
    case meshtastic_HardwareModel_LORA_TYPE:
        return "LORA_TYPE";
    case meshtastic_HardwareModel_TLORA_V1:
        return "TLORA_V1";
    case meshtastic_HardwareModel_WIPHONE:
        return "WIPHONE";
    case meshtastic_HardwareModel_WIO_WM1110:
        return "WIO_WM1110";
    case meshtastic_HardwareModel_RAK2560:
        return "RAK2560";
    case meshtastic_HardwareModel_HELTEC_HRU_3601:
        return "HELTEC_HRU_3601";
    case meshtastic_HardwareModel_HELTEC_WIRELESS_BRIDGE:
        return "HELTEC_WIRELESS_BRIDGE";
    case meshtastic_HardwareModel_STATION_G1:
        return "STATION_G1";
    case meshtastic_HardwareModel_PRIVATE_HW:
        return "PRIVATE_HW";
    case meshtastic_HardwareModel_RAK11310:
        return "RAK11310";
    case meshtastic_HardwareModel_SENSELORA_RP2040:
        return "SENSELORA_RP2040";
    case meshtastic_HardwareModel_SENSELORA_S3:
        return "SENSELORA_S3";
    case meshtastic_HardwareModel_CANARYONE:
        return "CANARYONE";
    case meshtastic_HardwareModel_TLORA_V2_1_1P6:
        return "TLORA_V2_1_1P6";
    case meshtastic_HardwareModel_RP2040_LORA:
        return "RP2040_LORA";
    case meshtastic_HardwareModel_STATION_G2:
        return "STATION_G2";
    case meshtastic_HardwareModel_LORA_RELAY_V1:
        return "LORA_RELAY_V1";
    case meshtastic_HardwareModel_NRF52840DK:
        return "NRF52840DK";
    case meshtastic_HardwareModel_PPR:
        return "PPR";
    case meshtastic_HardwareModel_GENIEBLOCKS:
        return "GENIEBLOCKS";
    case meshtastic_HardwareModel_NRF52_UNKNOWN:
        return "NRF52_UNKNOWN";
    case meshtastic_HardwareModel_PORTDUINO:
        return "PORTDUINO";
    case meshtastic_HardwareModel_ANDROID_SIM:
        return "ANDROID_SIM";
    case meshtastic_HardwareModel_DIY_V1:
        return "DIY_V1";
    case meshtastic_HardwareModel_TBEAM:
        return "TBEAM";
    case meshtastic_HardwareModel_NRF52840_PCA10059:
        return "NRF52840_PCA10059";
    case meshtastic_HardwareModel_DR_DEV:
        return "DR_DEV";
    case meshtastic_HardwareModel_M5STACK:
        return "M5STACK";
    case meshtastic_HardwareModel_HELTEC_V3:
        return "HELTEC_V3";
    case meshtastic_HardwareModel_HELTEC_WSL_V3:
        return "HELTEC_WSL_V3";
    case meshtastic_HardwareModel_BETAFPV_2400_TX:
        return "BETAFPV_2400_TX";
    case meshtastic_HardwareModel_BETAFPV_900_NANO_TX:
        return "BETAFPV_900_NANO_TX";
    case meshtastic_HardwareModel_RPI_PICO:
        return "RPI_PICO";
    case meshtastic_HardwareModel_HELTEC_WIRELESS_TRACKER:
        return "HELTEC_WIRELESS_TRACKER";
    case meshtastic_HardwareModel_HELTEC_WIRELESS_PAPER:
        return "HELTEC_WIRELESS_PAPER";
    case meshtastic_HardwareModel_HELTEC_V2_0:
        return "HELTEC_V2_0";
    case meshtastic_HardwareModel_T_DECK:
        return "T_DECK";
    case meshtastic_HardwareModel_T_WATCH_S3:
        return "T_WATCH_S3";
    case meshtastic_HardwareModel_PICOMPUTER_S3:
        return "PICOMPUTER_S3";
    case meshtastic_HardwareModel_HELTEC_HT62:
        return "HELTEC_HT62";
    case meshtastic_HardwareModel_EBYTE_ESP32_S3:
        return "EBYTE_ESP32_S3";
    case meshtastic_HardwareModel_ESP32_S3_PICO:
        return "ESP32_S3_PICO";
    case meshtastic_HardwareModel_CHATTER_2:
        return "CHATTER_2";
    case meshtastic_HardwareModel_HELTEC_WIRELESS_PAPER_V1_0:
        return "HELTEC_WIRELESS_PAPER_V1_0";
    case meshtastic_HardwareModel_HELTEC_WIRELESS_TRACKER_V1_0:
        return "HELTEC_WIRELESS_TRACKER_V1_0";
    case meshtastic_HardwareModel_UNPHONE:
        return "UNPHONE";
    case meshtastic_HardwareModel_TBEAM_V0P7:
        return "TBEAM_V0P7";
    case meshtastic_HardwareModel_TD_LORAC:
        return "TD_LORAC";
    case meshtastic_HardwareModel_CDEBYTE_EORA_S3:
        return "CDEBYTE_EORA_S3";
    case meshtastic_HardwareModel_TWC_MESH_V4:
        return "TWC_MESH_V4";
    case meshtastic_HardwareModel_NRF52_PROMICRO_DIY:
        return "NRF52_PROMICRO_DIY";
    case meshtastic_HardwareModel_RADIOMASTER_900_BANDIT_NANO:
        return "RADIOMASTER_900_BANDIT_NANO";
    case meshtastic_HardwareModel_HELTEC_CAPSULE_SENSOR_V3:
        return "HELTEC_CAPSULE_SENSOR_V3";
    case meshtastic_HardwareModel_HELTEC_VISION_MASTER_T190:
        return "HELTEC_VISION_MASTER_T190";
    case meshtastic_HardwareModel_HELTEC_VISION_MASTER_E213:
        return "HELTEC_VISION_MASTER_E213";
    case meshtastic_HardwareModel_HELTEC_VISION_MASTER_E290:
        return "HELTEC_VISION_MASTER_E290";
    case meshtastic_HardwareModel_HELTEC_MESH_NODE_T114:
        return "HELTEC_MESH_NODE_T114";
    case meshtastic_HardwareModel_T_ECHO:
        return "T_ECHO";
    case meshtastic_HardwareModel_SENSECAP_INDICATOR:
        return "SENSECAP_INDICATOR";
    case meshtastic_HardwareModel_TRACKER_T1000_E:
        return "TRACKER_T1000_E";
    case meshtastic_HardwareModel_RAK3172:
        return "RAK3172";
    case meshtastic_HardwareModel_WIO_E5:
        return "WIO_E5";
    case meshtastic_HardwareModel_RADIOMASTER_900_BANDIT:
        return "RADIOMASTER_900_BANDIT";
    case meshtastic_HardwareModel_ME25LS01_4Y10TD:
        return "ME25LS01_4Y10TD";
    case meshtastic_HardwareModel_RP2040_FEATHER_RFM95:
        return "RP2040_FEATHER_RFM95";
    case meshtastic_HardwareModel_M5STACK_COREBASIC:
        return "M5STACK_COREBASIC";
    case meshtastic_HardwareModel_M5STACK_CORE2:
        return "M5STACK_CORE2";
    case meshtastic_HardwareModel_RPI_PICO2:
        return "RPI_PICO2";
    case meshtastic_HardwareModel_TLORA_V1_1P3:
        return "TLORA_V1_1P3";
    case meshtastic_HardwareModel_M5STACK_CORES3:
        return "M5STACK_CORES3";
    case meshtastic_HardwareModel_SEEED_XIAO_S3:
        return "SEEED_XIAO_S3";
    case meshtastic_HardwareModel_MS24SF1:
        return "MS24SF1";
    case meshtastic_HardwareModel_TLORA_C6:
        return "TLORA_C6";
    case meshtastic_HardwareModel_RAK4631:
        return "RAK4631";
    default:
        return "UNKNOWN";
    }
}

#endif
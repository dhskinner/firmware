#include "RocketFlightDB.h"

#ifdef ROCKETFLIGHT_CONFIG

extern meshtastic_LocalConfig config;
extern meshtastic_LocalModuleConfig moduleConfig;
extern meshtastic_ChannelFile channelFile;
extern meshtastic_User &owner;
extern std::string rocketFlightConfig;

JsonDocument RocketFlightDB::loadJson()
{
    // Allocate a temporary JsonDocument
    JsonDocument doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, userPrefJson);
    if (error)
        LOG_ERROR("RocketflightDB failed to read file, using default configuration");

    return doc;
}

pb_size_t RocketFlightDB::deserialiseByteArray(JsonArray &arr, uint8_t *out, size_t max)
{
    if (arr.size() > max)
    {
        return 0;
    }

    pb_size_t count = 0;
    for (JsonVariant value : arr)
    {
        out[count] = value.as<uint8_t>();
        count++;
    }
    return count;
}

void RocketFlightDB::deserialiseLocalConfig(JsonDocument &doc, meshtastic_LocalConfig &out)
{
    // --------------------------------------------------------------------------------
    // General
    JsonObject obj = doc["config"].as<JsonObject>();
    out.has_device = obj["has_device"] | out.has_device;
    out.has_position = obj["has_position"] | out.has_position;
    out.has_power = obj["has_power"] | out.has_power;
    out.has_network = obj["has_network"] | out.has_network;
    out.has_display = obj["has_display"] | out.has_display;
    out.has_lora = obj["has_lora"] | out.has_lora;
    out.has_bluetooth = obj["has_bluetooth"] | out.has_bluetooth;
    out.has_security = obj["has_security"] | out.has_security;

    // --------------------------------------------------------------------------------
    // Device

    out.device.serial_enabled =
        obj["device"]["serial_enabled"] | out.device.serial_enabled;
    out.device.button_gpio =
        obj["device"]["button_gpio"] | out.device.button_gpio;
    out.device.buzzer_gpio =
        obj["device"]["buzzer_gpio"] | out.device.buzzer_gpio;
    out.device.node_info_broadcast_secs =
        obj["device"]["node_info_broadcast_secs"] |
        out.device.node_info_broadcast_secs;
    out.device.double_tap_as_button_press =
        obj["device"]["double_tap_as_button_press"] |
        out.device.double_tap_as_button_press;
    out.device.is_managed =
        obj["device"]["is_managed"] | out.device.is_managed;
    out.device.disable_triple_click =
        obj["device"]["disable_triple_click"] |
        out.device.disable_triple_click;
    out.device.led_heartbeat_disabled =
        obj["device"]["led_heartbeat_disabled"] |
        out.device.led_heartbeat_disabled;

    // Strings
    std::string temp = std::string(out.device.tzdef);
    strlcpy(out.device.tzdef, obj["device"]["tzdef"] | temp.c_str(), sizeof(out.device.tzdef));

    // Enums
    std::string rebroadcast_mode = obj["device"]["rebroadcast_mode"];
    if (rebroadcast_mode.length() > 0)
        out.device.rebroadcast_mode = getRebroadcastMode(rebroadcast_mode);

    std::string role = obj["device"]["role"];
    if (role.length() > 0)
        out.device.role = getDeviceRole(role);

    // --------------------------------------------------------------------------------
    // Position

    out.position.position_broadcast_secs = obj["position"]["position_broadcast_secs"] | out.position.position_broadcast_secs;
    out.position.position_broadcast_smart_enabled = obj["position"]["position_broadcast_smart_enabled"] | out.position.position_broadcast_smart_enabled;
    out.position.fixed_position = obj["position"]["fixed_position"] | out.position.fixed_position;
    out.position.gps_enabled = obj["position"]["gps_enabled"] | out.position.gps_enabled;
    out.position.gps_update_interval = obj["position"]["gps_update_interval"] | out.position.gps_update_interval;
    out.position.rx_gpio = obj["position"]["rx_gpio"] | out.position.rx_gpio;
    out.position.tx_gpio = obj["position"]["tx_gpio"] | out.position.tx_gpio;
    out.position.gps_en_gpio = obj["position"]["gps_en_gpio"] | out.position.gps_en_gpio;
    out.position.broadcast_smart_minimum_distance = obj["position"]["broadcast_smart_minimum_distance"] | out.position.broadcast_smart_minimum_distance;
    out.position.broadcast_smart_minimum_interval_secs = obj["position"]["broadcast_smart_minimum_interval_secs"] | out.position.broadcast_smart_minimum_interval_secs;

    // Flags
    JsonArray flags = obj["position"]["position_flags"].as<JsonArray>();
    if (flags.size() > 0)
    {
        out.position.position_flags = meshtastic_Config_PositionConfig_PositionFlags_UNSET;
        for (JsonVariant v : flags)
        {
            out.position.position_flags |= getPositionFlag(v.as<std::string>());
        }
    }

    // Enums
    std::string gps_mode = obj["position"]["gps_mode"];
    if (gps_mode.length() > 0)
        out.position.gps_mode = getGpsMode(gps_mode);

    // --------------------------------------------------------------------------------
    // Power

    out.power.is_power_saving = obj["power"]["is_power_saving"] | out.power.is_power_saving;
    out.power.on_battery_shutdown_after_secs = obj["power"]["on_battery_shutdown_after_secs"] | out.power.on_battery_shutdown_after_secs;
    out.power.adc_multiplier_override = obj["power"]["adc_multiplier_override"] | out.power.adc_multiplier_override;
    out.power.wait_bluetooth_secs = obj["power"]["wait_bluetooth_secs"] | out.power.wait_bluetooth_secs;
    out.power.sds_secs = obj["power"]["sds_secs"] | out.power.sds_secs;
    out.power.ls_secs = obj["power"]["ls_secs"] | out.power.ls_secs;
    out.power.min_wake_secs = obj["power"]["min_wake_secs"] | out.power.min_wake_secs;
    out.power.device_battery_ina_address = obj["power"]["device_battery_ina_address"] | out.power.device_battery_ina_address;
    out.power.powermon_enables = obj["power"]["powermon_enables"] | out.power.powermon_enables;

    // --------------------------------------------------------------------------------
    // Network

    out.network.wifi_enabled = obj["network"]["wifi_enabled"] | out.network.wifi_enabled;
    out.network.eth_enabled = obj["network"]["eth_enabled"] | out.network.eth_enabled;
    out.network.has_ipv4_config = obj["network"]["has_ipv4_config"] | out.network.has_ipv4_config;

    // Enums
    std::string address_mode = obj["network"]["address_mode"];
    if (address_mode.length() > 0)
        out.network.address_mode = getAddressMode(address_mode);

    // Strings
    temp = std::string(out.network.wifi_ssid);
    strlcpy(out.network.wifi_ssid,
            obj["network"]["wifi_ssid"] | temp.c_str(),
            sizeof(out.network.wifi_ssid));

    temp = std::string(out.network.wifi_psk);
    strlcpy(out.network.wifi_psk,
            obj["network"]["wifi_psk"] | temp.c_str(),
            sizeof(out.network.wifi_psk));

    temp = std::string(out.network.ntp_server);
    strlcpy(out.network.ntp_server,
            obj["network"]["ntp_server"] | temp.c_str(),
            sizeof(out.network.ntp_server));

    temp = std::string(out.network.rsyslog_server);
    strlcpy(out.network.rsyslog_server,
            obj["network"]["rsyslog_server"] | temp.c_str(),
            sizeof(out.network.rsyslog_server));

    // --------------------------------------------------------------------------------
    // IPV4 Config (TODO TODO IP addresses should be in [0.0.0.0] string format)
    out.network.ipv4_config.ip = obj["network"]["ipv4_config"]["ip"] | out.network.ipv4_config.ip;
    out.network.ipv4_config.gateway = obj["network"]["ipv4_config"]["gateway"] | out.network.ipv4_config.gateway;
    out.network.ipv4_config.subnet = obj["network"]["ipv4_config"]["subnet"] | out.network.ipv4_config.subnet;
    out.network.ipv4_config.dns = obj["network"]["ipv4_config"]["dns"] | out.network.ipv4_config.dns;

    // --------------------------------------------------------------------------------
    // Display

    out.display.screen_on_secs = obj["display"]["screen_on_secs"] | out.display.screen_on_secs;
    out.display.auto_screen_carousel_secs = obj["display"]["auto_screen_carousel_secs"] | out.display.auto_screen_carousel_secs;
    out.display.compass_north_top = obj["display"]["compass_north_top"] | out.display.compass_north_top;
    out.display.flip_screen = obj["display"]["flip_screen"] | out.display.flip_screen;
    out.display.heading_bold = obj["display"]["heading_bold"] | out.display.heading_bold;
    out.display.wake_on_tap_or_motion = obj["display"]["wake_on_tap_or_motion"] | out.display.wake_on_tap_or_motion;

    // Enums
    std::string gps_format = obj["display"]["gps_format"];
    if (gps_format.length() > 0)
        out.display.gps_format = getGpsFormat(gps_format);
    std::string units = obj["display"]["units"];
    if (units.length() > 0)
        out.display.units = getDisplayUnits(units);
    std::string oled = obj["display"]["oled"];
    if (oled.length() > 0)
        out.display.oled = getOledType(oled);
    std::string displaymode = obj["display"]["displaymode"];
    if (displaymode.length() > 0)
        out.display.displaymode = getDisplayMode(displaymode);
    std::string compass_orientation = obj["display"]["compass_orientation"];
    if (compass_orientation.length() > 0)
        out.display.compass_orientation = getCompassOrientation(compass_orientation);

    // --------------------------------------------------------------------------------
    // LORA (TODO manual bandwidth, spreading factor etc is not implemented)

    out.lora.use_preset = obj["lora"]["use_preset"] | out.lora.use_preset;
    out.lora.hop_limit = obj["lora"]["hop_limit"] | out.lora.hop_limit;
    out.lora.tx_enabled = obj["lora"]["tx_enabled"] | out.lora.tx_enabled;
    out.lora.tx_power = obj["lora"]["tx_power"] | out.lora.tx_power;
    out.lora.channel_num = obj["lora"]["channel_num"] | out.lora.channel_num;
    out.lora.override_duty_cycle = obj["lora"]["override_duty_cycle"] | out.lora.override_duty_cycle;
    out.lora.sx126x_rx_boosted_gain = obj["lora"]["sx126x_rx_boosted_gain"] | out.lora.sx126x_rx_boosted_gain;
    out.lora.override_frequency = obj["lora"]["override_frequency"] | out.lora.override_frequency;
    out.lora.pa_fan_disabled = obj["lora"]["pa_fan_disabled"] | out.lora.pa_fan_disabled;
    out.lora.ignore_mqtt = obj["lora"]["ignore_mqtt"] | out.lora.ignore_mqtt;
    out.lora.config_ok_to_mqtt = obj["lora"]["config_ok_to_mqtt"] | out.lora.config_ok_to_mqtt;

    // Enums
    std::string modem_preset = obj["lora"]["modem_preset"];
    if (modem_preset.length() > 0)
        out.lora.modem_preset = getModemPreset(modem_preset);
    std::string region = obj["lora"]["region"];
    if (region.length() > 0)
        out.lora.region = getRegionCode(region);

    // --------------------------------------------------------------------------------
    // Bluetooth

    out.bluetooth.enabled = obj["bluetooth"]["enabled"] | out.bluetooth.enabled;
    out.bluetooth.fixed_pin = obj["bluetooth"]["fixed_pin"] | out.bluetooth.fixed_pin;

    // Enums
    std::string bluetooth = obj["bluetooth"]["mode"];
    if (bluetooth.length() > 0)
        out.bluetooth.mode = getPairingMode(bluetooth);

    // --------------------------------------------------------------------------------
    // Security

    out.security.is_managed = obj["security"]["is_managed"] | out.security.is_managed;
    out.security.serial_enabled = obj["security"]["serial_enabled"] | out.security.serial_enabled;
    out.security.admin_key_count = obj["security"]["admin_key_count"] | out.security.admin_key_count;

    JsonArray admin_keys = obj["security"]["admin_key"].as<JsonArray>();
    int c = 0;
    for (JsonVariant admin_key : admin_keys)
    {
        JsonArray admin_key_arr = admin_key.as<JsonArray>();
        out.security.admin_key[c].size = deserialiseByteArray(admin_key_arr, out.security.admin_key[c].bytes, sizeof(out.security.admin_key[c].bytes));
        c++;
    }

    out.security.debug_log_api_enabled = obj["security"]["debug_log_api_enabled"] | out.security.debug_log_api_enabled;
    out.security.admin_channel_enabled = obj["security"]["admin_channel_enabled"] | out.security.admin_channel_enabled;
}

void RocketFlightDB::deserialiseModuleConfig(JsonDocument &doc, meshtastic_LocalModuleConfig &out)
{
    // --------------------------------------------------------------------------------
    // Module flags
    JsonObject obj = doc["module"].as<JsonObject>();
    out.has_ambient_lighting = obj["has_ambient_lighting"] | out.has_ambient_lighting;
    out.has_mqtt = obj["has_mqtt"] | out.has_mqtt;
    out.has_serial = obj["has_serial"] | out.has_serial;
    out.has_external_notification = obj["has_external_notification"] | out.has_external_notification;
    out.has_store_forward = obj["has_store_forward"] | out.has_store_forward;
    out.has_range_test = obj["has_range_test"] | out.has_range_test;
    out.has_telemetry = obj["has_telemetry"] | out.has_telemetry;
    out.has_canned_message = obj["has_canned_message"] | out.has_canned_message;
    out.has_audio = obj["has_audio"] | out.has_audio;
    out.has_remote_hardware = obj["has_remote_hardware"] | out.has_remote_hardware;
    out.has_neighbor_info = obj["has_neighbor_info"] | out.has_neighbor_info;
    out.has_ambient_lighting = obj["has_ambient_lighting"] | out.has_ambient_lighting;
    out.has_detection_sensor = obj["has_detection_sensor"] | out.has_detection_sensor;
    out.has_paxcounter = obj["has_paxcounter"] | out.has_paxcounter;

    // --------------------------------------------------------------------------------
    // Telemetry Module

    out.telemetry.device_update_interval = obj["telemetry"]["device_update_interval"] | out.telemetry.device_update_interval;
    out.telemetry.environment_update_interval = obj["telemetry"]["environment_update_interval"] | out.telemetry.environment_update_interval;
    out.telemetry.environment_measurement_enabled = obj["telemetry"]["environment_measurement_enabled"] | out.telemetry.environment_measurement_enabled;
    out.telemetry.environment_screen_enabled = obj["telemetry"]["environment_screen_enabled"] | out.telemetry.environment_screen_enabled;
    out.telemetry.environment_display_fahrenheit = obj["telemetry"]["environment_display_fahrenheit"] | out.telemetry.environment_display_fahrenheit;
    out.telemetry.air_quality_enabled = obj["telemetry"]["air_quality_enabled"] | out.telemetry.air_quality_enabled;
    out.telemetry.air_quality_interval = obj["telemetry"]["air_quality_interval"] | out.telemetry.air_quality_interval;
    out.telemetry.power_measurement_enabled = obj["telemetry"]["power_measurement_enabled"] | out.telemetry.power_measurement_enabled;
    out.telemetry.power_update_interval = obj["telemetry"]["power_update_interval"] | out.telemetry.power_update_interval;
    out.telemetry.power_screen_enabled = obj["telemetry"]["power_screen_enabled"] | out.telemetry.power_screen_enabled;

    // TODO TODO - not implemented
    // The part of the config that is specific to the MQTT module
    // meshtastic_ModuleConfig_MQTTConfig mqtt;
    // The part of the config that is specific to the Serial module
    // meshtastic_ModuleConfig_SerialConfig serial;
    // The part of the config that is specific to the ExternalNotification module
    // meshtastic_ModuleConfig_ExternalNotificationConfig external_notification;
    // The part of the config that is specific to the Store & Forward module
    // meshtastic_ModuleConfig_StoreForwardConfig store_forward;
    // The part of the config that is specific to the RangeTest module
    // meshtastic_ModuleConfig_RangeTestConfig range_test;
    // The part of the config that is specific to the Telemetry module
    // meshtastic_ModuleConfig_TelemetryConfig telemetry;
    // The part of the config that is specific to the Canned Message module
    // meshtastic_ModuleConfig_CannedMessageConfig canned_message;
    // The part of the config that is specific to the Remote Hardware module
    // meshtastic_ModuleConfig_RemoteHardwareConfig remote_hardware;
    // The part of the config that is specific to the Neighbor Info module
    // meshtastic_ModuleConfig_NeighborInfout neighbor_info;
    // The part of the config that is specific to the Ambient Lighting module
    // meshtastic_ModuleConfig_AmbientLightingConfig ambient_lighting;
    // The part of the config that is specific to the Detection Sensor module
    // meshtastic_ModuleConfig_DetectionSensorConfig detection_sensor;
    // The part of the config that is specific to the Audio module
    // meshtastic_ModuleConfig_Audiout audio;
    // Paxcounter Config
    // meshtastic_ModuleConfig_PaxcounterConfig paxcounter;
}

void RocketFlightDB::deserialiseOwnerConfig(JsonDocument &doc, meshtastic_User &out)
{
    // --------------------------------------------------------------------------------
    // General
    JsonObject obj = doc["owner"].as<JsonObject>();
    out.is_licensed = obj["is_licensed"] | out.is_licensed;

    // Strings
    std::string temp = std::string(out.id);
    strlcpy(out.id, obj["id"] | temp.c_str(), sizeof(out.id));
    temp = std::string(out.long_name);
    strlcpy(out.long_name, obj["long_name"] | temp.c_str(), sizeof(out.long_name));
    temp = std::string(out.short_name);
    strlcpy(out.short_name, obj["short_name"] | temp.c_str(), sizeof(out.short_name));

    // Add a 3 digit number onto the owner name
    bool append_number = obj["append_number"] | false;
    if (append_number)
    {
        snprintf(out.long_name, sizeof(out.long_name), "%s %03d",
                 out.long_name, out.macaddr[5]);
        snprintf(out.short_name, sizeof(out.short_name), "%s%03d",
                 out.short_name, out.macaddr[5]);
    }

    // Enums
    std::string role = obj["role"];
    if (role.length() > 0)
        out.role = getDeviceRole(role);
}

// Channel settings
void RocketFlightDB::deserialiseChannelConfig(JsonDocument &doc, meshtastic_ChannelFile &out)
{
    JsonObject obj = doc["channels"].as<JsonObject>();
    out.channels_count = obj["channels_count"] | out.channels_count;
    JsonArray channelsArr = obj["channels"].as<JsonArray>();
    for (JsonObject channelObj : channelsArr)
    {
        uint8_t index = channelObj["index"].as<uint8_t>();
        if (index < 8)
        {
            out.channels[index].index = index;
            out.channels[index].has_settings = channelObj["has_settings"] | false;

            JsonObject settings = channelObj["settings"].as<JsonObject>();
            out.channels[index].settings.channel_num = settings["channel_num"] | index;

            JsonArray psk = settings["psk"].as<JsonArray>();
            out.channels[index].settings.psk.size = deserialiseByteArray(psk, out.channels[index].settings.psk.bytes, sizeof(out.channels[index].settings.psk.bytes));

            strlcpy(out.channels[index].settings.name, settings["name"] | "", sizeof(out.channels[index].settings.name));

            out.channels[index].settings.id = settings["id"] | 0;
            out.channels[index].settings.uplink_enabled = settings["uplink_enabled"] | false;
            out.channels[index].settings.downlink_enabled = settings["downlink_enabled"] | false;
            out.channels[index].settings.has_module_settings = settings["has_module_settings"] | false;

            JsonObject module_settings = settings["module_settings"].as<JsonObject>();
            out.channels[index].settings.module_settings.position_precision = module_settings["position_precision"] | 0;
            out.channels[index].settings.module_settings.is_client_muted = module_settings["is_client_muted"] | false;
            out.channels[index].role = getChannelRole(channelObj["role"].as<std::string>());
        }
    }
    out.version = obj["version"] | out.version;
}

#endif
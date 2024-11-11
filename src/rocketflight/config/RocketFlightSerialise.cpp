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

template <typename T>
void RocketFlightDB::serialiseByteArray(JsonObject &obj, std::string key, T &in)
{
    JsonArray array = obj[key].to<JsonArray>();
    for (int i = 0; i < in.size; i++)
    {
        array.add((uint8_t)in.bytes[i]);
    }
}

void RocketFlightDB::serialiseLocalConfig(JsonDocument &doc, meshtastic_LocalConfig &in)
{
    JsonObject obj = doc["config"].to<JsonObject>();
    obj["version"] = in.version;

    obj["has_device"] = in.has_device;
    if (in.has_device)
    {
        JsonObject device = obj["device"].to<JsonObject>();
        device["role"] = getDeviceRole(in.device.role);
        device["serial_enabled"] = in.device.serial_enabled;
        device["button_gpio"] = in.device.button_gpio;
        device["buzzer_gpio"] = in.device.buzzer_gpio;
        device["rebroadcast_mode"] = getRebroadcastMode(in.device.rebroadcast_mode);
        device["node_info_broadcast_secs"] = in.device.node_info_broadcast_secs;
        device["double_tap_as_button_press"] = in.device.double_tap_as_button_press;
        device["is_managed"] = in.device.is_managed;
        device["disable_triple_click"] = in.device.disable_triple_click;
        device["tzdef"] = in.device.tzdef;
        device["led_heartbeat_disabled"] = in.device.led_heartbeat_disabled;
    }

    obj["has_position"] = in.has_position;
    if (in.has_position)
    {
        JsonObject position = obj["position"].to<JsonObject>();
        position["position_broadcast_secs"] = in.position.position_broadcast_secs;
        position["position_broadcast_smart_enabled"] = in.position.position_broadcast_smart_enabled;
        position["fixed_position"] = in.position.fixed_position;
        position["gps_enabled"] = in.position.gps_enabled;
        position["gps_update_interval"] = in.position.gps_update_interval;
        position["gps_attempt_time"] = in.position.gps_attempt_time;

        JsonArray position_flags = position["position_flags"].to<JsonArray>();
        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_DOP)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_DOP));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_HVDOP)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_HVDOP));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_SATINVIEW)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_SATINVIEW));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_HEADING)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_HEADING));

        if (in.position.position_flags & meshtastic_Config_PositionConfig_PositionFlags_SPEED)
            position_flags.add(getPositionFlag(meshtastic_Config_PositionConfig_PositionFlags_SPEED));

        position["rx_gpio"] = in.position.rx_gpio;
        position["tx_gpio"] = in.position.tx_gpio;
        position["broadcast_smart_minimum_distance"] = in.position.broadcast_smart_minimum_distance;
        position["broadcast_smart_minimum_interval_secs"] = in.position.broadcast_smart_minimum_interval_secs;
        position["gps_en_gpio"] = in.position.gps_en_gpio;
        position["gps_mode"] = getGpsMode(in.position.gps_mode);
    }

    obj["has_power"] = in.has_power;
    if (in.has_power)
    {
        JsonObject power = obj["power"].to<JsonObject>();
        power["is_power_saving"] = in.power.is_power_saving;
        power["on_battery_shutdown_after_secs"] = in.power.on_battery_shutdown_after_secs;
        power["adc_multiplier_override"] = in.power.adc_multiplier_override;
        power["wait_bluetooth_secs"] = in.power.wait_bluetooth_secs;
        power["sds_secs"] = in.power.sds_secs;
        power["ls_secs"] = in.power.ls_secs;
        power["min_wake_secs"] = in.power.min_wake_secs;
        power["device_battery_ina_address"] = in.power.device_battery_ina_address;
        power["powermon_enables"] = in.power.powermon_enables;
    }

    obj["has_network"] = in.has_network;
    if (in.has_network)
    {
        JsonObject network = obj["network"].to<JsonObject>();
        network["wifi_enabled"] = in.network.wifi_enabled;
        network["wifi_ssid"] = in.network.wifi_ssid;
        network["wifi_psk"] = in.network.wifi_psk;
        network["ntp_server"] = in.network.ntp_server;
        network["eth_enabled"] = in.network.eth_enabled;
        network["address_mode"] = getAddressMode(in.network.address_mode);
        network["has_ipv4_config"] = in.network.has_ipv4_config;
        if (in.network.has_ipv4_config)
        {
            // TODO TODO
            // JsonObject ipv4_config = obj.createNestedObject("ipv4_config");
        }
        network["rsyslog_server"] = in.network.rsyslog_server;
    }

    obj["has_display"] = in.has_display;
    if (in.has_display)
    {
        JsonObject display = obj["display"].to<JsonObject>();
        display["screen_on_secs"] = in.display.screen_on_secs;
        display["gps_format"] = getGpsFormat(in.display.gps_format);
        display["auto_screen_carousel_secs"] = in.display.auto_screen_carousel_secs;
        display["compass_north_top"] = in.display.compass_north_top;
        display["flip_screen"] = in.display.flip_screen;
        display["units"] = getDisplayUnits(in.display.units);
        display["oled"] = getOledType(in.display.oled);
        display["displaymode"] = getDisplayMode(in.display.displaymode);
        display["heading_bold"] = in.display.heading_bold;
        display["wake_on_tap_or_motion"] = in.display.wake_on_tap_or_motion;
        display["compass_orientation"] = getCompassOrientation(in.display.compass_orientation);
    }

    obj["has_lora"] = in.has_lora;
    if (in.has_lora)
    {
        JsonObject lora = obj["lora"].to<JsonObject>();
        lora["use_preset"] = in.lora.use_preset;
        lora["modem_preset"] = getModemPreset(in.lora.modem_preset);
        lora["bandwidth"] = in.lora.bandwidth;
        lora["spread_factor"] = in.lora.spread_factor;
        lora["coding_rate"] = in.lora.coding_rate;
        lora["frequency_offset"] = in.lora.frequency_offset;
        lora["region"] = getRegionCode(in.lora.region);
        lora["hop_limit"] = in.lora.hop_limit;
        lora["tx_enabled"] = in.lora.tx_enabled;
        lora["tx_power"] = in.lora.tx_power;
        lora["channel_num"] = in.lora.channel_num;
        lora["override_duty_cycle"] = in.lora.override_duty_cycle;
        lora["sx126x_rx_boosted_gain"] = in.lora.sx126x_rx_boosted_gain;
        lora["override_frequency"] = in.lora.override_frequency;
        lora["pa_fan_disabled"] = in.lora.pa_fan_disabled;
        // lora["ignore_incoming_count"] = in.lora.ignore_incoming_count;
        // lora["ignore_incoming"] = in.lora.ignore_incoming;
        lora["ignore_mqtt"] = in.lora.ignore_mqtt;
        lora["config_ok_to_mqtt"] = in.lora.config_ok_to_mqtt;
    }

    obj["has_bluetooth"] = in.has_bluetooth;
    if (in.has_bluetooth)
    {
        JsonObject bluetooth = obj["bluetooth"].to<JsonObject>();
        bluetooth["enabled"] = in.bluetooth.enabled;
        bluetooth["mode"] = getPairingMode(in.bluetooth.mode);
        bluetooth["fixed_pin"] = in.bluetooth.fixed_pin;
    }

    obj["has_security"] = in.has_security;
    if (in.has_security)
    {
        JsonObject security = obj["security"].to<JsonObject>();
        serialiseByteArray(security, "public_key", in.security.public_key);
        serialiseByteArray(security, "private_key", in.security.private_key);
        security["admin_key_count"] = in.security.admin_key_count;
        JsonArray admin_key = security["admin_key"].to<JsonArray>();
        for (int n = 0; n < in.security.admin_key_count && n < 3; n++)
        {
            JsonArray admin_key_n = admin_key.add<JsonArray>();
            for (int i = 0; i < in.security.admin_key[n].size; i++)
            {
                admin_key_n.add((uint8_t)in.security.admin_key[n].bytes[i]);
            }
        }
        security["is_managed"] = in.security.is_managed;
        security["serial_enabled"] = in.security.serial_enabled;
        security["debug_log_api_enabled"] = in.security.debug_log_api_enabled;
        security["admin_channel_enabled"] = in.security.admin_channel_enabled;
    }
}

void RocketFlightDB::serialiseModuleConfig(JsonDocument &doc, meshtastic_LocalModuleConfig &in)
{
    JsonObject obj = doc["module"].to<JsonObject>();
    obj["version"] = in.version;
    obj["has_mqtt"] = in.has_mqtt;
    obj["has_serial"] = in.has_serial;
    obj["has_external_notification"] = in.has_external_notification;
    obj["has_store_forward"] = in.has_store_forward;
    obj["has_range_test"] = in.has_range_test;
    obj["has_telemetry"] = in.has_telemetry;
    obj["has_canned_message"] = in.has_canned_message;
    obj["has_audio"] = in.has_audio;
    obj["has_remote_hardware"] = in.has_remote_hardware;
    obj["has_neighbor_info"] = in.has_neighbor_info;
    obj["has_ambient_lighting"] = in.has_ambient_lighting;
    obj["has_detection_sensor"] = in.has_detection_sensor;
    obj["has_paxcounter"] = in.has_paxcounter;

    if (in.has_mqtt)
    {
        // TODO TODO
        // JsonObject mqtt = obj.createNestedObject("mqtt");
    }
    if (in.has_serial)
    {
        // TODO TODO
        // JsonObject serial = obj.createNestedObject("serial");
    }
    if (in.has_external_notification)
    {
        // TODO TODO
        // JsonObject external_notification = obj.createNestedObject("external_notification");
    }
    if (in.has_store_forward)
    {
        // TODO TODO
        // JsonObject store_forward = obj.createNestedObject("store_forward");
    }
    if (in.has_range_test)
    {
        // TODO TODO
        // JsonObject range_test = obj.createNestedObject("range_test");
    }
    if (in.has_telemetry)
    {
        JsonObject telemetry = obj["telemetry"].to<JsonObject>();
        telemetry["device_update_interval"] = in.telemetry.device_update_interval;
        telemetry["environment_update_interval"] = in.telemetry.environment_update_interval;
        telemetry["environment_measurement_enabled"] = in.telemetry.environment_measurement_enabled;
        telemetry["environment_screen_enabled"] = in.telemetry.environment_screen_enabled;
        telemetry["environment_display_fahrenheit"] = in.telemetry.environment_display_fahrenheit;
        telemetry["air_quality_enabled"] = in.telemetry.air_quality_enabled;
        telemetry["air_quality_interval"] = in.telemetry.air_quality_interval;
        telemetry["power_measurement_enabled"] = in.telemetry.power_measurement_enabled;
        telemetry["power_update_interval"] = in.telemetry.power_screen_enabled;
    }
    if (in.has_canned_message)
    {
        // TODO TODO
        // JsonObject canned_message = obj.createNestedObject("canned_message");
    }
    if (in.has_audio)
    {
        // TODO TODO
        // JsonObject audio = obj.createNestedObject("audio");
    }
    if (in.has_remote_hardware)
    {
        // TODO TODO
        // JsonObject remote_hardware = obj.createNestedObject("remote_hardware");
    }
    if (in.has_neighbor_info)
    {
        // TODO TODO
        // JsonObject neighbor_info = obj.createNestedObject("neighbor_info");
    }
    if (in.has_ambient_lighting)
    {
        // TODO TODO
        // JsonObject ambient_lighting = obj.createNestedObject("ambient_lighting");
    }
    if (in.has_detection_sensor)
    {
        // TODO TODO
        // JsonObject detection_sensor = obj.createNestedObject("detection_sensor");
    }
    if (in.has_paxcounter)
    {
        // TODO TODO
        // JsonObject paxcounter = obj.createNestedObject("paxcounter");
    }
}

void RocketFlightDB::serialiseOwnerConfig(JsonDocument &doc, meshtastic_User &in)
{
    JsonObject obj = doc["owner"].to<JsonObject>();
    obj["is_licensed"] = in.is_licensed;
    obj["long_name"] = in.long_name;
    obj["short_name"] = in.short_name;
    JsonArray macaddr = obj["macaddr"].to<JsonArray>();
    for (int c = 0; c < sizeof(in.macaddr); c++)
        macaddr.add(in.macaddr[c]);
    obj["hw_model"] = getHardwareModel(in.hw_model);
    obj["is_licensed"] = in.is_licensed;
    obj["role"] = getDeviceRole(in.role);
    serialiseByteArray(obj, "public_key", in.public_key);
}

void RocketFlightDB::serialiseChannelConfig(JsonDocument &doc, meshtastic_ChannelFile &in)
{
    JsonObject obj = doc["channels"].to<JsonObject>();
    obj["channels_count"] = in.channels_count;
    JsonArray channelArr = obj["channels"].to<JsonArray>();
    for (int n = 0; n < 8; n++)
    {
        JsonObject channelObj = channelArr.add<JsonObject>();
        channelObj["index"] = in.channels[n].index;
        channelObj["has_settings"] = in.channels[n].has_settings;

        JsonObject settingsObj = channelObj["settings"].to<JsonObject>();
        settingsObj["channel_num"] = in.channels[n].settings.channel_num;
        serialiseByteArray(settingsObj, "psk", in.channels[n].settings.psk);
        settingsObj["name"] = in.channels[n].settings.name;
        settingsObj["id"] = in.channels[n].settings.id;
        settingsObj["uplink_enabled"] = in.channels[n].settings.uplink_enabled;
        settingsObj["downlink_enabled"] = in.channels[n].settings.downlink_enabled;
        settingsObj["has_module_settings"] = in.channels[n].settings.has_module_settings;

        JsonObject moduleObj = settingsObj["module_settings"].to<JsonObject>();
        moduleObj["position_precision"] = in.channels[n].settings.module_settings.position_precision;
        moduleObj["is_client_muted"] = in.channels[n].settings.module_settings.is_client_muted;

        channelObj["role"] = getChannelRole(in.channels[n].role);
    } 
    obj["version"] = in.version;
}

#endif
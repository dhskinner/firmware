#pragma once
#ifndef _USERPREFS_
#define _USERPREFS_

// --------------------------------------------------------------------------------
// Configuration flags - almost everything can be adjusted - refer to
// the original protobuf definition files for the meaning of each field
#define ROCKETFLIGHT_CONFIG 1

// Display a custom screens for RocketFlight
#define ROCKETFLIGHT_SCREEN

// Enable the RocketFlight always-on GPS
#define ROCKETFLIGHT_GPS 1

// Enable the main RocketFlight module
#define ROCKETFLIGHT_MODULE 1

// --------------------------------------------------------------------------------
// Rocketflight settings

#ifdef ROCKETFLIGHT_CONFIG
#define ROCKETFLIGHT_CONFIG_FORCE_DEFAULT_SETTINGS 1
#define USERPREFS_TZ_STRING "AEST-10"
#define METERS_TO_FEET 3.28084
#include <string>
#include <Arduino.h>
#endif

// ----------------------------------------
#ifdef ROCKETFLIGHT_GPS
#ifdef GPS_THREAD_INTERVAL
#undef GPS_THREAD_INTERVAL
#endif
#define GPS_THREAD_INTERVAL 100
#define GPS_LOST_LOCK_MILLIS 5000
#endif

// ----------------------------------------
#ifdef ROCKETFLIGHT_SCREEN
#define USERPREFS_HAS_SPLASH 1
#endif

// ----------------------------------------
#ifdef ROCKETFLIGHT_MODULE
#define METERS_TO_FEET 3.28084
#define ROCKETFLIGHT_MODULE_INTERVAL 1000
#define ROCKETFLIGHT_MODULE_POSITION_PRECISION 31
#define ROCKETFLIGHT_MAX_CHANNEL_UTIL_PERCENT 60
#define ROCKETFLIGHT_REFERENCE_ALTITUDE_SAMPLE_SIZE 10
#define ROCKETFLIGHT_REFERENCE_ALTITUDE_SAMPLE_MILLIS 500

#define BMP3XX_INTERRUPT_PIN 46             // interrupt pin for the BMP3XX to signal a new reading

#define MOTION_SENSOR_CHECK_INTERVAL_MS 50  // default is 100 millis
#define ICM_20948_MPU_GYRO_SCALE dps1000    // dps250, dps500, dps1000, dps2000
#define ICM_20948_MPU_ACCEL_SCALE gpm16     // gpm2, gpm4, gpm8, gpm16
#define ICM_20948_INTERRUPT_PIN 5           // interrupt pin
#define ICM_20948_WOM_THRESHOLD 16U         // threshold for Wake on Motion Sensing (0mg to 1020mg)
// #define ICM_20948_DEBUG 1                // uncomment to enable debug on Serial
// #define ICM_20948_DMP_IS_ENABLED 1       // uncomment to enable the digital motion processor

// Exclude modules that are not relevant to rocketry
#define MESHTASTIC_EXCLUDE_WIFI 1
#define MESHTASTIC_EXCLUDE_MQTT 1
#define MESHTASTIC_EXCLUDE_AUDIO 1
#define MESHTASTIC_EXCLUDE_DETECTIONSENSOR 1
//#define MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR 1
#define MESHTASTIC_EXCLUDE_ENVIRONMENTAL_TELEMETRY 1
#define MESHTASTIC_EXCLUDE_EXTERNALNOTIFICATION 1
#define MESHTASTIC_EXCLUDE_PAXCOUNTER 1
#define MESHTASTIC_EXCLUDE_REMOTEHARDWARE 1
#define MESHTASTIC_EXCLUDE_STOREFORWARD 1
#define MESHTASTIC_EXCLUDE_TEXTMESSAGE 1
#define MESHTASTIC_EXCLUDE_ATAK 1
#define MESHTASTIC_EXCLUDE_CANNEDMESSAGES 1
#define MESHTASTIC_EXCLUDE_NEIGHBORINFO 1
#define MESHTASTIC_EXCLUDE_TRACEROUTE 1
#define MESHTASTIC_EXCLUDE_INPUTBROKER 1
#define MESHTASTIC_EXCLUDE_SERIAL 1
#define MESHTASTIC_EXCLUDE_POWERSTRESS 1
#define MESHTASTIC_EXCLUDE_POWERMON 1
#define MESHTASTIC_EXCLUDE_WAYPOINT 1
#define MESHTASTIC_EXCLUDE_AMBIENT_LIGHTING 1
#define MESHTASTIC_EXCLUDE_VEXT_ON_OFF 1

//#define EVENT_MODE 1
#define EXCLUDE_EMOJI 1
#define SHOW_REDRAWS 1
// #define MESHTASTIC_EXCLUDE_ADMIN 1   // TODO TODO do we need the admin module?
// #define MESHTASTIC_EXCLUDE_BLUETOOTH 1
// #define MESHTASTIC_EXCLUDE_GPS 1
// #define MESHTASTIC_EXCLUDE_SCREEN 1
// #define MESHTASTIC_EXCLUDE_I2C 1
// #define MESHTASTIC_EXCLUDE_PKI 1
// #define MESHTASTIC_EXCLUDE_POWER_FSM 1
// #define MESHTASTIC_EXCLUDE_TZ 1
// #define MESHTASTIC_EXCLUDE_POWER_TELEMETRY 1
// #define MESHTASTIC_EXCLUDE_RANGETEST 1
#endif

// ----------------------------------------------------------------------------------------------------
// Original Userprefs from Meshtastic

// Slipstream values:

// #define USERPREFS_TZ_STRING "tzplaceholder                                         "

// Uncomment and modify to set device defaults

// #define USERPREFS_EVENT_MODE 1

// #define USERPREFS_CONFIG_LORA_REGION meshtastic_Config_LoRaConfig_RegionCode_US
// #define USERPREFS_LORACONFIG_MODEM_PRESET meshtastic_Config_LoRaConfig_ModemPreset_SHORT_FAST
// #define USERPREFS_LORACONFIG_CHANNEL_NUM 31
// #define USERPREFS_CONFIG_LORA_IGNORE_MQTT true

// #define USERPREFS_CONFIG_GPS_MODE meshtastic_Config_PositionConfig_GpsMode_ENABLED

// #define USERPREFS_CHANNELS_TO_WRITE 3
/*
#define USERPREFS_CHANNEL_0_PSK \
    {                                                                                                                            \
        0x38, 0x4b, 0xbc, 0xc0, 0x1d, 0xc0, 0x22, 0xd1, 0x81, 0xbf, 0x36, 0xb8, 0x61, 0x21, 0xe1, 0xfb, 0x96, 0xb7, 0x2e, 0x55,  \
            0xbf, 0x74, 0x22, 0x7e, 0x9d, 0x6a, 0xfb, 0x48, 0xd6, 0x4c, 0xb1, 0xa1                                               \
    }
*/
// #define USERPREFS_CHANNEL_0_NAME "DEFCONnect"
// #define USERPREFS_CHANNEL_0_PRECISION 14
// #define USERPREFS_CHANNEL_0_UPLINK_ENABLED true
// #define USERPREFS_CHANNEL_0_DOWNLINK_ENABLED true
/*
#define USERPREFS_CHANNEL_1_PSK \
    {                                                                                                                            \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                               \
    }
*/
// #define USERPREFS_CHANNEL_1_NAME "REPLACEME"
// #define USERPREFS_CHANNEL_1_PRECISION 14
// #define USERPREFS_CHANNEL_1_UPLINK_ENABLED true
// #define USERPREFS_CHANNEL_1_DOWNLINK_ENABLED true
/*
#define USERPREFS_CHANNEL_2_PSK \
    {                                                                                                                            \
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  \
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00                                              \
    }
*/
// #define USERPREFS_CHANNEL_2_NAME "REPLACEME"
// #define USERPREFS_CHANNEL_2_PRECISION 14
// #define USERPREFS_CHANNEL_2_UPLINK_ENABLED true
// #define USERPREFS_CHANNEL_2_DOWNLINK_ENABLED true

// #define USERPREFS_CONFIG_OWNER_LONG_NAME "My Long Name"
// #define USERPREFS_CONFIG_OWNER_SHORT_NAME "MLN"

// #define USERPREFS_SPLASH_TITLE "DEFCONtastic"
// #define icon_width 34
// #define icon_height 29
// #define USERPREFS_HAS_SPLASH
/*
static unsigned char icon_bits[] = {
    0x00, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0xF0, 0x3F, 0x00, 0x00, 0x00, 0xF8, 0x7F, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x00, 0x00,
    0x9E, 0xE7, 0x00, 0x00, 0x00, 0x0E, 0xC7, 0x01, 0x00, 0x1C, 0x0F, 0xC7, 0x01, 0x00, 0x1C, 0xDF, 0xE7, 0x63, 0x00, 0x1C, 0xFF,
    0xBF, 0xE1, 0x00, 0x3C, 0xF3, 0xBF, 0xE3, 0x00, 0x7F, 0xF7, 0xBF, 0xF1, 0x00, 0xFF, 0xF7, 0xBF, 0xF9, 0x03, 0xFF, 0xE7, 0x9F,
    0xFF, 0x03, 0xC0, 0xCF, 0xEF, 0xDF, 0x03, 0x00, 0xDF, 0xE3, 0x8F, 0x00, 0x00, 0x7C, 0xFB, 0x03, 0x00, 0x00, 0xF8, 0xFF, 0x00,
    0x00, 0x00, 0xE0, 0x0F, 0x00, 0x00, 0x00, 0xC0, 0x0F, 0x00, 0x00, 0x00, 0x78, 0x3F, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x00, 0x00,
    0x98, 0x3F, 0xF0, 0x23, 0x00, 0xFC, 0x0F, 0xE0, 0x7F, 0x00, 0xFC, 0x03, 0x80, 0xFF, 0x01, 0xFC, 0x00, 0x00, 0x3E, 0x00, 0x70,
    0x00, 0x00, 0x1C, 0x00, 0x70, 0x00, 0x00, 0x1C, 0x00, 0x70, 0x00, 0x00, 0x1C, 0x00, 0x70, 0x00, 0x00, 0x1C, 0x00};
*/
/*
#define USERPREFS_USE_ADMIN_KEY 1
static unsigned char USERPREFS_ADMIN_KEY[] = {0xcd, 0xc0, 0xb4, 0x3c, 0x53, 0x24, 0xdf, 0x13, 0xca, 0x5a, 0xa6,
                                       0x0c, 0x0d, 0xec, 0x85, 0x5a, 0x4c, 0xf6, 0x1a, 0x96, 0x04, 0x1a,
                                       0x3e, 0xfc, 0xbb, 0x8e, 0x33, 0x71, 0xe5, 0xfc, 0xff, 0x3c};
*/
#endif
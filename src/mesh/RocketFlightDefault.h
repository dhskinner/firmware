#pragma once
#ifndef _ROCKETFLIGHT_DEFAULT_H_
#define _ROCKETFLIGHT_DEFAULT_H_

#include <NodeDB.h>
#include <cstdint>

#ifdef ROCKETFLIGHT_EDITION

#define ONE_DAY 24 * 60 * 60
#define ONE_MINUTE_MS 60 * 1000
#define THIRTY_SECONDS_MS 30 * 1000
#define FIVE_SECONDS_MS 5 * 1000

#define default_device_role meshtastic_Config_DeviceConfig_Role_TRACKER
#define default_node_info_broadcast_secs IF_TRACKER(1 * 60, 3 * 60 * 60)
#define default_power_is_power_saving IF_TRACKER(false, true)
#define default_broadcast_smart_minimum_distance IF_TRACKER(10, 100)
#define default_broadcast_smart_minimum_interval_secs IF_TRACKER(2, 30)
#define default_position_broadcast_smart_enabled true
#define default_gps_update_interval IF_TRACKER(1, IF_ROUTER(ONE_DAY, 1 * 60))
#define default_broadcast_interval_secs IF_TRACKER(60, IF_ROUTER(ONE_DAY / 2, 15 * 60))
#define default_rocketflight_module_interval 1000
#define default_position_precision 32

// Possible position flags are:
// | meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE            // include altitude (if available)
// | meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL        // set altitude to metres AMSL
// | meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP           // include local TZ (from GPS solution)
// | meshtastic_Config_PositionConfig_PositionFlags_SPEED               // include speed - TODO TODO for rocketry this should be the velocity vector (lateral or vertical speed m/s)
// | meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO              // include sequence number incremented per packet
// | meshtastic_Config_PositionConfig_PositionFlags_HEADING             // include heading in degrees
// | meshtastic_Config_PositionConfig_PositionFlags_GEOIDAL_SEPARATION  // include geodal separation
// | meshtastic_Config_PositionConfig_PositionFlags_DOP                 // include the dilution of precision value - PDOP is used by default, see below
// | meshtastic_Config_PositionConfig_PositionFlags_HVDOP               // if POS_DOP is set, send separate HDOP/VDOP values instead of PDOP
// | meshtastic_Config_PositionConfig_PositionFlags_SATINVIEW           // include satellites in view
#define default_position_flags meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE \
    | meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL \
    | meshtastic_Config_PositionConfig_PositionFlags_TIMESTAMP \
    | meshtastic_Config_PositionConfig_PositionFlags_SPEED \
    | meshtastic_Config_PositionConfig_PositionFlags_SEQ_NO

#define default_telemetry_broadcast_interval_secs IF_ROUTER(ONE_DAY / 2, 30 * 60)
#define default_wait_bluetooth_secs IF_ROUTER(1, 60)
#define default_sds_secs IF_ROUTER(ONE_DAY, UINT32_MAX) // Default to forever super deep sleep
#define default_ls_secs IF_ROUTER(ONE_DAY, 5 * 60)
#define default_min_wake_secs 10
#define default_screen_on_secs IF_ROUTER(1, 60 * 10)
#define default_neighbor_info_broadcast_secs 6 * 60 * 60
#define min_node_info_broadcast_secs IF_TRACKER(5 * 60, 60 * 60)
#define min_neighbor_info_broadcast_secs IF_TRACKER(30 * 60, 2 * 60 * 60)

#define IF_TRACKER(trackerVal, normalVal) \
    ((config.device.role == meshtastic_Config_DeviceConfig_Role_TRACKER) ? (trackerVal) : (normalVal))

#endif

#endif
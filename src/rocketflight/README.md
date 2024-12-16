# Meshtastic Rocketflight Edition

This is custom firmware load of Meshtastic designed for amateur and high power rocketry.

## Design Aims

Design aims are to provide:

- a capable rocketry tracker that 'just works' out-of-the-box
- all the goody-goodness from Meshtastic like their IoS/Android app
- easy to add-on sensors altimeters, accelerometers, data logging etc
- a pathway to upgrade to a full-feature dual deploy flight computer/datalogger
- firmware that has long term support i.e. from more than just one person
- cost effective at less than AUD$100 (about US$65) all up

## Features

Tracker:

- [x] Transmit position, distance, direction
- [x] Fancy IoS and Android Meshtastic app
- [x] Onboard status display screen
- [x] Set initial launch position and ground altitude
- [ ] Low rate data logging
- [ ] Battery status (% charge etc)
- [ ] Beep codes

Altimeter:

- [ ] Transmit altitude from any barometer supported by Meshtastic
- [ ] Transmit motion from any sensor supported by Meshtastic
- [ ] Transmit state of flight (boost, coast, apogee etc)
- [ ] High rate data logging
- [ ] Lost and found mode

Flight Computer:

- [ ] Dual deploy pyro channels

## Getting Started

### Compatible Hardware

In theory any combination of board and I2C sensors supported by Meshtastic should be supportable.

The firmware is currently optimised for:

- [Heltec Wireless Tracker](https://heltec.org/project/wireless-tracker/)
- BMP388 barometer/altimeter
- ICM-20948 motion processor
- MAX-17048 Lipo Fuel Gauge

More board and sensor options will be tested over time.

### Flashing the Firmware

Placeholder

## Development

### Building from Source

The code is based on a forked copy of the Meshtastic development repository at [https://github.com/rocketflight/meshtastic_firmware](https://github.com/rocketflight/meshtastic_firmware)

This repository contains the device firmware - follow the same instructions to build from Meshtastic:

**[Building Instructions](https://meshtastic.org/docs/development/firmware/build)**

### Source Code

The [RocketFlight](https://github.com/rocketflight/meshtastic_firmware) repo is synced regularly with the current main branch of Meshtastic i.e. this codebase is moving quickly and our aim is to keep up with the latest enhancements available.

Rocketflight also aims to be as compatible as possible and tries to avoid instrusive changes which would 'break' over time - this means:

- we use inbuilt compiler flags in Meshtastic to turn off unecessary modules and features
- the Rocketflight code is fairly self-contained in just a couple of places to hopefully avoid merge issues

  // the default NodeDB::NodeDB() constructor loads all the settings from
  // file - this will always overwrite all changes from the app, for the
  // settings lited below. Ideally this produce a working tracker,
  // out-of-the-box on first boot after a factory reset

There are three main building blocks added to Meshtastic:

#### RocketFlightModule

replaces PositionModule and does asynchronous (low rate) tasks:

- - send positions and altitude over the mesh (range from 1 sec to 10 sec - default 1 sec when moving / 1 min when idle)
- - set a launch position and altitude (can we use a waypoint?)
- - how do we get range and bearing / do we need it if we have the app? (probably not)
- - sending range and bearing from launch into the mesh (e.g. for i'm lost)
- - lost and found mode
- - data logging (range from 1Hz to 100Hz - default 10 hz)
- - beep codes
    \*/

/\* RocketFlightModule replaces PositionModule and does asynchronous (low rate) tasks:

- - send positions and altitude over the mesh (range from 1 sec to 10 sec - default 2 sec (smart))
- - set a launch position and altitude (can we use a waypoint?)
- - how do we get range and bearing / do we need it if we have the app? (probably not)
- - sending range and bearing from launch into the mesh (e.g. for i'm lost)
- - lost and found mode
- - data logging
- -
- RocketryThread replaces AccelerometerThread and does synchronous (high rate) tasks:
- - phase of flight
- - apogee detection
- - pyro channels
- -
- - // TODO TODO ideally this should work out of the box - from factory reset on first boot
    // TODO TODO setup the radio - perhaps all settings except channel?
    // speed should be vertical or horizontal

- 4. data logging (range from 1Hz to 100Hz - default 10 hz)
- 5. beeping out stuff - altitude and whatnot
- 6. power management - high rate when moving then scale back when idle
- 7. maybe a 'ready to launch' message?
- 8. lost and found mode
- 9. turn off everything unecessary - all the stupid modules
- if another tracker appears - show a modified screen
- or should we have a modified firmward load for airborne as opposed to ground station? would be nice to be the same and have a mode switch (e.g. jumper over or assume if say altimeters and whatnot are present then its flight mode... or maybe 'client' becomes the ground station)

tripleclick to zero it

add vertical speed to position updates (in metres per second)

This repository contains the device firmware for the Meshtastic project.

logo from https://www.online-utility.org/image/convert/to/XBM

## Features

Tracker:

- [ ] Add SPI data logging
- [ ] Add SPI data extraction.... how will this work?
- [ ] Add Openlog SD card
- [ ] Add sending altitude if available (e.g. if GPS is not locked send altitude anyway)
- [ ] Add sending smart altitude (e.g. if > m vertically)
- [ ] Add send positions and altitude over the mesh (range from 1 sec to 10 sec - default 1 sec when moving / 1 min when idle)
- [ ] Add set a launch position and altitude (can we use a waypoint?)
- [ ] Add how do we get range and bearing / do we need it if we have the app? (probably not)
- [ ] Add sending range and bearing from launch into the mesh (e.g. for i'm lost)
- [ ] Add lost and found mode
- [ ] Add data logging (range from 1Hz to 100Hz - default 10 hz)
- [ ] Add beep codes

## Issues and Bugs

- [ ] MAX17049 doesn't register USB power properly when the battery is disconnected
- [x] ICM-20948 Wake on Motion doesnt work when RocketFlight edition is enabled
- [x] [Meshtastic] the GPS_EXTRAVERBOSE flag causes an error in GPS.cpp line 885

## Projects

Some projects and docs worth bookmarking to come back to are:

- https://shanetully.com/2016/07/inside-the-construction-of-an-amateur-rocketry-flight-computer/

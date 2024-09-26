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
- [x] [IoS](https://apps.apple.com/us/app/meshtastic/id1586432531) and [Android](https://play.google.com/store/apps/details?id=com.geeksville.mesh) app from [Meshtastic](https://meshtastic.org/docs/software/)
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

[placeholder for more info]

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

[placeholder for more info]

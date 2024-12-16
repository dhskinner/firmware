// #include "RocketFlightThread.h"

// #if !defined(ARCH_PORTDUINO) && !defined(ARCH_STM32WL) && !MESHTASTIC_EXCLUDE_I2C && defined(ROCKETFLIGHT_FLIGHT)

// namespace RocketFlight
// {

// void RocketFlightThread::init(ScanI2C::FoundDevice accelerometer_found, ScanI2C::FoundDevice altimeter_found)
// {

//     if (isInitialised)
//         return;

//     // if (accelerometer_found.address.port == ScanI2C::I2CPort::NO_I2C || accelerometer_found.address.address == 0 ||
//     // accelerometer_found.type == ScanI2C::NONE) {
//     //     LOG_DEBUG("Motion is disabled due to no sensors found");
//     //     disable();
//     //     return;
//     // }

//     // #ifndef RAK_4631
//     //         if (!config.display.wake_on_tap_or_motion && !config.device.double_tap_as_button_press)
//     //         {
//     //             LOG_DEBUG("AccelerometerThread disabled due to no interested configurations");
//     //             disable();
//     //             return;
//     //         }
//     // #endif

//     // TODO TODO TODO Add an guard
//     // Global GPS status

//     gps.observe(&gpsStatus->onNewStatus);

//     switch (accelerometer_found.type) {
//     case ScanI2C::DeviceType::BMA423:
//         accelerometer = make_unique<BMA423Sensor>(accelerometer_found);
//         break;
//     case ScanI2C::DeviceType::MPU6050:
//         accelerometer = make_unique<MPU6050Sensor>(accelerometer_found);
//         break;
//     case ScanI2C::DeviceType::BMX160:
//         accelerometer = make_unique<BMX160Sensor>(accelerometer_found);
//         break;
//     case ScanI2C::DeviceType::LIS3DH:
//         accelerometer = make_unique<LIS3DHSensor>(accelerometer_found);
//         break;
//     case ScanI2C::DeviceType::LSM6DS3:
//         accelerometer = make_unique<LSM6DS3Sensor>(accelerometer_found);
//         break;
//     case ScanI2C::DeviceType::STK8BAXX:
//         accelerometer = make_unique<STK8XXXSensor>(accelerometer_found);
//         break;
//     case ScanI2C::DeviceType::ICM20948:
//         accelerometer = make_unique<ICM20948Sensor>(accelerometer_found);
//         break;
// #ifdef HAS_QMA6100P
//     case ScanI2C::DeviceType::QMA6100P:
//         accelerometer = new QMA6100PSensor > (accelerometer_found);
//         break;
// #endif
//     default:
//         disable();
//         return;
//     }
//     isInitialised = accelerometer->init();
//     LOG_INFO("Accelerometer type: %s status: %s", accelerometer->name(), isInitialised ? "ok" : "failed");

//     // get an altimeter if one is available
// #if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR
//     switch (altimeter_found.type) {
//     case ScanI2C::DeviceType::BME_680:
//         altimeter = make_unique<BME680Sensor>(altimeter_found);
//         break;
//     case ScanI2C::DeviceType::BME_280:
//         altimeter = make_unique<BME280Sensor>(altimeter_found);
//         break;
//     case ScanI2C::DeviceType::BMP_280:
//         altimeter = make_unique<BMP280Sensor>(altimeter_found);
//         break;
//     case ScanI2C::DeviceType::BMP_085:
//         altimeter = make_unique<BMP085Sensor>(altimeter_found);
//         break;
//     case ScanI2C::DeviceType::BMP_3XX:
//         altimeter = make_unique<BMP3XXSensor>(altimeter_found);
//         break;
//     case ScanI2C::DeviceType::LPS22HB:
//         altimeter = make_unique<LPS22HBSensor>(altimeter_found);
//         break;
//     default:
//         // is there a manual position?
//         if (config.position.fixed_position) {
//             altimeter = make_unique<Altimeter>(meshtastic_TelemetrySensorType::meshtastic_TelemetrySensorType_CUSTOM_SENSOR,
//                                                "Fixed", meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_MANUAL,
//                                                static_cast<double>(localPosition.altitude));
//             break;
//         }
// #ifdef ROCKETFLIGHT_POSITION
//         // is Rocketflight GPS available?
//         if (gps != nullptr) {
//             altimeter = gps->asAltimeter();
//             break;
//         }
// #endif
//         // fallback to nothing
//         altimeter = new Altimeter(meshtastic_TelemetrySensorType::meshtastic_TelemetrySensorType_SENSOR_UNSET, "None",
//                                   meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_UNSET);
//         break;
//     }
//     LOG_INFO("Altimeter type: %s status: %s", altimeter->name(), altimeter->isValid() ? "ok" : "failed");
// #endif

//     // return sensor->runOnce();
//     //         if (!isInitialised)
//     //         {
//     //             clean();
//     //         }
//     //         LOG_DEBUG("AccelerometerThread::init %s", isInitialised ? "ok" : "failed");
// }

// void RocketFlightThread::run() {}

// int RocketFlightThread::handleGpsUpdate(const meshtastic::Status *arg)
// {

//     // // LOG_DEBUG("Screen got status update %d", arg->getStatusType());
//     // switch (arg->getStatusType()) {
//     // case STATUS_TYPE_NODE:
//     //     if (showingNormalScreen && nodeStatus->getLastNumTotal() != nodeStatus->getNumTotal()) {
//     //         setFrames(FOCUS_PRESERVE); // Regen the list of screen frames (returning to same frame, if possible)
//     //     }
//     //     nodeDB->updateGUI = false;
//     //     break;
//     // }

//     return 0;
// }

// } // namespace RocketFlight

// #endif

// #pragma once
// #ifndef _ROCKETFLIGHT_MOTION_H_
// #define _ROCKETFLIGHT_MOTION_H_

// #include "configuration.h"

// #if !defined(ARCH_PORTDUINO) && !defined(ARCH_STM32WL) && !MESHTASTIC_EXCLUDE_I2C && defined(ROCKETFLIGHT_FLIGHT)

// namespace RocketFlight
// {

// #include "GPSStatus.h"
// #include "thread/Measurement.h"
// #include <atomic>
// #include <memory>

// // Items from the global namespace
// using ::std::forward;
// using ::std::unique_ptr;

// // Altimeters
// #include "modules/Telemetry/Sensor/BME280Sensor.h"
// #include "modules/Telemetry/Sensor/BME680Sensor.h"
// #include "modules/Telemetry/Sensor/BMP085Sensor.h"
// #include "modules/Telemetry/Sensor/BMP280Sensor.h"
// #include "modules/Telemetry/Sensor/BMP3XXSensor.h"
// #include "modules/Telemetry/Sensor/LPS22HBSensor.h"

// // Accelerometers
// #include "motion/BMA423Sensor.h"
// #include "motion/BMX160Sensor.h"
// #include "motion/ICM20948Sensor.h"
// #include "motion/LIS3DHSensor.h"
// #include "motion/LSM6DS3Sensor.h"
// #include "motion/MPU6050Sensor.h"
// #include "motion/STK8XXXSensor.h"
// #ifdef HAS_QMA6100P
// #include "motion/QMA6100PSensor.h"
// #endif

// template <typename T, typename... Args> unique_ptr<T> make_unique(Args &&...args)
// {
//     return unique_ptr<T>(new T(forward<Args>(args)...));
// }

// extern meshtastic::GPSStatus *gpsStatus;

// // This thread is launched by using
// // function object as callable
// // thread th2(thread_obj(), 3);
// // https://www.geeksforgeeks.org/implement-thread-safe-queue-in-c/
// // https://www.geeksforgeeks.org/multithreading-in-cpp/
// class RocketFlightThread
// {

//   protected:
//     // sensors
//     unique_ptr<MotionSensor> accelerometer = nullptr;
//     unique_ptr<Altimeter> altimeter = nullptr;
//     CallbackObserver<RocketFlightThread, const meshtastic::Status *> gps =
//         CallbackObserver<RocketFlightThread, const meshtastic::Status *>(this, &RocketFlightThread::handleGpsUpdate);

//     // most recent data
//     Measurement<double> altitudeMetres;
//     Measurement<double> accelerationG;

//     bool isInitialised = false;

//     int RocketFlightThread::handleGpsUpdate(const meshtastic::Status *arg);

//   public:
//     void operator()(ScanI2C::FoundDevice accelerometer_found, ScanI2C::FoundDevice altimeter_found)
//     {
//         init(accelerometer_found, altimeter_found);
//     }

//     void init(ScanI2C::FoundDevice accelerometer_found, ScanI2C::FoundDevice altimeter_found);
//     void run();
// };

// } // namespace RocketFlight

// #endif

// #endif

#pragma once
#ifndef _ROCKETFLIGHT_MOTION_H_
#define _ROCKETFLIGHT_MOTION_H_

#include "configuration.h"

#if !defined(ARCH_PORTDUINO) && !defined(ARCH_STM32WL) && !MESHTASTIC_EXCLUDE_I2C && defined(ROCKETFLIGHT_FLIGHT)

#include "modules/Telemetry/Sensor/Altimeter.h"
#include "motion/AccelerometerThread.h"

// C++ implementation of the above approach
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

// Thread-safe queue
template <typename T> class TSQueue
{
  private:
    // Underlying queue
    std::queue<T> m_queue;

    // mutex for thread synchronization
    std::mutex m_mutex;

    // Condition variable for signaling
    std::condition_variable m_cond;

  public:
    // Pushes an element to the queue
    void push(T item)
    {

        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Add item
        m_queue.push(item);

        // Notify one thread that
        // is waiting
        m_cond.notify_one();
    }

    // Pops an element off the queue
    T pop()
    {

        // acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // wait until queue is not empty
        m_cond.wait(lock, [this]() { return !m_queue.empty(); });

        // retrieve item
        T item = m_queue.front();
        m_queue.pop();

        // return item
        return item;
    }
};

class RocketFlightThread : public AccelerometerThread
{
  public:
    explicit RocketFlightThread(ScanI2C::FoundDevice foundDevice) : AccelerometerThread(foundDevice)
    {
        device = foundDevice;
        init();
    }

  protected:
    MotionSensor *motion = nullptr;
    Altimeter *altimeter = nullptr;

    int32_t runOnce() override { return AccelerometerThread::runOnce(); }

    void init()
    {
        // init the motion sensor
        AccelerometerThread::init();

        // init the altimeter

        if (!isInitialised) {
            LOG_DEBUG("AccelerometerThread disable due to initialisation error");
            // TODO TODO NEED A BRIGHT ERROR AND BEEPING
            return;
        }
    }

    // Copy constructor (not implemented / included to avoid cppcheck warnings)
    RocketFlightThread(const RocketFlightThread &other) : AccelerometerThread(other.device) { this->copy(other); }

    // Destructor (included to avoid cppcheck warnings)
    virtual ~RocketFlightThread() { clean(); }

    // Copy assignment (not implemented / included to avoid cppcheck warnings)
    RocketFlightThread &operator=(const RocketFlightThread &other)
    {
        this->copy(other);
        return *this;
    }

    // Take a very shallow copy (does not copy OSThread state nor the sensor object)
    // If for some reason this is ever used, make sure to call init() after any copy
    void copy(const RocketFlightThread &other)
    {
        if (this != &other) {
            clean();
            this->device = ScanI2C::FoundDevice(other.device.type,
                                                ScanI2C::DeviceAddress(other.device.address.port, other.device.address.address));
        }
    }

    // Cleanup resources
    void clean()
    {
        isInitialised = false;
        if (sensor != nullptr) {
            delete sensor;
            sensor = nullptr;
        }
    }
};

#endif

#endif
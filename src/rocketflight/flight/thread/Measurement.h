#pragma once
#ifndef _MEASUREMENT_H_
#define _MEASUREMENT_H_

#include <mutex>
#include <stdio.h>

namespace RocketFlight
{

// Store measurement values for concurrent reading and writing from different
// threads, including the time the value was last updated
template <class T> class Measurement
{
  private:
    T value;
    unsigned long time = 0;
    bool updated = false;
    std::mutex guard;

  public:
    // Set the measurement value (thread safe))
    void Set(double val)
    {
        const std::lock_guard<std::mutex> lock(guard);
        value = val;
        time = millis();
        updated = true;
    };

    // Get the current value (thread safe)
    double Read()
    {
        const std::lock_guard<std::mutex> lock(guard);
        updated = false;
        return value;
    }

    // Get the Last updated millis (thread safe)
    unsigned long LastUpdated()
    {
        const std::lock_guard<std::mutex> lock(guard);
        return millis;
    }

    // check whether a new value has been updated since the last read
    bool IsUpdated()
    {
        const std::lock_guard<std::mutex> lock(guard);
        return updated;
    }
};

} // namespace RocketFlight

#endif

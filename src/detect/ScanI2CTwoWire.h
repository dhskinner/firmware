#pragma once

#include "configuration.h"
#if !MESHTASTIC_EXCLUDE_I2C

#include <map>
#include <memory>
#include <stddef.h>
#include <stdint.h>

#include <Wire.h>

#include "ScanI2C.h"

#ifndef ROCKETFLIGHT_MODULE
#include "../concurrency/Lock.h"
#endif

class ScanI2CTwoWire : public ScanI2C
{
  public:
    void scanPort(ScanI2C::I2CPort) override;

    void scanPort(ScanI2C::I2CPort, uint8_t *, uint8_t) override;

    ScanI2C::FoundDevice find(ScanI2C::DeviceType) const override;

    TwoWire *fetchI2CBus(ScanI2C::DeviceAddress) const;

    bool exists(ScanI2C::DeviceType) const override;

    size_t countDevices() const override;

  protected:
    FoundDevice firstOfOrNONE(size_t, DeviceType[]) const override;

  private:
    typedef struct RegisterLocation {
        DeviceAddress i2cAddress;
        RegisterAddress registerAddress;

        RegisterLocation(DeviceAddress deviceAddress, RegisterAddress registerAddress)
            : i2cAddress(deviceAddress), registerAddress(registerAddress)
        {
        }

    } RegisterLocation;

    typedef uint8_t ResponseWidth;

#ifdef ROCKETFLIGHT_MODULE
    std::map<ScanI2C::DeviceAddress, ScanI2C::DeviceType> foundDevices = {{ ScanI2C::ADDRESS_NONE, ScanI2C::DeviceType::NONE}};
    std::map<ScanI2C::DeviceType, ScanI2C::DeviceAddress> deviceAddresses = {{ScanI2C::DeviceType::NONE, ScanI2C::ADDRESS_NONE}};
#else
    std::map<ScanI2C::DeviceAddress, ScanI2C::DeviceType> foundDevices;

    // note: prone to overwriting if multiple devices of a type are added at different addresses (rare?)
    std::map<ScanI2C::DeviceType, ScanI2C::DeviceAddress> deviceAddresses;

    concurrency::Lock lock;
#endif

    void printATECCInfo() const;

    uint16_t getRegisterValue(const RegisterLocation &, ResponseWidth) const;

    DeviceType probeOLED(ScanI2C::DeviceAddress) const;
};
#endif
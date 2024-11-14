#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "Altimeter.h"
#include "TelemetrySensor.h"
#include <Adafruit_BMP280.h>

class BMP280Sensor : public TelemetrySensor, public Altimeter
{
  protected:
    Adafruit_BMP280 bmp280;
    virtual void setup() override;

  public:
    BMP280Sensor();
    ~BMP280Sensor();
    virtual int32_t runOnce() override;
    virtual bool getMetrics(meshtastic_Telemetry *measurement) override;
    virtual double getAltitude() override;

    // Return true if the sensor is ok
    inline bool isValid() override { return status; };
};

#endif
#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "TelemetrySensor.h"
#include "Altimeter.h"
#include <Adafruit_BMP085.h>

class BMP085Sensor : public TelemetrySensor, public Altimeter
{
  protected:
    Adafruit_BMP085 bmp085;
    virtual void setup() override;

  public:
    BMP085Sensor();
    ~BMP085Sensor();
    virtual int32_t runOnce() override;
    virtual bool getMetrics(meshtastic_Telemetry *measurement) override;
    virtual double getAltitude() override;

   // Return true if the sensor is ok
    inline bool isValid() override { return status; };
};

#endif
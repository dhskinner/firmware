#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "Altimeter.h"
#include "TelemetrySensor.h"
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>

class LPS22HBSensor : public TelemetrySensor, public Altimeter
{
  protected:
    Adafruit_LPS22 lps22hb;
    virtual void setup() override;

  public:
    LPS22HBSensor();
    ~LPS22HBSensor();
    virtual int32_t runOnce() override;
    virtual bool getMetrics(meshtastic_Telemetry *measurement) override;
    virtual double getAltitude() override;

    // Return true if the sensor is ok
    inline bool isValid() override { return status; };
};

#endif
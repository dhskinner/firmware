#include "RocketFlightModule.h"

#ifdef ROCKETFLIGHT_POSITION

bool RocketFlightModule::wantUIFrame()
{
    return true;
}

void RocketFlightModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    /*
    --------------------------------------------------------------------------------
    | battery, %, charging    nodes in view (%channel utilisation)      satellites |
    | short name                                                        baro alt   |
    |                                                                              |
    |                                                                              |
    |                                                                              |
    |                                                                              |
    |                                altitude                                      |
    |                                                                              |
    |                                                                              |
    |                                                                              |
    |  / -----              ( launched, landed, apogee )                           |
    |  |     |                 (apogee, agl, pressure)                             |
    |  | SD  |                                                                     |
    |  -------                      . . . . . . .                                  |
    --------------------------------------------------------------------------------

    change the node display to show altitude and whatnot
    does admin mode disable the app?
    add buzzer
    turn on high rate altitude and motion
    make sure gps never turns off  disable this
    
    */

    // Setup the display
    display->setFont(FONT_SMALL);
    display->setTextAlignment(TEXT_ALIGN_LEFT);
    if (config.display.displaymode == meshtastic_Config_DisplayConfig_DisplayMode_INVERTED)
    {
        display->fillRect(0 + x, 0 + y, x + display->getWidth(), y + FONT_HEIGHT_SMALL);
        display->setColor(BLACK);
    }

    // Display power status
    if (powerStatus->getHasBattery())
    {
        if (config.display.displaymode == meshtastic_Config_DisplayConfig_DisplayMode_DEFAULT)
            drawBattery(display, x, y + 2, powerStatus);
        else
            drawBattery(display, x + 1, y + 3, powerStatus);
    }
    else if (powerStatus->knowsUSB())
    {
        if (config.display.displaymode == meshtastic_Config_DisplayConfig_DisplayMode_DEFAULT)
            display->drawFastImage(x, y + 2, 16, 8, powerStatus->getHasUSB() ? imgUSB : imgPower);
        else
            display->drawFastImage(x + 1, y + 3, 16, 8, powerStatus->getHasUSB() ? imgUSB : imgPower);
    }

    // Display nodes status
    drawNodes(display, x + (display->getWidth() / 2), y + 2, nodeStatus);

#if HAS_GPS
    // Display GPS status
    drawGPS(display, x + display->getWidth() - 1, y + 2, gpsStatus);
#endif

    // Display the node shortname
    drawText(display, x, y + FONT_HEIGHT_SMALL, TEXT_ALIGN_LEFT, owner.short_name);

    // Display altitude and mode
    if (altimeter == nullptr) {
        drawAltitude(display, x, y + FONT_HEIGHT_SMALL, 0.0);
        drawText(display, x + display->getWidth() - 1, y + FONT_HEIGHT_SMALL, TEXT_ALIGN_RIGHT, "No Alt");
    }
    else {
        uint32_t pos_flags = config.position.position_flags;
        String level = "AMSL";
        if(pos_flags & meshtastic_Config_PositionConfig_PositionFlags_ALTITUDE_MSL) {
            drawAltitude(display, x, y + FONT_HEIGHT_SMALL, altimeter->getAltitude());
        } else {
            drawAltitude(display, x, y + FONT_HEIGHT_SMALL, altimeter->getAltitude() - altimeter->getReferenceAltitude());
            level = "AGL";
        }

        switch (altimeter->getAltitudeSource())
        {
        case meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_BAROMETRIC:
            drawText(display, x + display->getWidth() - 1, y + FONT_HEIGHT_SMALL, TEXT_ALIGN_RIGHT, "Baro " + level);
            break;
        case meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_EXTERNAL:
        case meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_INTERNAL:
            drawText(display, x + display->getWidth() - 1, y + FONT_HEIGHT_SMALL, TEXT_ALIGN_RIGHT, "GPS " + level);
            break;
        case meshtastic_Position_AltSource::meshtastic_Position_AltSource_ALT_MANUAL:
            drawText(display, x + display->getWidth() - 1, y + FONT_HEIGHT_SMALL, TEXT_ALIGN_RIGHT, "Fixed");
            break;
        default:
            break;
        };
    }

    // Display a pixel each time a GPS position is received

    // Display a heartbeat pixel that blinks every time the frame is redrawn
    if (heartbeat)
    {
        display->drawRect(display->getWidth() - 2, 0, 2, 2);
    }
    heartbeat = !heartbeat;

    // Draw any log messages
    // drawChannelName(display, x, y + FONT_HEIGHT_SMALL);
    // display->drawLogBuffer(x, y + (FONT_HEIGHT_SMALL * 2));
}

extern uint8_t imgBattery[];

// Draw power bars or a charging indicator on an image of a battery, determined by battery charge voltage or percentage.
void RocketFlightModule::drawBattery(OLEDDisplay *display, int16_t x, int16_t y, const meshtastic::PowerStatus *powerStatus)
{
    static const uint8_t powerBar[3] = {0x81, 0xBD, 0xBD};
    static const uint8_t lightning[8] = {0xA1, 0xA1, 0xA5, 0xAD, 0xB5, 0xA5, 0x85, 0x85};

    // This image definition is here instead of images.h because it's modified dynamically by the drawBattery function
    uint8_t imgBattery[16] = {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xE7, 0x3C};

    // If charging, draw a charging indicator
    if (powerStatus->getIsCharging())
    {
        memcpy(imgBattery + 3, lightning, 8);
        // If not charging, Draw power bars
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if (powerStatus->getBatteryChargePercent() >= 25 * i)
                memcpy(imgBattery + 1 + (i * 3), powerBar, 3);
        }
    }
    display->drawFastImage(x, y, 16, 8, imgBattery);
}

// Draw nodes status
void RocketFlightModule::drawNodes(OLEDDisplay *display, int16_t x, int16_t y, const meshtastic::NodeStatus *nodeStatus)
{
    char usersString[20];
    snprintf(usersString, sizeof(usersString), "%d (%2.0f%%)", nodeStatus->getNumOnline(), airTime->channelUtilizationPercent());
    drawText(display, x, y - 2, TEXT_ALIGN_CENTER, usersString);
    x -= (display->getStringWidth(usersString) / 2 + 10);
    display->drawFastImage(x, y, 8, 8, imgUser);
}

#if HAS_GPS

// Draw GPS status summary
void RocketFlightModule::drawGPS(OLEDDisplay *display, int16_t x, int16_t y, const meshtastic::GPSStatus *gps)
{
    // Threshold values for the GPS lock accuracy bar display
    static const uint32_t dopThresholds[5] = {2000, 1000, 500, 200, 100};

    // Is the GPS present
    if (config.position.gps_mode == meshtastic_Config_PositionConfig_GpsMode_NOT_PRESENT || !gps->getIsConnected())
    {
        drawText(display, x, y - 2, TEXT_ALIGN_RIGHT, "No GPS");
        return;
    }

    // Is the GPS enabled
    if (config.position.gps_mode == meshtastic_Config_PositionConfig_GpsMode_DISABLED)
    {
        drawText(display, x, y - 2, TEXT_ALIGN_RIGHT, "GPS Off");
        return;
    }

    // Are GPS coordinates currently fixed
    if (config.position.fixed_position)
    {
        drawText(display, x, y - 2, TEXT_ALIGN_RIGHT, "GPS Fixed");
        return;
    }

    // Are there GPS satellites in view
    if (!gps->getHasLock())
    {
        drawText(display, x, y - 2, TEXT_ALIGN_RIGHT, "No Sats");
        x -= (10 + display->getStringWidth("No Sats"));
        display->drawFastImage(x, y, 6, 8, imgPositionEmpty);
        return;
    }
    else
    {
        char satsString[3];
        uint8_t bar[2] = {0};

        // Draw the number of satellites
        snprintf(satsString, sizeof(satsString), "%u", gps->getNumSatellites());
        drawText(display, x, y - 2, TEXT_ALIGN_RIGHT, satsString);
        x -= (36 + display->getStringWidth(satsString));

        // Draw Position icon
        display->drawFastImage(x, y, 6, 8, imgPositionSolid);

        // Draw DOP signal bars
        for (int i = 0; i < 5; i++)
        {
            if (gps->getDOP() <= dopThresholds[i])
                bar[0] = ~((1 << (5 - i)) - 1);
            else
                bar[0] = 0b10000000;
            display->drawFastImage(x + 9 + (i * 2), y, 2, 8, bar);
        }

        // Draw satellite image
        display->drawFastImage(x + 24, y, 8, 8, imgSatellite);
    }
}

void RocketFlightModule::drawAltitude(OLEDDisplay *display, int16_t x, int16_t y, double altitude)
{
    char altitudeStr[10];
    if (altitude != INVALID_ALTITUDE)
    {
        if (config.display.units == meshtastic_Config_DisplayConfig_DisplayUnits_IMPERIAL)
            snprintf(altitudeStr, sizeof(altitudeStr), "%0.1fft", altitude * METERS_TO_FEET);
        else
            snprintf(altitudeStr, sizeof(altitudeStr), "%0.1fm", altitude);
    }
    else
    {
        snprintf(altitudeStr, sizeof(altitudeStr), "--.--");
    }
    display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display->setFont(FONT_LARGE);
    display->drawString(display->getWidth() / 2, display->getHeight() / 2, altitudeStr);
}

void RocketFlightModule::drawText(OLEDDisplay *display, int16_t x, int16_t y, OLEDDISPLAY_TEXT_ALIGNMENT textAlignment, const String &text)
{
    display->setColor(WHITE);
    display->setFont(FONT_SMALL);
    display->setTextAlignment(textAlignment);
    display->drawString(x, y, text);
}

#endif

#endif
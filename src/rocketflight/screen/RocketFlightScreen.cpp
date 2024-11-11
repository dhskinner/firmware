#include "graphics/Screen.h"
#include "graphics/images.h"
#include "mesh/RadioInterface.h"

#ifdef ROCKETFLIGHT_SCREEN

#define SCREEN_WIDTH display->getWidth()
#define SCREEN_HEIGHT display->getHeight()
#define getStringCenteredX(s) ((SCREEN_WIDTH - display->getStringWidth(s)) / 2)

extern graphics::Screen *screen;
extern const RegionInfo *myRegion;

namespace graphics
{
    extern size_t nodeIndex;
    extern int8_t prevFrame;
    bool pixel = false;

    void drawRocketFlightIconScreen(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
    {
        // draw a fullscreen xbm image centered left to right and top to bottom
        display->drawXbm(x + (SCREEN_WIDTH - icon_width) / 2,
                         y + (SCREEN_HEIGHT - icon_height) / 2, icon_width,
                         icon_height, (const uint8_t *)icon_bits);

        // draw region in the upper left
        String region = myRegion ? String(myRegion->name) : "";
        display->setColor(OLEDDISPLAY_COLOR::WHITE);
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        display->setFont(FONT_SMALL);
        display->drawString(x, y, region);

        // draw version in upper right
        char buf[25];
        snprintf(buf, sizeof(buf), "%s", xstr(APP_VERSION_SHORT));
        display->setTextAlignment(TEXT_ALIGN_RIGHT);
        display->drawString(x + SCREEN_WIDTH, y + 0, buf);

        // refresh the screen
        screen->forceDisplay();
    }

    // get the number of tracker nodes (ignore other types of nodes, and this node)
    size_t getRocketFlightNodeCount()
    {
        meshtastic_NodeInfoLite *node = nullptr;
        meshtastic_NodeInfoLite *ourNode = nodeDB->getMeshNode(nodeDB->getNodeNum());
        size_t nodeCount = 0;
        int tries = 0;
        while (tries < nodeDB->getNumMeshNodes())
        {
            node = nodeDB->getMeshNodeByIndex(tries);

            // count this node if it is a tracker, and is not our own current node
            if (node->user.role == meshtastic_Config_DeviceConfig_Role_TRACKER && node->num != ourNode->num)
            {
                nodeCount++;
            }
            tries++;
        }
        return nodeCount;
    }

    void drawRocketFlightNodeInfo(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
    {
        meshtastic_NodeInfoLite *node = nullptr;
        meshtastic_NodeInfoLite *ourNode = nodeDB->getMeshNode(nodeDB->getNodeNum());
        static char buffer[20];     
        float dist = -1.0;

        // We only advance our nodeIndex if the frame # has changed - because
        // drawNodeInfo will be called repeatedly while the frame is shown
        if (state->currentFrame != prevFrame)
        {
            // the frame has changed so find the next tracker to display
            // if no suitable trackers are found then the node is unchanged
            prevFrame = state->currentFrame;
            size_t newIndex = nodeIndex;
            int tries = 0;
            while (tries < nodeDB->getNumMeshNodes())
            {
                // get the next node
                tries++;
                newIndex = (newIndex + tries) % nodeDB->getNumMeshNodes();
                node = nodeDB->getMeshNodeByIndex(newIndex);

                // select this node if it is a tracker, and is not our own current node
                // its fine to skip other nodes as you can still see them on the phone app
                if (node->user.role == meshtastic_Config_DeviceConfig_Role_TRACKER && node->num != ourNode->num)
                {
                    nodeIndex = newIndex;
                    break;
                }
            }
        }
        node = nodeDB->getMeshNodeByIndex(nodeIndex);

        // set up the display
        display->setFont(FONT_SMALL);
        display->setTextAlignment(TEXT_ALIGN_LEFT);
        if (config.display.displaymode == meshtastic_Config_DisplayConfig_DisplayMode_INVERTED)
        {
            display->fillRect(0 + x, 0 + y, x + display->getWidth(), y + display->getHeight());
            display->setColor(BLACK);
        }
        int16_t textY = y;

        // draw the username
        const char *username = node->has_user ? node->user.long_name : "Unknown Node";
        display->drawString(x, textY, username);
        textY += FONT_HEIGHT_SMALL;

        // draw the time the node was last seen
        static char lastStr[20];
        screen->getTimeAgoStr(sinceLastSeen(node), lastStr, sizeof(lastStr));
        display->drawString(x, textY, lastStr);
        textY += FONT_HEIGHT_SMALL;

        // draw the signal strength, or display hops away if more than 0 hops away.
        if (node->hops_away > 0)
            snprintf(buffer, sizeof(buffer), "Hops away: %d", node->hops_away);
        else
            snprintf(buffer, sizeof(buffer), "Signal: %d%%", clamp((int)((node->snr + 10) * 5), 0, 100));
        display->drawString(x, textY, buffer);
        textY += FONT_HEIGHT_SMALL;

        // get dimensions and coordinates for the compass/circle
        int16_t compassX = 0, compassY = 0;
        uint16_t compassDiam = Screen::getCompassDiam(SCREEN_WIDTH, SCREEN_HEIGHT) - FONT_HEIGHT_SMALL;
        compassX = x + SCREEN_WIDTH * 3 / 4;
        compassY = y + FONT_HEIGHT_SMALL + (SCREEN_HEIGHT - FONT_HEIGHT_SMALL) / 2;

        // draw the altitude
        if (node->has_position) 
        {
            // snprintf(buffer, sizeof(buffer), "Altitude: %d ft", node->position.altitude * METERS_TO_FEET);
            // display->drawString(x, textY, buffer);
            // textY += FONT_HEIGHT_SMALL;
            display->setFont(FONT_MEDIUM);
            display->setTextAlignment(TEXT_ALIGN_CENTER);
            snprintf(buffer, sizeof(buffer), "%.0fft", static_cast<double>(node->position.altitude) * METERS_TO_FEET);
            display->drawString(compassX, y, buffer);
            
            display->setFont(FONT_SMALL);
            display->setTextAlignment(TEXT_ALIGN_LEFT);
            snprintf(buffer, sizeof(buffer), "Lat: %.6f", node->position.latitude_i * 1e-7);
            display->drawString(x, textY, buffer);
            textY += FONT_HEIGHT_SMALL; 
            snprintf(buffer, sizeof(buffer), "Lon: %.6f", node->position.longitude_i * 1e-7);
            display->drawString(x, textY, buffer);
            textY += FONT_HEIGHT_SMALL; 
        }

        // work out the bearing and distance from our node to the other one
        if (ourNode && (hasValidPosition(ourNode) || screen->hasHeading()))
        {
            const meshtastic_PositionLite &op = ourNode->position;
            float myHeading;
            if (screen->hasHeading())
                myHeading = (screen->getHeading()) * PI / 180; // gotta convert compass degrees to Radians
            else
                myHeading = screen->estimatedHeading(DegD(op.latitude_i), DegD(op.longitude_i));
            screen->drawCompassNorth(display, compassX, compassY, myHeading);

            if (hasValidPosition(node))
            {
                // display direction toward node
                const meshtastic_PositionLite &p = node->position;
                dist = GeoCoord::latLongToMeter(DegD(p.latitude_i), DegD(p.longitude_i), DegD(op.latitude_i), DegD(op.longitude_i));

                // TODO TODO TODO
                // get text for the distance away
                // if (config.display.units == meshtastic_Config_DisplayConfig_DisplayUnits_IMPERIAL)
                //     strncpy(distStr, "? mi", sizeof(distStr)); // might not have location data
                // else
                //     strncpy(distStr, "? km", sizeof(distStr));
                // if (config.display.units == meshtastic_Config_DisplayConfig_DisplayUnits_IMPERIAL)
                // {
                //     if (d < (2 * MILES_TO_FEET))
                //         snprintf(distStr, sizeof(distStr), "%.0f ft", d * METERS_TO_FEET);
                //     else
                //         snprintf(distStr, sizeof(distStr), "%.1f mi", d * METERS_TO_FEET / MILES_TO_FEET);
                // }
                // else
                // {
                // if (d < 2000)
                //     snprintf(distStr, sizeof(distStr), "%.0f m", d);
                // else
                //     snprintf(distStr, sizeof(distStr), "%.1f km", d / 1000);
                //}

                // TODO TODO always show metres and kilometers
                if (dist < 5000)
                    snprintf(buffer, sizeof(buffer), "%.0f m", dist);
                else
                    snprintf(buffer, sizeof(buffer), "%.1f km", dist / 1000);
                display->setFont(FONT_MEDIUM);
                display->setTextAlignment(TEXT_ALIGN_CENTER);
                display->drawString(compassX, 0, buffer);
                display->setTextAlignment(TEXT_ALIGN_LEFT);
                display->setFont(FONT_SMALL);

                // get the bearing to the other node
                float bearingToOther = GeoCoord::bearing(DegD(op.latitude_i), DegD(op.longitude_i), DegD(p.latitude_i), DegD(p.longitude_i));
                // If the top of the compass is a static north then bearingToOther can be drawn on the compass directly
                // If the top of the compass is not a static north we need adjust bearingToOther based on heading
                if (!config.display.compass_north_top)
                    bearingToOther -= myHeading;

                // draw the compass heading
                screen->drawNodeHeading(display, compassX, compassY, compassDiam, bearingToOther);
            }
        }

        // distance to node is unknown so display question mark
        // if (dist < 0.0)
        // {
        //     display->drawString(compassX - FONT_HEIGHT_SMALL / 4, compassY - FONT_HEIGHT_SMALL / 2, "?");
        // }

        // draw the compass outline
        display->drawCircle(compassX, compassY, compassDiam / 2);
    }
}

#endif

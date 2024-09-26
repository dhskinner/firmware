#pragma once
#ifndef _ROCKETFLIGHT_DB_H_
#define _ROCKETFLIGHT_DB_H_

#include "NodeDB.h"
#include "Default.h"

#ifdef ROCKETFLIGHT_EDITION

class RocketFlightDB : public NodeDB
{
public:
  RocketFlightDB();

protected:
  void storeSettings();

};

#endif

#endif
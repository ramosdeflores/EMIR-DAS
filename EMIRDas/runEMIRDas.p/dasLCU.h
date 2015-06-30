// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

#ifndef _dasLCU_h_
#define _dasLCU_h_

// GCS Includes
#include "LCU.h"
#include "CommandLine.h"

// Forward declarations
class dasApplication;

/** Implements the LCU interface */
class dasLCU : public LCU
{
 public:

  /** Constructor */
  dasLCU(char* name);

  /** Destructor */
  virtual ~dasLCU();

  /** Start LCU services and applications */
  int start(int update=0);

  /** Stop LCU services and applications */
  int stop();

 protected:

 private:

  dasApplication* das_application_;
};

#endif  // _dasLCU_h_

// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

#ifndef _dasApplication_h_
#define _dasApplication_h_

// GCS Includes
#include "Application.h"

// Forward declarations
class EMIRDas;

/** This class manages a group of related Device instances */
class dasApplication : public Application
{
 public:

  /** Constructor */
  dasApplication(char* name);

  /** Destructor */
  virtual ~dasApplication();

  /** Creates devices */
  int run(int update=0);

  /** Destroy devices */
  int quit();

 protected:

 private:

  EMIRDas* DAS_;
};

#endif  // _dasApplication_h_

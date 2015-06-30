// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

// GCS Includes
#include "dasLCU.h"
#include "dasApplication.h"
#include "TimeService.h"
#include "ace/ACE.h"
#ifdef VXWORKS
#include "DDFactory.h"
#endif
#include <string>

// Constructor
dasLCU::dasLCU(char* name) 
              :LCU(name)
{
}

// Destructor
dasLCU::~dasLCU()
{
}

// Create the Devices of the lcu
int dasLCU::start(int update)
{
  printf("-------------------------------------------------------------------------------\n");
  printf("Starting %s services...\n",name_);
  printf("-------------------------------------------------------------------------------\n");
#ifdef VXWORKS
  try
  {
    driverFactory_ = DDFactory::instance();
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasLCU::start");
    ex.addToTrace("Error initialising DDFactory");
    cerr << ex;
  }
  cerr << driverFactory_->report();
#endif

  //-------------------
  // Start ACE services
  //-------------------
  ACE::init();

  //---------------------------------------
  // Initialising Time Service for this LCU
  //---------------------------------------
  try
  {
    timeService_ = TimeService::instance();
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasLCU::start");
    ex.addToTrace("Error initialising TimeService");
    cerr << ex;
  }

  // Start your Applications and Devices here
  printf("-------------------------------------------------------------\n");
  printf("Starting %s Applications...\n",name_);
  printf("-------------------------------------------------------------\n");
  try
  {
    das_application_ = new dasApplication("dasApplication");
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasLCU::start");
    ex.addToTrace("Error creating application");
    cerr << ex;
  }
  try
  {
    das_application_->run(update);
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasLCU::start");
    ex.addToTrace("Error running application");
    cerr << ex;
  }

  return 0;
}

// Stop services and applications
int dasLCU::stop()
{
  printf("-------------------------------------------------------------------------------\n");
  printf("Stopping %s...\n",name_);
  printf("-------------------------------------------------------------------------------\n");

  das_application_->quit();
  delete das_application_;

  // Stop services here
  //timeService_->fini();
  //delete timeService_;
  // ACE, DDFactory...
  return 0;
}

#ifdef VXWORKS
//---------------------------------------------------------------------------
// These functions can be called from the VxWorks shell or VxWorks scripts
//---------------------------------------------------------------------------
dasLCU* _das_lcu_;

extern "C" void start_LCU(int update=0)
{
  _das_lcu_ = new dasLCU("dasLCU");
  _das_lcu_->start(update); 
}

extern "C" void stop_LCU()
{
  _das_lcu_->stop(); 
  delete _das_lcu_;
}

extern "C" void reboot_LCU()
{
  _das_lcu_->reBoot(); 
}
#else
int main(int argc, char** argv)
{
  dasLCU* _das_lcu_;
  int update=0;
  CommandLine::current(argc, argv);
  CommandLine cl(argc, argv);
  update = cl.getFlag("-update");
  _das_lcu_ = new dasLCU("dasLCU");
  _das_lcu_->start(update); 
  // Leave the program running
  while(true)
  {
    sleep(2);
  };
}
#endif


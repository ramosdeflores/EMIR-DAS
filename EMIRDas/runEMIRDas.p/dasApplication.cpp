// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

// GCS Includes
#include "dasApplication.h"
#include "EMIRDas.h"

// Constructor
dasApplication::dasApplication(char* name) 
              :Application(name)
{
}

// Destructor
dasApplication::~dasApplication()
{
}

// Create the Devices of the application
int dasApplication::run(int update)
{
  // Create Devices
  try
  {
    // Create Device 1
    DAS_  = new EMIRDas("EMIR/DAS/DataAcquisition_1");
    DAS_->connect("orb1",5,95,Device::INTEGRATED,update);
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasApplication:run");
    cerr << "Error creating Device for this application" << endl;
    throw;
  }
  // Start Devices (if required)
  try
  {
    DAS_->start();
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasApplication:run");
    cerr << "Error starting Device" << endl;
    throw;
  }
  // Init Devices (if required)
  try
  {
    DAS_->init();
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasApplication:run");
    cerr << "Error initialising Device" << endl;
    throw;
  }
  return(0);
}

// Delete the Devices of the family 
int dasApplication::quit()
{
  try
  {
    // Delete Device 
    DAS_->halt();
    DAS_->shutdown();
    DAS_->disconnect();
    delete DAS_;
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("dasApplication:quit");
    cerr << ex;
  }
  return(0);
}

#ifdef VXWORKS
//---------------------------------------------------------------------------
// These functions can be called from the VxWorks shell or VxWorks scripts
//---------------------------------------------------------------------------
dasApplication* _das_application_;

extern "C" void run_dasApplication(int update=0)
{
  _das_application_ = new dasApplication("dasApplication");
  _das_application_->run(update); 
}

extern "C" void quit_dasApplication(void)
{
  _das_application_->quit(); 
  delete _das_application_;
}
#endif

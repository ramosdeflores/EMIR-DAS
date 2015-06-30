// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

// Includes
#include "EMIRDas.h"
#include "EMIRDasAdapter.h"
#include "EMIRDasConfigAgent.h"
#include "ErrorCode.h"
#include "../include/EMIRDasFixed.h"

//----------------------------------------------------------------------
// Device Constructor
//----------------------------------------------------------------------
EMIRDas::EMIRDas(string name)
 : Device(name)
{
  trace_.out("constructor\n");
  className_ = "EMIRDas";
  isComposite_ = FALSE;
  
  // Initialize local variables
  controller_ = 0;

  // Init some properties with hardcoded default values
  poweronApp_  = "powerOn.lod";
  poweroffApp_ = "powerOff.lod";
  adqApp_      = "adqApp.lod";
  imgDir_      = "NONE";
  simFile_      = "NONE";
  dataFactoryName_ = "EmirDas";
  isDataFactoryEnabled_ = 1;
  firmware_  = 999999;
  poolSize_  = 30;
  voffset_   = -1.0;
  vreset_    = -1.0;
  biasgate_  = -1.0;
  ron_       = -1.0;
  gain_      = -1.0;
  detectorType_ = "UNKNOWN";
  saveFile_     = true;
  sendFrame_    = false;
  simDetector_  = false;
  simMode_      = 0;
  reorder_      = true;
  invert_       = false;
  confChange  = true;
  
  // Init subdevices to avoid they contain garbage
}

//----------------------------------------------------------------------
// Device destructor
//----------------------------------------------------------------------
EMIRDas::~EMIRDas()
{
  trace_.out("destructor\n");
  // Disconnect and delete aggregated Devices
}

/************************************************************************
*************************************************************************
**                                                                     **
**            Virtual methods inherited from Device interface          **
**                                                                     **
*************************************************************************
************************************************************************/
  
//----------------------------------------------------------------------
// Connects device with GCS services
//----------------------------------------------------------------------
void EMIRDas::connect(string orbname,unsigned threads,long priority,
                                bool mode,bool update)
{
  Device::connect(orbname,threads,priority,mode,update);
  
  // Create IDL/CORBA Adapter
  try
  {
    trace_.out("creating IDL adapter\n");
    adapter_ = new EMIRDasAdapter(this);
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("EMIRDas::connect()"); 
    trace_.err("IDL adapter can't be created\n");
    throw;
  }
  
  // Connect adapter with CORBA context
  try
  {
    trace_.out("connecting adapter with CORBA\n");
    adapter_->setupCorba((PortableServer::Servant)adapter_,corbaContext_);
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("EMIRDas::connect()"); 
    trace_.err("IDL adapter can't be connected to CORBAContext\n");
    throw;
  }
  
  // Create the MonitorAgent for this class
  try
  {
    trace_.out("creating monitor agent\n");
    
    monitorAgent_ = new MonitorAgent(name_.c_str());
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("EMIRDas::connect()"); 
    if(ex.isA("CorbaException")) trace_.err("MonitorManager is down or unreachable\n");
      else trace_.err("unable to create monitor agent\n");
    throw;
  }
  
  // Create the ConfigAgent for this class
  try
  {
    trace_.out("creating configuration agent\n");
    configAgent_ = new EMIRDasConfigAgent(this,mode_,lowPriorityTimer_);
  }
  catch(GCSException& ex)
  {
    ex.addToTrace("EMIRDas::connect()"); 
    if(ex.isA("CorbaException")) trace_.err("ConfigManager is down or unreachable\n");
      else trace_.err("unable to create config agent\n");
    throw;
  }
  
  // Update profile if required
  if (update) updateProfile_();
  
  trace_.out("connected\n");
}

//----------------------------------------------------------------------
// Performs starting procedure: OFF-->STARTING-->ON
//----------------------------------------------------------------------
void EMIRDas::start()
{
  bool transmitFrame;
  staticData profileData;

  trace_.out("start()\n");
  goStarting_();
  
  // Configure device
  try
  {
    configure("Default");
  }
  catch(GCSException& ex)
  {
    goFault_();
    ex.addToTrace("EMIRDas::start()"); 
    logError_("unable to configure device");
    throw;
  }

  // Check for existing controller object, and free in case it exists
  if(controller_ != 0)
  {
       delete controller_;
  }
 
  try
  {
   profileData.frameDir = imgDir_;
   profileData.simImgFile = simFile_;

   if ( ! ACE_OS::access((const char *)poweronApp_, R_OK) )
     profileData.powOn  = poweronApp_;
   else {
    goFault_();
    trace_.out("Unable to read power on LOD file %s\n", poweronApp_.c_str());
    logError_("Unable to read power on LOD file %s", poweronApp_.c_str());
    throw;
   }

   if ( ! ACE_OS::access((const char *)adqApp_, R_OK) )
     profileData.acqApp = adqApp_;
   else {
    goFault_();
    trace_.out("Unable to read aquisition LOD file %s", adqApp_.c_str());
    logError_("Unable to read aquisition LOD file %s", adqApp_.c_str());
    throw;
   }

   if ( ! ACE_OS::access((const char *)poweroffApp_, R_OK) )
     profileData.powOff = poweroffApp_;
   else {
    goFault_();
    trace_.out("Unable to read power off LOD file %s", poweroffApp_.c_str());
    logError_("Unable to read power off LOD file %s", poweroffApp_.c_str());
    throw;
   }


   profileData.dataFactoryName = dataFactoryName_;
   profileData.profileFirmware = firmware_;
   profileData.detectorType = detectorType_;
   profileData.saveFile = (bool)saveFile_;
   profileData.sendFrame = (bool)sendFrame_;
   profileData.simDetector = (bool)simDetector_;
   profileData.simMode = (short)simMode_;
   profileData.reorder = (bool)reorder_;
   profileData.invert = (bool)invert_;
   profileData.voffset   =  voffset_;
   profileData.vreset    =  vreset_;
   profileData.biasgate  =  biasgate_;
   profileData.ron       =  ron_;
   profileData.gain      =  gain_;

   controller_ = new EMIRDasController(profileData);
  }   
  catch(std::bad_alloc)
  {
        trace_.err(__PRETTY_FUNCTION__);
        logError_("start() - Unable to create controller object. Fix the ERROR and start the component again!");
        // No funciona goPreviousState, hay que hacer gofault, goreset,
        //goOn,goshuting, goOff)
        //goPreviousState_();
        goFault_();
        goResetting_();
        goOn_();
        goShuttingDown_();
        goOff_();    
        controller_ = 0;
        throw OperationFailed("EMIRDas::start()", "Cannot get memory for the controller object");
  }

  // connects and starts basic services
  try
  {   
      transmitFrame = (isDataFactoryEnabled_ == 1) ? true : false;
      controller_->start(transmitFrame, poolSize_);
  }
   
  catch(Timeout &ex)
  {
     ex.addToTrace(__PRETTY_FUNCTION__);
     logError_("start() - EADA02: Unknown exception %s.", ex.text());
     sendAlarm_("Timeout", "EADA02: Unknown exception %s.",ex.text());

     throw;
  }   
  catch(GCSException& ex)
  {
      //goFault_();
      ex.addToTrace(__PRETTY_FUNCTION__);
      logError_("start() - Unable to start controller",ex.text());
      goFault_();
      goResetting_();
      goOn_();
      goShuttingDown_();

      delete controller_;
      controller_ = 0;
      goOff_();
      throw;
   }

   catch(...)
   {
      logError_( "start() -  Unable to start the DAS Controller- Unknown Exception found");
      goFault_();
      goResetting_();
      goOn_();
      goShuttingDown_();

      delete controller_;
      controller_ = 0;
      goOff_();
      throw;     
   }

   try
   {
     createMonitors_();
   }
   catch(GCSException &ex)
   {
      ex.addToTrace(__PRETTY_FUNCTION__);
      logError_("start() - Unable to create DAS monitors ",ex.text());
      goFault_();
      goResetting_();
      goOn_();
      goShuttingDown_();
      //delete controller_;
      //controller_ = 0;
      goOff_();

     throw;
   }

  // ## Create other classes required for this Device
  
  // ## Create other threads required for this Device
  
   try
   {
     
   }
   catch(GCSException &ex)
   {
      ex.addToTrace(__PRETTY_FUNCTION__);
      logError_("start() - Unable to start DAS monitors ",ex.text());
      goFault_();
      goResetting_();
      goOn_();
      goShuttingDown_();
      //delete controller_;
      //controller_ = 0;
      goOff_();

     throw;
   }

  goOn_();
}

//----------------------------------------------------------------------
// Performs initialization procedure: ON-->INITIALISING-->READY
//----------------------------------------------------------------------
void EMIRDas::init()
{
  trace_.out("init()\n");
  
  //  Execute the init procedure of controller (load the acquisiton application)
  try
  {
     goInitialising_();
     controller_->init();                   // loads end executes the power on application and
                                            // loads the acquisition application
//     isInitialized_ = true;
//     magnitudeChange_("isInitialized", isInitialized_);

     // Init aggregated Devices
     goIdle_();  // or goRunning_();

     // Init monitors to avoid they contain garbage
     isSettingup_                = 0;
     isConfigurationDone_        = 0;
     isExposing_                 = 0;
     isDetectorControllerIdle_   = 0;

     currentConfiguration.title = "NO_NAME_GIVEN";
     currentConfiguration.readoutmodeval = 0;
     currentConfiguration.windowMode = 0;
     currentConfiguration.applySimulateImage = 0;
     currentElapsedTime_ = 0;
     currentNumberOfImages_ = 0;

     setupExposureTime(1000);
     setupNSEC(1);
     setupNO_EXPOSURES(1);
     setupNRDBL(0);
     setupNRDBL_ND(0);
     setupNPARAM_X(0);
     setupNRST(0);
     setupNRDIL(1);
     setupNRDIL_ND(0);
     setupOPTRST(2);
     setupX_SIZE(4096);
     setupY_SIZE(1024);
     setupY_SIZE_B(0);
  }
  catch(Timeout &ex)
  {
     ex.addToTrace(__PRETTY_FUNCTION__);
     logError_("init() - EADA03: %s. (acquisition/poweron application load failed )", ex.text());
     sendAlarm_("Timeout", "EADA03:  Initializing controller.  %s. Check controller leds!",ex.text());
     goFault_();

     throw;
  }   
  catch(CannotProceed &ex)
  {
    logError_( "init() - EADA04: %s", ex.text());
    sendAlarm_("DASFault", "EADA03: %s", ex.text());
    goFault_();
    throw;
  }   
  catch(OperationFailed &ex)
  {
    logError_( "init() - EADA01:  %s", ex.text());
    sendAlarm_("CommunicationFailure", "EADA01:  Initializing controller. %s", ex.text());
    goFault_();
    throw;
  }     
  catch(GCSException &ex)
  {
    ex.addToTrace(__PRETTY_FUNCTION__);
    logError_("init() - EADA04: %s", ex.text());
    sendAlarm_("DeviceFault", "EADA04:  Unable to Initialize controller. %s", ex.text());
    goFault_();
    throw;
  }   
}

//----------------------------------------------------------------------
// Performs halting procedure: READY-->HALTING-->ON
//----------------------------------------------------------------------
void EMIRDas::halt()
{
  trace_.out("halt()\n");
  
  // ## Halting procedure
  controller_->halt();

  goHalting_();

  // Halt aggregated Devices
  goOn_();
}

//----------------------------------------------------------------------
// Performs shutdown procedure: ON-->SHUTTING_DOWN-->OFF
//----------------------------------------------------------------------
void EMIRDas::shutdown()
{
  trace_.out("shutdown()\n");
  
  // ## Shutdown procedure

  //  loads and executes the power off app
  try
  { 
    goShuttingDown_();

    controller_->shutdown();                   // loads and executes the power off application

     //if started previously

    // Shutdown aggregated Devices
    goOff_();
  }

  catch(Timeout &ex)
  {
     ex.addToTrace(__PRETTY_FUNCTION__);
     logError_("halt() - EADA05: %s. (load ond execution of power off application failed)", ex.text());
     sendAlarm_("Timeout", "EADA05: stopping controller.  %s. Check controller leds!",ex.text());

     throw;
  } 
  catch(CannotProceed &ex)  // Errors and warning fron¡m DAS controller
  {
      logError_( "halt() - EADA03: %s", ex.text());
      sendAlarm_("DASFault", "EAAT03: %s", ex.text());
      goFault_();
      throw;
  } 
  catch(OperationFailed &ex)
  {
      logError_( "halt() - EADA01:  %s", ex.text());
      sendAlarm_("CommunicationFailure", "EADA01:  Operation failed. %s", ex.text());
      goFault_();
      throw;
  }
  catch(GCSException& ex)
  {
    ex.addToTrace(__PRETTY_FUNCTION__);
    logError_("halt() - EADA04:  %s",ex.text());
    sendAlarm_("DeviceFault", "EADA04:  Unable to halt the device. %s", ex.text());
    goFault_();
    throw;
  }

}

//----------------------------------------------------------------------
// Performs reset procedure: FAULT-->RESETTING-->ON
//----------------------------------------------------------------------
void EMIRDas::reset()
{
  trace_.out("reset()\n");
/*  goResetting_();*/
  
  // ## Resetting procedure
  try {
    controller_->reset();
  } catch (...) {
      logError_( "reset() -  ERROR - Unknown Exception found");
      goFault_();
      throw;
  }

  
  // Reset aggregated Devices
/*  goOn_();*/
}

//----------------------------------------------------------------------
// Performs restore procedure: goes to previous state
//----------------------------------------------------------------------
void EMIRDas::restore()
{
  trace_.out("restore()\n");
  if (isFault()==FALSE)
  {
    logError_("Restore is only posible in FAULT state");
    throw WrongState(name_.c_str(),"Not in FAULT state. Unable to restore");
  }
  
  // ## Restore procedure
  
  goPreviousState_();
  cancelAlarm_("DeviceFault","Device is restored");
}

//----------------------------------------------------------------------
// Performs test procedure: only possible in ON state
//----------------------------------------------------------------------
void EMIRDas::test()
{
  trace_.out("test()\n");
  if (isOn()==FALSE)
  {
    logError_("Test is only posible in ON state");
    throw WrongState(name_.c_str(),"Not in ON state. Unable to test");
  }
  
  // ## Test procedure
  // The test procedure consistes in writing values to the controller
  // read them back.
  try {
    controller_->test();
  } catch (...) {
      logError_( "test() -  ERROR - Unknown Exception found");
      goFault_();
      throw;
  }
    
  
  // ## Test aggregated Devices
}

//----------------------------------------------------------------------
// Performs abort: interrupt any current operation and goes to FAULT
//----------------------------------------------------------------------
void EMIRDas::abort()
{
  trace_.out("abort()\n");
  
  // ## Abort procedure
  try {
    controller_->abort();
  } catch (...) {
      logError_( "abort() -  ERROR - Unknown Exception found");
      goFault_();
      throw;
  }
  
/*  goFault_();
  goIdle_(); */
}

//----------------------------------------------------------------------
// Report device status: returns static memory, do not delete! 
//----------------------------------------------------------------------
char* EMIRDas::report(short level)
{
  trace_.out("report(%d)\n",level);
  char* dev_reply = new char[DEVICE_REPORT_SIZE_];
  ostrstream ost(dev_reply, DEVICE_REPORT_SIZE_);
  memset(dev_reply,0,DEVICE_REPORT_SIZE_);
  
  // Generic Device info
  ost << Device::report(level);
  
  // ## Use level parameter to show more/less info
  // Current Device report
  ost << endl << "Device Properties" << endl;   
  ost << "-------------------------------------------------------" << endl;
  ost << "poweronApp          =" << poweronApp()        << endl;
  ost << "poweroffApp         =" << poweroffApp()       << endl;
  ost << "adqApp              =" << adqApp()            << endl;
  ost << "imgDir              =" << imgDir()            << endl;
  ost << "simFile             =" << simFile()            << endl;
  ost << "dataFactoryName     =" << dataFactoryName()   << endl;
  ost << "isDataFactoryEnabled =" << isDataFactoryEnabled() << endl;
  ost << "firmware            =" << firmware()        << endl;
  ost << "detectorType        =" << detectorType()        << endl;
  ost << "saveFile            =" << saveFile()        << endl;
  ost << "sendFrame           =" << sendFrame()        << endl;
  ost << "simDetector         =" << simDetector()        << endl;
  ost << "simMode             =" << simMode()        << endl;
  ost << "reorder             =" << reorder()        << endl;
  ost << "invert              =" << invert()        << endl;
  ost << "poolSize            =" << poolSize()        << endl;
  ost << "voffset             =" << voffset()        << endl;
  ost << "vreset              =" << vreset()        << endl;
  ost << "biasgate            =" << biasgate()        << endl;
  ost << "ron                 =" << ron()        << endl;
  ost << "gain                =" << gain()        << endl;
  // ## Add here your other properties
  
  ost << endl << "Device Magnitudes" << endl;   
  ost << "-------------------------------------------------------" << endl;
  ost << "isSettingup         :" << isSettingup()       << endl;
  ost << "isConfigurationDone :" << isConfigurationDone() << endl;
  ost << "isExposing          :" << isExposing()        << endl;
  ost << "isDetectorControllerIdle :" << isDetectorControllerIdle() << endl;
  ost << "currentElapsedTime  :" << currentElapsedTime()<< endl;
  ost << "currentExposureTime :" << currentExposureTime() << endl;
  ost << "currentNumberOfImages :" << currentNumberOfImages() << endl;
  ost << "currentNSEC         :" << currentNSEC()       << endl;
  ost << "currentNO_EXPOSURES :" << currentNO_EXPOSURES() << endl;
  ost << "currentNRDBL        :" << currentNRDBL()      << endl;
  ost << "currentNRDBL_ND     :" << currentNRDBL_ND()   << endl;
  ost << "currentNPARAM_X     :" << currentNPARAM_X()   << endl;
  ost << "currentNRST         :" << currentNRST()       << endl;
  ost << "currentNRDIL        :" << currentNRDIL()      << endl;
  ost << "currentNRDIL_ND     :" << currentNRDIL_ND()   << endl;
  ost << "currentOPTRST       :" << currentOPTRST()     << endl;
  ost << "currentX_SIZE       :" << currentX_SIZE()     << endl;
  ost << "currentY_SIZE       :" << currentY_SIZE()     << endl;
  ost << "currentY_SIZE_B     :" << currentY_SIZE_B()   << endl;
  // ## Add here your other monitors
  
  // ## Add more information if required
  
  // Add report of aggregated Devices if required
  
  return(dev_reply);
}

//----------------------------------------------------------------------
// Returns RCSID. Returns static memory; do not delete!  
//----------------------------------------------------------------------
const char* EMIRDas::version()
{
  trace_.out("version()\n");
  return("UNKNOWN");
}

//----------------------------------------------------------------------
// Create monitors of this Device  
//----------------------------------------------------------------------
void EMIRDas::createMonitors_()
{
  trace_.out("creating monitors\n");
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentElapsedTime",&EMIRDas::currentElapsedTime);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentExposureTime",&EMIRDas::currentExposureTime);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNumberOfImages",&EMIRDas::currentNumberOfImages);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNSEC",&EMIRDas::currentNSEC);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNO_EXPOSURES",&EMIRDas::currentNO_EXPOSURES);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNRDBL",&EMIRDas::currentNRDBL);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNRDBL_ND",&EMIRDas::currentNRDBL_ND);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNPARAM_X",&EMIRDas::currentNPARAM_X);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNRST",&EMIRDas::currentNRST);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNRDIL",&EMIRDas::currentNRDIL);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentNRDIL_ND",&EMIRDas::currentNRDIL_ND);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentOPTRST",&EMIRDas::currentOPTRST);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentX_SIZE",&EMIRDas::currentX_SIZE);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentY_SIZE",&EMIRDas::currentY_SIZE);
  monitorAgent_->createMonitor<EMIRDas,long>(this,"currentY_SIZE_B",&EMIRDas::currentY_SIZE_B);
}

//----------------------------------------------------------------------
// Starts the monitors of this Device 
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Stops the monitors of this Device 
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Create aggregated Devices  
//----------------------------------------------------------------------
void EMIRDas::createDevices_(bool update)
{
  trace_.out("creating aggregated Devices\n");
}

/************************************************************************
*************************************************************************
**                                                                     **
**                 Specific methods of this Device                     **
**                                                                     **
*************************************************************************
*************************************************************************
* All specific methods:                                                 *
* - Shall make a transition to RUNNING (even when is already running)   *
* - Shall make a transition to IDLE when the Device in really idle      *
* - Use lock_()/unlock_() only for thread safe operations               *
* - If an error is detected use logError_() & throw GCSException        *
* - If an alarm is detected use sendAlarm_()                            *
* - If a failure is detected use goFault_(method)                       *
* - Acces to a DeviceDriver or aggregated Device can be required        *
************************************************************************/
  
//----------------------------------------------------------------------
// setupDetector 
//----------------------------------------------------------------------
void EMIRDas::setupDetector(AcquisitionConfiguration acquisitionConfiguration)
{
  trace_.out("setupDetector()\n");
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentConfiguration.title = acquisitionConfiguration.title;
  currentConfiguration.readoutmodeval = (int)acquisitionConfiguration.readoutmodeval;
  currentConfiguration.exposuretime = acquisitionConfiguration.exposuretime;
  currentConfiguration.x_size = acquisitionConfiguration.x_size;
  currentConfiguration.y_size = acquisitionConfiguration.y_size;
  currentConfiguration.windowMode = acquisitionConfiguration.windowMode;
  currentConfiguration.applySimulateImage = acquisitionConfiguration.applySimulateImage;
  currentConfiguration.nsec = acquisitionConfiguration.nsec;
  currentConfiguration.optrst = acquisitionConfiguration.optrst;
  currentConfiguration.nrst = acquisitionConfiguration.nrst;
  currentConfiguration.nrdbl = acquisitionConfiguration.nrdbl;
  currentConfiguration.nrdbl_nd = acquisitionConfiguration.nrdbl_nd;
  currentConfiguration.numExp = acquisitionConfiguration.numExp;
  currentConfiguration.nrdil = acquisitionConfiguration.nrdil;
  currentConfiguration.nrdil_nd = acquisitionConfiguration.nrdil_nd;
  currentConfiguration.nparam_x = acquisitionConfiguration.nparam_x;
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// expose 
//----------------------------------------------------------------------
void EMIRDas::expose()
{
  AcqConfig config;

  config = currentConfiguration;

  trace_.out("expose()\n");
  //## check if input parameters are valid here
  
  lock_();
  goRunning_();
  
  //## Add code as required
  try {
   
    if ( controller_->expose(currentConfiguration, confChange) != DAS::OPERATION_OK ) {
      trace_.out("ERROR: expose finnished with errors !!!\n");
    }
  } catch (...) {
    trace_.out("ERROR: expose finnished with errors !!!\n");
    throw;
  }

  confChange = false;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupReadoutMode 
//----------------------------------------------------------------------
void EMIRDas::setupReadoutMode(ReadoutMode readoutmodeval)
{
  trace_.out("setupReadoutMode(%d)\n",readoutmodeval);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentReadoutMode_ = readoutmodeval;
  currentConfiguration.readoutmodeval = (int)readoutmodeval;
  currentReadoutMode();

  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupImageTitle 
//----------------------------------------------------------------------
void EMIRDas::setupImageTitle(const char* exposureId)
{
  trace_.out("setupImageTitle(%s)\n",exposureId);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentConfiguration.title = exposureId;
  
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupExposureTime 
//----------------------------------------------------------------------
void EMIRDas::setupExposureTime(long exposureTime)
{
  trace_.out("setupExposureTime(%ld)\n",exposureTime);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentExposureTime_ = exposureTime;
  currentConfiguration.exposuretime = exposureTime;
  currentExposureTime();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupWindowMode 
//----------------------------------------------------------------------
void EMIRDas::setupWindowMode(bool windowmode)
{
  trace_.out("setupWindowMode(%u)\n",windowmode);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentExposureTime_ = windowmode;
  currentConfiguration.windowMode = windowmode;
  currentWindowMode();

  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNSEC 
//----------------------------------------------------------------------
void EMIRDas::setupNSEC(long nsec)
{
  trace_.out("setupNSEC(%ld)\n",nsec);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNSEC_ = nsec;
  currentConfiguration.nsec = nsec;
  currentNSEC();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNO_EXPOSURES 
//----------------------------------------------------------------------
void EMIRDas::setupNO_EXPOSURES(long nexp)
{
  trace_.out("setupNO_EXPOSURES(%ld)\n",nexp);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNO_EXPOSURES_ = nexp;
  currentConfiguration.numExp = nexp;
  currentNO_EXPOSURES();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNRDBL 
//----------------------------------------------------------------------
void EMIRDas::setupNRDBL(long nrdbl)
{
  trace_.out("setupNRDBL(%ld)\n",nrdbl);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNRDBL_ = nrdbl;
  currentConfiguration.nrdbl = nrdbl;
  currentNRDBL();
  
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNRDBL_ND 
//----------------------------------------------------------------------
void EMIRDas::setupNRDBL_ND(long nrdbl_nd)
{
  trace_.out("setupNRDBL_ND(%ld)\n",nrdbl_nd);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNRDBL_ND_ = nrdbl_nd;
  currentConfiguration.nrdbl_nd = nrdbl_nd;
  currentNRDBL_ND();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNRST 
//----------------------------------------------------------------------
void EMIRDas::setupNRST(long nrst)
{
  trace_.out("setupNRST(%ld)\n",nrst);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNRST_ = nrst;
  currentConfiguration.nrst = nrst;
  currentNRST();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNPARAM_X 
//----------------------------------------------------------------------
void EMIRDas::setupNPARAM_X(long nparam_x)
{
  trace_.out("setupNPARAM_X(%ld)\n",nparam_x);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNPARAM_X_ = nparam_x;
  currentConfiguration.nparam_x = nparam_x;
  currentNPARAM_X();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNRDIL 
//----------------------------------------------------------------------
void EMIRDas::setupNRDIL(long nrdil)
{
  trace_.out("setupNRDIL(%ld)\n",nrdil);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNRDIL_ = nrdil;
  currentConfiguration.nrdil = nrdil;
  currentNRDIL();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupNRDIL_ND 
//----------------------------------------------------------------------
void EMIRDas::setupNRDIL_ND(long nrdil_nd)
{
  trace_.out("setupNRDIL_ND(%ld)\n",nrdil_nd);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentNRDIL_ND_ = nrdil_nd;
  currentConfiguration.nrdil_nd = nrdil_nd;
  currentNRDIL_ND();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupOPTRST 
//----------------------------------------------------------------------
void EMIRDas::setupOPTRST(long optrst)
{
  trace_.out("setupOPTRST(%ld)\n",optrst);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentOPTRST_ = optrst;
  currentConfiguration.optrst = optrst;
  currentOPTRST();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupX_SIZE 
//----------------------------------------------------------------------
void EMIRDas::setupX_SIZE(long x_size)
{
  trace_.out("setupX_SIZE(%ld)\n",x_size);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentX_SIZE_ = x_size;
  currentConfiguration.x_size = x_size;
  currentX_SIZE();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupY_SIZE 
//----------------------------------------------------------------------
void EMIRDas::setupY_SIZE(long y_size)
{
  trace_.out("setupY_SIZE(%ld)\n",y_size);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentY_SIZE_ = y_size;
  currentConfiguration.y_size = y_size;
  currentY_SIZE();
  
  confChange = true;
  goIdle_();
  unlock_();
}

//----------------------------------------------------------------------
// setupY_SIZE_B 
//----------------------------------------------------------------------
void EMIRDas::setupY_SIZE_B(long y_size_b)
{
  trace_.out("setupY_SIZE_B(%ld)\n",y_size_b);
  //## check if input parameters are valid here
  lock_();
  goRunning_();
  
  //## Add code as required
  currentY_SIZE_B_ = y_size_b;
  //currentConfiguration.y_size_b = y_size_b;
  currentY_SIZE_B();
  
  confChange = true;
  goIdle_();
  unlock_();
}

/************************************************************************
*************************************************************************
**                                                                     **
**           Monitorable attributes (device magnitudes)                **
**                                                                     **
*************************************************************************
*************************************************************************
* All monitor methods:                                                  *
* - Can be invoked in any state of the Device                           *
* - Do not modify the state of the Device                               *
* - It is required to check the ranges of the monitored values          *
* - If an alarm is detected use sendAlarm_()                            *
* - Shall be as fast as possible (due to high monitoring rate)          *
* - Acces to a DeviceDriver or aggregated Device can be required        *
* - In bool & enum monitors use magnitudeChange_() to inform of change  *
************************************************************************/
  
//----------------------------------------------------------------------
// Monitor isSettingup
//----------------------------------------------------------------------
bool EMIRDas::isSettingup()
{
  //trace_.out("isSettingup()\n");
  
  // ## Obtain the isSettingup_ value, if required. Example:
  // isSettingup_ = memoryMap.bit(15);
  // If value has changed use:
  //magnitudeChange_("isSettingup",isSettingup_); // inform of change
  
  return isSettingup_;
}

//----------------------------------------------------------------------
// Monitor isConfigurationDone
//----------------------------------------------------------------------
bool EMIRDas::isConfigurationDone()
{
  //trace_.out("isConfigurationDone()\n");
  
  // ## Obtain the isConfigurationDone_ value, if required. Example:
  // isConfigurationDone_ = memoryMap.bit(15);
  // If value has changed use:
  //magnitudeChange_("isConfigurationDone",isConfigurationDone_); // inform of change
  
  return isConfigurationDone_;
}

//----------------------------------------------------------------------
// Monitor isExposing
//----------------------------------------------------------------------
bool EMIRDas::isExposing()
{
  //trace_.out("isExposing()\n");
  
  // ## Obtain the isExposing_ value, if required. Example:
  // isExposing_ = memoryMap.bit(15);
  // If value has changed use:
  //magnitudeChange_("isExposing",isExposing_); // inform of change
  
  return isExposing_;
}

//----------------------------------------------------------------------
// Monitor isDetectorControllerIdle
//----------------------------------------------------------------------
bool EMIRDas::isDetectorControllerIdle()
{
  //trace_.out("isDetectorControllerIdle()\n");
  
  // ## Obtain the isDetectorControllerIdle_ value, if required. Example:
  // isDetectorControllerIdle_ = memoryMap.bit(15);
  // If value has changed use:
  //magnitudeChange_("isDetectorControllerIdle",isDetectorControllerIdle_); // inform of change
  
  return isDetectorControllerIdle_;
}

//----------------------------------------------------------------------
// Monitor currentReadoutMode
//----------------------------------------------------------------------
EMIRDas::ReadoutMode EMIRDas::currentReadoutMode()
{
  //trace_.out("currentReadoutMode()\n");
  
  // ## Obtain the currentReadoutMode_ value, if required. Example:
  // If value has changed use:
  //magnitudeChange_("currentReadoutMode",currentReadoutMode_); // inform of change
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentReadoutMode_;
}

//----------------------------------------------------------------------
// Monitor window mode
//----------------------------------------------------------------------
bool EMIRDas::currentWindowMode()
{
  //trace_.out("currentWindowMode()\n");
  
  // ## Obtain the currentExposureTime_ value, if required. Example:
  // currentExposureTime_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentWindowMode_;
}

//----------------------------------------------------------------------
// Monitor currentSDSUControllerStatus
//----------------------------------------------------------------------
EMIRDas::SDSUControllerStatus EMIRDas::currentSDSUControllerStatus()
{
  //trace_.out("currentSDSUControllerStatus()\n");
  
  // ## Obtain the currentSDSUControllerStatus_ value, if required. Example:
  // If value has changed use:
  //magnitudeChange_("currentSDSUControllerStatus",currentSDSUControllerStatus_); // inform of change
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentSDSUControllerStatus_;
}

//----------------------------------------------------------------------
// Monitor currentElapsedTime
//----------------------------------------------------------------------
long EMIRDas::currentElapsedTime()
{
  //trace_.out("currentElapsedTime()\n");
  
  // ## Obtain the currentElapsedTime_ value, if required. Example:
  // currentElapsedTime_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentElapsedTime_;
}

//----------------------------------------------------------------------
// Monitor currentExposureTime
//----------------------------------------------------------------------
long EMIRDas::currentExposureTime()
{
  //trace_.out("currentExposureTime()\n");
  
  // ## Obtain the currentExposureTime_ value, if required. Example:
  // currentExposureTime_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentExposureTime_;
}

//----------------------------------------------------------------------
// Monitor currentNumberOfImages
//----------------------------------------------------------------------
long EMIRDas::currentNumberOfImages()
{
  //trace_.out("currentNumberOfImages()\n");
  
  // ## Obtain the currentNumberOfImages_ value, if required. Example:
  // currentNumberOfImages_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNumberOfImages_;
}

//----------------------------------------------------------------------
// Monitor currentNSEC
//----------------------------------------------------------------------
long EMIRDas::currentNSEC()
{
  //trace_.out("currentNSEC()\n");
  
  // ## Obtain the currentNSEC_ value, if required. Example:
  // currentNSEC_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNSEC_;
}

//----------------------------------------------------------------------
// Monitor currentNO_EXPOSURES
//----------------------------------------------------------------------
long EMIRDas::currentNO_EXPOSURES()
{
  //trace_.out("currentNO_EXPOSURES()\n");
  
  // ## Obtain the currentNO_EXPOSURES_ value, if required. Example:
  // currentNO_EXPOSURES_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNO_EXPOSURES_;
}

//----------------------------------------------------------------------
// Monitor currentNRDBL
//----------------------------------------------------------------------
long EMIRDas::currentNRDBL()
{
  //trace_.out("currentNRDBL()\n");
  
  // ## Obtain the currentNRDBL_ value, if required. Example:
  // currentNRDBL_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNRDBL_;
}

//----------------------------------------------------------------------
// Monitor currentNRDBL_ND
//----------------------------------------------------------------------
long EMIRDas::currentNRDBL_ND()
{
  //trace_.out("currentNRDBL_ND()\n");
  
  // ## Obtain the currentNRDBL_ND_ value, if required. Example:
  // currentNRDBL_ND_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNRDBL_ND_;
}

//----------------------------------------------------------------------
// Monitor currentNPARAM_X
//----------------------------------------------------------------------
long EMIRDas::currentNPARAM_X()
{
  //trace_.out("currentNPARAM_X()\n");
  
  // ## Obtain the currentNPARAM_X_ value, if required. Example:
  // currentNPARAM_X_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNPARAM_X_;
}

//----------------------------------------------------------------------
// Monitor currentNRST
//----------------------------------------------------------------------
long EMIRDas::currentNRST()
{
  //trace_.out("currentNRST()\n");
  
  // ## Obtain the currentNRST_ value, if required. Example:
  // currentNRST_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNRST_;
}

//----------------------------------------------------------------------
// Monitor currentNRDIL
//----------------------------------------------------------------------
long EMIRDas::currentNRDIL()
{
  //trace_.out("currentNRDIL()\n");
  
  // ## Obtain the currentNRDIL_ value, if required. Example:
  // currentNRDIL_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNRDIL_;
}

//----------------------------------------------------------------------
// Monitor currentNRDIL_ND
//----------------------------------------------------------------------
long EMIRDas::currentNRDIL_ND()
{
  //trace_.out("currentNRDIL_ND()\n");
  
  // ## Obtain the currentNRDIL_ND_ value, if required. Example:
  // currentNRDIL_ND_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentNRDIL_ND_;
}

//----------------------------------------------------------------------
// Monitor currentOPTRST
//----------------------------------------------------------------------
long EMIRDas::currentOPTRST()
{
  //trace_.out("currentOPTRST()\n");
  
  // ## Obtain the currentOPTRST_ value, if required. Example:
  // currentOPTRST_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentOPTRST_;
}

//----------------------------------------------------------------------
// Monitor currentX_SIZE
//----------------------------------------------------------------------
long EMIRDas::currentX_SIZE()
{
  //trace_.out("currentX_SIZE()\n");
  
  // ## Obtain the currentX_SIZE_ value, if required. Example:
  // currentX_SIZE_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentX_SIZE_;
}

//----------------------------------------------------------------------
// Monitor currentY_SIZE
//----------------------------------------------------------------------
long EMIRDas::currentY_SIZE()
{
  //trace_.out("currentY_SIZE()\n");
  
  // ## Obtain the currentY_SIZE_ value, if required. Example:
  // currentY_SIZE_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentY_SIZE_;
}

//----------------------------------------------------------------------
// Monitor currentY_SIZE_B
//----------------------------------------------------------------------
long EMIRDas::currentY_SIZE_B()
{
  //trace_.out("currentY_SIZE_B()\n");
  
  // ## Obtain the currentY_SIZE_B_ value, if required. Example:
  // currentY_SIZE_B_ = adcConversion();
  
  // ## check ranges: use magnitudeLowerLimit_() and magnitudeUpperLimit_()
  
  return currentY_SIZE_B_;
}

/************************************************************************
*************************************************************************
**                                                                     **
**          Configurable attributes (device properties)                **
**                                                                     **
*************************************************************************
*************************************************************************
* All property methods:                                                 *
* - Used ONLY in STARTING,ON,IDLE,FAULT. Use checkConfigurability_()    *
* - Do not modify the state of the Device                               *
* - Both get/set methods shall be provided for each property            *
* - Properties shall have a default value initialized at startup        *
* - It is required to check the ranges before to apply properties       *
* - If an error is detected: use logError_() & throw GCSException       *
* - Inform that a property is changed: use propertyChange_()            *
************************************************************************/
  
//----------------------------------------------------------------------
// Get poweronApp property
//----------------------------------------------------------------------
const char* EMIRDas::poweronApp()
{
  trace_.out("poweronApp()\n");
  return poweronApp_.in();
}

//----------------------------------------------------------------------
// Set poweronApp property
//----------------------------------------------------------------------
void EMIRDas::poweronApp(const char* new_poweronApp)
{
  trace_.out("poweronApp(%s)\n",new_poweronApp);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  poweronApp_ = new_poweronApp;
  
  propertyChange_("poweronApp");
}

//----------------------------------------------------------------------
// Get poweroffApp property
//----------------------------------------------------------------------
const char* EMIRDas::poweroffApp()
{
  trace_.out("poweroffApp()\n");
  return poweroffApp_.in();
}

//----------------------------------------------------------------------
// Set poweroffApp property
//----------------------------------------------------------------------
void EMIRDas::poweroffApp(const char* new_poweroffApp)
{
  trace_.out("poweroffApp(%s)\n",new_poweroffApp);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  poweroffApp_ = new_poweroffApp;
  
  propertyChange_("poweroffApp");
}

//----------------------------------------------------------------------
// Get adqApp property
//----------------------------------------------------------------------
const char* EMIRDas::adqApp()
{
  trace_.out("adqApp()\n");
  return adqApp_.in();
}

//----------------------------------------------------------------------
// Set adqApp property
//----------------------------------------------------------------------
void EMIRDas::adqApp(const char* new_adqApp)
{
  trace_.out("adqApp(%s)\n",new_adqApp);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  adqApp_ = new_adqApp;
  
  propertyChange_("adqApp");
}

//----------------------------------------------------------------------
// Get imgDir property
//----------------------------------------------------------------------
const char* EMIRDas::imgDir()
{
  trace_.out("imgDir()\n");
  return imgDir_.in();
}

//----------------------------------------------------------------------
// Set imgDir property
//----------------------------------------------------------------------
void EMIRDas::imgDir(const char* new_imgDir)
{
  trace_.out("imgDir(%s)\n",new_imgDir);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  imgDir_ = new_imgDir;
  
  propertyChange_("imgDir");
}

//----------------------------------------------------------------------
// Get simFIle property
//----------------------------------------------------------------------
const char* EMIRDas::simFile()
{
  trace_.out("simFIle()\n");
  return simFile_.in();
}

//----------------------------------------------------------------------
// Set simFile property
//----------------------------------------------------------------------
void EMIRDas::simFile(const char* new_simFile)
{
  trace_.out("simFile(%s)\n",new_simFile);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  simFile_ = new_simFile;
  
  propertyChange_("simFile");
}

//----------------------------------------------------------------------
// Get dataFactoryName property
//----------------------------------------------------------------------
const char* EMIRDas::dataFactoryName()
{
  trace_.out("dataFactoryName()\n");
  return dataFactoryName_.in();
}

//----------------------------------------------------------------------
// Set dataFactoryName property
//----------------------------------------------------------------------
void EMIRDas::dataFactoryName(const char* new_dataFactoryName)
{
  trace_.out("dataFactoryName(%s)\n",new_dataFactoryName);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  dataFactoryName_ = new_dataFactoryName;
  
  propertyChange_("dataFactoryName");
}

//----------------------------------------------------------------------
// Get isDataFactoryEnabled property
//----------------------------------------------------------------------
const short EMIRDas::isDataFactoryEnabled()
{
  trace_.out("isDataFactoryEnabled()\n");
  return isDataFactoryEnabled_;
}

//----------------------------------------------------------------------
// Set isDataFactoryEnabled property
//----------------------------------------------------------------------
void EMIRDas::isDataFactoryEnabled(const short new_isDataFactoryEnabled)
{
  trace_.out("isDataFactoryEnabled(%d)\n",new_isDataFactoryEnabled);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  isDataFactoryEnabled_ = new_isDataFactoryEnabled;
  
  propertyChange_("isDataFactoryEnabled");
}

//----------------------------------------------------------------------
// Get firmware property
//----------------------------------------------------------------------
const long EMIRDas::firmware()
{
  trace_.out("firmware()\n");
  return firmware_;
}

//----------------------------------------------------------------------
// Set firmware property
//----------------------------------------------------------------------
void EMIRDas::firmware(const long new_firmware)
{
  trace_.out("firmware(%d)\n",new_firmware);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  firmware_ = new_firmware;
  
  propertyChange_("firmware");
}

//----------------------------------------------------------------------
// Get detectorType property
//----------------------------------------------------------------------
const char* EMIRDas::detectorType()
{
  trace_.out("detectorType()\n");
  return detectorType_;
}

//----------------------------------------------------------------------
// Set detectorType property
//----------------------------------------------------------------------
void EMIRDas::detectorType(const char* new_detectorType)
{
  trace_.out("detectorType(%s)\n",new_detectorType);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  detectorType_ = new_detectorType;
  
  propertyChange_("detectorType");
}

//----------------------------------------------------------------------
// Get saveFile property
//----------------------------------------------------------------------
const short EMIRDas::saveFile()
{
  trace_.out("saveFile()\n");
  return saveFile_;
}

//----------------------------------------------------------------------
// Set saveFile property
//----------------------------------------------------------------------
void EMIRDas::saveFile(const short new_saveFile)
{
  trace_.out("saveFile(%d)\n",new_saveFile);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  saveFile_ = new_saveFile;
  
  propertyChange_("saveFile");
}

//----------------------------------------------------------------------
// Get sendFrame property
//----------------------------------------------------------------------
const short EMIRDas::sendFrame()
{
  trace_.out("sendFrame()\n");
  return sendFrame_;
}

//----------------------------------------------------------------------
// Set sendFrame property
//----------------------------------------------------------------------
void EMIRDas::sendFrame(const short new_sendFrame)
{
  trace_.out("sendFrame(%d)\n",new_sendFrame);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  sendFrame_ = new_sendFrame;
  
  propertyChange_("sendFrame");
}

//----------------------------------------------------------------------
// Get simDetector property
//----------------------------------------------------------------------
const short EMIRDas::simDetector()
{
  trace_.out("simDetector()\n");
  return simDetector_;
}

//----------------------------------------------------------------------
// Set simDetector property
//----------------------------------------------------------------------
void EMIRDas::simDetector(const short new_simDetector)
{
  trace_.out("simDetector(%d)\n",new_simDetector);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  simDetector_ = new_simDetector;
  
  propertyChange_("simDetector");
}

//----------------------------------------------------------------------
// Get simMode property
//----------------------------------------------------------------------
const short EMIRDas::simMode()
{
  trace_.out("simMode()\n");
  return simMode_;
}

//----------------------------------------------------------------------
// Set simMode property
//----------------------------------------------------------------------
void EMIRDas::simMode(const short new_simMode)
{
  trace_.out("simMode(%d)\n",new_simMode);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  simMode_ = new_simMode;
  
  propertyChange_("simMode");
}

//----------------------------------------------------------------------
// Get reorder property
//----------------------------------------------------------------------
const short EMIRDas::reorder()
{
  trace_.out("reorder()\n");
  return reorder_;
}

//----------------------------------------------------------------------
// Set reorder property
//----------------------------------------------------------------------
void EMIRDas::reorder(const short new_reorder)
{
  trace_.out("reorder(%d)\n",new_reorder);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  reorder_ = new_reorder;
  
  propertyChange_("reorder");
}

//----------------------------------------------------------------------
// Get invert property
//----------------------------------------------------------------------
const short EMIRDas::invert()
{
  trace_.out("invert()\n");
  return invert_;
}

//----------------------------------------------------------------------
// Set invert property
//----------------------------------------------------------------------
void EMIRDas::invert(const short new_invert)
{
  trace_.out("invert(%d)\n",new_invert);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  invert_ = new_invert;
  
  propertyChange_("invert");
}

//----------------------------------------------------------------------
// Get poolSize property
//----------------------------------------------------------------------
const short EMIRDas::poolSize()
{
  trace_.out("poolSize()\n");
  return poolSize_;
}

//----------------------------------------------------------------------
// Set poolSize property
//----------------------------------------------------------------------
void EMIRDas::poolSize(const short new_poolSize)
{
  trace_.out("poolSize(%d)\n",new_poolSize);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  poolSize_ = new_poolSize;
  
  propertyChange_("poolSize");
}

//----------------------------------------------------------------------
// Get voffset property
//----------------------------------------------------------------------
const double EMIRDas::voffset()
{
  trace_.out("voffset()\n");
  return voffset_;
}

//----------------------------------------------------------------------
// Set voffset property
//----------------------------------------------------------------------
void EMIRDas::voffset(const double new_voffset)
{
  trace_.out("voffset(%f)\n",new_voffset);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  voffset_ = new_voffset;
  
  propertyChange_("voffset");
}

//----------------------------------------------------------------------
// Get vreset property
//----------------------------------------------------------------------
const double EMIRDas::vreset()
{
  trace_.out("vreset()\n");
  return vreset_;
}

//----------------------------------------------------------------------
// Set vreset property
//----------------------------------------------------------------------
void EMIRDas::vreset(const double new_vreset)
{
  trace_.out("vreset(%f)\n",new_vreset);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  vreset_ = new_vreset;
  
  propertyChange_("vreset");
}

//----------------------------------------------------------------------
// Get biasgate property
//----------------------------------------------------------------------
const double EMIRDas::biasgate()
{
  trace_.out("biasgate()\n");
  return biasgate_;
}

//----------------------------------------------------------------------
// Set biasgate property
//----------------------------------------------------------------------
void EMIRDas::biasgate(const double new_biasgate)
{
  trace_.out("biasgate(%f)\n",new_biasgate);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  biasgate_ = new_biasgate;
  
  propertyChange_("biasgate");
}

//----------------------------------------------------------------------
// Get ron property
//----------------------------------------------------------------------
const double EMIRDas::ron()
{
  trace_.out("ron()\n");
  return ron_;
}

//----------------------------------------------------------------------
// Set ron property
//----------------------------------------------------------------------
void EMIRDas::ron(const double new_ron)
{
  trace_.out("ron(%f)\n",new_ron);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  ron_ = new_ron;
  
  propertyChange_("ron");
}

//----------------------------------------------------------------------
// Get gain property
//----------------------------------------------------------------------
const double EMIRDas::gain()
{
  trace_.out("gain()\n");
  return gain_;
}

//----------------------------------------------------------------------
// Set gain property
//----------------------------------------------------------------------
void EMIRDas::gain(const double new_gain)
{
  trace_.out("gain(%f)\n",new_gain);
  
  // checkConfigurability_();
  // ## check new values before to apply the property
  
  // Change property
  gain_ = new_gain;
  
  propertyChange_("gain");
}


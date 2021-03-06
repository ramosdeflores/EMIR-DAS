// Copyright (c) 2006 Instituto de Astrofisica de Canarias
// All Rights Reserved

#include "EMIRDasController.h"
#include <ace/Event_Handler.h>

unsigned int AcqStatusFlag;
EMIRDasController *globCnt;

/* Thread Wrappers */
int runStorageManagerWp(void) {
  return globCnt->runStorageManager();
}

int runProcManagerWp(void) {
  return globCnt->runProcManager();
}

int runExposeWp(void) {
  return globCnt->runExpose();
}

// Thread responsible for storing the frame
int EMIRDasController::runStorageManager(void) {
  EMIRFrame *frame;
  SimpleStorageManager stMng(fixData);

  trace_->out ("runStorageManager: starts");
  storageThreadRunning = true;

  if ( stMng.initFrame() == DAS::OPERATION_OK ) {
    while ( storageThreadRunning ) {
      memoryMgrStr->waitForFull();
      frame = (EMIRFrame *)memoryMgrStr->getFullBuffer(false);
      trace_->out ("runStorageManager: acquiring frame");
      if ( frame == NULL ) {
        fixData.logAgent_->logError("runStorageManager() - EADA06: GOT NULL FRAME - EXITING");
        fixData.alarmAgent_->sendAlarm("DASFault", "runStorageManager() - EADA06: GOT NULL FRAME - EXITING", AlarmState::ACTIVE_NACKED);
        trace_->out ("runStorageMamanger - Got it wrong out");
        storageThreadRunning = false;
      } else  {
        try {
          trace_->out ("runStorageManager: calling sendFrame");
          stMng.sendFrame(frame);
          trace_->out ("runStorageManager: sendFrame returns, releasing frame");
	  memoryMgrProc->putFreeBuffer(frame);
        } catch(...) {
	  memoryMgrProc->putFreeBuffer(frame);
          storageThreadRunning = false;
          trace_->out ("runStorageMamanger - Bailing out");
          fixData.logAgent_->logError("runStorageManager: EXCEPTION - EXITING");
          fixData.alarmAgent_->sendAlarm("DASFault", "runStorageManager() - EADA07: Unknown exception - EXITING", AlarmState::ACTIVE_NACKED);
        }
      }
    }
  }
  return DAS::OPERATIONFAILED;
}

// Thread responsible for proccesing the frame
int EMIRDasController::runProcManager(void) {
  EMIRFrame *frame;
  SimpleProcManager spMng(fixData);

  trace_->out ("runProcManager: starts");
  procThreadRunning = true;

  if ( spMng.initFrame() == DAS::OPERATION_OK ) {
    while ( procThreadRunning ) {
      memoryMgrProc->waitForFull();
      frame = (EMIRFrame *)memoryMgrProc->getFullBuffer(false);
      trace_->out ("runProcManager: acquiring frame");
      if ( frame == NULL ) {
        trace_->out ("runProcMamanger - Got it wrong out");
        fixData.logAgent_->logError("runProcManager: GOT NULL FRAME - EXITING");
        fixData.alarmAgent_->sendAlarm("DASFault", "runProcManager() - EADA06: GOT NULL FRAME - EXITING", AlarmState::ACTIVE_NACKED);
        procThreadRunning = false;
      } else  {
        try {
          trace_->out ("runProcManager: calling sendFrame");
          spMng.procFrame(frame);
          trace_->out ("runProcManager: sendFrame returns, releasing frame");
	  memoryMgrStr->putFullBuffer(frame);
        } catch(...) {
	  memoryMgrProc->putFreeBuffer(frame);
          procThreadRunning = false;
          trace_->out ("runProcMamanger - Bailing out");
          fixData.logAgent_->logError("runProcManager: EXCEPTION - EXITING");
          fixData.alarmAgent_->sendAlarm("DASFault", "runProcManager() - EADA07: Unknown exception - EXITING", AlarmState::ACTIVE_NACKED);
        }
      }
    }
  }
  return DAS::OPERATIONFAILED;
}

EMIRDasController::EMIRDasController( staticData profileData ) {

  trace_ = new Trace("EMIRDasController");

  created_ = false;
  aborting_ = false;
  abortDAS = false;
  fixData = profileData;

  dchardwarefacade_ = new EMIRDasControllerHw();
  fixData.logAgent_ = new LogAgent("EMIR/DAS/DataAcquisition_1", 1, 0, TimeValue(1));
  fixData.alarmAgent_ = new AlarmAgent("EMIR/DAS/DataAcquisition_1", 1, 0, AlarmAgent::BUFFER, TimeValue(1), 1);

}

EMIRDasController::~EMIRDasController() {
;
}

int EMIRDasController::reset() {
  int result = DAS::OPERATION_OK;
  char *frameDevice;
  char *controllerDevice;

  trace_->out ("reset() starts");

  trace_->out ("cclose() starts");
  cclose(0);
  trace_->out ("fgclose() starts");
  fgclose(0);

  trace_->out ("copen() starts");
  if ( fixData.simDetector == true ) {
     controllerDevice = "SIMULATED_DTC";
     trace_->out ("reset() opening controller in simulated mode");
  } else
     if ( fixData.simMode == SFILE ) {
        controllerDevice = (char *)fixData.simImgFile.c_str();
     } else {
            controllerDevice = "SDSU_CONTROLLER";
       }

  if ( copen(controllerDevice, fixData.simMode) < 0 ) {
    trace_->out ("reset() copen failed");
    fixData.logAgent_->logError("start(): copen - failed to initialize %s", controllerDevice);
    fixData.alarmAgent_->sendAlarm("DeviceFault", "start() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
    NoSpace ex("EMIRDasController::start","copen");
    throw ex;
  }

  trace_->out ("fgopen() starts");
  if ( fixData.simDetector == true ) {
     frameDevice = "SIMULATED_FGR";
     trace_->out ("reset() opening frame grabber in simulated mode");
  } else {
     frameDevice = "SDSU_FGR";
  }

  if ( fgopen(frameDevice, fixData.simMode) < 0 ) {
    trace_->out ("reset() fgopen failed");
    fixData.logAgent_->logError("reset(): fgopen - failed to initialize the controller");
    fixData.alarmAgent_->sendAlarm("DeviceFault", "reset() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
    cclose(0);
    NoSpace ex("EMIRDasController::reset","fgopen");
    throw ex;
  }

  trace_->out ("reset() ends");

  return result;
}

int EMIRDasController::test() {
  int result = DAS::OPERATION_OK;

  trace_->out ("test() starts");


  trace_->out ("test() ends");
  return result;
}

int EMIRDasController::halt() {

  int result;

  trace_->out ("halt() starts");

  result = dchardwarefacade_->shutdown(fixData.powOff);

  if (result == DAS::OPERATION_OK)
    fixData.logAgent_->logInfo("halt(): power off completed");
  else {
    fixData.logAgent_->logError("halt(): failed to power off the controller");
    fixData.alarmAgent_->sendAlarm("DeviceFault", "halt() - EADA11: failed to power off the controller", AlarmState::ACTIVE_NACKED);
  }

  trace_->out ("halt() ends");

  return result;
}

int EMIRDasController::shutdown() {

  int result = DAS::OPERATION_OK;

  trace_->out ("shutdown() starts");

  trace_->out ("shutdown() Waiting for queue to empty");
  memoryMgrProc->waitForEmpty();

  trace_->out ("shutdown() destroying thread");
#ifdef VXWORKS
  taskDelete(storageThId_);
  taskDelete(procThId_);
#else
  ACE_Thread::cancel(storageThId_);
  ACE_Thread::cancel(procThId_);
#endif

  trace_->out ("shutdown() destroying memory manager");
  delete memoryMgrProc;

  trace_->out ("shutdown() ends");
  return result;
}

int EMIRDasController::start(bool sendFrame, const short poolSize) {
  int result = DAS::OPERATION_OK;
  size_t sizeofData;
  EMIRFrame *frame;
  int i;
  char *controllerDevice;
  char *frameDevice;

  trace_->out ("start() starts");

  if ( fixData.simDetector == true ) {
     controllerDevice = "SIMULATED_DTC";
     trace_->out ("start() opening controller in simulated mode");
  } else {
     controllerDevice = "SDSU_CONTROLLER";
  }
  if ( copen(controllerDevice, fixData.simMode) < 0 ) {
    trace_->out ("start() copen failed");
    fixData.logAgent_->logError("start(): copen - failed to initialize the controller");
    fixData.alarmAgent_->sendAlarm("DeviceFault", "start() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
    NoSpace ex("EMIRDasController::start","copen");
    throw ex;
  }

  if ( fixData.simDetector == true ) {
     frameDevice = "SIMULATED_FGR";
     trace_->out ("start() opening frame grabber in simulated mode");
  } else {
     frameDevice = "SDSU_FGR";
  }
  if ( fgopen(frameDevice, fixData.simMode) < 0 ) {
    trace_->out ("start() fgopen failed");
    fixData.logAgent_->logError("start(): fgopen - failed to initialize the controller");
    fixData.alarmAgent_->sendAlarm("DeviceFault", "start() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
    cclose(0);
    NoSpace ex("EMIRDasController::start","fgopen");
    throw ex;
  }

  // Create the memory manager
  trace_->out ("start() creating memory manager");
  trace_->out ("Pool size = %d", poolSize);
  try {
    // Create a memory manager with pool_size empty slots
    memoryMgrProc = new SimpleMemoryManager(0, poolSize);
    memoryMgrStr  = new SimpleMemoryManager(0, poolSize);
  } catch (std::bad_alloc ) {
    fixData.alarmAgent_->sendAlarm("DASFault", "start() - EADA09: OUT OF MEMORY", AlarmState::ACTIVE_NACKED);
    NoSpace ex("EMIRDasController::start","SimpleMemoryManager");
    throw ex;
  }

  // Load the memory manager with frames
  trace_->out ("start() loading memory manager");
  for (i = 0; i < poolSize; ++i) {
       if ( (frame = (EMIRFrame *)malloc(sizeof(EMIRFrame))) == NULL ) {
          std::cout << "EMIRDasController::start() ERROR ALLOCATIONG MEMORY for frame" << endl;
          fixData.logAgent_->logError("start() ERROR ALLOCATIONG MEMORY for frame");
          fixData.alarmAgent_->sendAlarm("DASFault", "start() - EADA09: OUT OF MEMORY", AlarmState::ACTIVE_NACKED);
          return DAS::OPERATIONFAILED;
       }
       if ( (frame->DetectorData = (INT16 *)memalign(SDSU_BURST_BLOCK, DETECTOR_SIZE * sizeof(INT16) + SDSU_HEADER_SIZE)) == NULL ) {
          free(frame);
          std::cout << "EMIRDasController::start() ERROR ALLOCATIONG MEMORY for DetectorData" << endl;
          fixData.logAgent_->logError("start()  ERROR ALLOCATIONG MEMORY for DetectorData");
          fixData.alarmAgent_->sendAlarm("DASFault", "start() - EADA09: OUT OF MEMORY", AlarmState::ACTIVE_NACKED);
          return DAS::OPERATIONFAILED;
       }
       memoryMgrProc->loadMemoryManager(frame);
  }

  // Create the storage and processing managers
  globCnt = this;
  try {
#ifdef VXWORKS
    trace_->out ("VX start() creating Processing Manager");
    procThId_ = taskSpawn("tprcFrDAS", 96, 0, 50000, (FUNCPTR)runProcManagerWp, 0,0,0,0,0,0,0,0,0,0);
    trace_->out ("VX start() creating Storage Manager");
    storageThId_ = taskSpawn("tstrFrDAS", 70, 0, 50000, (FUNCPTR)runStorageManagerWp, 0,0,0,0,0,0,0,0,0,0);
#else
    trace_->out ("start() creating Processing Manager");
    ACE_Thread::spawn((ACE_THR_FUNC)runProcManagerWp,   (void *)NULL,THR_NEW_LWP, (ACE_thread_t *)&procThId_);
    trace_->out ("start() creating Storage Manager");
    ACE_Thread::spawn((ACE_THR_FUNC)runStorageManagerWp,(void *)NULL,THR_NEW_LWP, (ACE_thread_t *)&storageThId_);
#endif

  } catch (...) {
      trace_->out ("start() COULD NOT CREATE THREAD");
      fixData.logAgent_->logError("start(): failed to create proccessing/storage threads");
      fixData.alarmAgent_->sendAlarm("DASFault", "start() - EADA10: failed to create proccessing/storage threads", AlarmState::ACTIVE_NACKED);
      delete memoryMgrProc;
      delete memoryMgrStr;
      NoSpace ex("EMIRDasController::start","SimpleStorageManager");
      throw ex;
  }

  // If we have to drop a frame, this is thebuffer to be used.
  try {
    if ((dumpFrame = (void *)memalign(SDSU_BURST_BLOCK, DETECTOR_SIZE * sizeof(INT16) + SDSU_HEADER_SIZE)) == NULL) {
      trace_->out ("start() ERROR ALLOCTING MEMORY FOR DUMP BUFFER");
      std::cout << "EMIRDasController::start() ERROR ALLOCTING MEMORY FOR DUMP BUFFER" << endl;
      fixData.logAgent_->logError("start() ERROR ALLOCTING MEMORY FOR DUMP BUFFER");
      fixData.alarmAgent_->sendAlarm("DASFault", "start() - EADA09: OUT OF MEMORY", AlarmState::ACTIVE_NACKED);
      return DAS::OPERATIONFAILED;
    }
  } catch (...) {
    trace_->out ("start() UNABLE TO ALLOCATE MEMORY FOR DUMP BUFFER - Unknown Exception found");
    std::cout << "EMIRDasController::start() Unable to allocate memory for detector - Unknown Exception found" << endl;
    fixData.logAgent_->logError("start() Unable to allocate memory for detector - Unknown Exception found"); 
    fixData.alarmAgent_->sendAlarm("DASFault", "start() - EADA09: OUT OF MEMORY", AlarmState::ACTIVE_NACKED);
    NoSpace ex("EMIRDasController::start","SimpleMemoryManager");
    throw ex;
  }

   trace_->out ("start() ends");

  return result;
}

// Load and execute the power on code.
int EMIRDasController::init() {
  int result = DAS::OPERATION_OK;
  long version;
  AcqConfig	baseConfig;

  trace_->out ("init() starts");

  try {
    result = dchardwarefacade_->init(fixData.powOn, fixData.acqApp);
  } catch (...) {
    trace_->out ("init()  Unable to init SDSU Controller- Unknown Exception found");
    fixData.logAgent_->logError("init()  Unable to init SDSU Controller- Unknown Exception found");
    fixData.alarmAgent_->sendAlarm("DeviceFault", "init() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
    NoSpace ex("EMIRDasController::init","Init Failed");
    throw ex;
  }
    
  // Check if the firmware version stored in the profile is the same as the
  // the version of the acquisition application on loaded in the controller
  result = dchardwarefacade_->getVal("VERSION", (INT32 *)&version);
  trace_->out ("version = %d", (UINT32)version);
  if ( result != DAS::OPERATION_OK ) {
     if ( fixData.simDetector == false ) {
        std::cout << " ERROR GETTING VERSION INFORMATION dchardwarefacade_->getVar =  " << result << endl;
        fixData.logAgent_->logError("init() ERROR GETTING VERSION INFORMATION dchardwarefacade_->getVar = %d", result);
        fixData.alarmAgent_->sendAlarm("DeviceFault", "init() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
#ifdef VXWORKS
        throw;
#endif
     }
  }

  fixData.controllerFirmware = version;
  if ( fixData.controllerFirmware != fixData.profileFirmware ) {
     if ( fixData.simDetector == false ) {
        fixData.logAgent_->logError("CONTROLLER APPLICATION VERSION MISMATCH (values in decimal): PROFILE = %d, CONTROLLER = %d", fixData.profileFirmware, fixData.controllerFirmware);
        fixData.logAgent_->logError("init()  !!!!!! EXECUTION TERMINATED !!!!!!!!");
        fixData.alarmAgent_->sendAlarm("DeviceFault", "init() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
        std::cout << "CONTROLLER APPLICATION VERSION MISMATCH: FROM CONTROLLER = " << (unsigned long)fixData.controllerFirmware;
        std::cout << " FROM PROFILE = " << (unsigned long)fixData.profileFirmware  << endl;
#ifdef VXWORKS
        throw;
#endif
     }
  } else {
     std::cout << "COTROLLER ACQUISITION APPLICATION VERSION NUMBER = (hex)" << std::hex << (unsigned long)fixData.controllerFirmware << endl;
     std::cout << "COTROLLER ACQUISITION APPLICATION VERSION NUMBER = (dec)" << std::dec << (unsigned long)fixData.controllerFirmware << endl;
     fixData.logAgent_->logInfo("COTROLLER ACQUISITION APPLICATION VERSION = %d (decimal)", fixData.controllerFirmware);
  }


  baseConfig.exposuretime = 0;
  baseConfig.x_size = 4096;
  baseConfig.y_size = 1024;
  baseConfig.windowMode = 0;
  baseConfig.nsec = 1;
  baseConfig.optrst = 2;
  baseConfig.nrst = 0;
  baseConfig.nrdbl = 0;
  baseConfig.nrdbl_nd = 0;
  baseConfig.numExp = 1;
  baseConfig.nrdil = 1;
  baseConfig.nrdil_nd = 0;
  baseConfig.nparam_x = 0;

  trace_->out ("Loading default controller configuration");
  if ( fixData.simDetector == false )
     result = dchardwarefacade_->config(baseConfig);

  trace_->out ("init() ends");

  return result;
}

int EMIRDasController::config(const AcqConfig & acqparameters) {
  int result;

  trace_->out ("config() starts");

  try {
    if ( fixData.simDetector == false )
      result = dchardwarefacade_->config(acqparameters);
  } catch (...) {
    trace_->out ("config() Unable to config SDSU Controller- Unknown Exception found");
    fixData.logAgent_->logError("config() Unable to config SDSU Controller- Unknown Exception found");
    fixData.alarmAgent_->sendAlarm("DeviceFault", "init() - EADA08: failed to initialize the controller", AlarmState::ACTIVE_NACKED);
    throw;
  }

  trace_->out ("config() ends");

  return result;

}


// This operation cancels an expose
int EMIRDasController::abort() {
  int result = DAS::OPERATION_OK;

  trace_->out ("abort() starts");
  aborting_ = true;
  abortDAS = true;

  result = dchardwarefacade_->abort();

  if (result != DAS::OPERATION_OK) {
    fixData.logAgent_->logError("abort() abort FAILED !!!");
    trace_->out ("abort() abort FAILED");
  } else {
    fixData.logAgent_->logInfo("abort() abort completed");
  }

  trace_->out ("abort() ends");
  return result;
}

/** Returns a report of the Device. */
const char * EMIRDasController::report(const short level) {
  return DAS::OPERATION_OK;
}

int EMIRDasController::check() {

  trace_->out ("check() starts");
  trace_->out ("check() FUNCTION NOT IMPLEMENTED !!!!!!!!!!!!!!!!!!!!!!!!");
  trace_->out ("check() ends");
  return DAS::OPERATION_OK;
}

int EMIRDasController::readDetector(unsigned int iter, AcqConfig params, bool last) {
  EMIRFrame *frame;
  void *readMem;
  bool dropFrame = false;
  TimeValue utc;
  long timerId;

  // Get a free buffer
  if ( (frame = (EMIRFrame *)memoryMgrProc->getFreeBuffer(false)) == NULL ) {
    dropFrame = true;
    if ( fgread(FRAME_GRABBER, (void *)dumpFrame, SDSU_TOTAL_SIZE) != SDSU_TOTAL_SIZE) {
      std::cout << "readDetector() ERROR READING FRAME FROM CONTROLLER" << endl;
      fixData.alarmAgent_->sendAlarm("DeviceFault", "EADA11: cannot read frame from controller", AlarmState::ACTIVE_NACKED);
      return -1;
    }
    std::cout << "readDetector() WARNING FRAME LOST" << endl;
    return 1;
  } else {
    frame->timeStamp1 = TimeService::getTAI();
    frame->status = SEC_RUN;

//    TimeValue delay(2 + params.exposuretime / 1000,0);
//    TimeValue period(0,0);

    try {
       // Read the detector
       if ( fgread(FRAME_GRABBER, (void *)frame->DetectorData, SDSU_TOTAL_SIZE) != SDSU_TOTAL_SIZE ) {
         std::cout << "readDetector() ERROR READING FRAME FROM CONTROLLER" << endl;
         fixData.logAgent_->logError("readDetector() ERROR READING FRAME FROM CONTROLLER");
         fixData.alarmAgent_->sendAlarm("DeviceFault", "EADA11: cannot read frame from controller", AlarmState::ACTIVE_NACKED);
         frame->status = SEC_ERROR;
       } 
    } catch(...) {
       frame->status = SEC_ERROR;
       fixData.logAgent_->logError("readDetector() EXCEPTION CATCHED");
       std::cout << "readDetector() EXCEPTION CATCHED" << endl;
    }
    frame->timeStamp2 = TimeService::getTAI();

// Todo esto puede desaparecer cuando creemos los frames de start/end llevandolo a expose()
    if ( ! iter ) {
      // Load header data
       if ( frame->status == SEC_RUN )
          frame->status = SEC_START;

       frame->readoutmodeval = (int)params.readoutmodeval;
       frame->texp = params.exposuretime;
       frame->x_size = params.x_size;
       frame->y_size = params.y_size;
       frame->windowMode = params.windowMode;
       frame->nsec = params.nsec;
       frame->optrst = params.optrst;
       frame->nrst = params.nrst;
       frame->nrdbl = params.nrdbl;
       frame->nrdbl_nd = params.nrdbl_nd;
       frame->nrdil = params.nrdil;
       frame->nrdil_nd = params.nrdil_nd;
       frame->nparam_x = params.nparam_x;
       frame->numExp = params.numExp;
       sprintf(frame->imageName, "%s", params.title.c_str());
    } 

    if ( last && frame->status != SEC_ERROR )
       frame->status = SEC_END;

    if ( abortDAS ) {
       frame->status = SEC_ABORT;
       std::cout << "readDetector() setting status to SEC_ABORT" << endl;
    }

    frame->nloop = iter;

    // Put the frame in the storage queue
    memoryMgrProc->putFullBuffer((void *)frame);
  }
   
  if ( frame->status == SEC_ERROR )
     return -1;
  else
     return 0;
}

ACE_Semaphore *exposeSema_ = new ACE_Semaphore(0);
AcqConfig params;

// The mother of all the module functions: get the data from the detector.
int EMIRDasController::runExpose(void) {
  int lostFrames = 0;
  int i,i0,i1,i2,i3,i4, result;
  int rResets, totalReads;
  unsigned int readSize;

  Trace *trace_ = new Trace("runExpose()");
  trace_->out ("starts");

  readSize = SDSU_TOTAL_SIZE;

  totalReads = params.numReads - 1;
  rResets = (params.optrst & 2) ? params.nrst : 0;
  trace_->out ("READING %d frames", params.numReads);
  try {
    // Tell the controller to start reading
    if ( dchardwarefacade_->startRead() != DAS::OPERATION_OK ) {
      trace_->out ("runexpose() ERROR executing controller code !!!");
      fixData.logAgent_->logError("runExpose() ERROR executing controller code !!!");
      fixData.alarmAgent_->sendAlarm("DeviceFault", "EADA12: PANIC - failed to execute controller app", AlarmState::ACTIVE_NACKED);
      exposeSema_->release();
      return DAS::OPERATIONFAILED;
    }

    for (i0 = 0, i = 0; i0  < params.nsec; ++i0) {
      trace_->out ("i = %d, in nsec(%d) loop i0 = %d", i, params.nsec, i0);

      if ( abortDAS ) {
        trace_->out ("ABORT DETECTED - BREAKING OUT FROM OUTER LOOP ...");
        trace_->out ("ABORT DETECTED - QUE DIOS REPARTA SUERTE ...");
        break;
      }

      for (i1 = 0; i1 < rResets; ++i1, ++i) {
        trace_->out ("i = %d, in reset(%d) loop i1 = %d", i, rResets, i1);
        trace_->out ("calling readDetector()");

	result = readDetector(i, params, (i == totalReads));
        if ( result != -1 ) {
          lostFrames += result;
          std::cout << "***** ***** runExpose(RESET) CURRENT FRAME = " << i << " FRAMES LOST = " << lostFrames << endl;
        } else {
          trace_->out ("readDetector returned ERROR");
          fixData.logAgent_->logError("readDetector() returned ERROR");
          break;
          exposeSema_->release();
          return DAS::OPERATIONFAILED;
        }
      }

      for (i2 = 0; i2 < params.nrdbl; ++i2, ++i) {
        trace_->out ("i = %d, in nrdbl(%d) loop i2 = %d", i, params.nrdbl, i2);
        trace_->out ("calling readDetector()");

        result = readDetector(i, params, (i == totalReads));
        if ( result != -1 ) {
          lostFrames += result;
          std::cout << "***** ***** runExpose(NRDBL) CURRENT FRAME = " << i << " FRAMES LOST = " << lostFrames << endl;
        } else {
          trace_->out ("readDetector returned ERROR");
          exposeSema_->release();
          return DAS::OPERATIONFAILED;
        }
      }

      for (i3 = 0; i3 < params.numExp; ++i3) {
        trace_->out ("i = %d, in numExp(%d) loop i3 = %d", i, params.numExp, i3);
        for (i4 = 0; i4 < params.nrdil; ++i4, ++i) {
           trace_->out ("i = %d, in nrdil(%d) loop i4 = %d", i, params.nrdil, i4);
           trace_->out ("calling readDetector()");

           result = readDetector(i, params, (i == totalReads));
           if ( result != -1 ) {
             lostFrames += result;
             std::cout << "***** ***** ***** runExpose(NRDIL) CURRENT FRAME = " << i << " FRAMES LOST = " << lostFrames << endl;
           } else {
             trace_->out ("readDetector returned ERROR");
             exposeSema_->release();
             return DAS::OPERATIONFAILED;
           }
        }
      }
    }
  } catch (...) {
    trace_->out ("EMIRDasController::runExpose() - UNEXPECTED EXCEPTION !!!");
    exposeSema_->release();
    throw;
  }

  if ( lostFrames ) {
	  fixData.logAgent_->logInfo("runExpose() FRAMES LOST = %d !!!", lostFrames);
  }

  trace_->out ("out of read loop");
  trace_->out ("ends");
  trace_->out("END abortDAS = %d", abortDAS);

  exposeSema_->release();
  return DAS::OPERATION_OK;
}

int EMIRDasController::expose(AcqConfig & acqparameters, bool confChange) {

  int result;

  trace_->out ("expose() starts");

  // Configure the detector for the current acqusition
  if ( confChange ) {
    try {
      trace_->out ("expose() calling config");
      if ( fixData.simDetector == false )
        if ( (result = dchardwarefacade_->config(acqparameters)) != DAS::OPERATION_OK) { 
          return result;
      }
    } catch (...) {
      fixData.logAgent_->logError("expose() Unknown Exception  -  Unable to read from SDSU ");
      fixData.alarmAgent_->sendAlarm("DeviceFault", "EADA07: Unknown exception - EXITING", AlarmState::ACTIVE_NACKED);
      trace_->out ("EMIRDasController::expose() Unknown Exception -  Unable to read from SDSU ");
      throw;
    }
  } else {
   trace_->out ("\n***************  NO CONFIGURATION CHANGE ***********************************************\n");
  }

  // Calculate the number of frames that will be generated by the controller
  acqparameters.numReads = acqparameters.nsec * ( ((acqparameters.optrst & 2) ? acqparameters.nrst : 0) +
             acqparameters.nrdbl + acqparameters.numExp * acqparameters.nrdil);

  params = acqparameters;

  trace_->out ("numRead(%d) = nsec(%d) * (((optrst(%d) & 2) ? nrst(%d) : 0) + nrdbl(%d) + numExp(%d) * nrdil(%d))\n", params.numReads, params.nsec, params.optrst, params.nrst, params.nrdbl, params.numExp, params.nrdil);
  fixData.logAgent_->logInfo("numRead(%d) = nsec(%d) * (((optrst(%d) & 2) ? nrst(%d) : 0) + nrdbl(%d) + numExp(%d) * nrdil(%d))\n", params.numReads, params.nsec, params.optrst, params.nrst, params.nrdbl, params.numExp, params.nrdil);

  // Send a expose init frame to test connection

  AcqStatusFlag = OK;
  abortDAS = false;
  trace_->out ("expose() launching thread");
#ifdef VXWORKS
    exposeThId_ = taskSpawn("texpThDAS", 000, 0, 40000, (FUNCPTR)runExposeWp,0,0,0,0,0,0,0,0,0,0);
#else
    ACE_Thread::spawn((ACE_THR_FUNC)runExposeWp,(void *)NULL,THR_NEW_LWP, (ACE_thread_t *)&exposeThId_);
#endif

  trace_->out ("expose() waiting for runExpose thread to end");
  exposeSema_->acquire();

  if ( abortDAS ) {
     abortDAS = false;
     aborting_ = false;
     reset();
  }

  trace_->out ("expose() ends");

  return DAS::OPERATION_OK;
}


// EMIRDasFixed.h

#ifndef _EMIRDasFixed_h_
#define _EMIRDasFixed_h_

#include "LogAgent.h"
#include "AlarmAgent.h"

struct staticData {
   String powOn;
   String acqApp;
   String powOff;
   String dataFactoryName;
   String detectorType;			// ENG, SCI, MUX, ...
   long profileFirmware;
   long controllerFirmware;
   double voffset;
   double vreset;
   double biasgate;
   double ron;
   double gain;

   string frameDir;
   string simImgFile;
   bool saveFile;
   bool sendFrame;
   bool simDetector;
   bool reorder;
   bool invert;
   short simMode;

   LogAgent* logAgent_;
   AlarmAgent* alarmAgent_;
};

#endif

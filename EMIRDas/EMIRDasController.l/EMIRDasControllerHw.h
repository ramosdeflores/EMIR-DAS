//#include "AcquisitionParameters.h"

#ifndef _EMIRDasControllerHw_h_
#define _EMIRDasControllerHw_h_

#include "Trace.h"

#include <stdlib.h>

#include "DetectorControllerApp.h"
#include "DetectorControllerDefs.h"

#include "EMIRDasConf.h"

#define	ACQAPP		"/scratch/EMIR/lods/acqApp.lod"
#define POWERON		"/scratch/EMIR/lods/powerOn.lod"
#define POWEROFF	"/scratch/EMIR/lods/powerOff.lod"

class EMIRDasControllerHw{
public:

 EMIRDasControllerHw();
 ~EMIRDasControllerHw();


  int controller;
  int frameGrabber;

  // Creates internal of EMIRDasControllerHw
  int create();

  // Performs initial "initialisation of SDSUController"
  // 1) Loads the power on application
  // 2) Executes the power on application
  // 3) Load the acquision application, which is stored in acqApp.
  int init(const String powerOn, const String acqApp);

  // Applies configuration for current acquisition mode.
  // Loads the parameters into the controller
  int config(const AcqConfig & acqparameters);

  // Returns info about EMIRDasControllerHw
  const char * report(const short level);

  // Seems it has to execute the readExperiemntal :-(
//  void read((void  *)buffer, size_t size);
//  void readSynchro(AcquisitionParameters & acqparameters, Bucket *currentbucket, Bucket *nextbucket,bool appliesnextbucket,int currentiteration);

  // This operations checks the correct work of DC and returns a code error indicating
  // if it worked or if some fail ocurred.
  int check();

  // Resets the controller
  int reset();

  // Aborts the acquisition
  int abort();

  // Starts the b-board controller aquisition routine
  int startRead();

  // Turns off the SDSUcontroller
  // 1) Loads the power off application into the controller
  // 2) Executes the power off application
  int shutdown(const String powerOff);

  int getVal(const char* variable, INT32 *val);

private:

  Trace *trace_;

  DetectorControllerApp *aquisitionCode;

};

#endif _EMIRDasControllerHw_h_

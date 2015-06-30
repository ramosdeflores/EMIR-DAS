// SimpleStorageManager.h

#ifndef _SimpleStorageManager_h_
#define _SimpleStorageManager_h_

#include <Keyword.h>
#include <Trace.h>
#include <GCSException.h>
#include <RawFrameReceiverC.h>
#include <GCSTypes.h>
#include <CORBARef.h>
#include <IDL_Adapters_DGT.h>
#include <IDL_Adapters_DPKF.h>
#include <ErrorCode.h>
#include <stdio.h>
#include <stdlib.h>  
#include <Frame.h>
#include <unistd.h>
#include <fcntl.h>
#include "DeviceAdapter.h"
#include "TimeService.h"
#include "../include/EMIRDasFixed.h"
#include "../include/EMIRDasDetector.h"

#include "EMIRDFAGENTC.h"
#include "EMIRDasFrame.h"

#define	MAKEFITS	1
#define	SENDFRAME	2

class SimpleStorageManager {

public:

	SimpleStorageManager(staticData data);

	~SimpleStorageManager();

	int initFrame();
	int initFits(string imageDir);
	int sendFrame(EMIRFrame *frame);
	int saveFits(EMIRFrame *frame);
	void initEMIRDFAgent_();

private:

	string baseDir;
	unsigned int frameCount;
	Trace *trace_;
        char baseName[256];
	bool flushingFrames;

        staticData fixData;

	String dataFactoryName_;
};

#endif

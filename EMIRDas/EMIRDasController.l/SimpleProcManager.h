// SimpleProcManager.h

#ifndef _SimpleProcManager_h_
#define _SimpleProcManager_h_

#include <Trace.h>
#include "EMIRDasFrame.h"
#include <GCSException.h>
#include <GCSTypes.h>
#include <ErrorCode.h>
#include <stdio.h>
#include <stdlib.h>  
#include <unistd.h>
#include "TimeService.h"
#include "../include/EMIRDasFixed.h"

#define	INVER		1
#define	REORDER		2

#define NLINES  1024
#define NPIX    4096
#define ROWS    2048
#define COLS    2048
#define CHANELS 8
#define CCDS    4
#define SDSU_SIZE   NLINES * NPIX


#ifndef VXWORKS
#define INT16	short
#define UINT16	unsigned short
#endif

#ifndef VXWORKS
#define INT16	short
#define UINT16	unsigned short
#endif

class SimpleProcManager {

public:

	SimpleProcManager(staticData data);

	~SimpleProcManager();

	int initFrame(void);
	int procFrame(EMIRFrame *frame);

private:

	void bTransform();

	long int *tIndex;
	UINT16 *Matrix1;

	staticData fixData;

	Trace *trace_;
};

#endif

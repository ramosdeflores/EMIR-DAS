#ifndef _EMIRFRAME_
#define _EMIRFRAME_

#include "TimeValue.h"

#ifndef VXWORKS
#define	INT16	short
#endif

#define SDSU_HEADER_SIZE	24
#define DETECTOR_SIZE		2048 * 2048
#define SDSU_TOTAL_SIZE		DETECTOR_SIZE + SDSU_HEADER_SIZE

// For use with status
#define	SEC_START	0
#define	SEC_RUN		1
#define	SEC_END		2
#define	SEC_ABORT	3
#define SEC_ERROR	4


struct EMIRFrame {
        char imageName[256];
        int readoutmodeval;
        long texp;
        long x_size;
        long y_size;
        bool windowMode;
	long nsec;                      // Number of sequences in an observation
	long optrst;                    // Type of detector resets to be performed
	long nrst;                      // Number of detector resets
	long nrdbl;                     // Number of detector resets before loop
	long nrdbl_nd;                  // Detectors resets before the loop with no data
	long nloop;                    // Number of expositions per sequence
	long nrdil;                     // Number of detector resets inside the loop
	long nrdil_nd;                  // Detector resets inside the loop with no data
	long nparam_x;                  // Number of dummy readouts of a line
        long numExp;

	// Internal use
	unsigned long status;		// Type of frame
	TimeValue timeStamp1;
	TimeValue timeStamp2;

	// Static values
	double gain;
	double voffset;
	double vreset;
	double biasgate;

	INT16 *DetectorData;
};


#define	OK		0
#define	UNKNOWN_ERROR	1
#define FRAME_LOST_C	2	// Frame lost on capture
#define	TRANS_FAILED	4	// General transmision errror
#define	FRAME_LOST_T	8	// Frame lost on transmision
#define	ABORT		16	// Abort aqusition
#define	FLUSH		32	// Flusing frame cache

#endif // _EMIRFRAME_ 


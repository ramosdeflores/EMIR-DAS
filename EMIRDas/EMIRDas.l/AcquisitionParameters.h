
#ifndef _AcquisitionParameters_h_
#define _AcquisitionParameters_h_

#include <iostream>
#include <string>
//#include "AcquisitionParametersChanges.h"
#include "ConcreteArrays.h"
#include <vector>
#include <time.h>


/**
 * Provides a data structure that contains the parameters for an acquisition
 **/


class AcquisitionParameters {
public:

	AcquisitionParameters();
	// Copy constructor.
	//AcquisitionParameters(AcquisitionParameters const& copy);
	~AcquisitionParameters();


	/**
	* @return Returns current Acquisition mode
	*/
	int acquisitionMode() const;


	/**
	* 	return Returns current Acquisition mode
	*/
	int coordId() const;

	/**
	* Returns an string with internal information about this object.
	*/
	char *  report();


	//AcquisitionParametersChanges * getChangeControl();

	std::string title;

	//Acquisition Mode
	enum ReadoutMode { SINGLE, CDS, FOWLER, RAMP, HDR_SIMPLE, BIAS, MRAMP, UNDEFINED  };
	ReadoutMode acquisitionmode;
	unsigned short pixelSize;

	//ExposureTime
	long exposureTime;			// For the whole detector;
	long nparam_x;				// For High Dynamic Range mode

	// numReads = nsec * ( ((optrst & 2) ? nrst : 0) + nrdbl + numExp * nrdil)

	unsigned int nsec;			// Number of sequences in an observation
	unsigned int optrst;			// Type of detector resets to be performed
	unsigned int nrst;			// Number of detector resets
	unsigned int nrdbl;			// Number of detector resets before loop
	unsigned int nrdbl_nd;			// Detectors resets before the loop with no data
	unsigned int nexp;			// Number of expositions per sequence
	unsigned int nrdil;			// Number of detector resets inside the loop
	unsigned int nrdil_nd;			// Detector resets inside the loop with no data
	unsigned int numReads;

	//Parameters for the positions  of the internal windows
	unsigned short x_size; 			//Window Size X
	unsigned short y_size; 			//Window Size Y
	unsigned short y_size_b; 		//Window Size Y_B
	unsigned short x_start;
	unsigned short y_start;

	bool simulateImage;
	bool applyconfiguration;
	bool applysaveimages;
	bool windowmode;

	long acquisitionId;
	// Coordinator id that is taking the acquisition process.
	short coordid;


	enum {DEVICE_REPORT_SIZE_   = 32*1024,DEVICE_INTERNAL_SIZE_=32*1024}; // 16K
	char dev_reply_ [DEVICE_REPORT_SIZE_];
	timespec acquisitionStartTime;


};
#endif // _AcquisitionParameters_h_


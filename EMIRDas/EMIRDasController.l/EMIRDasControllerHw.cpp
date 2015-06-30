#include "EMIRDasController.h"
#include "EMIRDasControllerHw.h"
#include "DetectorControllerApp.h"
#include "Trace.h"
#define CPLUS
#ifdef VXWORKS
#include "msgQLib.h"
#include "../SDSUlcu.l/ct_interface.h"
#include "../SDSUlcu.l/sdsu_emir_driver.h"    /* sdsu_init(), sdsu_cleanup() */
#else
#include "../SDSUsim.l/ct_interface.h"
#endif
#include "DASException.h"
#include "ErrorCode.h"
#include "Trace.h"

int loadLODFile(const char *file, char *&buff, size_t *filesize) {
        ACE_HANDLE fp;
        off_t fileSize;
        char *p;
        size_t i;

        if (ACE_OS::access ((char *)file, R_OK) == -1) {
		buff = NULL;
                return DAS::OPERATIONFAILED;
	}

        fp = ACE_OS::open (file, O_RDONLY);
        fileSize = ACE_OS::lseek(fp, 0, SEEK_END);
        if ((buff = (char *)ACE_OS::malloc((size_t)fileSize)) == NULL) {
                ACE_OS::close(fp);
		buff = NULL;
                return DAS::OPERATIONFAILED;
        }
        ACE_OS::lseek(fp, 0, SEEK_SET);
        ACE_OS::read(fp, buff, (size_t)fileSize);
        ACE_OS::close(fp);

        *filesize = fileSize;

        return DAS::OPERATION_OK;
}


EMIRDasControllerHw::EMIRDasControllerHw()
{
	trace_ = new Trace("EMIRDasControllerHw");
}

EMIRDasControllerHw::~EMIRDasControllerHw()
 {;}


// Creates internal of EMIRDasControllerHw
int EMIRDasControllerHw::create() {

	return DAS::OPERATION_OK;
}

// Performs initial "initialization of SDSUController"
// 1) Loads power on application.
// 2) Executes the power on application.
// 3) Loads the acquisition appliction.
// 4) Sets the default parameters.
int EMIRDasControllerHw::init(const String powerOn, const String acqApp) {
	int retVal;
	char *buffer;
	size_t sizeOfBuffer;
	DetectorControllerApp *powerOnCode;

        trace_->out ("init() starts");

	retVal = loadLODFile(powerOn.c_str(), buffer, &sizeOfBuffer);

	if ( buffer == NULL ) {
		return retVal;
	} 

	powerOnCode = new DetectorControllerApp::DetectorControllerApp(buffer, sizeOfBuffer);
        trace_->out ("init()- loading Power ON application %s", powerOn.c_str());
	powerOnCode->load();
        trace_->out ("init()- executing Power ON application %s", powerOn.c_str());
	powerOnCode->exec();

        ACE_OS::free(buffer);

	retVal = loadLODFile(acqApp.c_str(), buffer, &sizeOfBuffer);
	if ( buffer == NULL ) {
		return retVal;
	}

	EMIRDasControllerHw::aquisitionCode = new DetectorControllerApp::DetectorControllerApp(buffer, sizeOfBuffer);

        trace_->out ("init()- loading aquisition application %s", acqApp.c_str());
	EMIRDasControllerHw::aquisitionCode->load();

	return retVal;
}

// Applies configuration for current acquisition mode.
int EMIRDasControllerHw::config(const AcqConfig & acqparameters) {

	long y_size_b;
        int result;

std::cout << "EMIRDasControllerHw::config() Starts" << endl;
	y_size_b = 1024 - acqparameters.y_size;

#ifdef VXWORKS
	if ( (result = EMIRDasControllerHw::check()) == DAS::OPERATION_OK ) {
		result = aquisitionCode->write("NROWS", 1024);
std::cout << "EMIRDasControllerHw::config - NROWS = 1024, got " << result <<  endl;
		result = aquisitionCode->write("X_START", 1);
std::cout << "EMIRDasControllerHw::config - X_START = 1, got " << result <<  endl;
		result = aquisitionCode->write("Y_START", 1);
std::cout << "EMIRDasControllerHw::config - Y_START = 1, got " << result <<  endl;
		result = aquisitionCode->write("NSEC", acqparameters.nsec);
std::cout << "EMIRDasControllerHw::config - NSEC = " << acqparameters.nsec << ", got " << result << endl;
		result = aquisitionCode->write("OPTRST", acqparameters.optrst);
std::cout << "EMIRDasControllerHw::config - OPTRST = " << acqparameters.optrst << ", got " << result << endl;
		result = aquisitionCode->write("NRST", acqparameters.nrst);
std::cout << "EMIRDasControllerHw::config - NRST = " << acqparameters.nrst << ", got " << result << endl;
		result = aquisitionCode->write("NRDBL", acqparameters.nrdbl);
std::cout << "EMIRDasControllerHw::config - NRDBL = " << acqparameters.nrdbl << ", got " << result << endl;
		result = aquisitionCode->write("NRDBL_ND", acqparameters.nrdbl_nd);
std::cout << "EMIRDasControllerHw::config - NRDBL_ND = " << acqparameters.nrdbl_nd << ", got " << result << endl;
		result = aquisitionCode->write("NO_EXPOSURES", acqparameters.numExp);
std::cout << "EMIRDasControllerHw::config - NO_EXPOSURES = " << acqparameters.numExp << ", got " << result << endl;
		result = aquisitionCode->write("NRDIL", acqparameters.nrdil);
std::cout << "EMIRDasControllerHw::config - NRDIL = " << acqparameters.nrdil << ", got " << result << endl;
		result = aquisitionCode->write("NRDIL_ND", acqparameters.nrdil_nd);
std::cout << "EMIRDasControllerHw::config - NRDIL_ND = " << acqparameters.nrdil_nd << ", got " << result << endl;
		result = aquisitionCode->write("X_SIZE", acqparameters.x_size);
std::cout << "EMIRDasControllerHw::config - X_SIZE = " << acqparameters.x_size << ", got " << result << endl;
		result = aquisitionCode->write("Y_SIZE", acqparameters.y_size);
std::cout << "EMIRDasControllerHw::config - Y_SIZE = " << acqparameters.y_size << ", got " << result << endl;
		result = aquisitionCode->write("Y_SIZE_B", y_size_b);
std::cout << "EMIRDasControllerHw::config - Y_SIZE_B = " << y_size_b << ", got " << result << endl;
		result = aquisitionCode->write("NPARAM_X", acqparameters.nparam_x);
std::cout << "EMIRDasControllerHw::config - NPARAM_X = " << acqparameters.nparam_x << ", got " << result << endl;
		result = aquisitionCode->write("EXPOSE_TIME", acqparameters.exposuretime);
std::cout << "EMIRDasControllerHw::config - EXPOSE_TIME = " << acqparameters.exposuretime << ", got " << result << endl;

		return DAS::OPERATION_OK;
	} else {
std::cout << "EMIRDasControllerHw::config() FAILS !!!!!!!! " << endl;
		return result;
        }
#else
	return DAS::OPERATION_OK;
#endif
}

// Returns info about EMIRDasControllerHw
const char * EMIRDasControllerHw::report(const short level) {
        std::cout << "EMIRDasControllerHw::report()" << endl;

	
	return DAS::OPERATION_OK;
 }

// Reset controller
int EMIRDasControllerHw::reset() {
        std::cout << "EMIRDasControllerHw::reset()" << endl;

//	cioctl(0, RESET, NULL);
//	cioctl(0, RST_PCI, NULL);
//	cioctl(0, RST_RCO, NULL);

	return DAS::OPERATION_OK;
}

// Aborts the acquisition
int EMIRDasControllerHw::abort() {
        std::cout << "EMIRDasControllerHw::abort() starts" << endl;

	EMIRDasControllerHw::aquisitionCode->stop();

        std::cout << "EMIRDasControllerHw::abort() ends" << endl;
	return DAS::OPERATION_OK;
}

// Turns off the SDSUcontroller
int EMIRDasControllerHw::shutdown(const String powerOff) {
	int retVal;
	char *buffer;
	size_t sizeOfBuffer;
	DetectorControllerApp *powerOffCode;

	retVal = loadLODFile(powerOff, buffer, &sizeOfBuffer);
	if ( buffer == NULL ) {
		return retVal;
	}

	powerOffCode = new DetectorControllerApp::DetectorControllerApp(buffer, sizeOfBuffer);

        std::cout << "EMIRDasControllerHw::shutdown - running Power OFF application" << powerOff << endl;
	powerOffCode->run();

	return DAS::OPERATION_OK;
}

int EMIRDasControllerHw::startRead()
{
	EMIRDasControllerHw::aquisitionCode->startAqu();

	return DAS::OPERATION_OK;
;}

// This operations checks the correct operation of DC and returns a code error indicating
// success or failor. The symbol in which it writes has to be present in the controller.
// If it is not found, the function will return OPERATIONFAILED
int EMIRDasControllerHw::check()
{
	int val, wVal = 9999, rVal;
	int result;
	
	if ( aquisitionCode->read("NROWS", &val) != APP_OK  )
		return DAS::OPERATIONFAILED;
			
	if ( aquisitionCode->write("NROWS", wVal) != APP_OK )
		return DAS::OPERATIONFAILED;

	if ( aquisitionCode->read("NROWS", &rVal) != APP_OK )
		return DAS::OPERATIONFAILED;

        if ( wVal == rVal )
		if ( aquisitionCode->write("NROWS", val) != APP_OK )
			return DAS::OPERATIONFAILED;
		else
			return DAS::OPERATION_OK;
	else
		return DAS::OPERATIONFAILED;
}

int EMIRDasControllerHw::getVal(const char* variable, INT32 *val)
{
	int result;

	result = aquisitionCode->read(variable, (int *)val);
        
	if ( result != APP_OK )
		result = DAS::OPERATIONFAILED;
	else 
		result = DAS::OPERATION_OK;

	return result;
}

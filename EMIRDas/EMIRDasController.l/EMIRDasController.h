#ifndef _EMIRDasController_h_
#define _EMIRDasController_h_

#include <stdlib.h>

#include "TimeService.h"
#include "SimpleMemoryManager.h"
#include "EMIRDasControllerHw.h"
#include "EMIRDasConf.h"
#include "ace/Thread_Manager.h"
#include "../include/EMIRDasFixed.h"


#include "ace/Log_Msg.h"
#include "ace/OS.h"
#include "ace/High_Res_Timer.h"
#include "ace/Thread.h"
#include "TimeService.h"
#include "ErrorCode.h"
#define CPLUS
#ifdef VXWORKS
#include "msgQLib.h"
#include "../SDSUlcu.l/ct_interface.h"
#include "../SDSUlcu.l/sdsu_emir_driver.h"    /* sdsu_init(), sdsu_cleanup() */
#else
#include "../SDSUsim.l/ct_interface.h"
#endif
#include "EMIRDasControllerHw.h"
#include "EMIRDasFrame.h"
#include "SimpleMemoryManager.h"
#include "SimpleProcManager.h"
#include "SimpleStorageManager.h"


#define IMG_DIR	"/scratch/EMIR/frames/"

/**
 * The EMIRDasController class implements specific detector
 * controller operations. This class follows the pimpl pattern.
 *

 * It provides access to specific detector controller operations
 * through the ioctl command.The operations provided by EMIR
 * ,"ioctl operations" are:
 *         1. DC_CREATE, Creates the Detector Controller Resources.
 *                       Called only once.
 *         2. DC_INIT, Performs the initial operations on system
 *         3. DC_ABORT, Aborts the current operation.
 *         4. DC_CONFIG, Applied before the DC_START command. It is used
 *         5. DC_CHANNEL_NAME. Sets the name of the channel used to
 *                             nofify events ocurred in the DC.
 *
 */

class EMIRDasController {

public:

	EMIRDasController( staticData profileData );

	~EMIRDasController();

	/**
	 * Ioctl operation that allows managing the DC, indicating which
	 * kind of operation it is desired to perform with it.
	 * The operations  that can be performed by ioctl are encapsultated
	 * as private operations of this class.
	 * @param cmd  Command to execute.
	 * @param arg  Operation parameters.
	 */
	const char * report(const short level);
	//Functions called by DetectorControllerEventInfo.
	//void handler_add_associate_data(Event & evt);


	/**
	 * Executes the init command on SDSUController. This corresponds
	 * to send some initial commands to SDSUController so that it
	 * will be ready to receive configuration and start commands.
	 * The constants DETECTOR_CONTROLLER_INIT_BETWEEN_TIMEOUT and
	 * DETECTOR_CONTROLLER_INIT_TIMEOUT_WAITING_ANSWER are used in
	 * this operation.
	 *
	 * Possible Returns sending initialization command.
	 *
	 *  @return DAS::TIMEOUT
	 *              If the timeout expires.
	 *
	 *  @return DAS::OPERATIONFAILED
	 *              Error in the answer of the DC. Indicates some problem in
	 *              the communication.
	 *
	 *  @return DAS::OPERATION_OK
	 *              All commands send correctly.
	 *
	 * No Exceptions.
	 */
	int init();

        int halt();

	/**
	 * Sends the configuration commands to SDSU Controller and configures
	 * the timeouts of the FrameGrabber.
	 *
	 * This operation is needed to be set, always that the configuration
	 * of SDSU Controller changes. This is when the acquisition parameters
	 * changes between acquisition and acquisition. If the acquisition parameters
	 * are the same between acquisition and acquisition this operation is not
	 * needed to be called.
	 *
	 * @param acqparameters    Acquisition parameters.
	 * @todo. In this function framegrabber timeouts are fixed. Change appropiately.
	 * @todo TimeoutBeforeCapture and TimeouDataTransfer.
	 * Possible Returns sending command.
	 *
	 *  @return DAS::TIMEOUT
	 *              If the timeout expires.
	 *
	 *  @return DAS::OPERATIONFAILED
	 *              Error in the answer of the DC. Indicates some problem in
	 *              the communication.
	 *
	 *  @return DAS::OPERATION_OK
	 *              All commands send correctly.
	 */
	int config(const AcqConfig &acqparameters);

	/**
	 * Precondition: Call before(once) "configure" operation.
	 *               Call before(once) "read" operation.
	 * This operation starts the acquisition operation in the controller command line,
	 * reads each of the frames through the data collector communication channel.
	 * Each time a frame is read  provokes an event DataIsAvailable. This event is used by
	 * external entities to provide a new bucket for the next acquisition,
	 * if it is needed. The operation that is used to provide a new bucket is setMemory.
	 * Note: This method may be aborted by the operation abort.
	 * When this operation ends, a AcquisitionDoneDC event will be generated.
	 *
	 *  @return DAS::TIMEOUT
	 *              If the timeout expires.
	 *
	 *  @return DAS::OPERATIONFAILED
	 *              Error in the answer of the DC. Indicates some problem in
	 *              the communication.
	 *
	 *  @return DAS::OPERATION_OK
	 *              All commands send correctly.

	 */
	int start(bool sendFrame, const short poolSize);

	/**
	 * This operation starts the aborting mechanism. This operation
	 * does not block the thread that calls it. This operation is called
	 * when the start operation is in progress.
	 * This operation is responsible for cancelling the current acquisition,
	 * its scope is limited by the start operation.
	 *
	 * The operation start will be aborted sometime after this operation
	 * is called. We will know that the operation AcquisitionTask has been
	 * aborted because the event DetectorControllerAborted is launched by the
	 * aborting mechanism.
	 * @exception  CannotProceed if FrameGrabber LWDevice::CRITICAL_
	 * @exception  CannotProceed  "DIG16_capture_to_image", "FrameGrabber::cancel"     *
	 * @return DAS::TIMEOUT
	 *              If the timeout expires.
	 *
	 * @return DAS::OPERATIONFAILED
	 *              Error in the answer of the DC. Indicates some problem in
	 *              the communication.
	 *
	 * @return DAS::OPERATION_OK
	 *              All commands send correctly.

	 */
	int abort(void);

	/**
	 * Checks the detector controller devices.
	 * @returns a value different from 0 in case of error.
	 *     0: Ok
	 *    -1: Sending commands to SDSU Fails
	 *    -2: Sending acquisition configuraion to SDSU fails.
	 *    -3: Executing capture frame fails.
	 *    -4: Sending start command to SDSU fails
	 *    -5: Wait acquisition ends with error.
	 *    -6: Values received from SDSU controller are not correct.
	 *
	 */
	virtual int check();

	int shutdown();

	int reset();

	int test();

	int expose(AcqConfig & acqparameters, bool confChange);

        int runStorageManager(void);
	int runProcManager(void);
	int runExpose(void);


        Trace *trace_;

	bool storageThreadRunning;
	bool procThreadRunning;
/*	unsigned int storageOps;
	unsigned int procOps;*/

	staticData fixData;

	EMIRDasControllerHw *dchardwarefacade_;		// Low level access
	SimpleMemoryManager *memoryMgrProc;		// Frame processing queue
	SimpleMemoryManager *memoryMgrStr;		// Frame storage queue
	void *dumpFrame;				
	bool abortDAS;


//	int runProcManager(void);
private:

	int readDetector(unsigned int iter, AcqConfig params, bool last);

	bool aborting_; //Indicates that aborting is in progress.
	bool created_; // Indicates if detector controller has been created.
	bool continuereadexperimental_;// Indates when the loop of read experimental must be broken.
	bool requirereleasebucket_;// Indicates when the bucket is acquired.

#ifdef VXWORKS
	int storageThId_;
	int procThId_;
        int exposeThId_;
#else
        ACE_thread_t storageThId_;
        ACE_thread_t procThId_;
        ACE_thread_t exposeThId_;
#endif

  // Functions provided by logAgent
//  void logInfo_(const char* format, ...);
//  void logError_(const char* format, ...);
//  void logDebug_(const char* file, unsigned line, const char* format, ...);

  // Agents
//  LogAgent*         logAgent_;
};
#endif // _EMIRDasController_h_

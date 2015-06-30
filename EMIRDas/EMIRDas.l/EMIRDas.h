// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

#ifndef _EMIRDas_h_
#define _EMIRDas_h_

// Includes
#include "Device.h"
#include "ConcreteArrays.h"
#include "EMIRDasController.h"
#include "EMIRDasConf.h"

#include <CORBARef.h>
#include <IDL_Adapters_DPKF.h>
#include "EMIRDFAGENTC.h"

/**
* 
*/
class EMIRDas : public Device
{
  //--------------------------------------------------
  // public section 
  //--------------------------------------------------
  public:

  /**
   *  Current readout mode
   */
  enum ReadoutMode
  {
    SINGLE, CDS, FOWLER, RAMP, HDR_SIMPLE, BIAS, MRAMP, UNDEFINED 
  };

  /**
   *  Current SDSUController state
   */
  enum SDSUControllerStatus
  {
    DETECTOR_IDLE, DETECTOR_ERROR, DETECTOR_RUNNING, DETECTOR_ENDS
  };

  struct AcquisitionConfiguration
  {
    String title;
    ReadoutMode readoutmodeval;
    long  exposuretime;
    long  x_size;
    long  y_size;
    bool windowMode;
    bool applyConfiguration;
    bool applySimulateImage;
    bool saveImages;
    long nsec;                      // Number of sequences in an observation
    long optrst;                    // Type of detector resets to be performed
    long nrst;                      // Number of detector resets
    long nrdbl;                     // Number of detector resets before loop
    long nrdbl_nd;                  // Detectors resets before the loop with no data
    long numExp;                    // Number of expositions per sequence
    long nrdil;                     // Number of detector resets inside the loop
    long nrdil_nd;                  // Detector resets inside the loop with no data
    long nparam_x;                  // Number of dummy readouts of a line
  };

  //####################### CONSTRUCTOR & DESTRUCTOR #####################
  
  /** Device Constructor */
  EMIRDas(string name);

  /** Destructor */
  virtual ~EMIRDas();

  //########################## DEVICE METHODS ############################
  
  /** Connects the device with a CORBA and other GCS Services*/
  void  connect(string orbname, unsigned threads,long priority,
                bool mode  =Device::INTEGRATED,
                bool update=Device::DO_NOT_UPDATE);
  
  /** Starting procedure */
  void  start();
  
  /** Initialization procedure */
  void  init();
  
  /** Halting procedure */
  void  halt();
  
  /** Shutdown procedure */
  void  shutdown();
  
  /** Reset procedure */
  void  reset();
  
  /** Restore procedure */
  void  restore();
  
  /** Test procedure */
  void  test();
  
  /** Stops all commands and goes to fault */
  void  abort();
  
  /** Report device info */
  char* report(short level);
  
  /** Reports device version */
  const char* version();
  
  //####################### SPECIFIC METHODS #############################

  /**
  * Setup the intrument detector.
  * @param:acquisitionConfiguration As their name tell us is the configuration of an acquisition..
  */
  void setupDetector(AcquisitionConfiguration acquisitionConfiguration);

  /**
  * Instructs the detector controller to start taking the specified number of
  * images.
  * @:Repeats last configuration.
  */
  void expose();

  /**
  * Setup the readout mode.
  * @param:readoutMode Readout mode.
  */
  void setupReadoutMode(ReadoutMode readoutmodeval);

  /**
  * Setup exposureId
  * @param:exposureId Sets the acquisitionId
  */
  void setupImageTitle(const char* exposureId);

  /**
  * Setup exposureTime.
  * @param:exposureTime Sets the desired exposure time
  */
  void setupExposureTime(long exposureTime);

  /**
  * Setup the window mode
  * @param:window mode
  */
  void setupWindowMode(bool windowmode);

  /**
  * Setup the NSEC controller parameter
  * @param:nsec
  */
  void setupNSEC(long nsec);

  /**
  * Setup the NO_EXPOSURES controller parameter
  * @param:nexp
  */
  void setupNO_EXPOSURES(long nexp);

  /**
  * Setup the NRDBL controller parameter
  * @param:nrdbl
  */
  void setupNRDBL(long nrdbl);

  /**
  * Setup the NRDBL_ND controller parameter
  * @param:nrdbl_nd
  */
  void setupNRDBL_ND(long nrdbl_nd);

  /**
  * Setup the NRST controller parameter
  * @param:nrst
  */
  void setupNRST(long nrst);

  /**
  * Setup the NPARAM_X controller parameter
  * @param:nparam_x
  */
  void setupNPARAM_X(long nparam_x);

  /**
  * Setup the NRDIL controller parameter
  * @param:nrdil
  */
  void setupNRDIL(long nrdil);

  /**
  * Setup the NRDIL_ND controller parameter
  * @param:nrdil_nd
  */
  void setupNRDIL_ND(long nrdil_nd);

  /**
  * Setup the OPTRST controller parameter
  * @param:optrst
  */
  void setupOPTRST(long optrst);

  /**
  * Setup the X_SIZE controller parameter
  * @param:x_size
  */
  void setupX_SIZE(long x_size);

  /**
  * Setup the Y_SIZE controller parameter
  * @param:y_size
  */
  void setupY_SIZE(long y_size);

  /**
  * Setup the Y_SIZE_B controller parameter
  * @param:y_size_b
  */
  void setupY_SIZE_B(long y_size_b);
  
  //####################### MONITORS #####################################

  /**
  * DAS is setting up.
  * @magnitude:
  * @description:It is True if DAS is applying the configuration.
  * @maximum:True
  * @minimum:False
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  bool isSettingup();

  /**
  * DAS is configurationDone.
  * @magnitude:
  * @description:It is True if DAS is the configuration has been applied.
  * @maximum:True
  * @minimum:False
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  bool isConfigurationDone();

  /**
  * DAS is exposing.
  * @magnitude:
  * @description:It is True if the CCD Controller is taking exposures.
  * @maximum:True
  * @minimum:False
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  bool isExposing();

  /**
  * Detector controller is in idle.
  * @magnitude:
  * @description:It is True if the Acquisition system is in Idle mode.
  * @maximum:True
  * @minimum:False
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  bool isDetectorControllerIdle();

  /**
  * Current readout mode
  * @magnitude:
  * @description:Readout mode.
  * @maximum:7
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  * @values:SINGLE, CDS, FOWLER, RAMP, HDR_SIMPLE, BIAS, MRAMP, UNDEFINED 
  */
  ReadoutMode currentReadoutMode();

  /**
  * Current window mode
  * @magnitude:
  * @description:window mode.
  * @maximum:True
  * @minimum:False
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  bool currentWindowMode();

  /**
  * Current SDSUController state
  * @magnitude:
  * @description:Current status SDSU Controller
  * @maximum:4
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  * @values:DETECTOR_IDLE, DETECTOR_ERROR, DETECTOR_RUNNING, DETECTOR_ENDS
  */
  SDSUControllerStatus currentSDSUControllerStatus();

  /**
  * Elapsed Time
  * @magnitude:
  * @description:Current elapsed Time
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentElapsedTime();

  /**
  * Desired Exposure Time
  * @magnitude:
  * @description:Current Exposure Time
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentExposureTime();

  /**
  * Current Number of Images
  * @magnitude:
  * @description:Current number of images
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNumberOfImages();

  /**
  * Current NSEC
  * @magnitude:
  * @description:Current NSEC controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNSEC();

  /**
  * Current NO_EXPOSURES
  * @magnitude:
  * @description:Current NO_EXPOSURES controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNO_EXPOSURES();

  /**
  * Current NRDBL
  * @magnitude:
  * @description:Current NRDBL controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNRDBL();

  /**
  * Current NRDBL_ND
  * @magnitude:
  * @description:Current NRDBL_ND controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNRDBL_ND();

  /**
  * Current NPARAM_X
  * @magnitude:
  * @description:Current NPARAM_X controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNPARAM_X();

  /**
  * Current NRST
  * @magnitude:
  * @description:Current NRST controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNRST();

  /**
  * Current NRDIL
  * @magnitude:
  * @description:Current NRDIL controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNRDIL();

  /**
  * Current NRDIL_ND
  * @magnitude:
  * @description:Current NRDIL_ND controller value
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentNRDIL_ND();

  /**
  * Current OPTRST
  * @magnitude:
  * @description:Current OPTRST controller value
  * @maximum:3
  * @minimum:0
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentOPTRST();

  /**
  * Current X_SIZE
  * @magnitude:
  * @description:Current X_SIZE controller value
  * @maximum:4096
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentX_SIZE();

  /**
  * Current Y_SIZE
  * @magnitude:
  * @description:Current Y_SIZE controller value
  * @maximum:2048
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentY_SIZE();

  /**
  * Current Y_SIZE_B
  * @magnitude:
  * @description:Current Y_SIZE_B controller value
  * @maximum:2048
  * @minimum:1
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  long currentY_SIZE_B();
  
  //####################### PROPERTIES ##################################

  /**
  * Power on LOD
  * @property:
  * @description:Path to the power on controller program
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* poweronApp();
  void poweronApp(const char* new_value); 

  /**
  * Power off LOD
  * @property:
  * @description:Path to the power off controller program
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* poweroffApp();
  void poweroffApp(const char* new_value); 

  /**
  * Adquisiton LOD
  * @property:
  * @description:Path to the adquisition controller program
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* adqApp();
  void adqApp(const char* new_value); 

  /**
  * Strorage location
  * @property:
  * @description:Path to the image storage directory
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* imgDir();
  void imgDir(const char* new_value); 
  
  /**
  * Simulated Image location
  * @property:
  * @description:Path to the directory where the simulated mode images are stored
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* simFile();
  void simFile(const char* new_value); 
  
  /**
  * Strorage location
  * @property:
  * @description:Data Factory ID
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* dataFactoryName();
  void dataFactoryName(const char* new_value); 
  
  /**
  * Strorage location
  * @property:
  * @description:Is the Data Factory being used ?
  * @maximum:1
  * @minimum:0
  * @units:None
  * @sampling:1.0s
  * @storage:0.0s
  */
  const short isDataFactoryEnabled();
  void isDataFactoryEnabled(const short new_value); 
  
  /**
  * Controller firmware version
  * @property:
  * @description: Version number of the acquisition application
  * @maximum:999999
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const long firmware();
  void firmware(const long new_value); 

  /**
  * Detector Type
  * @property:
  * @description: The detector being used ENG, SCI, MUX, ...
  * @maximum:1
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const char* detectorType();
  void detectorType(const char* new_value); 
  
  /**
  * saveFile
  * @property:
  * @description: When set to 1, the DAS will create a FITS file in imgDir for every frame
  * @maximum:1
  * @minimum:0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const short saveFile();
  void saveFile(const short new_value); 

  /**
  * sendFrame
  * @property:
  * @description: The DAS will send frames to the DFAgent
  * @maximum:1
  * @minimum:0
  * @units:Non0
  * @sampling:1.0s
  * @storage:0.0s
  */
  const short sendFrame();
  void sendFrame(const short new_value); 

  /**
  * simDetector
  * @property:
  * @description: The DAS will simulate access to the controller
  * @maximum:1
  * @minimum:0
  * @units:Non0
  * @sampling:1.0s
  * @storage:0.0s
  */
  const short simDetector();
  void simDetector(const short new_value); 

  /**
  * simMode
  * @property:
  * @description: Simulation Mode, 0 = Pattern, 1 = File
  * @maximum:1
  * @minimum:0
  * @units:Non0
  * @sampling:1.0s
  * @storage:0.0s
  */
  const short simMode();
  void simMode(const short new_value); 

  /**
  * @reorder
  * @property:
  * @description: Reorder the image from the detector
  * @maximum:1
  * @minimum:0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const short reorder();
  void reorder(const short new_value); 

  /**
  * @invert
  * @property:
  * @description: Invert the image
  * @maximum:1
  * @minimum:0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const short invert();
  void invert(const short new_value); 

  /**
  * Frame pool size
  * @property:
  * @description: Size of the frame queue 
  * @maximum:37
  * @minimum:1
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const short poolSize();
  void poolSize(const short new_value); 

  /**
  * Voffset
  * @property:
  * @description: voffset
  * @maximum:999999.0
  * @minimum:-999999.0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const double voffset();
  void voffset(const double new_value); 

  /**
  * vreset
  * @property:
  * @description: vreset
  * @maximum:999999.0
  * @minimum:-999999.0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const double vreset();
  void vreset(const double new_value); 

  /**
  * biasgate
  * @property:
  * @description: biasgate
  * @maximum:999999.0
  * @minimum:-999999.0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const double biasgate();
  void biasgate(const double new_value); 

  /**
  * ron
  * @property:
  * @description: ron
  * @maximum:999999.0
  * @minimum:-999999.0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const double ron();
  void ron(const double new_value); 

  /**
  * gain
  * @property:
  * @description: gain
  * @maximum:999999.0
  * @minimum:-999999.0
  * @units:None
  * @sampling:0.0s
  * @storage:0.0s
  */
  const double gain();
  void gain(const double new_value); 

  //-----------------------------------------------------------------
  // protected section 
  //-----------------------------------------------------------------
  protected:
  
  //-----------------------------------------------------------------
  // private section 
  //-----------------------------------------------------------------
  private:
  
  // Monitorable attributes (magnitudes)
  bool                 isSettingup_                ; ///<DAS is setting up.
  bool                 isConfigurationDone_        ; ///<DAS is configurationDone.
  bool                 isExposing_                 ; ///<DAS is exposing.
  bool                 isDetectorControllerIdle_   ; ///<Detector controller is in idle.
  bool                 currentWindowMode_          ; ///<Current window mode.
  ReadoutMode          currentReadoutMode_         ; ///<Current readout mode
  SDSUControllerStatus currentSDSUControllerStatus_; ///<Current SDSUController state
  long                 currentElapsedTime_         ; ///<Elapsed Time
  long                 currentExposureTime_        ; ///<Desired Exposure Time
  long                 currentNumberOfImages_      ; ///<Current Number of Images
  long                 currentNSEC_                ; ///<Current NSEC
  long                 currentNO_EXPOSURES_        ; ///<Current NO_EXPOSURES
  long                 currentNRDBL_               ; ///<Current NRDBL
  long                 currentNRDBL_ND_            ; ///<Current NRDBL_ND
  long                 currentNPARAM_X_            ; ///<Current NPARAM_X
  long                 currentNRST_                ; ///<Current NRST
  long                 currentNRDIL_               ; ///<Current NRDIL
  long                 currentNRDIL_ND_            ; ///<Current NRDIL_ND
  long                 currentOPTRST_              ; ///<Current OPTRST
  long                 currentX_SIZE_              ; ///<Current X_SIZE
  long                 currentY_SIZE_              ; ///<Current Y_SIZE
  long                 currentY_SIZE_B_            ; ///<Current Y_SIZE_B
  
  // Configurable attributes (properties)
  String poweronApp_ ; ///<Power on LOD
  String poweroffApp_; ///<Power off LOD
  String adqApp_     ; ///<Adquisiton LOD
  String imgDir_     ; ///<Strorage location
  String simFile_     ; ///<Strorage location
  String dataFactoryName_ ; ///<Data Factory ID
  String detectorType_ ; ///<Detector Type ENG, MUX, SCI, ...
  short  isDataFactoryEnabled_; ///<Data Factory On/Off
  long firmware_     ; ///<Controller firmware
  short saveFile_    ; ///<To save in FITS file or not to save, that is the question.
  short sendFrame_   ; ///<Send frame to DFAgent
  short simDetector_ ; ///<Simulate controller operation
  short simMode_     ; ///<Simulator Mode
  short reorder_     ; ///<Reorder the image from the detector
  short invert_      ; ///<Invert the frame
  short poolSize_    ; ///<Frame queue size
  double voffset_     ; ///<voffset
  double vreset_      ; ///<vreset
  double biasgate_    ; ///<biasgate
  double ron_         ; ///<ron
  double gain_        ; ///<gain

  // 
  
  // Methods to create, start and stop monitors
  void createMonitors_();
  
  
  
  // Method to create Devices
  void createDevices_(bool update);

  // Manualy added

  AcqConfig currentConfiguration;

  EMIRDasController* controller_;

  bool confChange;

};

#endif // _EMIRDas_h_ 


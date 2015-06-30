// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

#ifndef _EMIRDasAdapter_h_
#define _EMIRDasAdapter_h_

// Includes
#include "EMIRDASS.h"
#include "DeviceAdapter.h"
#include "EMIRDas.h"

class EMIRDas;

//This class implements the IDL interface EMIRDas_ifce
class EMIRDasAdapter : public virtual POA_EMIRDAS::EMIRDas_ifce,  public DeviceAdapter
{
  //--------------------------------------------------
  // public section 
  //--------------------------------------------------
  public:
  
  // Constructor
  EMIRDasAdapter(EMIRDas* device);

  // Destructor
  ~EMIRDasAdapter();


  //----------------------------------------------------------------------
  // Device specific operations
  //----------------------------------------------------------------------
  void   setupDetector    (const EMIRTYPES::AcquisitionConfiguration& acquisitionConfiguration);
  void   expose           ();
  void   setupReadoutMode (EMIRTYPES::ReadoutMode readoutmodeval);
  void   setupImageTitle  (const char* exposureId);
  void   setupExposureTime(CORBA::Long exposureTime);
  void   setupWindowMode  (CORBA::Boolean windowmode);
  void   setupNSEC        (CORBA::Long nsec);
  void   setupNO_EXPOSURES(CORBA::Long nexp);
  void   setupNRDBL       (CORBA::Long nrdbl);
  void   setupNRDBL_ND    (CORBA::Long nrdbl_nd);
  void   setupNRST        (CORBA::Long nrst);
  void   setupNPARAM_X    (CORBA::Long nparam_x);
  void   setupNRDIL       (CORBA::Long nrdil);
  void   setupNRDIL_ND    (CORBA::Long nrdil_nd);
  void   setupOPTRST      (CORBA::Long optrst);
  void   setupX_SIZE      (CORBA::Long x_size);
  void   setupY_SIZE      (CORBA::Long y_size);
  void   setupY_SIZE_B    (CORBA::Long y_size_b);

  //----------------------------------------------------------------------
  // Monitorable attributes (device magnitudes) 
  //----------------------------------------------------------------------
  CORBA::Boolean       isSettingup                ();
  CORBA::Boolean       isConfigurationDone        ();
  CORBA::Boolean       isExposing                 ();
  CORBA::Boolean       isDetectorControllerIdle   ();
  CORBA::Boolean       currentWindowMode          ();
  EMIRTYPES::ReadoutMode currentReadoutMode       ();
  EMIRTYPES::SDSUControllerStatus  currentSDSUControllerStatus();
  CORBA::Long          currentElapsedTime         ();
  CORBA::Long          currentExposureTime        ();
  CORBA::Long          currentNumberOfImages      ();
  CORBA::Long          currentNSEC                ();
  CORBA::Long          currentNO_EXPOSURES        ();
  CORBA::Long          currentNRDBL               ();
  CORBA::Long          currentNRDBL_ND            ();
  CORBA::Long          currentNPARAM_X            ();
  CORBA::Long          currentNRST                ();
  CORBA::Long          currentNRDIL               ();
  CORBA::Long          currentNRDIL_ND            ();
  CORBA::Long          currentOPTRST              ();
  CORBA::Long          currentX_SIZE              ();
  CORBA::Long          currentY_SIZE              ();
  CORBA::Long          currentY_SIZE_B            ();

  //----------------------------------------------------------------------
  // Configurable attributes (device properties) 
  //----------------------------------------------------------------------
  char*                poweronApp ();
  void                 poweronApp (const char* value); 
  char*                poweroffApp();
  void                 poweroffApp(const char* value); 
  char*                adqApp     ();
  void                 adqApp     (const char* value); 
  char*                imgDir     ();
  void                 imgDir     (const char* value); 
  char*                simFile     ();
  void                 simFile     (const char* value); 
  char*                dataFactoryName ();
  void                 dataFactoryName (const char* value); 
  short                isDataFactoryEnabled ();
  void                 isDataFactoryEnabled (const short value); 
  CORBA::Long          firmware ();
  void                 firmware (CORBA::Long value); 
  char*                detectorType ();
  void                 detectorType (const char* value); 
  short                saveFile ();
  void                 saveFile (const short value); 
  short                sendFrame ();
  void                 sendFrame (const short value); 
  short                simDetector ();
  void                 simDetector (const short value); 
  short                simMode ();
  void                 simMode (const short value); 
  short                reorder ();
  void                 reorder (const short value); 
  short                invert ();
  void                 invert (const short value); 
  short                poolSize ();
  void                 poolSize (const short value); 
  double                voffset ();
  void                 voffset (const double value); 
  double                vreset ();
  void                 vreset (const double value); 
  double                biasgate ();
  void                 biasgate (const double value); 
  double                ron ();
  void                 ron (const double value); 
  double                gain ();
  void                 gain (const double value); 

  
  //-----------------------------------------------------------------
  // protected section 
  //-----------------------------------------------------------------
  protected:

  //--------------------------------------------------
  // private section 
  //--------------------------------------------------
  private:

  EMIRDas* device_;

  void toIDL_(const EMIRDas::AcquisitionConfiguration& st, EMIRTYPES::AcquisitionConfiguration& idl_struct);
  void fromIDL_(const  EMIRTYPES::AcquisitionConfiguration& idl_struct, EMIRDas::AcquisitionConfiguration& st);

};

#endif // _EMIRDasAdapter_h_ 


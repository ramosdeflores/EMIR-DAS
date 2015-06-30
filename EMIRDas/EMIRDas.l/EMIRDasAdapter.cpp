// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

// Includes
#include "EMIRDasAdapter.h"
#include "EMIRDas.h"
#include "IDL_Adapters_DGT.h"
//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
EMIRDasAdapter::EMIRDasAdapter(EMIRDas* device)
 : DeviceAdapter(device)
{
  device_=device;
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
EMIRDasAdapter::~EMIRDasAdapter()
{
}


//----------------------------------------------------------------------
// Device specific operations
//----------------------------------------------------------------------
void EMIRDasAdapter::setupDetector(const EMIRTYPES::AcquisitionConfiguration& acquisitionConfiguration)
{
  try
  {
    EMIRDas::AcquisitionConfiguration acquisitionConfiguration_cpp;
    fromIDL_(acquisitionConfiguration,acquisitionConfiguration_cpp);
    device_->setupDetector(acquisitionConfiguration_cpp);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::expose()
{
  try
  {
    device_->expose();
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupReadoutMode(EMIRTYPES::ReadoutMode readoutmodeval)
{
  try
  {
    device_->setupReadoutMode((EMIRDas::ReadoutMode)readoutmodeval);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupImageTitle(const char* exposureId)
{
  try
  {
    device_->setupImageTitle(exposureId);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupExposureTime(CORBA::Long exposureTime)
{
  try
  {
    device_->setupExposureTime(exposureTime);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupWindowMode(CORBA::Boolean windowmode)
{
  try
  {
    device_->setupWindowMode(windowmode);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNSEC(CORBA::Long nsec)
{
  try
  {
    device_->setupNSEC(nsec);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNO_EXPOSURES(CORBA::Long nexp)
{
  try
  {
    device_->setupNO_EXPOSURES(nexp);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNRDBL(CORBA::Long nrdbl)
{
  try
  {
    device_->setupNRDBL(nrdbl);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNRDBL_ND(CORBA::Long nrdbl_nd)
{
  try
  {
    device_->setupNRDBL_ND(nrdbl_nd);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNRST(CORBA::Long nrst)
{
  try
  {
    device_->setupNRST(nrst);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNPARAM_X(CORBA::Long nparam_x)
{
  try
  {
    device_->setupNPARAM_X(nparam_x);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNRDIL(CORBA::Long nrdil)
{
  try
  {
    device_->setupNRDIL(nrdil);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupNRDIL_ND(CORBA::Long nrdil_nd)
{
  try
  {
    device_->setupNRDIL_ND(nrdil_nd);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupOPTRST(CORBA::Long optrst)
{
  try
  {
    device_->setupOPTRST(optrst);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupX_SIZE(CORBA::Long x_size)
{
  try
  {
    device_->setupX_SIZE(x_size);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupY_SIZE(CORBA::Long y_size)
{
  try
  {
    device_->setupY_SIZE(y_size);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------
void EMIRDasAdapter::setupY_SIZE_B(CORBA::Long y_size_b)
{
  try
  {
    device_->setupY_SIZE_B(y_size_b);
  }
  catch(GCSException& ex)
  {
    throw toIDL(ex);
  }
}

//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Monitorable attributes (device magnitudes) 
//----------------------------------------------------------------------

// Monitor isSettingup
CORBA::Boolean EMIRDasAdapter::isSettingup()
{
  return device_->isSettingup();
}

// Monitor isConfigurationDone
CORBA::Boolean EMIRDasAdapter::isConfigurationDone()
{
  return device_->isConfigurationDone();
}

// Monitor isExposing
CORBA::Boolean EMIRDasAdapter::isExposing()
{
  return device_->isExposing();
}

// Monitor isDetectorControllerIdle
CORBA::Boolean EMIRDasAdapter::isDetectorControllerIdle()
{
  return device_->isDetectorControllerIdle();
}

// Monitor currentReadoutMode
EMIRTYPES::ReadoutMode EMIRDasAdapter::currentReadoutMode()
{
  int enumerated = (int)device_->currentReadoutMode();
  return (EMIRTYPES::ReadoutMode)enumerated;
}

// Monitor currentSDSUControllerStatus
EMIRTYPES::SDSUControllerStatus EMIRDasAdapter::currentSDSUControllerStatus()
{
  int enumerated = (int)device_->currentSDSUControllerStatus();
  return (EMIRTYPES::SDSUControllerStatus)enumerated;
}

// Monitor currentWindowMode
CORBA::Boolean EMIRDasAdapter::currentWindowMode()
{
  return device_->currentWindowMode();
}

// Monitor currentElapsedTime
CORBA::Long EMIRDasAdapter::currentElapsedTime()
{
  return device_->currentElapsedTime();
}

// Monitor currentExposureTime
CORBA::Long EMIRDasAdapter::currentExposureTime()
{
  return device_->currentExposureTime();
}

// Monitor currentNumberOfImages
CORBA::Long EMIRDasAdapter::currentNumberOfImages()
{
  return device_->currentNumberOfImages();
}

// Monitor currentNSEC
CORBA::Long EMIRDasAdapter::currentNSEC()
{
  return device_->currentNSEC();
}

// Monitor currentNO_EXPOSURES
CORBA::Long EMIRDasAdapter::currentNO_EXPOSURES()
{
  return device_->currentNO_EXPOSURES();
}

// Monitor currentNRDBL
CORBA::Long EMIRDasAdapter::currentNRDBL()
{
  return device_->currentNRDBL();
}

// Monitor currentNRDBL_ND
CORBA::Long EMIRDasAdapter::currentNRDBL_ND()
{
  return device_->currentNRDBL_ND();
}

// Monitor currentNPARAM_X
CORBA::Long EMIRDasAdapter::currentNPARAM_X()
{
  return device_->currentNPARAM_X();
}

// Monitor currentNRST
CORBA::Long EMIRDasAdapter::currentNRST()
{
  return device_->currentNRST();
}

// Monitor currentNRDIL
CORBA::Long EMIRDasAdapter::currentNRDIL()
{
  return device_->currentNRDIL();
}

// Monitor currentNRDIL_ND
CORBA::Long EMIRDasAdapter::currentNRDIL_ND()
{
  return device_->currentNRDIL_ND();
}

// Monitor currentOPTRST
CORBA::Long EMIRDasAdapter::currentOPTRST()
{
  return device_->currentOPTRST();
}

// Monitor currentX_SIZE
CORBA::Long EMIRDasAdapter::currentX_SIZE()
{
  return device_->currentX_SIZE();
}

// Monitor currentY_SIZE
CORBA::Long EMIRDasAdapter::currentY_SIZE()
{
  return device_->currentY_SIZE();
}

// Monitor currentY_SIZE_B
CORBA::Long EMIRDasAdapter::currentY_SIZE_B()
{
  return device_->currentY_SIZE_B();
}

//----------------------------------------------------------------------
// Configurable attributes (device properties) 
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// get poweronApp
char* EMIRDasAdapter::poweronApp()
{
  return (char*) device_->poweronApp();
}
//-------------------------------
// set poweronApp
void EMIRDasAdapter::poweronApp(const char* value)
{
  try
  {
    device_->poweronApp(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get poweroffApp
char* EMIRDasAdapter::poweroffApp()
{
  return (char*) device_->poweroffApp();
}
//-------------------------------
// set poweroffApp
void EMIRDasAdapter::poweroffApp(const char* value)
{
  try
  {
    device_->poweroffApp(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get adqApp
char* EMIRDasAdapter::adqApp()
{
  return (char*) device_->adqApp();
}
//-------------------------------
// set adqApp
void EMIRDasAdapter::adqApp(const char* value)
{
  try
  {
    device_->adqApp(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get imgDir
char* EMIRDasAdapter::imgDir()
{
  return (char*) device_->imgDir();
}
//-------------------------------
// set imgDir
void EMIRDasAdapter::imgDir(const char* value)
{
  try
  {
    device_->imgDir(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get simFile
char* EMIRDasAdapter::simFile()
{
  return (char*) device_->simFile();
}
//-------------------------------
// set simFile
void EMIRDasAdapter::simFile(const char* value)
{
  try
  {
    device_->simFile(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get dataFactoryName
char* EMIRDasAdapter::dataFactoryName()
{
  return (char*) device_->dataFactoryName();
}
//-------------------------------
// set dataFactoryName
void EMIRDasAdapter::dataFactoryName(const char* value)
{
  try
  {
    device_->dataFactoryName(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get isDataFactoryEnabled
short EMIRDasAdapter::isDataFactoryEnabled()
{
  return (short) device_->isDataFactoryEnabled();
}
//-------------------------------
// set isDataFactoryEnabled
void EMIRDasAdapter::isDataFactoryEnabled(const short value)
{
  try
  {
    device_->isDataFactoryEnabled(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get firmware
CORBA::Long EMIRDasAdapter::firmware()
{
  return (long) device_->firmware();
}
//-------------------------------
// set firmware
void EMIRDasAdapter::firmware(CORBA::Long value)
{
  try
  {
    device_->firmware(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

//----------------------------------------------------------------------
// get detectorType
char* EMIRDasAdapter::detectorType()
{
  return (char*) device_->detectorType();
}
//-------------------------------
// set detectorType
void EMIRDasAdapter::detectorType(const char* value)
{
  try
  {
    device_->detectorType(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get saveFile
short EMIRDasAdapter::saveFile()
{
  return (short) device_->saveFile();
}
//-------------------------------
// set saveFile
void EMIRDasAdapter::saveFile(short value)
{
  try
  {
    device_->saveFile(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get sendFrame
short EMIRDasAdapter::sendFrame()
{
  return (short) device_->sendFrame();
}
//-------------------------------
// set sendFrame
void EMIRDasAdapter::sendFrame(short value)
{
  try
  {
    device_->sendFrame(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get simDetector
short EMIRDasAdapter::simDetector()
{
  return (short) device_->simDetector();
}
//-------------------------------
// set simDetector
void EMIRDasAdapter::simDetector(short value)
{
  try
  {
    device_->simDetector(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get simMode
short EMIRDasAdapter::simMode()
{
  return (short) device_->simMode();
}
//-------------------------------
// set simMode
void EMIRDasAdapter::simMode(short value)
{
  try
  {
    device_->simMode(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get reorder
short EMIRDasAdapter::reorder()
{
  return (short) device_->reorder();
}
//-------------------------------
// set reorder
void EMIRDasAdapter::reorder(short value)
{
  try
  {
    device_->reorder(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get invert
short EMIRDasAdapter::invert()
{
  return (short) device_->invert();
}
//-------------------------------
// set invert
void EMIRDasAdapter::invert(short value)
{
  try
  {
    device_->invert(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get poolSize
short EMIRDasAdapter::poolSize()
{
  return (short) device_->poolSize();
}
//-------------------------------
// set poolSize
void EMIRDasAdapter::poolSize(const short value)
{
  try
  {
    device_->poolSize(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get voffset
double EMIRDasAdapter::voffset()
{
  return (double) device_->voffset();
}
//-------------------------------
// set voffset
void EMIRDasAdapter::voffset(const double value)
{
  try
  {
    device_->voffset(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get vreset
double EMIRDasAdapter::vreset()
{
  return (double) device_->vreset();
}
//-------------------------------
// set vreset
void EMIRDasAdapter::vreset(const double value)
{
  try
  {
    device_->vreset(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get biasgate
double EMIRDasAdapter::biasgate()
{
  return (double) device_->biasgate();
}
//-------------------------------
// set biasgate
void EMIRDasAdapter::biasgate(const double value)
{
  try
  {
    device_->biasgate(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get ron
double EMIRDasAdapter::ron()
{
  return (double) device_->ron();
}
//-------------------------------
// set ron
void EMIRDasAdapter::ron(const double value)
{
  try
  {
    device_->ron(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

// get gain
double EMIRDasAdapter::gain()
{
  return (double) device_->gain();
}
//-------------------------------
// set gain
void EMIRDasAdapter::gain(const double value)
{
  try
  {
    device_->gain(value);
  }
  catch(GCSException& ex)
  {
    throw CORBA::BAD_PARAM();
  }
}

/**************************************************************
 *                private methods                             *
 **************************************************************/

void EMIRDasAdapter::toIDL_(const EMIRDas::AcquisitionConfiguration& st, EMIRTYPES::AcquisitionConfiguration& idl_struct)
{
  idl_struct.title = CORBA::string_dup(st.title);
  idl_struct.readoutmodeval = (EMIRTYPES::ReadoutMode)st.readoutmodeval;
  idl_struct.exposuretime = st.exposuretime;
  idl_struct.x_size = st.x_size;
  idl_struct.y_size = st.y_size;
  idl_struct.windowMode = st.windowMode;
  idl_struct.applyConfiguration = st.applyConfiguration;
  idl_struct.saveImages = st.saveImages;
  idl_struct.nsec = st.nsec;
  idl_struct.optrst = st.optrst;
  idl_struct.nrst = st.nrst;
  idl_struct.nrdbl = st.nrdbl;
  idl_struct.nrdbl_nd = st.nrdbl_nd;
  idl_struct.numExp = st.numExp;
  idl_struct.nrdil = st.nrdil;
  idl_struct.nrdil_nd = st.nrdil_nd;
  idl_struct.nparam_x = st.nparam_x;
}


void EMIRDasAdapter::fromIDL_(const EMIRTYPES::AcquisitionConfiguration& idl_struct, EMIRDas::AcquisitionConfiguration& st)
{
  st.title = (char*)idl_struct.title.in();
  st.readoutmodeval = (EMIRDas::ReadoutMode)idl_struct.readoutmodeval;
  st.exposuretime = idl_struct.exposuretime;
  st.x_size = idl_struct.x_size;
  st.y_size = idl_struct.y_size;
  st.windowMode = idl_struct.windowMode;
  st.applyConfiguration = idl_struct.applyConfiguration;
  st.saveImages = idl_struct.saveImages;
  st.nsec = idl_struct.nsec;
  st.optrst = idl_struct.optrst;
  st.nrst = idl_struct.nrst;
  st.nrdbl = idl_struct.nrdbl;
  st.nrdbl_nd = idl_struct.nrdbl_nd;
  st.numExp = idl_struct.numExp;
  st.nrdil = idl_struct.nrdil;
  st.nrdil_nd = idl_struct.nrdil_nd;
  st.nparam_x = idl_struct.nparam_x;
}


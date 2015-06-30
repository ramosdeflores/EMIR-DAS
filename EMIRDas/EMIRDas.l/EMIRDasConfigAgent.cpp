// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

// Includes
#include "EMIRDasConfigAgent.h"

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------
EMIRDasConfigAgent::EMIRDasConfigAgent(EMIRDas* device, const int mode, TimerEngine* timer)
 : ConfigAgent(device->name(),device->className(),mode,timer),trace_(device->name())
{
  device_ = device;

  // Assign default values to properties so they never contain garbage
  poweronApp_          = "0";
  poweroffApp_         = "0";
  adqApp_              = "0";
  imgDir_              = "NONE";
  simFile_              = "NONE";
  dataFactoryName_     = "0";
  isDataFactoryEnabled_ = 0;
  firmware_     = 999999;
  detectorType_ = "UNKNOWN";
  saveFile_     = 0;
  sendFrame_    = 1;
  simDetector_  = 1;
  simMode_      = 0;
  reorder_      = 1;
  invert_       = 0;
  poolSize_     = 30;
  vreset_       = -1.0;
  voffset_      = -1.0;
  biasgate_     = -1.0;
  ron_          = -1.0;
  gain_         = -1.0;
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------
EMIRDasConfigAgent::~EMIRDasConfigAgent()
{
}

//----------------------------------------------------------------------
// applyProperties: applies the stored configuration to the Device
//----------------------------------------------------------------------
void EMIRDasConfigAgent::applyProperties()
{
  trace_.out("EMIRDasConfigAgent::applyProperties()");
  try
  {
    // Apply properties one by one
    device_->poweronApp(this->poweronApp_);
    device_->poweroffApp(this->poweroffApp_);
    device_->adqApp(this->adqApp_);
    device_->imgDir(this->imgDir_);
    device_->simFile(this->simFile_);
    device_->dataFactoryName(this->dataFactoryName_);
    device_->isDataFactoryEnabled(this->isDataFactoryEnabled_);
    device_->firmware(this->firmware_);
    device_->detectorType(this->detectorType_);
    device_->saveFile(this->saveFile_);
    device_->sendFrame(this->sendFrame_);
    device_->simDetector(this->simDetector_);
    device_->simMode(this->simMode_);
    device_->reorder(this->reorder_);
    device_->invert(this->invert_);
    device_->poolSize(this->poolSize_);
    device_->voffset(this->voffset_);
    device_->vreset(this->vreset_);
    device_->biasgate(this->biasgate_);
    device_->ron(this->ron_);
    device_->gain(this->gain_);
  }
  catch (GCSException& ex)
  {
    ex.addToTrace("EMIRDasConfigAgent::applyProperties");
    throw;
  }
}

//----------------------------------------------------------------------
// readProperties: obtains the property values from the Device
//----------------------------------------------------------------------
int EMIRDasConfigAgent::readProperties()
{
  trace_.out("EMIRDasConfigAgent::readProperties()");
  try
  {
    // Read properties one by one
    this->poweronApp_          = device_->poweronApp();
    this->poweroffApp_         = device_->poweroffApp();
    this->adqApp_              = device_->adqApp();
    this->imgDir_              = device_->imgDir();
    this->simFile_              = device_->simFile();
    this->dataFactoryName_     = device_->dataFactoryName();
    this->isDataFactoryEnabled_ = device_->isDataFactoryEnabled();
    this->firmware_            = device_->firmware();
    this->detectorType_        = device_->detectorType();
    this->saveFile_            = device_->saveFile();
    this->sendFrame_           = device_->sendFrame();
    this->simDetector_         = device_->simDetector();
    this->simMode_             = device_->simMode();
    this->reorder_             = device_->reorder();
    this->invert_              = device_->invert();
    this->poolSize_            = device_->poolSize();
    this->voffset_             = device_->voffset();
    this->vreset_              = device_->vreset();
    this->biasgate_            = device_->biasgate();
    this->ron_                 = device_->ron();
    this->gain_                = device_->gain();
  }
  catch (GCSException& ex)
  {
    ex.addToTrace("EMIRDasConfigAgent::readProperties");
    throw;
  }
  return(0);
}

//----------------------------------------------------------------------
// toConfiguration: Converts the Device properties to a Configuration
//----------------------------------------------------------------------
void EMIRDasConfigAgent::toConfiguration(CONFIG::Configuration& c)
{
  trace_.out("EMIRDasConfigAgent::toConfiguration()");
  try
  {
    // Let ConfigAgent base class do the common stuff
    this->ConfigAgent::toConfiguration(c);

    addProperty(c, "poweronApp", poweronApp_);
    addProperty(c, "poweroffApp", poweroffApp_);
    addProperty(c, "adqApp", adqApp_);
    addProperty(c, "imgDir", imgDir_);
    addProperty(c, "simFile", simFile_);
    addProperty(c, "dataFactoryName", dataFactoryName_);
    addProperty(c, "isDataFactoryEnabled", isDataFactoryEnabled_);
    addProperty(c, "firmware", firmware_);
    addProperty(c, "detectorType", detectorType_);
    addProperty(c, "saveFile", saveFile_);
    addProperty(c, "sendFrame", sendFrame_);
    addProperty(c, "simDetector", simDetector_);
    addProperty(c, "simMode", simMode_);
    addProperty(c, "reorder", reorder_);
    addProperty(c, "invert", invert_);
    addProperty(c, "poolSize", poolSize_);
    addProperty(c, "voffset", voffset_);
    addProperty(c, "vreset", vreset_);
    addProperty(c, "biasgate", biasgate_);
    addProperty(c, "ron", ron_);
    addProperty(c, "gain", gain_);
  }
  catch (GCSException& ex)
  {
    ex.addToTrace("EMIRDasConfigAgent::toConfiguration");
    throw;
  }
}

//----------------------------------------------------------------------
// fromConfiguration: Converts the configuration to the Device format
//----------------------------------------------------------------------
void EMIRDasConfigAgent::fromConfiguration(const CONFIG::Configuration& c)
{
  trace_.out("EMIRDasConfigAgent::fromConfiguration()");
  try
  {
    // Let ConfigAgent base class do the common stuff
    this->ConfigAgent::fromConfiguration(c);

    extractProperty(c, "poweronApp", poweronApp_);
    extractProperty(c, "poweroffApp", poweroffApp_);
    extractProperty(c, "adqApp", adqApp_);
    extractProperty(c, "imgDir", imgDir_);
    extractProperty(c, "simFile", simFile_);
    extractProperty(c, "dataFactoryName", dataFactoryName_);
    extractProperty(c, "isDataFactoryEnabled", isDataFactoryEnabled_);
    extractProperty(c, "firmware", firmware_);
    extractProperty(c, "detectorType", detectorType_);
    extractProperty(c, "saveFile", saveFile_);
    extractProperty(c, "sendFrame", sendFrame_);
    extractProperty(c, "simDetector", simDetector_);
    extractProperty(c, "simMode", simMode_);
    extractProperty(c, "reorder", reorder_);
    extractProperty(c, "invert", invert_);
    extractProperty(c, "poolSize", poolSize_);
    extractProperty(c, "voffset", voffset_);
    extractProperty(c, "vreset", vreset_);
    extractProperty(c, "biasgate", biasgate_);
    extractProperty(c, "ron", ron_);
    extractProperty(c, "gain", gain_);
  }
  catch (GCSException& ex)
  {
    ex.addToTrace("EMIRDasConfigAgent::fromConfiguration");
    throw;
  }
}


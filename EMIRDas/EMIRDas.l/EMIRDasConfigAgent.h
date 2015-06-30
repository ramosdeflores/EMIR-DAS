// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

#ifndef _EMIRDasConfigAgent_h_
#define _EMIRDasConfigAgent_h_

// Includes
#include "EMIRDas.h"
#include "Trace.h"
#include "ConfigAgent.h"

class EMIRDas;

// Class used to configure EMIRDasConfigAgent objects
class EMIRDasConfigAgent : public ConfigAgent
{
  //--------------------------------------------------
  // public section 
  //--------------------------------------------------
  public:
  
  // Constructor
  EMIRDasConfigAgent(EMIRDas* device, const int mode, TimerEngine* timer);

  // Destructor
  virtual ~EMIRDasConfigAgent();

  // This method applies the stored configuration to the Device
  void applyProperties();
  
  // This method obtains the properties from the Device
  int readProperties();
  
  // Converts the Device properties to a Configuration
  void toConfiguration(CONFIG::Configuration& c);
  
  // Converts a Configuration to the Device format
  void fromConfiguration(const CONFIG::Configuration& c);
  
  
  //-----------------------------------------------------------------
  // protected section 
  //-----------------------------------------------------------------
  protected:
    Trace trace_;
  //--------------------------------------------------
  // private section 
  //--------------------------------------------------
  private:
  
  EMIRDas* device_;
  
  //
  // Configurable attributes (properties)
  //
  String               poweronApp_     ; 
  String               poweroffApp_    ; 
  String               adqApp_         ; 
  String               imgDir_         ; 
  String               simFile_         ; 
  String               dataFactoryName_; 
  String               detectorType_   ; 
  short                isDataFactoryEnabled_ ; 
  long                 firmware_ ; 
  short                saveFile_ ; 
  short                sendFrame_ ; 
  short                simDetector_ ; 
  short                simMode_ ; 
  short                reorder_ ; 
  short                invert_ ; 
  short                poolSize_ ; 
  double               voffset_ ; 
  double               vreset_ ; 
  double               biasgate_ ; 
  double               ron_ ; 
  double               gain_ ; 
};

#endif // _EMIRDasConfigAgent_h_ 


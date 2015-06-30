// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

#ifndef _EMIRDasAcqConfig_h_
#define _EMIRDasAcqConfig_h_

class EMIRDasAcqConfig
{
  public:

  struct AcquisitionConfiguration
  {
    string title;
    int readoutmodeval;
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

};
#endif  _EMIRDasAcqConfig_h_

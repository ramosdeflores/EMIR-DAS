// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

//$Log$


#ifndef _EMIRType_
#define _EMIRType_

struct AcquisitionConfiguration
{
  string title;
  EMIRDAS::ReadoutMode readoutmodeval;
  long  exposuretime;
  long  x_size;
  long  y_size;
  long  y_size_b;
  bool windowMode;
  bool applyConfiguration;
  long nsec;                      // Number of sequences in an observation
  long optrst;                    // Type of detector resets to be performed
  long nrst;                      // Number of detector resets
  long nrdbl;                     // Number of detector resets before loop
  long nrdbl_nd;                  // Detectors resets before the loop with no data
  long numExp;                    // Number of expositions per sequence
  long nrdil;                     // Number of detector resets inside the loop
  long nrdil_nd;                  // Detector resets inside the loop with no data
  long nparam_x;
};

char *ReadModes[] = { "SINGLE", "CDS", "FOWLER", "RAMP", "HDR_SIMPLE", "BIAS", "MRAMP", "NONE" }

#endif // _EMIRType_ 


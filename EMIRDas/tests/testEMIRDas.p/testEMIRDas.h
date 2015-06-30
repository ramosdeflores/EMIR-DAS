// Copyright (c) 2010 Gran Telescopio Canarias, S.A. (GRANTECAN)
// All Rights Reserved

// GCS includes
#include "EMIRDas.h"
#include "CommandLine.h"

// VxWorks Includes
#include <stdio.h>
#include <stdlib.h>

// Prototypes
void helpShow(void);
#ifdef VXWORKS
#include <usrLib.h>
#include <tickLib.h>

/// This function can be called directly from the VxWorks shell
extern "C" int testEMIRDas(char* devicename,int updateProfile=0,int mode=0,long priority=100,unsigned threads=4);
#else
int main(int argc, char** argv);
#endif

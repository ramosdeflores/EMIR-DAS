// EMIRDasDetector.h

#ifndef _EMIRDasDetector_h_
#define _EMIRDasDetector_h_

#define NLINES		1024
#define NPIX		4096
#define ROWS		2048
#define COLS		2048
#define CHANELS		8
#define CCDS		4
#define SDSU_SIZE	NLINES * NPIX

#ifndef VXWORKS
#define INT16	short
#define UINT16	unsigned short
#endif

/* SImulation modes */
#define SFILE           1       /* Read from file       */
#define SPATT           2       /* Generate a pattern   */

#endif

/**
 * \file sdsu_interface.h
 *
 * \brief Header file for library of functions providing the RTAI SDSU driver
 * module interface
 *
 * \author Stewart McLay, UKATC (sam@roe.ac.uk)
 *
 * \version 1.0
 *
 * \date Thu Mar 21 15:06:37 GMT 2002
 *
 * This library contains functions that interface directly with the RTAI SDSU
 * driver module through RTAI FIFOs. There are three threads in the SDSU driver
 * each of which has a FIFO pair for writing data to and receiving data from.
 * The three threads are...
 *
 * -# PCI thread: sends commands from a USER process (or another driver thread)
 * to the PCI card and returns replies
 * -# TIME thread: sends commands from a USER process (or another driver thread)
 * to the SDSU controller and returns replies
 * -# DATA thread: sends empty buffers from a USER process to the PCI card for
 * writing data and returns full buffers
 *
 * The list of the FIFOs available to USER space programs for communicating to
 * the RTAI SDSU driver are...
 *
 * -# PCI command FIFO
 * -# PCI reply FIFO
 * -# TIME command FIFO
 * -# TIME reply FIFO
 * -# DATA empty buffer FIFO
 * -# DATA full buffer FIFO	
 *
 * Copyright (C) 2003 UK Astronomy Technology Centre (UK ATC), an establishment
 * of the Particle Physics and Astronomy Research Council (PPARC).
 * Web: www.roe.ac.uk
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 */

#ifndef __SDSU_INTERFACE_H__
#define __SDSU_INTERFACE_H__

#include "sdsu_platform.h"

#define SDSU_LOW_PRIORITY 1
#define SDSU_LAST_BUFFER  1

#define SDSU_OK            0    /**< Successful */
#define SDSU_ERR_BUFSIZE   -128 /**< Buffer size exceeds big physical memory */
#define SDSU_ERR_CLOSEFIFO -129 /**< Closing FIFO failed */
#define SDSU_ERR_CLOSESDSU -130 /**< Close SDSU device file failed */
#define SDSU_ERR_CMD       -131 /**< Command failed */
#define SDSU_ERR_DEVNAME   -132 /**< Device name too long */
#define SDSU_ERR_GETBUF    -133 /**< Get next buffer failed */
#define SDSU_ERR_INSDRV    -134 /**< Inserting SDSU driver module failed */
#define SDSU_ERR_INVCON    -135 /**< Context structure invalid */
#define SDSU_ERR_KEY       -136 /**< Unique key value generation failed */
#define SDSU_ERR_MALLOC    -137 /**< Dynamic memory allocation failed */
#define SDSU_ERR_MAP       -138 /**< Mapping SDSU device file failed */
#define SDSU_ERR_MBUFF     -139 /**< Shared memory allocation failed */
#define SDSU_ERR_MUTEX     -140 /**< Mutual exclusion failed */
#define SDSU_ERR_NOCON     -141 /**< Context structure does not exist */
#define SDSU_ERR_NOWRITER  -142 /**< Writer thread not running */
#define SDSU_ERR_NUMBUF    -143 /**< Number of buffers to low */
#define SDSU_ERR_OPENFIFO  -144 /**< Opening FIFO failed */
#define SDSU_ERR_OPENSDSU  -145 /**< Open SDSU device file failed */
#define SDSU_ERR_PROCRUN   -147 /**< Process still running */
#define SDSU_ERR_READFIFO  -148 /**< Read FIFO failed */
#define SDSU_ERR_RMDRV     -149 /**< Removing SDSU driver module failed */
#define SDSU_ERR_SEMID     -151 /**< Semaphore ID unrecognised */
#define SDSU_ERR_TIMEOUT   -154 /**< Timed out waiting for reply on FIFO */
#define SDSU_ERR_UNMAP     -155 /**< Unmapping SDSU device file failed */
#define SDSU_ERR_USECON    -156 /**< Context already exists */
#define SDSU_ERR_WRITEFIFO -157 /**< Write FIFO failed */
#define SDSU_ERR_WRITER    -158 /**< WRITER thread failed */
#define SDSU_ERR_RESETFIFO -160 /**< Flushing FIFO failed */

#define SDSU_DEVNAME_LEN   16   /**< Maximum length for device name */
#define SDSU_BUFNAME_LEN   20   /**< Maximum length for buffer name */
#define SDSU_FIFO_LEN      16   /**< Maximum length for FIFO device file name */

#define SDSU_PCI_X_MEM     0x00005f58 /**< PCI card X memory type word */
#define SDSU_PCI_Y_MEM     0x00005f59 /**< PCI card Y memory type word */
#define SDSU_PCI_P_MEM     0x00005f50 /**< PCI card P memory type word */

#define SDSU_TIME_X_MEM    0x00200000 /**< PCI card X memory type word */
#define SDSU_TIME_Y_MEM    0x00400000 /**< SDSU controller Y memory type word */
#define SDSU_TIME_P_MEM    0x00100000 /**< SDSU controller P memory type word */

#define SDSU_MAX_FIFO      11  /**< Maximum number of FIFOs */
#define SDSU_MAX_MSG       10  /**< Maximum number of FIFO messages */
#define SDSU_MAX_BUFFER    100 /**< Maximum number of frame buffers */
#define SDSU_MIN_BUFFER    2   /**< Minimum number of frame buffers */
#define SDSU_MUTEX_SEM_ID  11  /**< Unique ID for generating unique key */
#define SDSU_SYNC_SEM_ID   12  /**< Unique ID for generating unique key */
#define SDSU_CMD_SEM_ID    13  /**< Unique ID for generating unique key */

#define SDSU_DEV_FILE "/dev/sdsu" /**< Device file for SDSU driver module */
#define SDSU_DIR "ULTRACAM_TOP"   /**< Base directory (system variable) */

#define SDSU_TRUE  1 /**< TRUE status flag value */
#define SDSU_FALSE 0 /**< FALSE status flag value */

/** Number of 'big physical memory' pages used for storing buffers */
#define SDSU_BIGPHYSAREA_PAGES 32769 /* 128MB + 2 pages */

/**
 * Burst mode writing on PCI bus uses 64MB block size to overcome 16 bit
 * address limitation
 */
#define SDSU_BURST_BLOCK 0x10000

/**
 * List of RTAI FIFO device file numbers called "/dev/rtf#" (where # represents
 * the file number). FIFOs may be calssified as either internal or external.
 * Those that communicate between USER space and the RTAI SDSU driver are
 * considered to be external where as all others are internally used by the RTAI
 * SDSU driver.
 */
#define USER_DATA_EMPTY 0  /**< USER-DATA empty buffer FIFO (external) */
#define USER_DATA_FULL  1  /**< USER-DATA full buffer FIFO (external) */
#define USER_USER_EMPTY 2  /**< USER-USER empty buffer FIFO (external) */
#define USER_PCI_CMD    3  /**< USER-PCI command FIFO (external) */
#define USER_PCI_REPLY  4  /**< USER-PCI reply FIFO (external) */
#define USER_TIME_CMD   5  /**< USER-TIME command FIFO (external) */
#define USER_TIME_REPLY 6  /**< USER-TIME reply FIFO (external) */
#define TIME_PCI_CMD    7  /**< TIME-PCI command FIFO (internal) */
#define TIME_PCI_REPLY  8  /**< TIME-PCI reply FIFO (internal) */
#define DATA_PCI_CMD    9  /**< DATA-PCI command FIFO (internal) */
#define DATA_PCI_REPLY  10 /**< DATA-PCI reply FIFO (internal) */

#define	RDM	0x0052444d
#define	WRM	0x0057524d
#define	GOA	0x00474f41
#define	STP	0x00535450
#define	RST	0x00525354
#define	CON	0x00434f4e
#define	HST	0x00485354
#define	RCO	0x0052434f
#define	BIG	0x00424947
#define	WEE	0x00574545
#define	ACK	0x0041434b

#define WM	0x4d570000
#define	RM	0x4d520000
#define	GO	0x4f470000
#define	ST	0x54530000
#define	RS	0x53520000
#define ER      0x00004355
#define	TCMD_OK	0x00004b4f
#define DUMMY	0x0000

#define	ON	1
#define OFF	0

/** PCI command data structure */
typedef struct {
    UINT32 cmd; /**< Three character command encoded as 24-bit word */
    UINT32 arg1; /**< First argument, usually required */
    UINT32 arg2; /**< Second argument */
    UINT32 arg3; /**< Third argument */
} SDSU_PCI_CMD;

/** PCI reply data structure */
typedef struct {
    UINT32 cmd;    /**< Three character command encoded as 24-bit word */
    UINT32 status; /**< Three character status encoded as 24-bit word */
    UINT32 data;    /**< Data argument */
} SDSU_PCI_REPLY;

/** Timing board command structure */
typedef struct {
    volatile UINT32 cmd;    /**< 2 char command encoded as 24-bit word */
    volatile UINT32 arg1;   /**< First argument */
    volatile UINT32 arg2;   /**< Second argument */
} SDSU_TIME_CMD;

/** Timing board reply structure */
typedef struct {
    volatile UINT32 cmd;    /**< 2 char command encoded as 24-bit word */
    volatile UINT32 data;    /**< Data argument */
} SDSU_TIME_REPLY;

/** Data frame buffer structure */
typedef struct {
    volatile UINT32 bufphysadr; /**< Buffer physical memory address */
    volatile INT32 bufsize;             /**< Buffer size (bytes) */
    volatile UINT32 bufid;             /**< Buffer ID */
} SDSU_BUFFER;

/** FIFO data structure  */
typedef struct {
    char name[SDSU_FIFO_LEN]; /**< Device file name */
    MSG_Q_ID id;              /**< FIFO ID */
} SDSU_FIFODEV;

/** SDSU context structure */
typedef struct {
    unsigned int frameGrabber = OFF;
    unsigned int fgStatus = OFF;
    unsigned int controller   = OFF;
    unsigned int cStatus = OFF;
    UINT32	mempos[3] = { 0,0,0 };
    SDSU_FIFODEV fifo[SDSU_MAX_FIFO]; /**< FIFOs */
} SDSU_CONTEXT;

/** Creates a new SDSU context and loads the RTAI SDSU driver module */
extern int sdsu_create(const char *devname, SDSU_CONTEXT **con);

/** Attach to shared memory context structure */
extern int sdsu_get_context(const char *devname, SDSU_CONTEXT **con);

/** Free shared memory context structure */
extern int sdsu_free_context(SDSU_CONTEXT *con);

/** Unloads the RTAI SDSU driver module and removes the SDSU context */
extern int sdsu_free(SDSU_CONTEXT *con);

/** Write the contents of the SDSU context structure to standard output */
extern void sdsu_display(SDSU_CONTEXT *con);

/** Write a word to the PCI card memory */
extern int sdsu_write_pci(SDSU_CONTEXT *con, const char memtype,
	       	          const UINT32 memadr, const INT32 word);

/** Read a word from the PCI card memory */
extern int sdsu_read_pci(SDSU_CONTEXT *con, const char memtype,
	       	         const UINT32 memadr, INT32 *word);

/** Start an application loaded in the PCI card */
extern int sdsu_start_pci(SDSU_CONTEXT *con, const INT32 id);

/** Stop an application running on the PCI card */
extern int sdsu_stop_pci(SDSU_CONTEXT *con);

/** Reset the PCI card */
extern int sdsu_reset_pci(SDSU_CONTEXT *con);

/** Hardware reset for the SDSU controller */
extern int sdsu_rco_pci(SDSU_CONTEXT *con);

/** Big endian byte swapping for the PCI card */
extern int sdsu_big_endian_pci(SDSU_CONTEXT *con);

/** Little endian byte swapping for the PCI card */
extern int sdsu_little_endian_pci(SDSU_CONTEXT *con);

/** Write a word to SDSU controller memory */
extern int sdsu_write_sdsu(SDSU_CONTEXT *con, const char memtype,
                           const UINT32 memadr, const INT32 word);

/** Read a word from SDSU controller memory */
extern int sdsu_read_sdsu(SDSU_CONTEXT *con, const char memtype,
                          const UINT32 memadr, INT32 *word);

/** Start an application loaded in the SDSU controller */
extern int sdsu_start_sdsu(SDSU_CONTEXT *con, const INT32 id);

/** Stop an application running on the SDSU controller */
extern int sdsu_stop_sdsu(SDSU_CONTEXT *con);

/** Reset the SDSU controller */
extern int sdsu_reset_sdsu(SDSU_CONTEXT *con);

/** Send an empty data buffer to the SDSU driver */
extern int sdsu_write_data(SDSU_CONTEXT *con,
		           volatile const UINT32 bufphysadr,
			   const INT32 bufsize, const UINT32 bufid);

/** Get a full data buffer from the SDSU driver */
extern int sdsu_read_data(SDSU_CONTEXT *con, volatile UINT32 *bufphysadr,
                          INT32 *bufsize, UINT32 *bufid);

#endif /* __SDSU_INTERFACE_H__ */

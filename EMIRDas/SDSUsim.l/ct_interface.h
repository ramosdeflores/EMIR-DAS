#ifndef __SDSU_INTERFACE_H__
#define SDSU_HDRSIZE 24

#define UINT32		unsigned int
#define INT32		int
#define UINT16		unsigned short
#define INT16		short

#define __SDSU_INTERFACE_H__

#define	CONTROLLER	0
#define	FRAME_GRABBER	1

#define	SEEK_SET	0
#define	SEEK_CUR	1

#define	RESET		0
#define	RST_PCI		1
#define	RST_RCO		2
#define	LITTLE		3
#define	BIGE		4
#define	SET_MTYPE	5
#define	START_SDSU	6
#define	START_PCI	7
#define	STOP_SDSU	8
#define	STOP_PCI	9
#define GET_HDR		10
#define START_AQU	11
#define FRMCNT		12
#define FRMHDR		13
#define CTHDR		14

#define PCI_CMD_OK    OK
#define PCI_REPLY_OK  OK
#define TIME_CMD_OK   OK
#define TIME_REPLY_OK OK
#define CON_CMD_OK    OK
#define HST_CMD_OK    OK
#define BUFFER_OK     OK


/* Frame grabber mask */
#define NODELAY		1
#define DELAY		0

#define ERR_BASE	128
/* Error codes */
#define	NO_ERROR	0 /* Successful */

#define ERR_BUFSIZE   -128 /**< Buffer size exceeds big physical memory */
#define ERR_CLOSEFIFO -129 /**< Closing FIFO failed */
#define ERR_CLOSESDSU -130 /**< Close SDSU device file failed */
#define ERR_CMD       -131 /**< Command failed */
#define ERR_DEVNAME   -132 /**< Device name too long */
#define ERR_GETBUF    -133 /**< Get next buffer failed */
#define ERR_INSDRV    -134 /**< Inserting SDSU driver module failed */
#define ERR_INVCON    -135 /**< Context structure invalid */
#define ERR_KEY       -136 /**< Unique key value generation failed */
#define ERR_MALLOC    -137 /**< Dynamic memory allocation failed */
#define ERR_MAP       -138 /**< Mapping SDSU device file failed */
#define ERR_MBUFF     -139 /**< Shared memory allocation failed */
#define ERR_MUTEX     -140 /**< Mutual exclusion failed */
#define ERR_NOCON     -141 /**< Context structure does not exist */
#define ERR_NOWRITER  -142 /**< Writer thread not running */
#define ERR_NUMBUF    -143 /**< Number of buffers to low */
#define ERR_OPENFIFO  -144 /**< Opening FIFO failed */
#define ERR_OPENFG    -145 /**< Open frame grabber device file failed */
#define ERR_PROCRUN   -147 /**< Process still running */
#define ERR_READFIFO  -148 /**< Read FIFO failed */
#define ERR_RMDRV     -149 /**< Removing SDSU driver module failed */
#define ERR_SEMID     -151 /**< Semaphore ID unrecognised */
#define ERR_TIMEOUT   -154 /**< Timed out waiting for reply on FIFO */
#define ERR_UNMAP     -155 /**< Unmapping SDSU device file failed */
#define ERR_USECON    -156 /**< Context already exists */
#define ERR_WRITEFIFO -157 /**< Write FIFO failed */
#define ERR_WRITER    -158 /**< WRITER thread failed */
#define ERR_RESETFIFO -160 /**< Flushing FIFO failed */
#define	ERR_BADDEVICE -161 /**< Bad device name */
#define	ERR_MEMTYPE   -162 /**< Unknown memory type */
#define	ERR_WHENCE    -163 /**< Wrong position parameter */
#define	ERR_CTRUN     -164 /**< Can not start SDSU application */
#define	ERR_MEMCPY    -165 /**< Copy to user buffer failed */
#define	ERR_BADRQT    -166 /**< Unknown ioctl request */
#define ERR_OPENCT    -167 /**< Open controller device file failed */

static char *errMessages[] = { "ERR_BUFSIZE - Buffer size exceeds big physical memory",
                               "ERR_CLOSEFIFO - Closing FIFO failed",
                               "ERR_CLOSESDSU - Close SDSU device file failed",
                               "ERR_CMD - Command failed",
                               "ERR_DEVNAME - Device name too long",
                               "ERR_GETBUF - Get next buffer failed",
                               "ERR_INSDRV - Inserting SDSU driver module failed",
                               "ERR_INVCON - Context structure invalid",
                               "ERR_KEY - Unique key value generation failed",
                               "ERR_MALLOC - Dynamic memory allocation failed",
                               "ERR_MAP - Mapping SDSU device file failed",
                               "ERR_MBUFF - Shared memory allocation failed",
                               "ERR_MUTEX - Mutual exclusion failed",
                               "ERR_NOCON - Context structure does not exist",
                               "ERR_NOWRITER - Writer thread not running",
                               "ERR_NUMBUF - Number of buffers to low",
                               "ERR_OPENFIFO - Opening FIFO failed",
                               "ERR_OPENFG - Open frame grabber device file failed",
                               "ERR_READFIFO - Read FIFO failed",
                               "ERR_RMDRV - Removing SDSU driver module failed",
                               "ERR_SEMID - Semaphore ID unrecognised",
                               "ERR_TIMEOUT - Timed out waiting for reply on FIFO",
                               "ERR_UNMAP - Unmapping SDSU device file failed",
                               "ERR_USECON - Context already exists",
                               "ERR_WRITEFIFO - Write FIFO failed",
                               "ERR_WRITER - WRITER thread failed",
                               "ERR_RESETFIFO - Flushing FIFO failed",
                               "ERR_BADDEVICE - Bad device name",
                               "ERR_MEMTYPE - Unknown memory type",
                               "ERR_WHENCE - Wrong position parameter",
                               "ERR_CTRUN - Can not start SDSU application",
                               "ERR_MEMCPY - Copy to user buffer failed",
                               "ERR_BADRQT - Unknown ioctl request",
                               "ERR_OPENCT - Open controller device file failed" };


#define SDSU_HDRSIZE 24

/* Device status */
#define	SDSU_FREE		0
#define	WRITE		1

/* Frame grabber modes */
#define	WAIT		0

/* Memory types */
#define	MEMTYPES	3
#define	MEMP		0
#define	MEMX		1
#define	MEMY		2




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

#define SDSU_TRUE  1 /**< TRUE status flag value */
#define SDSU_FALSE 0 /**< FALSE status flag value */

#define	SDSU_WSWAP(x)	((x & 0xff000000) >> 24 | (x & 0x00ff0000) >> 8 | (x & 0x0000ff00) << 8  | (x & 0x000000ff) << 24)

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
    UINT32 cmd;  /**< Three character command encoded as 24-bit word */
    UINT32 arg1; /**< First argument, usually required */
    UINT32 arg2; /**< Second argument */
    UINT32 arg3; /**< Third argument */
} SDSU_PCI_CMD;

/** PCI reply data structure */
typedef struct {
    UINT32 cmd;     /**< Three character command encoded as 24-bit word */
    UINT32 status;  /**< Three character status encoded as 24-bit word */
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
    volatile UINT32 bufphysadr;        /**< Buffer physical memory address */
    volatile INT32 bufsize;            /**< Buffer size (bytes) */
    volatile UINT32 bufid;             /**< Buffer ID */
} SDSU_BUFFER;

/** FIFO data structure  */
typedef struct {
    char name[SDSU_FIFO_LEN]; /**< Device file name */
    long id;              /**< FIFO ID */
} SDSU_FIFODEV;

/** SDSU context structure */
typedef struct {
    unsigned int frameGrabber;
    unsigned int fgStatus;
    unsigned int fgMask;
    char         fHardwareInfo[256];
    char         fSoftwareInfo[256];
    UINT32       framesRead;
    UINT16       lastAqHeader[SDSU_HDRSIZE];

    unsigned int controller;
    unsigned int cStatus;
    unsigned int cMask;
    char         cHardwareInfo[256];
    char         cSoftwareInfo[256];
    UINT16       lastCtHeader[SDSU_HDRSIZE];


    unsigned int memType;
    UINT32       memPos[3];

    SDSU_FIFODEV fifo[SDSU_MAX_FIFO]; /**< FIFOs */
} SDSU_CONTEXT;


/*SDSU_CONTEXT *context;*/

/** Operation of controller device */
int copen(char *device, unsigned int mask);
int cclose(int id);
int cread(int id, INT32 *buffer, int size);
int cwrite(int id, INT32 *buffer, int size);
int cioctl(int id, int request, void *args);
UINT32 clseek(int id, const UINT32 offset, int whence);
int fgread(int id, void *buffer, int size);
int fgioctl(int id, int request, void *args);
int fgopen(char *device, unsigned int mask);
int fgclose(int fd);

#endif /* __SDSU_INTERFACE_H__ */

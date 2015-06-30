/*#define DEBUG_MESSAGE 1  */
#define ERROR_MESSAGE 1

#include <stdlib.h>          /* getenv(), system() */

/* Standard C libraries */
#include <string.h>     /* strncmp() */

/* VxWorks libraries */
#include <semLib.h>     /* semGive() */
#include <msgQLib.h>    /* msgQSend(), msgQReceive() */
#include <sysLib.h>     /* sysClkRateGet() */
#include <cacheLib.h>

#include "ct_interface.h"

int intBuffSize = SDSU_HDRSIZE;
void *intVirBuff;
void *intPhyBuff;
SDSU_BUFFER intBuf;
unsigned int timeMemType[3] = { SDSU_TIME_P_MEM,
                                SDSU_TIME_X_MEM,
                                SDSU_TIME_Y_MEM };


SDSU_CONTEXT *context;

unsigned short Simulate = 0;

extern SEM_ID pci_sem;  /**< USER/TIME task command to PCI task semaphore */
extern SEM_ID time_sem; /**< USER command to TIME task semaphore */


/* Position de memory address for writes to the controller 
 *
 * Parameters: id   - file descriptor (not used)
 *             whence - action      = SEEK_SET  or SEEK_CUR
 *             offset - memory offset
 *
 * Returns: NO_ERROR
 *          BAD_TYPE   - if the value of type is not MEMP, MEMX or MEMY
 *          BAD_WHENCE - if the valeu of whence is not SEEK_SET or SEEK_CUR
 */
UINT32 clseek(int id, const UINT32 offset, int whence)  {

     if ( Simulate )
          return NO_ERROR;

     if ( context->memType > MEMTYPES )
          return ERR_MEMTYPE;

     switch (whence) {
          case SEEK_SET: context->memPos[context->memType] = offset;
                         break;

          case SEEK_CUR: context->memPos[context->memType] += offset;
                         break;

          default: return ERR_WHENCE;
                   break;
     }

     return context->memPos[context->memType];
}

/* Write to the controller. Will write to memory address context->memPos[context->memType]
 * (where context->memType is set by ioctl) set by lseek or by the last write.
 * The normal procedure to set the memory address is ioctl follow by lseek.
 *
 * Parameters: id - file descriptor (not used)
 *             *buffer - data to be writen
 *             size - size of buffer
 */

int cwrite(int id, INT32 *buffer, int size) {

     INT32  arg1base;
     UINT32 memadr;
     UINT32 arg1 = 0;
     UINT32 arg2 = 0;
     SDSU_TIME_CMD time_cmd;
     SDSU_TIME_REPLY time_reply;
     int    index;

     if ( Simulate )
          return NO_ERROR;

     if ( context->memType > MEMTYPES )
          return ERR_MEMTYPE;

     context->cStatus = WRITE;

     /* Base address for the current memory type */
     arg1base = timeMemType[context->memType];

     /* Write command */
     time_cmd.cmd = WM;

     /* Initial memory position */
     memadr = context->memPos[context->memType];
     for (index = 0; index < size; ++index) {
          /* Set arguments for the write command */
          arg1 = arg1base | (0x00ffffff & memadr);
          ++memadr;
          arg2 = 0x00ffffff & *buffer;
          ++buffer;
          /* Write command structure */
          time_cmd.arg1 = SDSU_WSWAP(arg1);
          time_cmd.arg2 = SDSU_WSWAP(arg2);

          DBGMSG("write> cmd <%x> arg1=%#lx arg2=%#lx\n", time_cmd.cmd, time_cmd.arg1, time_cmd.arg2);

          /* Write command to FIFO */
          if ( WRITE_FIFO(context->fifo[USER_TIME_CMD], (void *)&time_cmd, sizeof(time_cmd)) != TIME_CMD_OK ) {
               ERRMSG("write> ERROR - write to FIFO USER_TIME_CMD\n");
               context->cStatus = SDSU_FREE;
               return ERR_WRITEFIFO;
          }

          SIGNAL_FIFO(time_sem);

          /* Wait on reply from FIFO */
          if (READ_FIFO(context->fifo[USER_TIME_REPLY], (void *) &time_reply, sizeof(time_reply)) != sizeof(time_reply)) {
               ERRMSG("write> ERROR - read from FIFO USER_TIME_REPLY\n");
               context->cStatus = SDSU_FREE;
               return ERR_READFIFO;
          } else {
               DBGMSG("write> cmd <%2.2s> data=%#lx\n", time_reply.cmd, time_reply.data);
               /* Check status is "OK" */
               if ((time_reply.cmd & TCMD_OK) != TCMD_OK) {
                    ERRMSG("write> command reply <%x>\n", time_reply.cmd);
                    context->cStatus = SDSU_FREE;
                    return ERR_CMD;
               }
          }
     }

     /* Point to the next memory address */
     context->memPos[context->memType] = memadr;
     context->cStatus = SDSU_FREE;

     return index;
}

/* Read from the controller
 * 
 */

int cread(int id, INT32 *buffer, int size) {

     INT32  arg1base;
     UINT32 memadr;
     UINT32 arg1 = 0;
     SDSU_TIME_CMD time_cmd;
     SDSU_TIME_REPLY time_reply;
     int index;

     if ( Simulate )
          return NO_ERROR;

     if ( context->memType > MEMTYPES )
          return ERR_MEMTYPE;

     context->cStatus = READ;

     arg1base = timeMemType[context->memType];
     time_cmd.cmd = RM;
     time_cmd.arg2 = 0;
     memadr = context->memPos[context->memType];
     for (index = 0; index < size; ++index) {
          arg1 = arg1base | (0x00ffffff & memadr);
          ++memadr;
           /* Write command structure */
          time_cmd.arg1 = SDSU_WSWAP(arg1);

          DBGMSG("read> cmd <%x> arg1=%#lx arg2=%#lx\n", time_cmd.cmd, time_cmd.arg1, time_cmd.arg2);

          /* Write command to FIFO */
          if (WRITE_FIFO(context->fifo[USER_TIME_CMD], (void *) &time_cmd, sizeof(time_cmd)) != TIME_CMD_OK) {
               ERRMSG("read> ERROR - write to FIFO USER_TIME_CMD\n");
               context->cStatus = SDSU_FREE;
               return ERR_WRITEFIFO;
          }

          SIGNAL_FIFO(time_sem);

          /* Wait on reply from FIFO */
          if (READ_FIFO(context->fifo[USER_TIME_REPLY], (void *) &time_reply, sizeof(time_reply)) != sizeof(time_reply)) {
               ERRMSG("read> ERROR - read from FIFO USER_TIME_REPLY\n");
               context->cStatus = SDSU_FREE;
               return ERR_READFIFO;
          } else {
               DBGMSG("read> cmd <%x> data=%#lx\n", time_reply.cmd, time_reply.data);
               /* Check status is "OK" */
               if ((time_reply.cmd & TCMD_OK) == TCMD_OK) {
                   *buffer = time_reply.data;
                   ++buffer;
               } else {
                   ERRMSG("read> command reply <%x>\n", time_reply.cmd);
                   context->cStatus = SDSU_FREE;
                   return ERR_CMD;
               }
          }
     }

     context->memPos[context->memType] = memadr;
     context->cStatus = SDSU_FREE;

     return index;
}

int WritePCICommand(SDSU_PCI_CMD *pci_cmd) {

    SDSU_PCI_REPLY pci_reply;

    /* Write command to FIFO */
    if (WRITE_FIFO(context->fifo[USER_PCI_CMD], (void *) pci_cmd, sizeof(SDSU_PCI_CMD)) != PCI_CMD_OK) {
        ERRMSG("WritePCICommand> ERROR - write FIFO USER_PCI_CMD\n");
        return ERR_WRITEFIFO;
    }

    SIGNAL_FIFO(pci_sem);

    /* Wait for reply on FIFO */
    if (READ_FIFO(context->fifo[USER_PCI_REPLY], (void *) &pci_reply, sizeof(pci_reply)) != sizeof(pci_reply)) {
        ERRMSG("WritePCICommand> ERROR - read FIFO USER_PCI_REPLY\n");
        return ERR_READFIFO;
    } else {
        DBGMSG("WritePCICommand> cmd <%x> status <%x> data=%#lx\n", pci_reply.cmd, pci_reply.status, pci_reply.data);
        if (pci_reply.status != ACK) {
            ERRMSG("WritePCICommand> ERROR - command reply <%x>\n", pci_reply.status);
            return ERR_CMD;
        }
    }

    return NO_ERROR;
}

int WriteSDSUCommand(void) {

     if ( intVirBuff == NULL )
          if ( (intVirBuff = (void *)memalign(SDSU_BURST_BLOCK, intBuffSize)) == NULL ) {
               ERRMSG("WriteSDSUCommand> ERROR allocating buffer memory.\n");
               return ERR_MBUFF;
          } else {
               intPhyBuff = CACHE_DMA_VIRT_TO_PHYS(intVirBuff);

               /* Write command structure */
               intBuf.bufphysadr = (volatile const UINT32)intPhyBuff;
               intBuf.bufsize = intBuffSize;
               intBuf.bufid = 4321;
          }

     DBGMSG("WriteSDSUCommand> bufphysadr=%#lx bufsize=%ld bufid=%#lx\n", intBuf.bufphysadr, intBuf.bufsize, intBuf.bufid);

     if (WRITE_FIFO(context->fifo[USER_DATA_EMPTY], (void *) &intBuf, sizeof(intBuf)) != BUFFER_OK) {
         ERRMSG("WriteSDSUCommand> ERROR - write to FIFO USER_DATA_EMPTY\n");
         return ERR_WRITEFIFO;
     }

     return NO_ERROR;
}

int ReadSDSUCommand(void) {

    int bytes;
    int retVal;

    if ((bytes = READ_FIFO(context->fifo[USER_DATA_FULL], (void *) &intBuf, sizeof(intBuf))) != sizeof(intBuf)) {
        if (bytes > 0) {
            ERRMSG("ReadSDSUCommand> ERROR - read from FIFO USER_DATA_FULL %d bytes\n", bytes);
        } else if (bytes < 0) {
            ERRMSG("ReadSDSUCommand> ERROR - read from FIFO USER_DATA_FULL errno %d\n", bytes);
        } else {
            /* When zero bytes are read this probably means the task or FIFO has been deleted */
            ERRMSG("ReadSDSUCommand> read %d bytes, read from FIFO USER_DATA_FULL aborted!\n", bytes);
        }
        retVal = ERR_READFIFO;
    } else {
        DBGMSG("ReadSDSUCommand> bufphysadr=%#lx bufsize=%ld bufid=%#lx\n", intBuf.bufphysadr, intBuf.bufsize, intBuf.bufid);
        retVal = NO_ERROR;
    }

    return retVal;
}


int WriteTimeCommand(SDSU_TIME_CMD *time_cmd) {
    SDSU_TIME_REPLY time_reply;

    /* Write command to FIFO */
    if (WRITE_FIFO(context->fifo[USER_TIME_CMD], (void *)time_cmd, sizeof(SDSU_TIME_CMD)) != TIME_CMD_OK) {
        ERRMSG("WriteTimeCommand> ERROR - write to FIFO USER_TIME_CMD\n");
        return ERR_WRITEFIFO;
    }

    SIGNAL_FIFO(time_sem);

    /* Wait on reply from FIFO */
    if (READ_FIFO(context->fifo[USER_TIME_REPLY], (void *)&time_reply, sizeof(time_reply)) != sizeof(time_reply)) {
        ERRMSG("WriteTimeCommand> ERROR - read from FIFO USER_TIME_REPLY\n");
        return ERR_READFIFO;
    } else {
        DBGMSG("WtiteTimeCommand> cmd <%x> data=%#lx\n", time_reply.cmd, time_reply.data);
        /* Check status is "OK" */
        if ((time_reply.cmd & TCMD_OK) != TCMD_OK) {
            ERRMSG("WtiteTimeCommand> command reply <%x>\n", time_reply.cmd);
            return ERR_CMD;
        }
    }
    return NO_ERROR;
}

int cioctl(int fd, int request, void *args) {

     SDSU_PCI_CMD	pci_cmd;
     SDSU_TIME_CMD	time_cmd;
     int	retVal = NO_ERROR;
     INT32	id;
     INT32	tmpArg;

     if ( Simulate )
          return NO_ERROR;

     context->controller = WRITE;

     pci_cmd.arg1 = 0;
     pci_cmd.arg2 = 0;
     pci_cmd.arg3 = 0;

     time_cmd.arg1 = 0;
     time_cmd.arg2 = 0;

     switch (request) {
          case RESET     : if ( (retVal = ioctl(0, RST_PCI, NULL)) == NO_ERROR ) {
                                retVal = ioctl(0, RST_RCO, NULL);
                           }
                           break;

          case RST_RCO   : pci_cmd.cmd = RCO;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx arg3=%#lx\n", pci_cmd.cmd, pci_cmd.arg1, pci_cmd.arg2, pci_cmd.arg3);
                           if ( (retVal = WritePCICommand(&pci_cmd)) != NO_ERROR ) {
                                ERRMSG("ioctl> ERROR on RST_RCO.\n");
                           } else {
                                taskDelay(sysClkRateGet() * 0.1);
                           }
                           break;

          case RST_PCI   : pci_cmd.cmd = RST;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx arg3=%#lx\n", pci_cmd.cmd, pci_cmd.arg1, pci_cmd.arg2, pci_cmd.arg3);
                           if ( (retVal = WritePCICommand(&pci_cmd)) != NO_ERROR ) {
                                ERRMSG("ioctl> ERROR on RST_PCI.\n");
                           } else {
                                taskDelay(sysClkRateGet() * 0.1);
                           }
                           break;

          case BIGE      : pci_cmd.cmd = BIG;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx arg3=%#lx\n", pci_cmd.cmd, pci_cmd.arg1, pci_cmd.arg2, pci_cmd.arg3);
                           retVal = WritePCICommand(&pci_cmd);
                           break;

          case LITTLE    : pci_cmd.cmd = WEE;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx arg3=%#lx\n", pci_cmd.cmd, pci_cmd.arg1, pci_cmd.arg2, pci_cmd.arg3);
                           retVal = WritePCICommand(&pci_cmd);
                           break;

          case SET_MTYPE : id = *(unsigned int *)args;
                           switch(id) {
                                case MEMX:
                                case MEMY:
                                case MEMP: context->memType = id;
                                           break;

                                default  : retVal = ERR_MEMTYPE;
                                           break;
                           }
                           break;

          case START_AQU : /* Write command structure */
                           time_cmd.cmd = GO;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx\n", time_cmd.cmd, time_cmd.arg1, time_cmd.arg2);
                           if ( (retVal = WriteTimeCommand(&time_cmd)) != NO_ERROR ) {
                                ERRMSG("ioctl> ERROR on general START_SDSU.\n");
                                break;
                           }
                           break;

          case START_SDSU: /* Pass a buffer for the header of the GO command */
                           if ( (retVal = WriteSDSUCommand()) != NO_ERROR ) {
                                ERRMSG("ioctl> ERROR on CONTROLLER START_SDSU.\n");
                                break;
                           }

                           /* Write command structure */
                           time_cmd.cmd = GO;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx\n", time_cmd.cmd, time_cmd.arg1, time_cmd.arg2);
                           if ( (retVal = WriteTimeCommand(&time_cmd)) != NO_ERROR ) {
                                ERRMSG("ioctl> ERROR on general START_SDSU.\n");
                                break;
                           }

                           if ( (retVal = ReadSDSUCommand()) != NO_ERROR )
                                ERRMSG("ioctl> ERROR on CONTROLLER START_SDSU.\n");

                           break;

          case START_PCI : id = *(INT32 *)args;
                           /* Write command structure */
                           pci_cmd.cmd = GOA;
                           pci_cmd.arg1 = 0x00ffffff & id;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx arg3=%#lx\n", pci_cmd.cmd, pci_cmd.arg1, pci_cmd.arg2, pci_cmd.arg3);
                           retVal = WritePCICommand(&pci_cmd);
                           break;


          case STOP_SDSU : time_cmd.cmd = ST;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx\n", time_cmd.cmd, time_cmd.arg1, time_cmd.arg2);
printf("ioctl> cmd <%x> arg1=%#lx arg2=%#lx\n", time_cmd.cmd, time_cmd.arg1, time_cmd.arg2);
                           retVal = WriteTimeCommand(&time_cmd);
printf("ioctl> retVal = %d\n", retVal);
                           break;

          case STOP_PCI  : pci_cmd.cmd = STP;
                           DBGMSG("ioctl> cmd <%x> arg1=%#lx arg2=%#lx arg3=%#lx\n", pci_cmd.cmd, pci_cmd.arg1, pci_cmd.arg2, pci_cmd.arg3);
                           retVal = WritePCICommand(&pci_cmd);
                           break;

          case GET_HDR   : args = context->lastCtHeader;
                           break;

          case FRMCNT    : args = &context->framesRead;
                           break;


          default        : retVal = ERR_BADRQT;
                           break;

     }

     context->controller = SDSU_FREE;

     return retVal;
}

int copen(char *device, unsigned int mask) {

     printf("IN copen.\n");

     if ( strcmp(device, "SIMULATE_DTC") == 0 )
         Simulate = 1;

     if ( (context = (SDSU_CONTEXT *)sdsu_init(CONTROLLER)) == NULL )
          return ERR_OPENCT;

     sprintf(context->cHardwareInfo, "%s", device);
     context->cMask = mask;

     return CONTROLLER;
}

int cclose(int fd) {

     if ( Simulate )
          return NO_ERROR;

     if ( fd == CONTROLLER )
          context->controller = OFF;

     if ( context->controller == OFF && context->frameGrabber == OFF )
          sdsu_cleanup();

     return NO_ERROR;
}

int sfgread(int id, void *buffer, int size) {

     unsigned short *p;
     unsigned short counter = 0;
     int i;

     if ( buffer == NULL ) {
         printf("**** fgread: Gota a NULL buffer.\n");
         return -1;
     }

     sprintf((char *)buffer, "%-24s", "** SDSU HEADER INFO **");
     p = (unsigned short *)buffer + 12;

     for (i = 0; i < size - 24; ++i) {
        *p++ = counter;
	counter = (counter + 1) % 65536;
     }

     sleep(1);

     return size;
}

/************************************************************************************/


/* Read from the frame grabber
 * 
 */

SDSU_BUFFER	fg_buf;
int		bytes;
void		*intFgPhBuff;

int fgread(int id, void *buffer, int size) {

     if ( Simulate )
          return sfgread(id, buffer, size);

     context->fgStatus = READ;

     intFgPhBuff = CACHE_DMA_VIRT_TO_PHYS(buffer);
     /* Write command structure */
     fg_buf.bufphysadr = (const UINT32)intFgPhBuff;
     fg_buf.bufsize = size;
     fg_buf.bufid = 1234;

     DBGMSG("fgread> bufphysadr=%#lx bufsize=%ld bufid=%#lx\n", fg_buf.bufphysadr, fg_buf.bufsize, fg_buf.bufid);

     if (WRITE_FIFO(context->fifo[USER_DATA_EMPTY], (void *) &fg_buf, sizeof(fg_buf)) != BUFFER_OK) {
           ERRMSG("fgread> ERROR - write to FIFO USER_DATA_EMPTY\n");
           return ERR_WRITEFIFO;
     }

     if ((bytes = READ_FIFO(context->fifo[USER_DATA_FULL], (void *) &fg_buf, sizeof(fg_buf))) != sizeof(fg_buf)) {
        if (bytes > 0) {
            ERRMSG("fgread> ERROR - read from FIFO USER_DATA_FULL %d bytes\n", bytes);
            return ERR_READFIFO;
        } else if (bytes < 0) {
            ERRMSG("fgread> ERROR - read from FIFO USER_DATA_FULL, errno %d\n", bytes);
            return ERR_READFIFO;
        } else {
            /* When zero bytes are read this probably means the task or FIFO has been deleted */
            ERRMSG("fgread> read from FIFO USER_DATA_FULL %d bytes, read FIFO aborted!\n", bytes);
            return ERR_READFIFO;
        }
    } else {
        DBGMSG("fgread> bufphysadr=%#lx bufsize=%ld bufid=%#lx\n", fg_buf.bufphysadr, fg_buf.bufsize, fg_buf.bufid);
    }

    if ( fg_buf.bufphysadr == 0 && fg_buf.bufsize == 0 && fg_buf.bufid == 0 ) {
         ERRMSG("fgread> ERROR - got null buffer\n");
         return ERR_READFIFO;
    }

    DBGMSG("fgread> bufphysadr=%#lx bufsize=%ld bufid=%#lx\n", fg_buf.bufphysadr, fg_buf.bufsize, fg_buf.bufid);


    context->fgStatus = SDSU_FREE;

    return size;
}

int fgioctl(int id, int request, void *args) {

     int retVal = NO_ERROR;

     if ( Simulate )
          return NO_ERROR;

     switch (request) {
          case FRMCNT: *(UINT32 *)args = context->framesRead;
                       break;

          default    : retVal = ERR_BADRQT;
     }

     return retVal;
}

int fgopen(char *device, unsigned int mask) {

     if ( Simulate )
          return NO_ERROR;

     printf("IN fgopen.\n");

     if ( (context = (SDSU_CONTEXT *)sdsu_init(FRAME_GRABBER)) == NULL )
          return ERR_OPENFG;

     sprintf(context->fHardwareInfo, "%s (virtual)", device);
     context->fgMask = mask;

     context->frameGrabber = ON;

     return FRAME_GRABBER;
}

int fgclose(int fd) {

     if ( Simulate )
          return NO_ERROR;

     context->frameGrabber = OFF;

     if ( context->controller == OFF && context->frameGrabber == OFF )
          sdsu_cleanup();

     return NO_ERROR;
}

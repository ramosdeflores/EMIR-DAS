#ifndef __SDSU_PLATFORM_H__
#define __SDSU_PLATFORM_H__

/* Motorolla architectures are big endian (left-to-right) */
#define SDSU_BIG_ENDIAN

#define SEM_TIMOUT    ERROR

#define PCI_CMD_OK    OK
#define PCI_REPLY_OK  OK
#define TIME_CMD_OK   OK
#define TIME_REPLY_OK OK
#define CON_CMD_OK    OK
#define HST_CMD_OK    OK
#define BUFFER_OK     OK

#define SDSU_PCI_TASK  "tSDSU_PCI"
#define SDSU_TIME_TASK "tSDSU_Time"
#define SDSU_DATA_TASK "tSDSU_Data"

/* POSIX semaphore declarations "semPXLib" */
#define POST_SEM(a)		semGive(a)
#define WAIT_SEM(a)		semTake(a, WAIT_FOREVER)
#define WAIT_SEM_TIMED(a,b)	semTake(a,b*sysClkRateGet())

#define SIGNAL_FIFO(a) semGive(a);

/* VxWorks message queue "msgQLib" */
#define GET_FIFO(a,b,c)		msgQReceive(a.id,b,c,NO_WAIT)
#define READ_FIFO(a,b,c)	msgQReceive(a.id,b,c,WAIT_FOREVER)
#define READ_FIFO_TIMED(a,b,c,d) msgQReceive(a.id,b,c,d*sysClkRateGet())
#define PUT_FIFO(a,b,c)		msgQSend(a.id,b,c,NO_WAIT,MSG_PRI_NORMAL)
#define WRITE_FIFO(a,b,c)	msgQSend(a.id,b,c,WAIT_FOREVER,MSG_PRI_NORMAL)
#define WRITE_FIFO_TIMED(a,b,c,d) msgQSend(a.id,b,c,d*sysClkRateGet(),MSG_PRI_NORMAL)

#define INT_ENABLE(a)		intEnable(a)
#define INT_ENABLE_AGAIN(a) /* intEnable(a) */
#define INT_DISABLE(a) /* intDisable(a) */

#define PCI_READ32(a,b)		{sysPciRead32(a,b); DBGMSG("PCI_READ32> value %#x read from address %#x\n",*(b),a); }
#define PCI_WRITE32(a,b)	{sysPciWrite32(a,b);DBGMSG("PCI_WRITE32> value %#x written to address %#x\n",b,a);}

/* int printf(const char *fmt); */
/* Macro for printing debug statements to standard output */
#ifdef DEBUG_MESSAGE
#define DBGMSG(x...) printf(x)
#else
#define DBGMSG(x...)
#endif
/* Macro for printing error statements to standard output */
#ifdef ERROR_MESSAGE
#define ERRMSG(x...) printf(x)
#else
#define ERRMSG(x...)
#endif

#define PHYS2BUSADR(a) (a);

/* Using local prototypes as they are missing in VxWorks */
IMPORT void sysPciWrite32(UINT32, UINT32);
IMPORT void sysPciRead32(UINT32, UINT32 *);

#endif /* __SDSU_PLATFORM_H__ */

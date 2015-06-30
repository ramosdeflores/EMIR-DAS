/* Compilation macros */
/* Disable timeout wait operations on semaphores when waiting for replies from the PCI card */
/* #define SDSU_NOTIMEOUT 1 */

/* Use DBGMSG() macro */
/*#define DEBUG_MESSAGE 1  */

/* Use ERRMSG() macro */
#define ERROR_MESSAGE 1

/* Standard C Libraries */
#include <stdio.h>      /* printf() */
#include <stdlib.h>     /* malloc(), free() */
#include <string.h>     /* strcpy(), strncpy() */

/* VxWorks Libraries */
#include "vxWorks.h"
#include "config.h"
#include "mv2400.h"                   /* PMC_INT_LVL1, PMC_INT_LVL4 */
#include "intLib.h"                   /* intConnect() */
#include "arch/ppc/ivPpc.h"           /* INUM_TO_IVEC() */
#include "drv/pci/pciConfigLib.h"     /* PCI_CFG_CACHE_LINE_SIZE, PCI_CFG_LATENCY_TIMER */
#include "drv/pci/pciAutoConfigLib.h" /* pciFindDevice() */
#include "taskLib.h"                  /* taskSpawn(), taskDelete() */
#include "msgQLib.h"                  /* msgQCreate(), msgQDelete(), msgQSend(), msgQReceive() */
#include "semLib.h"                   /* semBCreate(), semGive(), semTake() */
#include "cacheLib.h"                 /* CACHE_DMA_VIRT_TO_PHYS() */

/* Application headers */
#include "ct_interface.h"
#include "sdsu_emir_driver.h"

/* Constants */
#define PCI_BUFFER_SIZE 3  /* Number of words in PCI REP message buffer */
#define PCI_TOUT        30 /* Timeout period for PCI card replies (sec) */
#define INT_TOUT        30 /* Timeout period for PCI card interrupts (sec) */

/* Local varibales (all platforms) */
static SDSU_CONTEXT *con = NULL;              /* Local pointer to context structure */
static void *time_cmd_memadr = NULL;	      /* Virtual memory address for TIME command message to SDSU controller */
static void *time_reply_memadr = NULL;	      /* Virtual memory address for TIME reply message to SDSU controller */
static UINT32 reply_buffer[PCI_BUFFER_SIZE];  /* PCI card data buffer for REP messages */
static UINT32 notify_buffer[PCI_BUFFER_SIZE]; /* PCI card data buffer for SDSU controller NFY messages */
static UINT32 msgcnt = 0;                     /* Debug message counter */
static unsigned int intlvl = 0;               /* PCI board interrupt IRQ/level */ 
static UINT32 iobaseadr = 0;                  /* PCI board IO base address */
static UINT32 SDSU_HSTR_adr = 0;              /* PCI board IO address */
static UINT32 SDSU_HCVR_adr = 0;              /* PCI board IO address */
static UINT32 SDSU_HCTR_adr = 0;              /* PCI board IO address */
static UINT32 SDSU_HTXR_HRXS_adr   = 0;       /* PCI board IO address */
static UINT32 SDSU_HTXR_HRXS_adr4  = 0;       /* PCI board IO address */
static UINT32 SDSU_HTXR_HRXS_adr8  = 0;       /* PCI board IO address */
static UINT32 SDSU_HTXR_HRXS_adr12 = 0;       /* PCI board IO address */

static int pci_task;            /* PCI task ID */
static int time_task;           /* TIME task ID */
static int data_task;           /* DATA task ID */

SEM_ID pci_sem;                 /* USER/TIME task command to PCI task semaphore */
SEM_ID time_sem;                /* USER command to TIME task semaphore */
static SEM_ID int2pci_sem;      /* PCI interrupt (NFY) to PCI task semaphore */
static SEM_ID int2time_sem;     /* PCI interrupt (NFY) to TIME task semaphore */
static SEM_ID int2data_sem;     /* PCI interrupt (NFY) to DATA task semaphore */
static SEM_ID pci2time_sem;     /* PCI reply to TIME thread semaphore */
static SEM_ID pci2data_sem;     /* PCI reply to DATA thread semaphore */

static void *time_cmd_busadr;   /* Bus address for TIME command (data packet) to SDSU controller */
static void *time_reply_busadr; /* Bus address for TIME reply (data packet) from SDSU controller */
static int intvecset = 0;       /* Flag inidicating if the interrupt handler has been added to the vector table */

/*
 * List of the allowed PCI cmds (3 byte strings), allowed arguments and
 * corresponding interrupt (HCVR) vector number
 */
static SDSU_PCI_CMD_DEF pci_cmds[] = {
    {RDM, 1, 1, 0, 0x807B}, /* READ memory  */
    {WRM, 1, 1, 1, 0x8079}, /* WRITE memory */
    {GOA, 1, 0, 0, 0x807D}, /* Application GO */
    {STP, 0, 0, 0, 0x807F}, /* Application STOP */
    {RST, 0, 0, 0, 0x8081}, /* PCI board software RESET */
    {CON, 1, 1, 0, 0x8083}, /* Data packet to CONTROLLER */
    {HST, 1, 1, 0, 0x8085}, /* Data packet to HOST */
    {RCO, 0, 0, 0, 0x8087}, /* Timing board hardware RESET */
#ifdef SDSU_BYTE_SWAP
    {BIG, 0, 0, 0, 0x8089}, /* BIG endian byte swapping */
    {WEE, 0, 0, 0, 0x808B}, /* LITTLE endian byte swapping */
#endif /* SDSU_BYTE_SWAP */
};

/* Macro calculating the number of PCI card commands */
#define NUM_PCI_CMDS (int)(sizeof(pci_cmds)/sizeof(SDSU_PCI_CMD_DEF))

/* Macro calculating the number of SDSU controller commands */
#define NUM_TIME_CMDS (int)(sizeof(time_cmds)/sizeof(SDSU_TIME_CMD_DEF))


/**
 * \fn void pci_card_handler(void)
 *
 * \brief Interrupt handler that services PCI card messages
 *
 * \retval 0
 *
 * \sa pci_fifo_handler(), time_fifo_handler(), pci_task_func(), time_task_func(), data_task_func()
 */
void pci_card_handler(void)
{
    UINT32 word;
    register UINT32 *p;
    register int i;

    /* Detected interrupt from PCI card. Clear the interrupt. */
    PCI_WRITE32(SDSU_HCVR_adr, HCVR_INT_RST);

    /* Got an IRQ from the PCI card */
    DBGMSG("%d INT> woken up\n", msgcnt++);

    /* Read HRXS register for PCI card reply message */
    /* Check HRXS register is full before reading */
    PCI_READ32(SDSU_HSTR_adr, &word);
    if (!(word & HSTR_HRRQ_MASK)) {
        /* Register is empty */
/*        ERRMSG("%d INT> ERROR - no words in HRXS\n", msgcnt++);*/

        /* Clear the interrupt, re-enable the ISR */
        INT_ENABLE_AGAIN(intlvl);

        /* Inform PCI interface that message has been read and return to prepare for next message */
        PCI_WRITE32(SDSU_HCVR_adr, HCVR_INT_DON);
        return;
    }

    /* Read the first word from HRXS register */
    PCI_READ32(SDSU_HTXR_HRXS_adr, &word);

    /* Determine if this is a NOTIFY (from the SDSU) or a REPLY (from the PCI) */
    switch (word) {
	case NFY: /* Received a NFY interrupt from the SDSU */
		DBGMSG("%d INT> got NFY message\n", msgcnt++);

		/* Read off the message words */
		for (i = 0,  p = notify_buffer; i < PCI_BUFFER_SIZE; ++i, ++p) {
			PCI_READ32(SDSU_HSTR_adr, &word);
			if (word & HSTR_HRRQ_MASK) {
				PCI_READ32(SDSU_HTXR_HRXS_adr, p);
			} else {
				ERRMSG("%d INT> ERROR - reading HRXS word %d, FIFO empty\n", msgcnt++, i);
				return;
			}
		}

		/* Read the second word from HRXS register and */
		/* determine the type of NFY message */
		switch (*notify_buffer) {
			case DA: /* Wake up DATA thread */
				if (POST_SEM(int2data_sem)) {
					ERRMSG("%d INT> ERROR - posting int2data_sem\n", msgcnt++);
				}
				break;

			case RP: /* Wake up TIME thread */
				if (POST_SEM(int2time_sem)) {
					ERRMSG("%d INT> ERROR - posting int2time_sem\n", msgcnt++);
				}
				break;

			default: ERRMSG("%d INT> ERROR - unrecognised message recieved <%x>\n", msgcnt++, word);
				break;
		}
		break;

	case REP: /* Received a REPLY interrupt from the PCI */
		DBGMSG("%d INT> got REP message\n", msgcnt++);

		/* Read off the message words */
		for (i = 0, p = reply_buffer; i < PCI_BUFFER_SIZE; ++p, ++i) {
			PCI_READ32(SDSU_HSTR_adr, &word);
			if (word & HSTR_HRRQ_MASK) {
				PCI_READ32(SDSU_HTXR_HRXS_adr, p);
			} else {
				ERRMSG("%d INT> ERROR - reading HRXS word %d, FIFO empty\n", msgcnt++, i);
				return;
			}
		}

		/* Wake up PCI thread */
		if (POST_SEM(int2pci_sem)) {
			ERRMSG("%d INT> ERROR - posting int2pci_sem\n", msgcnt++);
		}
		break;

	default: ERRMSG("%d INT> ERROR - unrecognised message received <> (%#x)\n", msgcnt++, word);
		ERRMSG("%d INT> ERROR - read the following words from HRXS:\n", msgcnt++);
		for (i = 0; i < 100; i++) {
			PCI_READ32(SDSU_HSTR_adr, &word);
			if (word & HSTR_HRRQ_MASK) {
				PCI_READ32(SDSU_HTXR_HRXS_adr, &word);
				ERRMSG("%d %x ", i, word);
			}
		}
		ERRMSG("\n%d INT> ERROR - %d words cleared from HRXS\n", msgcnt++, i);
		if (i == 100) {
			ERRMSG("%d INT> ERROR - clearing HRXS register on PCI card, exiting\n", msgcnt++);
		}
		break;
    }

    /* Re-enable the ISR */
    INT_ENABLE_AGAIN(intlvl);

    /* Inform PCI interface that message has been read and return to prepare for next message */
    DBGMSG("%d INT> go to sleep\n", msgcnt++);
    PCI_WRITE32(SDSU_HCVR_adr, HCVR_INT_DON);
}

/**
 * \fn void pci_task_func(int arg)
 *
 * \brief PCI thread function
 *
 * \param arg Unused
 *
 * This function is the PCI thread which is responsible for recieving commands from the TIME thread, DATA thread or a
 * USER space process and passing them onto the PCI card. It then waits for a response from the PCI card interrupt
 * handler function and parses the reply before sending a reply to sender of the command.
 *
 * \sa pci_fifo_handler(), time_fifo_handler(), pci_card_handler(), time_task_func(), data_task_func()
 */
int pci_task_func(int arg)
{
    SDSU_PCI_CMD pci_cmd;     /* Buffer to hold command read from FIFO */
    SDSU_PCI_REPLY pci_reply; /* Buffer to hold reply to send to USER or TIME  */
    SDSU_ACTION action;       /* Task switches on this */
    char *byte;               /* Pointer for re-ordering little-endian words */
    UINT32 CurrCmd;
    UINT32 tmp;               /* Scratch */
    register int i;           /* Counter */

    /* Kill any unserviced interrupt and interrupt the DSP with this command */
    PCI_WRITE32(SDSU_HCVR_adr, HCVR_INT_RST);

    /* To set the card to PCI mode (32->24 bit conversion at the transmit/receive registers) */
    PCI_WRITE32(SDSU_HCTR_adr, 0x900);

    /* We'll use this as a check that the card has been located */
    PCI_READ32(SDSU_HCTR_adr, &tmp);
    if (tmp != 0x900) {
	ERRMSG("%d PCI> ERROR - locating PCI interface card\n", msgcnt++);
	return -1;
    }

    /* Readout contents of HRXS register to clear out garbage */
    for (i = 0; i < 100; i++) {
        PCI_READ32(SDSU_HSTR_adr, &tmp);
        if (tmp & HSTR_HRRQ_MASK) {
	    PCI_READ32(SDSU_HTXR_HRXS_adr, &tmp);
        } else
            break;
    }
    DBGMSG("%d PCI> %d words cleared from HRXS\n", msgcnt++, i);
    if (i == 100) {
	ERRMSG("%d PCI> ERROR - clearing HRXS register on PCI card, exiting\n", msgcnt++);
	return -1;
    }

    /* Disable interrupt before enable, in case things weren't cleanly shut down last time */
    INT_DISABLE(intlvl);
    INT_ENABLE(intlvl);
    PCI_WRITE32(SDSU_HCVR_adr, HCVR_INT_DON);

    while (1) {
	/* We have a message from the outside world */
	/* Collect it, deciding where it came from and what to do with it along the way */
	/* Commands from the data board thread get highest priority */
	if (GET_FIFO(con->fifo[DATA_PCI_CMD], (char *) &pci_cmd, sizeof(pci_cmd)) == sizeof(pci_cmd)) {
            DBGMSG("%d PCI> got command from DATA-PCI command FIFO\n", msgcnt++);
            action = SEND_CMD_FROM_DATA;
	} else {
		/* Commands from the timing board thread get medium priority */
		if (GET_FIFO(con->fifo[TIME_PCI_CMD], (char *) &pci_cmd, sizeof(pci_cmd)) == sizeof(pci_cmd)) {
			DBGMSG("%d PCI> got command from TIME-PCI command FIFO\n", msgcnt++);
			action = SEND_CMD_FROM_TIME;
		} else {
			/* Commands from the user get sent at lowest priority */
			if (GET_FIFO(con->fifo[USER_PCI_CMD], (char *) &pci_cmd, sizeof(pci_cmd)) == sizeof(pci_cmd)) {
				DBGMSG("%d PCI> got command from USER-PCI command FIFO\n", msgcnt++);
				action = SEND_CMD_FROM_USER;
			} else {
					/* We've reached here because there are no commands waiting to be sent */
					/* Go to sleep and wait to be woken when there's activity on one of the FIFOs */
					action = GO_TO_SLEEP;
					DBGMSG("%d PCI> no commands to send, go to sleep\n", msgcnt++);
					WAIT_SEM(pci_sem);
					DBGMSG("%d PCI> woken up\n", msgcnt++);
					continue;
				}
                        }
                }

	/* Write command structure into write register on the PCI card */
	DBGMSG("%d PCI> COMMAND cmd <%#x> arg1=%#x arg2=%#x arg3=%#x\n", msgcnt++, pci_cmd.cmd, pci_cmd.arg1,
	pci_cmd.arg2, pci_cmd.arg3);

	/* Check HTXR register is empty */
       	PCI_READ32(SDSU_HSTR_adr, &tmp);
	if (tmp & HSTR_TRDY_MASK) {
		DBGMSG("%d PCI> writing command to HTXR\n", msgcnt++);
		PCI_WRITE32(SDSU_HTXR_HRXS_adr, (UINT32) pci_cmd.cmd);
		PCI_WRITE32(SDSU_HTXR_HRXS_adr4, (UINT32) pci_cmd.arg1);
		PCI_WRITE32(SDSU_HTXR_HRXS_adr8, (UINT32) pci_cmd.arg2);
		PCI_WRITE32(SDSU_HTXR_HRXS_adr12, (UINT32) pci_cmd.arg3);
	} else {
		ERRMSG("%d PCI> ERROR - writing to HTXR, FIFO not empty\n", msgcnt++);
		continue;
	}

	/* Write the command vector to interrupt the DSP */
	/* Use default if no match to the command is found */
        switch(pci_cmd.cmd) {
             case RDM: tmp = 0x807B;
                       break;
             case WRM: tmp = 0x8079;
                       break;
             case GOA: tmp = 0x807D;
                       break;
             case STP: tmp = 0x807F;
                       break;
             case RST: tmp = 0x8081;
                       break;
             case CON: tmp = 0x8083;
                       break;
             case HST: tmp = 0x8085;
                       break;
             case RCO: tmp = 0x8087;
                       break;
             case BIG: tmp = 0x8089;
                       break;
             case WEE: tmp = 0x808B;
                       break;
             default : tmp = 0x807B;
                       break;
        }


	/* Interrupt the DSP with command */
	DBGMSG("%d PCI> writing interrupt to HCVR\n", msgcnt++);
	PCI_WRITE32(SDSU_HCVR_adr, tmp);

	/* Wait for reply */
	WAIT_SEM(int2pci_sem);

	/* Woken up - we know this is the reply from PCI board */
	DBGMSG("%d PCI> woken up - got reply?\n", msgcnt++);

	/* These data are little-endian - be careful about the conversion to string */
	pci_reply.cmd = reply_buffer[0];

	/* ... next the status string */
	/* These data are little-endian, be careful about the conversion to string */
	pci_reply.status = reply_buffer[1];

	/* ... last the data/error word */
	pci_reply.data = reply_buffer[2];
	    
	DBGMSG("%d PCI> REPLY cmd <%x> status <%x> data=%#x\n", msgcnt++, &pci_reply.cmd,
	&pci_reply.status, pci_reply.data);

	switch (action) {
		case SEND_CMD_FROM_DATA:	/* We have a message from the DATA thread */
						/* Write reply to DATA-PCI reply FIFO */
			if (PUT_FIFO(con->fifo[DATA_PCI_REPLY], (char *)&pci_reply, sizeof(pci_reply)) != PCI_REPLY_OK) {
				ERRMSG("%d PCI> ERROR - writing to DATA-PCI reply FIFO\n", msgcnt++);
				continue;
			}
			DBGMSG("%d PCI> forwarded reply, waking DATA thread\n", msgcnt++);
			POST_SEM(pci2data_sem);
			break;

		case SEND_CMD_FROM_TIME:	/* We have a message from the TIME thread */
						/* Write reply to TIME-PCI reply FIFO */
			if (PUT_FIFO(con->fifo[TIME_PCI_REPLY],(char *)&pci_reply, sizeof(pci_reply)) != PCI_REPLY_OK) {
				ERRMSG("%d PCI> ERROR - writing to TIME-PCI reply FIFO\n", msgcnt++);
				continue;
			}
			DBGMSG("%d PCI> forwarded reply, waking TIME thread\n", msgcnt++);
			POST_SEM(pci2time_sem);
			break;

		case SEND_CMD_FROM_USER:	/* We have a message from a USER process */
						/* Write reply to USER-PCI reply FIFO */
			if (PUT_FIFO(con->fifo[USER_PCI_REPLY],(char *)&pci_reply, sizeof(pci_reply)) != PCI_REPLY_OK) {
				ERRMSG("%d PCI> ERROR - writing to USER-PCI reply FIFO\n", msgcnt++);
				continue;
			}
			DBGMSG("%d PCI> forwarded reply\n", msgcnt++);
			break;

		default:			/* A serious bug */
			ERRMSG("%d PCI> ERROR - PCI action unrecognised\n", msgcnt++);
			WAIT_SEM(pci_sem);
			ERRMSG("%d PCI> woken from error state\n", msgcnt++);
			break;
	}
    }
    return -1;
}

/**
 * \fn void time_task_func(int arg)
 *
 * \brief TIME thread function
 *
 * \param arg Unused
 *
 * This function is the TIME thread which is responsible for recieving command from a USER space process and passing it
 * onto the SDSU controller timing board. However, it does send the command directly to the timing board, it sends the
 * command to the PCI card which passes it to the PCI card who then passes it to the timing board. It then waits for a
 * response from the PCI card interrupt handler function and parses the reply before sending a reply to sender of the
 * command.
 *
 * \sa pci_fifo_handler(), time_fifo_handler(), pci_card_handler(), pci_task_func(), data_task_func()
 */
int time_task_func(int arg)
{
    SDSU_PCI_CMD con_pci_cmd;
    SDSU_PCI_CMD hst_pci_cmd;
    SDSU_PCI_REPLY pci_reply;
    SDSU_TIME_CMD *time_cmd;
    SDSU_TIME_REPLY *time_reply;
    char tmp;			/* For re-ordering little-endian words */
    char *time_reply_byte0;
    char *time_reply_byte1;
    char *time_reply_byte2;
    char *time_reply_byte3;
    UINT32 word;

    /* Set-up the PCI CON command */
    con_pci_cmd.cmd = CON;
    /* Write bus address of memory used for storing command (write 2 MSB in arg1 and 2 LSB in arg2) */
    con_pci_cmd.arg1 = ((UINT32) time_cmd_busadr) >> 16;
    con_pci_cmd.arg2 = ((UINT32) time_cmd_busadr) & 0xFFFF;
    con_pci_cmd.arg3 = 0;

    /* Set-up the PCI HST command */
    hst_pci_cmd.cmd = HST;
    /* Write bus address of memory used for storing reply (write 2 MSB in arg1 and 2 LSB in arg2) */
    hst_pci_cmd.arg1 = ((UINT32) time_reply_busadr) >> 16;
    hst_pci_cmd.arg2 = ((UINT32) time_reply_busadr) & 0xFFFF;
    hst_pci_cmd.arg3 = 0;

    /* We'll use this pointer for the command byte re-ordering */
    time_cmd = (SDSU_TIME_CMD *) time_cmd_memadr;
    /* We'll use this pointer for the reply byte re-ordering */
    time_reply = (SDSU_TIME_REPLY *) time_reply_memadr;

    time_reply_byte0 = (char *) &time_reply->data;
    time_reply_byte1 = time_reply_byte0 + 1;
    time_reply_byte2 = time_reply_byte0 + 2;
    time_reply_byte3 = time_reply_byte0 + 3;

    while (1) {
	DBGMSG("%d TIME> no commands to send, go to sleep\n", msgcnt++);
	WAIT_SEM(time_sem);

	/* Woken up, must be a message from the user space process */
	/* Get the USER command from the USER-TIME FIFO and write to command buffer          */
	if (GET_FIFO(con->fifo[USER_TIME_CMD],(char *)time_cmd_memadr,sizeof(SDSU_TIME_CMD)) != sizeof(SDSU_TIME_CMD)) {
	    ERRMSG("%d TIME> ERROR - reading TIME command from USER-TIME FIFO\n", msgcnt++);
	    goto error;
	}

	/* Byte swap the command characters */
	DBGMSG("%d TIME> COMMAND cmd <%#x> arg1=%#x arg2=%#x\n", msgcnt++, time_cmd->cmd, time_cmd->arg1,
	       time_cmd->arg2);

	/* Write CON command to TIME-PCI command FIFO */
	if ((PUT_FIFO(con->fifo[TIME_PCI_CMD], (char *)&con_pci_cmd, sizeof(con_pci_cmd))) != CON_CMD_OK) {
	    ERRMSG("%d TIME> ERROR - writing CON command to TIME-PCI command FIFO\n", msgcnt++);
	    goto error;
	}
	/* Wake the PCI thread */
	DBGMSG("%d TIME> sent CON command, wake up PCI, go to sleep\n", msgcnt++);
	POST_SEM(pci_sem);

	/* Wait for PCI reply to CON command */
	WAIT_SEM(pci2time_sem);

	/* Get PCI reply and check status */
	DBGMSG("%d TIME> woken up, received reply to CON command?\n", msgcnt++);
	if (GET_FIFO(con->fifo[TIME_PCI_REPLY], (char *) &pci_reply, sizeof(pci_reply)) != sizeof(pci_reply)) {
	    ERRMSG("%d TIME> ERROR - reading reply from TIME-PCI reply FIFO\n", msgcnt++);
	    goto error;
	}
	if (pci_reply.status != ACK) {
	    ERRMSG("%d TIME> ERROR - received reply <%x> from CON command\n", msgcnt++, pci_reply.status);
	    goto error;
	}
	DBGMSG("%d TIME> waiting for NFY message, go to sleep\n", msgcnt++);

	/* Wait for SDSU reply to USER command */
	WAIT_SEM(int2time_sem);

	/* Write HST command to TIME-PCI command FIFO */
	DBGMSG("%d TIME> woken up, got NFY message?\n", msgcnt++);
	if ((PUT_FIFO(con->fifo[TIME_PCI_CMD], (char *) &hst_pci_cmd, sizeof(hst_pci_cmd))) != HST_CMD_OK) {
	    ERRMSG("%d TIME> ERROR - writing HST command to TIME-PCI command FIFO\n", msgcnt++);
	    goto error;
	}
	/* Wake PCI thread */
	DBGMSG("%d TIME> sent HST command, wake up PCI, then go to sleep\n", msgcnt++);
	POST_SEM(pci_sem);

	/* Wait for PCI reply to HST command */
	WAIT_SEM(pci2time_sem);

	/* Get PCI reply and check status */
	DBGMSG("%d TIME> woken up, received reply to HST command?\n", msgcnt++);
	if (GET_FIFO(con->fifo[TIME_PCI_REPLY], (char *) &pci_reply, sizeof(pci_reply)) != sizeof(pci_reply)) {
	    ERRMSG("%d TIME> ERROR - reading reply from TIME-PCI reply FIFO\n", msgcnt++);
	    goto error;
	}
	if (pci_reply.status != ACK) {
	    ERRMSG("%d TIME> ERROR - received reply <%x> from HST command\n", msgcnt++, pci_reply.status);
	    goto error;
	}
	DBGMSG("%d TIME> received good reply from HST command\n", msgcnt++);

	/* 2 LSB swap */
	tmp = *time_reply_byte0;
	*time_reply_byte0 = *time_reply_byte1;
	*time_reply_byte1 = tmp;
	/* 2 MSB swap */
	tmp = *time_reply_byte2;
	*time_reply_byte2 = *time_reply_byte3;
	*time_reply_byte3 = tmp;

	DBGMSG("%d TIME> REPLY cmd <%2.2s> data=%#x\n", msgcnt++, time_reply->cmd,
               time_reply->data);

	/* Write reply to USER command to USER-TIME reply FIFO */
	if (PUT_FIFO (con->fifo[USER_TIME_REPLY],(char *) time_reply_memadr,sizeof(SDSU_TIME_REPLY)) != TIME_REPLY_OK) {
	    ERRMSG("%d TIME> ERROR - writing to USER-TIME reply FIFO\n", msgcnt++);
	    goto error;
	}
	DBGMSG("%d TIME> command sent, go to sleep\n", msgcnt++);
	continue;

      error:
	/* Write error status message to USER-TIME reply FIFO */
        time_reply->cmd = time_cmd->cmd | ER; /* ADDED */
	time_reply->data = 0;
	if (PUT_FIFO(con->fifo[USER_TIME_REPLY], (char *) time_reply_memadr, sizeof(SDSU_TIME_REPLY)) != TIME_REPLY_OK)
	    ERRMSG("%d TIME> ERROR - writing to USER-TIME reply FIFO\n", msgcnt++);
	else {
	    DBGMSG("%d TIME> forwarded reply\n", msgcnt++);
        }
    }

    return -1;
}

/**
 * \fn void data_task_func(int arg)
 *
 * \brief DATA thread function
 *
 * \param arg Unused
 *
 * This function is the DATA thread which is responsible for recieving interrupts from the PCI card when data from
 * exposures have been sent from the SDSU controller. It gives the address of the data buffer to be used for storing
 * the exposure data to the PCI thread which then passes it to the PCI card.
 *
 * \sa pci_fifo_handler(), time_fifo_handler(), pci_card_handler(), pci_task_func(), time_task_func()
 */
int data_task_func(int arg)
{
    SDSU_PCI_CMD hst_pci_cmd; /* PCI command structure used for "HST" commands to timing board */
    SDSU_PCI_REPLY pci_reply; /* PCI reply structure */
    SDSU_BUFFER buf;          /* Data buffer structure */
    UINT32 busadr;            /* Bus address for data buffer */
    int bytes;

    /* Set-up the PCI command */
    hst_pci_cmd.cmd = HST;

    while (1) {
	DBGMSG("%d DATA> go to sleep, waiting for NFY message\n", msgcnt++);

	/* Wait for NFY interrupt */
	WAIT_SEM(int2data_sem);

	DBGMSG("%d DATA> woken up - got NFY message?\n", msgcnt++);

	/* Get buffer details from USER-DATA command FIFO */
	if ((bytes = GET_FIFO(con->fifo[USER_DATA_EMPTY], (char *) &buf, sizeof(buf))) != sizeof(buf)) {
	    ERRMSG("%d DATA> ERROR - reading reply from USER-DATA empty buffer FIFO, read %d bytes\n", msgcnt++, bytes);
            buf.bufphysadr = 0;
            buf.bufsize = 0;
            buf.bufid = 0;
	    ERRMSG("%d DATA> ERROR - writing NULL buffer to USER-DATA full buffer FIFO, go to sleep\n", msgcnt++);
	    if (PUT_FIFO(con->fifo[USER_DATA_FULL], (char *) &buf, sizeof(buf)) != BUFFER_OK) {
	        ERRMSG("%d DATA> ERROR - writing to USER-DATA full buffer FIFO\n", msgcnt++);
	    }
	    continue;
	}
	DBGMSG("%d DATA> empty buffer: bufphysadr=%#x bufsize=%ld bufid=%#ld\n", msgcnt++, buf.bufphysadr, buf.bufsize,
               buf.bufid);

	/* Convert data buffer virtual address into physical bus address */
	busadr = PHYS2BUSADR(buf.bufphysadr);

	/* Write bus address of memory used for storing reply (write 2 MSB in arg1 and 2 LSB in arg2) */
	hst_pci_cmd.arg1 = ((UINT32) busadr) >> 16;
	hst_pci_cmd.arg2 = ((UINT32) busadr) & 0xFFFF;
	hst_pci_cmd.arg3 = 0;

	/* Write command again to DATA-PCI command FIFO */
	if ((PUT_FIFO(con->fifo[DATA_PCI_CMD], (char *) &hst_pci_cmd, sizeof(hst_pci_cmd))) != HST_CMD_OK) {
	    ERRMSG("%d DATA> ERROR - writing HST command to DATA-PCI command FIFO\n", msgcnt++);
	    continue;
	}

	/* Wake PCI thread */
	POST_SEM(pci_sem);
	DBGMSG("%d DATA> sent HST command, wake PCI thread, then go to sleep\n", msgcnt++);

	/* Wait for reply to HST command */
	WAIT_SEM(pci2data_sem);

	/* Get PCI reply and check status */
	DBGMSG("%d DATA> woken up, received reply to HST command?\n", msgcnt++);
	if (GET_FIFO(con->fifo[DATA_PCI_REPLY], (char *) &pci_reply, sizeof(pci_reply)) != sizeof(pci_reply)) {
	    ERRMSG("%d DATA> ERROR - reading reply from DATA-PCI reply FIFO\n", msgcnt++);
	    continue;
	}

	if (pci_reply.status != ACK) {
	    ERRMSG("%d DATA> ERROR - received reply <%x> from HST command\n", msgcnt++, pci_reply.status);
	    continue;
	}

	DBGMSG("%d DATA> received good reply from HST command\n", msgcnt++);

	/* Send off reply to USER-DATA reply FIFO */
	DBGMSG("%d DATA> full buffer: bufphysadr=%#x bufsize=%ld bufid=%#ld\n", msgcnt++, buf.bufphysadr, buf.bufsize,
	       buf.bufid);
	if (PUT_FIFO(con->fifo[USER_DATA_FULL], (char *) &buf, sizeof(buf)) != BUFFER_OK) {
	    ERRMSG("%d DATA> ERROR - writing to USER-DATA full buffer FIFO\n", msgcnt++);
	    continue;
	}
	DBGMSG("%d DATA> full buffer sent, go to sleep\n", msgcnt++);
    }

    return -1;
}

/**
 * \fn void remove_vxworks_resources(SDSU_RESOURCE_TYPE resource)
 *
 * \brief Removes the VxWorks SDSU driver resources
 *
 * \param resource Resource type from which to start removing
 *
 * Cleans up the SDSU driver resources in the order listed below starting from the resource type specified in the
 * argument.
 *
 * -# Destoy all tasks
 * -# Destroy all semaphores
 * -# Destroy all FIFOs
 * -# Free buffer memory
 * -# Disable interrupt
 * -# Free data structure
 *
 * \sa sdsu_init(), sdsu_cleanup(), pci_task_func(), time_task_func(), data_task_func()
 */
static void remove_vxworks_resources(SDSU_RESOURCE_TYPE resource)
{
    int i;

    switch (resource) {
    case SDSU_TASK:
	/* Kill tasks */
	DBGMSG("SDSU cleanup> destroying tasks\n");
	if (taskDelete(pci_task) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - deleting PCI task\n");
	if (taskDelete(time_task) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - deleting TIME task\n");
	if (taskDelete(data_task) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - deleting DATA task\n");

    case SDSU_SEMAPHORE:
	/* Destory any existing semaphores */
	DBGMSG("SDSU cleanup> destroying semaphores\n");
	if (semDelete(pci_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying pci_sem semaphore\n");
	if (semDelete(time_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying time_sem semaphore\n");
	if (semDelete(pci2time_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying pci2time_sem semaphore\n");
	if (semDelete(pci2data_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying pci2data_sem semaphore\n");
	if (semDelete(int2pci_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying int2pci_sem semaphore\n");
	if (semDelete(int2time_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying int2time_sem semaphore\n");
	if (semDelete(int2data_sem) == ERROR)
	    ERRMSG("SDSU cleanup> ERROR - destroying int2data_sem semaphore\n");

    case SDSU_FIFO:
	/* Destroy any existing FIFOs running */
	DBGMSG("SDSU cleanup> destroying RTAI FIFOs\n");
	for (i = 0; i < SDSU_MAX_FIFO; i++)
	    if (msgQDelete(con->fifo[i].id) == ERROR)
                ERRMSG("SDSU cleanup> ERROR - destroying FIFO <%s>\n", con->fifo[i].name);

    case SDSU_KERNEL_MEM:
	/* Free kernel memory */
	DBGMSG("SDSU cleanup> freeing dynamic kernel memory\n");
	if (time_cmd_memadr == NULL)
	    ERRMSG("SDSU cleanup> ERROR - invalid time command structure\n");
	else
	    free(time_cmd_memadr);
	if (time_reply_memadr == NULL)
	    ERRMSG("SDSU cleanup> ERROR - invalid time reply structure\n");
	else
	    free(time_reply_memadr);

    case SDSU_IRQ:
	/* Remove IRQ */
	DBGMSG("SDSU cleanup> disabling interrupt handler\n");
	if (intDisable(intlvl) == ERROR)
            ERRMSG("SDSU cleanup> ERROR - disabling SDSU PCI card interrupt handler\n");

    case SDSU_DATA:
	/* Free memory */
	DBGMSG("SDSU cleanup> freeing memory\n");
	if (con == NULL)
	    ERRMSG("SDSU cleanup> ERROR - invalid context structure\n");
	else {
	    free(con);
            con = NULL;
        }
    break;

    default:
	ERRMSG("SDSU cleanup> ERROR - unknown resource type\n");
    }
    DBGMSG("SDSU cleanup> clean up complete\n");
}

/**
 * \fn int sdsu_init(void)
 *
 * \brief Initialises the VxWorks SDSU driver
 *
 * \retval 0 if successful
 * \retval -1 if module initialisatin failed
 *
 * Initialising the SDSU driver resources includes the following...
 *
 * -# Create the SDSU context data structure
 * -# Creating all FIFOs
 * -# Attaching interrupt handler
 * -# Allocating memory for storing SDSU controller commands and replies
 * -# Creating all semaphores
 * -# Creating all tasks
 *
 * \sa cleanup_module(), pci_task_func(), time_task_func(), data_task_func()
 */
SDSU_CONTEXT *sdsu_init(int device)
{
    UINT8 intvec;
    int unit = 0;
    int busnum;
    int devnum;
    int funcnum;

    /* Attach to shared memory context structure */
    if (con != NULL) {
        if ( device == FRAME_GRABBER )
             con->frameGrabber = ON;
        else
             con->controller = ON;
    } else {
    	if ((con = (SDSU_CONTEXT *) malloc(sizeof(SDSU_CONTEXT))) == NULL) {
             ERRMSG("SDSU init> ERROR - failed to allocate context structure\n");
             return NULL;
        }

        con->framesRead = 0;
        con->fgStatus = SDSU_FREE;
        con->fgMask = 0;
        con->cStatus = SDSU_FREE;
        con->cMask = 0;
        sprintf(con->cSoftwareInfo, "/EMIR SDSU driver V.01");
        sprintf(con->cHardwareInfo, "/SDSU");
        sprintf(con->fSoftwareInfo, "/EMIR SDSU driver V.01");
        sprintf(con->fHardwareInfo, "/SDSU");

        if ( device == FRAME_GRABBER )
             con->frameGrabber = ON;
        else
             con->controller = ON;

        /* Find the PCI card, let's ask vxWorks where it is */
        DBGMSG("SDSU init> searching for SDSU PCI card\n");
        if (pciFindDevice(SDSU_VENDOR_ID, SDSU_DEVICE_ID, unit, &busnum, &devnum, &funcnum) == ERROR) {
             ERRMSG("SDSU init> ERROR - SDSU PCI card not known to vxWorks\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }
        DBGMSG("SDSU init> PCI bus number %d\n", busnum);
        DBGMSG("SDSU init> PCI device number %d\n", devnum);
        DBGMSG("SDSU init> PCI function number %d\n", funcnum);

        /* Set the PCI base address */
        if (pciConfigInLong(busnum, devnum, funcnum, PCI_CFG_BASE_ADDRESS_0, &iobaseadr) == ERROR) {
             ERRMSG("SDSU init> ERROR - failed to read PCI base address register\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }

        SDSU_HSTR_adr = iobaseadr + SDSU_HSTR;
        SDSU_HCTR_adr = iobaseadr + SDSU_HCTR;
        SDSU_HCVR_adr = iobaseadr + SDSU_HCVR;
        SDSU_HTXR_HRXS_adr   = iobaseadr + SDSU_HTXR_HRXS;
        SDSU_HTXR_HRXS_adr4  = SDSU_HTXR_HRXS_adr + 4;
        SDSU_HTXR_HRXS_adr8  = SDSU_HTXR_HRXS_adr + 8;
        SDSU_HTXR_HRXS_adr12 = SDSU_HTXR_HRXS_adr + 12;


        /* Set the PCI base address */
        if (pciConfigInByte(busnum, devnum, funcnum, PCI_CFG_DEV_INT_LINE, &intvec) == ERROR) {
             ERRMSG("SDSU init> ERROR - failed to read PCI interrupt vector register\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }

        /* Write new command register value */
        if (pciConfigOutLong(busnum, devnum, funcnum, PCI_CFG_COMMAND, 0x0116) == ERROR) {
             ERRMSG("SDSU init> ERROR - failed to write command register\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }

        /* Write new latency timer register value */
        if (pciConfigOutByte(busnum, devnum, funcnum, PCI_CFG_LATENCY_TIMER, 0xff) == ERROR) {
             ERRMSG("SDSU init> ERROR - failed to write latency timer register\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }

        /* Write new cache line size register value */
        if (pciConfigOutByte(busnum, devnum, funcnum, PCI_CFG_CACHE_LINE_SIZE, 0x00) == ERROR) {
             ERRMSG("SDSU init> ERROR - failed to write cache line size register\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }
    
        /* Display the SDSU PCI card header registers */
#ifdef DEBUG_MESSAGE
        DBGMSG("SDSU init> SDSU PCI Header Configuration Registers:\n");
        if (pciHeaderShow(busnum, devnum, funcnum) == ERROR) {
             ERRMSG("SDSU init> ERROR - failed to display SDSU PCI card header registers\n");
             remove_vxworks_resources(SDSU_DATA);
             return NULL;
        }
#endif

        /* Determine which PMC slot the SDSU board is in and set the appropriate interrupt level */
        if (intvecset == 0) {
             DBGMSG("SDSU init> SDSU PCI interrupt vector %#x\n", intvec);
             /* Set the interrupt level and connect the interrupt handler */
             if (intvec == (UINT8)INUM_TO_IVEC(PCI_PRI_INTA_VEC)) {
                  DBGMSG("SDSU init> SDSU PMC card in PMC slot 1\n");
                  intlvl = PMC_INT_LVL1;
                  if (intConnect((VOIDFUNCPTR *)INUM_TO_IVEC(PCI_PRI_INTA_VEC), (VOIDFUNCPTR)pci_card_handler, 0) == ERROR) {
                       ERRMSG("SDSU init> ERROR - failed to connect PCI interrupt handler\n");
                       remove_vxworks_resources(SDSU_DATA);
                       return NULL;
                  }
             } else if (intvec == (UINT8)INUM_TO_IVEC(PCI_PRI_INTD_VEC)) {
                  DBGMSG("SDSU init> SDSU PMC card in PMC slot 2\n");
                  intlvl = PMC_INT_LVL4;
                  if (intConnect((VOIDFUNCPTR *)INUM_TO_IVEC(PCI_PRI_INTD_VEC), (VOIDFUNCPTR)pci_card_handler, 0) == ERROR) {
                       ERRMSG("SDSU init> ERROR - failed to connect PCI interrupt handler\n");
                       remove_vxworks_resources(SDSU_DATA);
                       return NULL;
                  }
             } else {
                  ERRMSG("SDSU init> SDSU PMC card in PMC slot unknown\n");
                  remove_vxworks_resources(SDSU_DATA);
                  return NULL;
             }
        }

        intvecset = 1;

        /* Reserve a block of memory for the timing board commands */
        DBGMSG("SDSU init> allocating memory\n");
        if ((time_cmd_memadr = (SDSU_TIME_CMD *) memalign(SDSU_BURST_BLOCK, sizeof(SDSU_TIME_CMD))) == NULL) {
             ERRMSG("SDSU init> ERROR - allocating timing board command buffer memory\n");
             remove_vxworks_resources(SDSU_IRQ);
             return NULL;
        }

        DBGMSG("SDSU init> timing board command buffer allocated at %#x\n", time_cmd_memadr);
        time_cmd_busadr = CACHE_DMA_VIRT_TO_PHYS(time_cmd_memadr);
        DBGMSG("SDSU init> timing board command buffer on bus at %#x\n", time_cmd_busadr);

        /* Reserve a block of memory for the timing board replies */
        if ((time_reply_memadr = (SDSU_TIME_REPLY *) memalign(SDSU_BURST_BLOCK, sizeof(SDSU_TIME_REPLY))) == NULL) {
             ERRMSG("SDSU init> ERROR - allocating timing board reply buffer memory\n");
             remove_vxworks_resources(SDSU_IRQ);
             return NULL;
        }
        DBGMSG("SDSU init> timing board reply buffer allocated at %#x\n", time_reply_memadr);
        time_reply_busadr = CACHE_DMA_VIRT_TO_PHYS(time_reply_memadr);
        DBGMSG("SDSU init> timing board reply buffer on bus at %#x\n", time_reply_busadr);

        /* Create new FIFOs */
        DBGMSG("SDSU init> creating FIFOs\n");
        strcpy((char *) con->fifo[USER_DATA_EMPTY].name, "USER_DATA_EMPTY");
        if ((con->fifo[USER_DATA_EMPTY].id = msgQCreate(SDSU_MAX_BUFFER, sizeof(SDSU_BUFFER), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_DATA_EMPTY].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[USER_DATA_FULL].name, "USER_DATA_FULL");
        if ((con->fifo[USER_DATA_FULL].id = msgQCreate(SDSU_MAX_BUFFER, sizeof(SDSU_BUFFER), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_DATA_FULL].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[USER_USER_EMPTY].name, "USER_USER_EMPTY");
        if ((con->fifo[USER_USER_EMPTY].id = msgQCreate(SDSU_MAX_BUFFER, sizeof(SDSU_BUFFER), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_USER_EMPTY].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[USER_PCI_CMD].name, "USER_PCI_CMD");
        if ((con->fifo[USER_PCI_CMD].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_PCI_CMD), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_PCI_CMD].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[USER_PCI_REPLY].name, "USER_PCI_REPLY");
        if ((con->fifo[USER_PCI_REPLY].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_PCI_REPLY), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_PCI_REPLY].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[USER_TIME_CMD].name, "USER_TIME_CMD");
        if ((con->fifo[USER_TIME_CMD].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_TIME_CMD), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_TIME_CMD].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[USER_TIME_REPLY].name, "USER_TIME_REPLY");
        if ((con->fifo[USER_TIME_REPLY].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_TIME_REPLY), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[USER_TIME_REPLY].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[TIME_PCI_CMD].name, "TIME_PCI_CMD");
        if ((con->fifo[TIME_PCI_CMD].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_PCI_CMD), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[TIME_PCI_CMD].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[TIME_PCI_REPLY].name, "TIME_PCI_REPLY");
        if ((con->fifo[TIME_PCI_REPLY].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_PCI_REPLY), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[TIME_PCI_REPLY].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[DATA_PCI_CMD].name, "DATA_PCI_CMD");
        if ((con->fifo[DATA_PCI_CMD].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_PCI_CMD), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[DATA_PCI_CMD].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        strcpy((char *) con->fifo[DATA_PCI_REPLY].name, "DATA_PCI_REPLY");
        if ((con->fifo[DATA_PCI_REPLY].id = msgQCreate(SDSU_MAX_MSG, sizeof(SDSU_PCI_REPLY), MSG_Q_FIFO)) == NULL) {
             ERRMSG("SDSU cleanup> ERROR - creating FIFO <%s>\n", con->fifo[DATA_PCI_REPLY].name);
             remove_vxworks_resources(SDSU_KERNEL_MEM);
             return NULL;
        }

        /* Create semaphores */
        DBGMSG("SDSU init> creating semaphores\n");
        if (((int2data_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL) ||
             ((int2pci_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL) ||
             ((int2time_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL) ||
             ((pci2data_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL) ||
             ((pci2time_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL) ||
             ((pci_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL) ||
             ((time_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY)) == NULL)) {
             ERRMSG("SDSU init> ERROR - creating semaphores\n");
             remove_vxworks_resources(SDSU_FIFO);
             return NULL;
        }

        /* Initialise all tasks */
        DBGMSG("SDSU init> creating tasks\n");
        pci_task = taskSpawn(SDSU_PCI_TASK, SDSU_PCI_PRIORITY, 0, SDSU_STACK_SIZE, pci_task_func,0,0,0,0,0,0,0,0,0,0);
        if (pci_task == ERROR) {
             ERRMSG("SDSU init> ERROR - spawning PCI task\n");
             remove_vxworks_resources(SDSU_SEMAPHORE);
             return NULL;
        }

        time_task = taskSpawn(SDSU_TIME_TASK, SDSU_TIME_PRIORITY, 0, SDSU_STACK_SIZE, time_task_func,0,0,0,0,0,0,0,0,0,0);
        if (time_task == ERROR) {
             ERRMSG("SDSU init> ERROR - spawning TIME task\n");
             remove_vxworks_resources(SDSU_SEMAPHORE);
             return NULL;
        }

        data_task = taskSpawn(SDSU_DATA_TASK, SDSU_DATA_PRIORITY, 0, SDSU_STACK_SIZE, data_task_func,0,0,0,0,0,0,0,0,0,0);
        if (data_task == ERROR) {
             ERRMSG("SDSU init> ERROR - spawning DATA task\n");
             remove_vxworks_resources(SDSU_SEMAPHORE);
             return NULL;
        }
    }

    return con;
}

/**
 * \fn void sdsu_cleanup(void)
 *
 * \brief Cleans up VxWorks SDSU driver resources when it is removed from the system
 *
 * Calls \a remove_vxworks_resource() to remove all driver resources.
 *
 * \sa sdsu_init(), remove_vxworks_resources(), pci_task_func(), time_task_func(), data_task_func()
 */
void sdsu_cleanup(void)
{
    remove_vxworks_resources(SDSU_TASK);
}

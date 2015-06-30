/**
 * \file sdsu_driver.h
 *
 * \brief SDSU controller driver module
 *
 * \author Stewart McLay, UKATC (sam@roe.ac.uk)
 * \author Chris Tierney, UKATC (cjt@roe.ac.uk)
 *
 * \version 1.0
 *
 * \date Thu Mar 21 15:06:37 GMT 2002
 *
 * The SDSU driver module runs in an RTLinux environment. It receives commands
 * from user space processes on the RTL FIFOs and sends replies on alternative
 * RTL FIFOs. These commands are directed towards the PCI card DSP or the SDSU
 * controller DSP each executing code which processes the commands and sends
 * replies of their own. All commands/replies for the SDSU controller must be
 * sent through the PCI card.
 *
 * As well as processing commands and replies the SDSU driver receives empty
 * data buffers from the user space processes on RTL FIFOs. These data buffers
 * are filled with data by the PCI card from the SDSU controller. Once full the
 * data buffer is sent back to the user space process on the RTL FIFO.
 *
 * The driver contains three RTL threads which handle specific types of commands
 * /replies. A summary of the threads is given below...
 *
 * -# PCI thread: sends commands from a USER process (or another driver thread)
 * to the PCI card and returns replies
 * -# TIME thread: sends commands from a USER process (or another driver thread)
 * to the SDSU controller and returns replies
 * -# DATA thread: sends empty buffers from a USER process to the PCI card for
 * writing data and returns full buffers
 *
 * The list of the FIFOs available to USER space programs for communicating to
 * the RTL threads are...
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
#ifndef __SDSU_DRIVER_H__
#define __SDSU_DRIVER_H__

#define RTAI_CPU            1      /**< CPU running RTAI tasks(1=CPU0,2=CPU1) */
#define SDSU_VENDOR_ID      0x1057 /**< SDSU PCI card vendor ID */
#define SDSU_DEVICE_ID      0x1801 /**< SDSU PCI card device ID */
#define SDSU_PCI_MEMSIZE    16384  /**< SDSU PCI card IO memory size */

#ifndef SDSU_MAJOR
#define SDSU_MAJOR          0      /**< Default SDSU major device number */
#endif


/** IRQs to be allocated to the non-RT CPU */
#define IRQs { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 15, 17, 18, 19, 20, 21, 22, 23, 24, 25 ,26, 27, 28, 29, 30, 31 }

#define SDSU_STACK_SIZE     2000

#define SDSU_DATA_PRIORITY  0
#define SDSU_PCI_PRIORITY   1
#define SDSU_TIME_PRIORITY  2

#define SDSU_TIME_CMD_MEM   "timecmd"
#define SDSU_TIME_REPLY_MEM "timereply"

typedef unsigned long u32;
/* Initialises the VxWorks SDSU driver */
extern SDSU_CONTEXT *sdsu_init(int device);
/* Cleans up VxWorks SDSU driver resources when it is removed from the system */
extern void sdsu_cleanup(void);

/** Internal PCI thread states */
typedef enum {
   GO_TO_SLEEP = 1,
   SEND_CMD_FROM_DATA,
   SEND_CMD_FROM_TIME,
   SEND_CMD_FROM_USER
} SDSU_ACTION;

typedef enum {
   SDSU_DATA = 1,
   SDSU_IRQ,
   SDSU_TASK,
   SDSU_SEMAPHORE,
   SDSU_FIFO,
   SDSU_DEVICE,
   SDSU_KERNEL_MEM,
   SDSU_BIGPHYS_MEM,
   SDSU_SHARED_MEM
} SDSU_RESOURCE_TYPE;

/**
 * PCI memory address space register locations for the HTXR/HRXS mapping, two
 * single read and write registers attached to the FIFOs
 */
typedef struct {
    volatile u32 unused[4];        /**< Filler bytes */
    volatile u32 hctr;		   /**< Host control reg (base+$010) */
    volatile u32 hstr;		   /**< Host status reg (base+$014) */
    volatile u32 hcvr;		   /**< Host command vector reg (base+$018) */
    volatile u32 htxr_hrxs[16377]; /**< Host trasmit data, host slave receive */
                                   /**< reg(base+01c-$fffc) */
} PCI_REG;

#define SDSU_HCTR      0x0010
#define SDSU_HSTR      0x0014
#define SDSU_HCVR      0x0018
#define SDSU_HTXR_HRXS 0x001C

#define	NFY		0x004e4659
#define	DA		0x00004441
#define	RP		0x00005250
#define	REP		0x00524550

/** Host Received Data Request (HRXS register contains data) */
#define HSTR_HRRQ_MASK 0x4
/** Host Transmit Data Request (HTXR register is not full) */
#define HSTR_HTRQ_MASK 0x2
/** Transmitter Ready (HTXR register is empty) */
#define HSTR_TRDY_MASK 0x1
/** Host Command Vector Register interrupt reset value */
#define HCVR_INT_RST   0x8073
/** Host Command Vector Register interrupt finished flag */
#define HCVR_INT_DON   0x8075
/* Host Command Vector Register byte swap enable command */
#define HCVR_BYTE_SWAP_ENABLE 0x8089
/* Host Command Vector Register byte swap disable command */
#define HCVR_BYTE_SWAP_DISABLE 0x808B

/** PCI card command structure */
typedef struct {
    u32 cmd;		/* MODIFICADO char cmd[3]; */
    int arg1;
    int arg2;
    int arg3;
    u32 hcvr_vector;
} SDSU_PCI_CMD_DEF;

/** SDSU controller command structure */
typedef struct {
    char cmd[3];
    int arg1;
    int arg2;
} SDSU_TIME_CMD_DEF;

#endif /* __SDSU_DRIVER_H__ */

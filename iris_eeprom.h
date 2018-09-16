/*	$NetBSD: smc93cx6var.h,v 1.6.2.1 2005/11/10 14:04:15 skrll Exp $	*/
/*
 * Interface to the 93C46 serial EEPROM that is used to store BIOS
 * settings for the aic7xxx based adaptec SCSI controllers.  It can
 * also be used for 93C26 and 93C06 serial EEPROMS.
 *
 * Copyright (c) 1994, 1995 Justin T. Gibbs.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer,
 *    without modification.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * the GNU Public License ("GPL").
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: src/sys/dev/aic7xxx/aic7xxx.c,v 1.40 2000/01/07 23:08:17 gibbs Exp $
 */

/*
 * Most of the following was adapted from sys/dev/ic/smc93cx6var.h
 */

#include <mips/cpuregs.h>
#include "iris_machdep.h"

typedef enum {
	C46 = 6,
	C56_66 = 8
} seeprom_chip_t;

struct seeprom_descriptor {
	seeprom_chip_t sd_chip;
	u_int32_t sd_MS;
	u_int32_t sd_RDY;
	u_int32_t sd_CS;
	u_int32_t sd_CK;
	u_int32_t sd_DO;
	u_int32_t sd_DI;
};

/*
 * This function will read count 16-bit words from the serial EEPROM and
 * return their value in buf.  The port address of the aic7xxx serial EEPROM
 * control register is passed in as offset.  The following parameters are
 * also passed in:
 *
 *   CS  - Chip select
 *   CK  - Clock
 *   DO  - Data out
 *   DI  - Data in
 *   RDY - SEEPROM ready
 *   MS  - Memory port mode select
 *
 *  A failed read attempt returns 0, and a successful read returns 1.
 */

static inline u_int8_t
SEEPROM_INB(void)
{
	DELAY(4);
	return *(volatile u_int8_t *)(void *)MIPS_PHYS_TO_KSEG1(HPC_EEPROM_DATA);
}

static inline void
SEEPROM_OUTB(u_int32_t value)
{
	DELAY(4);
	*(volatile u_int8_t *)(void *)MIPS_PHYS_TO_KSEG1(HPC_EEPROM_DATA) = (u_int8_t) (value);
}

static inline u_int8_t
SEEPROM_STATUS_INB(void)
{
	DELAY(4);
	return *(volatile u_int8_t *)(void *)MIPS_PHYS_TO_KSEG1(HPC_EEPROM_DATA);
}

static inline u_int8_t
SEEPROM_DATA_INB(void)
{
	DELAY(4);
	return *(volatile u_int8_t *)(void *)MIPS_PHYS_TO_KSEG1(HPC_EEPROM_DATA);
}
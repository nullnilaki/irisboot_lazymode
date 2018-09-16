/*
 * Copyright (c) 2018 Naruaki Etomi
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Silicon Graphics "IRIS" series MIPS processors machine bootloader.
 */

#include <sys/param.h>
#include <lib/libsa/stand.h>

/* iris_boot.c */
void again(void);
void reboot(void);

/* iris_parse.c */
void parse(char **, char *);
void partitionparse(char *);

/* iris_autoconf.c */
void find_devs(void);

/* iris_start.S */
void romrestart(void);

 /* iris_cons.c */
char *cninit(int *, int *);
int   cngetc(void);
void  cnputc(int);
int   cnscan(void);

struct wd33c93_softc;
/* iris_scsi.c */
void wd33c93_init(void *, void*);
int wd33c93_go(struct wd33c93_softc *, uint8_t *, size_t, uint8_t *, size_t *);

/* iris_scsictl.c */
int scsi_test_unit_rdy(void);
int scsi_readx25(u_char *, size_t);
int scsi_readx28(u_char *, size_t, daddr_t, size_t);
int scsi_writex2A(u_char *, size_t, daddr_t, size_t);

/*
 * Traditional delay(9) implementation derived from
 * 4.4BSD/mips (also used in pmax and news3400).
 * (see sys/arch/mips/mips/mips_mcclock.c etc.)
 */

#define INDIGO_R3K_MODE

#ifdef INDIGO_R3K_MODE
 /* iris_eeprom.c */
int read_eeprom(u_int16_t *, size_t, size_t);

#define ZS_ADDR 0x1fb80d10
#define SCSIA_ADDR 0x1FB80122
#define SCSID_ADDR 0x1FB80126
#define HPC_EEPROM_DATA 0x1FB801BF
#define HPC_EEPROM_BOOTFILE 52 
#define BOOTFILE_LEN 50

/* Target is Personal IRIS R3000 36Mhz. */
#define DELAY(n)							\
do {							\
        register int __N = 21 * n;							\
        do {							\
                __asm("addiu %0,%1,-1" : "=r" (__N) : "0" (__N));							\
        } while (__N > 0);							\
} while (/* CONSTCOND */ 0)
#endif

#ifdef INDIGO_R4K_MODE
#define ZS_ADDR 0x1fb80d10
#define SCSIA_ADDR 0x1FB80122
#define SCSID_ADDR 0x1FB80126
#define HPC_EEPROM_DATA 0x1FB801BF
#define HPC_EEPROM_BOOTFILE 136 
#define BOOTFILE_LEN 48

/* Target is IRIS Indigo R4000 100Mhz. */
#define DELAY(n)							\
do {							\
        register int __N = 50 * n;							\
        do {							\
                __asm("addiu %0,%1,-1" : "=r" (__N) : "0" (__N));							\
        } while (__N > 0);							\
} while (/* CONSTCOND */ 0)
#endif

#ifdef INDY_MODE
#define ZS_ADDR 0x1fbd9830
#define SCSIA_ADDR 0x1FBC0003
#define SCSID_ADDR 0x1FBC0007

/* Target is Indy 180Mhz. */
#define DELAY(n)							\
do {							\
        register int __N = 90 * n;							\
        do {							\
                __asm("addiu %0,%1,-1" : "=r" (__N) : "0" (__N));							\
        } while (__N > 0);							\
} while (/* CONSTCOND */ 0)
#endif

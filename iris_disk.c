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
 * Disk I/O API routine.
 * Most of the following was adapted from /sys/arch/hp300/stand/common/sd.c.
 */

#include <lib/libsa/stand.h>

#include <sys/param.h>
#include <sys/disklabel.h>

#ifndef	INDIGO_R3K_MODE
#include <dev/arcbios/arcbios.h>
#endif

#include "disk.h"

#include "iris_machdep.h"
#include "iris_scsivar.h"

struct	disk_softc {
	int		sc_part;			/* disk partition number */
	char	sc_retry;
	char	sc_alive;
	short	sc_blkshift;
	struct	disklabel sc_label;	/* disk label for this disk */

};

static int diskinit(struct disk_softc *);

#define	DISKRETRY  2

int
diskstrategy(void *devdata, int rw, daddr_t bn, size_t reqcnt, void *addr, size_t *cnt)
{
	struct disk_softc *sc;
	struct disklabel *lp;
	uint8_t *buf;
	daddr_t blk;
	u_int nblk;
	int stat;

	sc = devdata;

	buf = addr;
	lp = &sc->sc_label;
	blk = bn + (lp->d_partitions[sc->sc_part].p_offset >> sc->sc_blkshift);
	nblk = reqcnt >> sc->sc_blkshift;

	sc->sc_retry = 0;
	
retry:
	stat = scsi_readx28(buf, reqcnt, blk, nblk);

	if (stat) {
		DELAY(1000000);
		if (++sc->sc_retry > DISKRETRY) {
			return EIO;
		}
		printf("diskstrategy retry\n");
		goto retry;
	}

	*cnt = reqcnt;

	return 0;
}

static int
diskinit(struct disk_softc *sc)
{
	u_char capbuf[2];

	u_char stat;

	stat = scsi_test_unit_rdy();

	if (stat) {
		/* drive may be doing RTZ - wait a bit */
		if (stat == STS_CHECKCOND) {
			DELAY(1000000);
			stat = scsi_test_unit_rdy();
		}

		if (stat) {
			printf("diskinit abort!\n");
			printf("Boot failed!  Halting...\n");
			reboot();
		}
	}

	/*
	 * try to get the drive block size.
	 */
	capbuf[0] = 0;
	capbuf[1] = 0;

	stat = scsi_readx25((u_char *)capbuf, sizeof(capbuf));

	if (stat == 0) {
		if (capbuf[1] > DEV_BSIZE)
			for (; capbuf[1] > DEV_BSIZE; capbuf[1] >>= 1)
				++sc->sc_blkshift;
	}

	sc->sc_alive = 1;
	return 1;
}

int
diskopen(struct open_file *f, ...)
{
	struct disk_softc *sc;
	struct disklabel *lp;
	size_t cnt;
	char *msg, buf[DEV_BSIZE];
	int error;

	cnt = 0;

	sc = alloc(sizeof(struct disk_softc));
	memset(sc, 0, sizeof(struct disk_softc));
	f->f_devdata = (void *)sc;

	sc->sc_part = scsi_part;

	if (sc->sc_alive == 0) {
		if (diskinit(sc) == 0)
			return ENXIO;
	}

	/* try to read disk label and partition table information */
	lp = &sc->sc_label;
	lp->d_secsize = DEV_BSIZE;
	lp->d_secpercyl = 1;
	lp->d_npartitions = MAXPARTITIONS;
	lp->d_partitions[scsi_part].p_offset = 0;
	lp->d_partitions[scsi_part].p_size = 0x7fffffff;

	error = diskstrategy(sc, F_READ, (daddr_t)LABELSECTOR, DEV_BSIZE, buf, &cnt);

	if (error || cnt != DEV_BSIZE) {
		printf("diskstrategy error...\n");
		dealloc(sc, sizeof(struct disk_softc));
		return ENXIO;
	}

	msg = getdisklabel(buf, lp);

	if (msg) {
		/* If no label, just assume 0 and return */
		printf("No disklabel...\n");
		return 0;
	}

	return 0;
}

int
diskclose(struct open_file *f)
{
	struct disk_softc *sc = f->f_devdata;

	dealloc(sc, sizeof *sc);
	f->f_devdata = NULL;

	return 0;
}
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
 * Most of the following was adapted from /sys/dev/scsipi/scsi_message.h.
 */

/* Messages (1 byte) */		     /* I/T (M)andatory or (O)ptional */
#define MSG_CMDCOMPLETE		0x00 /* M/M */
#define MSG_TASK_COMPLETE 	0x00 /* M/M */ /* SPI3 Terminology */
#define MSG_EXTENDED		0x01 /* O/O */
#define MSG_SAVEDATAPOINTER	0x02 /* O/O */
#define MSG_RESTOREPOINTERS	0x03 /* O/O */
#define MSG_DISCONNECT		0x04 /* O/O */
#define MSG_INITIATOR_DET_ERR	0x05 /* M/M */
#define MSG_ABORT		0x06 /* O/M */
#define MSG_ABORT_TASK_SET	0x06 /* O/M */ /* SPI3 Terminology */
#define MSG_MESSAGE_REJECT	0x07 /* M/M */
#define MSG_NOOP		0x08 /* M/M */
#define MSG_PARITY_ERROR	0x09 /* M/M */
#define MSG_LINK_CMD_COMPLETE	0x0a /* O/O */
#define MSG_LINK_CMD_COMPLETEF	0x0b /* O/O */
#define MSG_BUS_DEV_RESET	0x0c /* O/M */
#define MSG_TARGET_RESET	0x0c /* O/M */ /* SPI3 Terminology */
#define MSG_ABORT_TAG		0x0d /* O/O */
#define MSG_ABORT_TASK		0x0d /* O/O */ /* SPI3 Terminology */
#define MSG_CLEAR_QUEUE		0x0e /* O/O */
#define MSG_CLEAR_TASK_SET	0x0e /* O/O */ /* SPI3 Terminology */
#define MSG_INIT_RECOVERY	0x0f /* O/O */ /* Deprecated in SPI3 */
#define MSG_REL_RECOVERY	0x10 /* O/O */ /* Deprecated in SPI3 */
#define MSG_TERM_IO_PROC	0x11 /* O/O */ /* Deprecated in SPI3 */
#define MSG_QAS_REQUEST		0x55 /* O/O */ /* SPI3 */

/* Messages (2 byte) */
#define MSG_SIMPLE_Q_TAG	0x20 /* O/O */
#define MSG_SIMPLE_TASK		0x20 /* O/O */ /* SPI3 Terminology */
#define MSG_HEAD_OF_Q_TAG	0x21 /* O/O */
#define MSG_HEAD_OF_QUEUE_TASK	0x21 /* O/O */ /* SPI3 Terminology */
#define MSG_ORDERED_Q_TAG	0x22 /* O/O */
#define MSG_ORDERED_TASK	0x22 /* O/O */ /* SPI3 Terminology */
#define MSG_IGN_WIDE_RESIDUE	0x23 /* O/O */
#define MSG_ACA_TASK		0x24 /* 0/0 */ /* SPI3 */

/* Identify message */		     /* M/M */
#define MSG_IDENTIFYFLAG	0x80
#define MSG_IDENTIFY_DISCFLAG	0x40
#define MSG_IDENTIFY(lun, disc)	\
	(MSG_IDENTIFYFLAG | ((disc) ? MSG_IDENTIFY_DISCFLAG : 0) | (lun))
#define MSG_ISIDENTIFY(m)	((m) & MSG_IDENTIFYFLAG)
#define MSG_IDENTIFY_LUNMASK	0x3f

/* Extended messages (opcode and length) */
#define MSG_EXT_SDTR		0x01
#define MSG_EXT_SDTR_LEN	0x03

#define MSG_EXT_WDTR		0x03
#define MSG_EXT_WDTR_LEN	0x02
#define MSG_EXT_WDTR_BUS_8_BIT	0x00
#define MSG_EXT_WDTR_BUS_16_BIT	0x01
#define MSG_EXT_WDTR_BUS_32_BIT	0x02 /* Deprecated in SPI3 */

#define MSG_ISEXTENDED(m)	((m) == MSG_EXTENDED)

/* message length */
#define MSG_IS1BYTE(m)							\
	((!MSG_ISEXTENDED(m) && (m) < 0x20) ||				\
	    (m) == MSG_QAS_REQUEST || MSG_ISIDENTIFY(m))
#define MSG_IS2BYTE(m)		(((m) & 0xf0) == 0x20)
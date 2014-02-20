/*
 * Author - Julian Martinez del Campo
 * Descr  - This file contains the operation codes, and register 
 *          definitions for the SVM virtual machine
 */

#ifndef _OPCODES_H_
#define _OPCODES_H_

/* opcodes for 80d201 processor */
#define HALT 0x41       /* shutdown machine */ /* 1 byte */
#define LOAD 0x70       /* reg1, immSrc     */ /* 4 byte */
#define LOADI 0x71      /* reg1, reg2       */ /* 2 byte */
#define STORE 0x72      /* reg1, immDest    */ /* 4 byte */
#define STOREI 0x73     /* reg1, reg2       */ /* 2 byte */
#define JMP 0x74        /* immDest          */ /* 4 byte */
#define JMPZ 0x75       /* immDest          */ /* 4 byte */
#define JMPN 0x76       /* immDest          */ /* 4 byte */
#define JMPO 0x77       /* ImmDest          */ /* 4 byte */
#define ADD 0x78        /* reg1, immSrc     */ /* 4 byte */
#define ADDR 0x79       /* reg1, reg2       */ /* 2 byte */
#define SUB 0x7a        /* reg1, immSrc     */ /* 4 byte */
#define SUBR 0x7b       /* reg1, reg2       */ /* 2 byte */
#define OUT 0x7c        /* immSrc           */ /* 4 byte */
#define OUTC 0x7d       /* immSrc           */ /* 4 byte */
#define OUTR 0x7e       /* reg1             */ /* 2 byte */
#define OUTRC 0x7f      /* reg1             */ /* 2 byte */
#define OUTI 0x80       /* reg1             */ /* 2 byte */
#define OUTIC 0x81      /* reg1             */ /* 2 byte */

/* DATA is not an instruction */

/* Registers for 80d201 processor */
#define A1 0
#define A2 2
#define R1 1
#define R2 3

/* Register checks */
#define IS_REG(a) (a <= 3)
#define IS_DREG(a) (a == 1 || a == 3)
#define IS_AREG(a) (a == 0 || a == 2)

#endif

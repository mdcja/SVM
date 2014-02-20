/*
 * Author - Julian Martinez del Campo
 * Descr  - This file contains the implementation file for cpu.h
 */

#include "cpu.h"
#include "memory.h"
#include "opcodes.h"
#include <stdio.h>
#include <stdlib.h>

#define ON 1
#define OFF 0

#define MAX_INT 0x7fff
#define MIN_INT 0x8000

/***************************************************************
 *
 *  Data structure 
 *
 ***************************************************************/

static struct Flags { 
    int z;                  /* Zero */ 
    int n;                  /* Negative */
    int o;                  /* Overflow */ 
 } flags; 
/*
 * Desc - Processor flags
 *  The results of a program instruction are stored here
 *  If z is 1 then the result of the instruction is zero; else 0
 *  If n is 1 then the result of instruction was negative; else 0
 *  If o is 1 then the result of the instruction resulted in overflow; else 0
 */

static struct Data_Registers {
    short int r1;                 /* Register 1 */
    short int r2;                 /* Register 2 */
} data_reg;
/*
 * Desc - Data registers
 */

static struct Address_Registers {
    short int a1;                 /* Register 1 */
    short int a2;                 /* Register 2 */
} adr_reg;
/*
 * Desc - Address registers
 */

static int pc;                    /* program counter */


/***************************************************************
 *
 * Private functions
 *
 **************************************************************/

static void set_pc( int byte_offset ) {
    pc = byte_offset;
}

static void incr_pc( int n ) {
    pc += n;        /* increase PC by n */
}

static short int get_data_reg1( void ) {

    #ifdef DEBUG_REG
        /* print register contents */
        printf( "\tget_data_reg1 : data_reg.r1 = %d\n", data_reg.r1 );
    #endif

    return data_reg.r1; 
}

static short int get_data_reg2( void ) {

    #ifdef DEBUG_REG
        /* print register contents */
        printf( "\tget_data_reg2 : data_reg.r2 = %d\n", data_reg.r2 );
    #endif

    return data_reg.r2;
}

static void set_data_reg1( short int data ) {
    data_reg.r1 = data;

    #ifdef DEBUG_REG
        /* print incoming data */
        printf( "\tset_data_reg1 : data = %d\n", data );

        /* print register contents */
        printf( "\tset_data_reg1 : data_reg.r1 = %d\n", data_reg.r1 );
    #endif
}

static void set_data_reg2( short int data ) {
    data_reg.r2 = data;

    #ifdef DEBUG_REG
        /* print incoming data */
        printf( "\tset_data_reg2 : data = %d\n", data );

        /* print register contents */
        printf( "\tset_data_reg2 : data_reg.r2 = %d\n", data_reg.r2 );
    #endif
}

static short int get_address_reg1( void ) {

    #ifdef DEBUG_REG
        /* print register contents */
        printf( "\tget_address_reg1 : adr_reg.a1 = %d\n", adr_reg.a1 );
    #endif

    return adr_reg.a1;  
}

static short int get_address_reg2( void ) {

    #ifdef DEBUG_REG
        /* print register contents */
        printf( "\tget_address_reg2 : adr_reg.a2 = %d\n", adr_reg.a2 );
    #endif

    return adr_reg.a2;
}

static void set_address_reg1( short int data ) {
    adr_reg.a1 = data ;

    #ifdef DEBUG_REG
        /* print incoming data */
        printf( "\tset_address_reg1 : data = %d\n", data );

        /* print register contents */
        printf( "\tset_address_reg1 : adr_reg.a1 = %d\n", adr_reg.a1 );
    #endif
}

static void set_address_reg2( short int data ) {
    adr_reg.a2 = data;

    #ifdef DEBUG_REG
        /* print incoming data */
        printf( "\tset_address_reg2 : data = %d\n", data );

        /* print register contents */
        printf( "\tset_address_reg2 : adr_reg.a2 = %d\n", adr_reg.a2 );
    #endif
}

static void set_z_flag( int state ) {
    flags.z = state;
}

static void set_n_flag( int state ) {
    flags.n = state;
}

static void set_o_flag( int state ) {
    flags.o = state;
}

static int is_add_overflow( short int  n1, short int n2 ) {

    /* if both numbers are positive */
    if( n1 > 0 && n2 > 0 ) {
        /* check if overflow */
        if( n1 > MAX_INT - n2 ) {
            return 1;
        }
    }
    /* if both numbers are negative */
    else if( n1 < 0 && n2 < 0 ) {
        /* check if underflow */
        if( n1 < MIN_INT - n2 ) {
            return 1;
        }
    }
    
    /* no overflow */
    return 0;
}

static int is_sub_overflow( short int n1, short int n2 ) {
    /* if first number is positive, second number negative */
    if( n1 > 0 && n2 < 0 ) {
        /* check if overflow */
        if( n1 > MAX_INT - n2 ) {
            return 1;
        }
    }
    /* if first number is negative, second number is positive */
    else if( n1 < 0 && n2 > 0 ) {
        /* check for underflow */
        if( n1 < MIN_INT - n2 ) {
            return 1;
        }
    }

    /* no overflow */
    return 0;
}


/***************************************************************
 *
 * Public functions
 *
 ***************************************************************/

void initialize_cpu( void ) {
    pc = 0;
}

int get_pc( void ) {
    return pc;
}

int get_z_flag( void ) {
    return flags.z;
}

int get_n_flag( void ) {
    return flags.n;
}

int get_o_flag( void ) {
    return flags.o;
}

int cycle( void ) {
    int offset;             /* holds location in memory */
    Byte op;                /* holds the operation code */
    int reg1, reg2;         /* holds register values */
    short int imm;          /* holds the value of imm */
    short int temp1, temp2; /* holds temporary values for arithmetic */
    short int result;       /* holds temporary values for arithmetic */
   
    /* get current instruction in memory */
    offset = get_pc();

    /* fetch next instruction from memory */
    op = get_byte( offset );
    
    /* get instruction details */
    switch( op ){

        case HALT:
            /* shutdown machine */

            #if DEBUG_MEM
                /* print contents */
                printf( "HALT  \n" );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x -- -- ----\n", offset, op );
            #endif

            return CPU_STOP; 
            break;

        case LOAD:
            /* reg1, immSrc */
            /* load immSRC (consant val) into register (data/address) */
            /* 4 byte */

            /* read in registers */
            get_register( &reg1, &reg2, offset + 1 );
            
            /* get contents of imm */
            read_block( &imm, offset + 2 );
           
            #if DEBUG_MEM
                /* print contents */
                printf( "LOAD   %2d %4d\n", reg1, imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x -- %4x\n", offset, op, reg1, imm );
            #endif

            /* put contents of imm into reg */
            set_reg( imm, reg1 );
            
            /* increase program counter */
            incr_pc( 4 );
            
            break;

        case LOADI:
            /* reg1, reg2 */
            /* load reg2 (address) into reg1 (data/address) */
            /* 2 byte */
            
            /* read in registers */
            get_register( &reg1, &reg2, offset + 1 );

            #if DEBUG_MEM
                /* print contents */
                printf( "LOADI  %2d %4d\n", reg1, reg2 );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x %2x ----\n", offset, op, reg1, reg2 );
            #endif
            
            /* check if register is valid */
            if( !IS_AREG( reg2 ) ) {
                return INVALID_REG;
            }

            /* get address */
            temp1 = get_reg( reg2 );
            
            /* get contents at address */
            read_block( &temp2, temp1 );

            /* put contents of reg2 into reg1 */
            set_reg( temp2, reg1 );

            /* increase pc */
            incr_pc( 2 );

            break;

        case STORE:
            /* reg1, immDEST */
            /* store data in reg1 (data/address) into immDEST (address) */
            /* 4 byte */
            
            /* read in registers */
            get_register( &reg1, &reg2, offset + 1 );

            /* get imm */
            read_block( &imm, offset + 2 );

            #if DEBUG_MEM
                /* print contents */
                printf( "STORE  %2d %4d\n", reg1, imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x -- %4x\n", offset, op, reg1, imm );
            #endif

            /* store data in reg1 into imm */
            temp1 = get_reg( reg1 );
            write_block( temp1, imm );

            /* increase pc */
            incr_pc( 4 );

            break;

        case STOREI:
            /* reg1, reg2 */
            /* store data in reg1 (data/address) into reg2 (address) */
            /* 2 byte */
            
            /* read in registers */
            get_register( &reg1, &reg2, offset + 1 );

            #if DEBUG_MEM
                /* print contents */
                printf( "STOREI %2d %2d\n", reg1, reg2 );
            #endif
            
            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x %2x ----\n", offset, op, reg1, reg2 );
            #endif
            
            /* check if register is valid */
            if( !IS_AREG( reg2 ) ) {
                return INVALID_REG;
            }

            /* store data in reg1 into reg2 */
            temp1 = get_reg( reg1 );
            temp2 = get_reg( reg2 );
            
            write_block( temp1, temp2 );

            /* increase pc */
            incr_pc( 2 );

            break;

        case JMP:
            /* immDEST */
            /* jump to constant valued address immDEST (address ) */
            /* 4 byte */
            
            /* get contents of imm */
            read_block( &imm, offset + 2 );
            
            #if DEBUG_MEM
                /* print contents */
                printf( "JMP   %4d\n", imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x -- -- %4x\n", offset, op, imm );
            #endif

            /* jump to value at address imm */
            set_pc( imm );

            break;

        case JMPZ:
            /* immDEST */
            /* jump to constant valued address immDEST (address) if previous instruciton set z flag */
            /* 4 byte */
 
            /* get imm */
            read_block( &imm, offset + 2 );

            #if DEBUG_MEM
                /* print contents */
                printf( "JMPZ  %4d\n", imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x -- -- %4x\n", offset, op, imm );
            #endif

        
            /* check if flag was set */
            if( get_z_flag() == ON ) {
                /* clear flag */
                set_z_flag( OFF );
                /* change pc */
                set_pc( imm );
            } else {
                /* flag not set */
                /* change pc */
                incr_pc( 4 );
            }

            break;

        case JMPN:
            /* immDEST */
            /* jump to constant valued address immDEST (address) if previous instruciton set n flag */
            /* 4 byte */

            /* get imm */
            read_block( &imm, offset + 2 );
        
            #if DEBUG_MEM
                /* print contents */
                printf( "JMPN  %4d\n", imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x -- -- %4x\n", offset, op, imm );
            #endif

            /* check if flag was set */
            if( get_n_flag() == ON ) {

                /* clear flag */
                set_n_flag( OFF );

                /* change pc */
                set_pc( imm );
            } else {
                /* flag not set */
                /* increase pc */
                incr_pc( 4 );
            }

            break;

        case JMPO:
            /* immDEST */
            /* jump to constant valued address immDEST (address) if previous instruciton set o flag */
            /* 4 byte */

            /* get imm */
            read_block( &imm, offset + 2 );

            #if DEBUG_MEM
                /* print contents */
                printf( "JMPO  %4d\n", imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x -- -- %4x\n", offset, op, imm );
            #endif

            /* check if flag was set */
            if( get_o_flag() == ON ) {

                /* clear flag */
                set_o_flag( OFF );
                /* change pc */
                set_pc( imm );
            } else {
                /* flag not set */
                /* increase pc */
                incr_pc( 4 );
            }

            break;

        case ADD:
            /* reg1, immSRC */
            /* add constant value immSRC to register reg1 (data) and store in reg1 (data) */
            /* set flags */
            /* 4 byte */
            
            /* get register */
            get_register( &reg1, &reg2, offset + 1 );

            /* get imm */
            read_block( &imm, offset + 2 );

            #if DEBUG_MEM
                /* print contents */
                printf( "ADD    %2d %4d\n", reg1, imm );
            #endif
            
            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x -- %2x\n", offset, op, reg1, imm );
            #endif
    
            /* check if registers are valid */
            if( !IS_DREG( reg1 ) ) {
                return INVALID_REG;
            }

            /* get contents of reg1 */
            temp1 = get_reg( reg1 );

            /* add */
            result = temp1 + imm;

            /* put contents back */
            set_reg( result, reg1 );

            /* check for overflow/underflow */
            if( is_add_overflow( temp1, imm ) == 1 ) {
                set_o_flag( ON );
            }
            else {
                /* set flags */
                if( result == 0 ) {
                    /* Z flag */
                    set_z_flag( ON );
                }
                
                if( result < 0 ) {
                    /* N flag */
                    set_n_flag( ON );
                }
            }
            
            #if DEBUG_FLAGS
                /* print flag state */
                printf( "\tN flag = %d\n", get_n_flag() );
                printf( "\tZ flag = %d\n", get_z_flag() );
                printf( "\tO flag = %d\n", get_o_flag() );
            #endif

            /* increase pc */
            incr_pc( 4 );

            break;

        case ADDR:
            /* reg1, reg2 */
            /* add value in reg2 (data) to reg1 (data) and store in reg1 (data) */
            /* note reg1 and reg2 cannot be the same register */
            /* set flags */
            /* 2 byte */
    
            /* get registers */
            get_register( &reg1, &reg2, offset + 1 );

            #if DEBUG_MEM
                /* print contents */
                printf( "ADDR   %2d %4d\n", reg1, reg2 );
            #endif
            
            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x %2x ----\n", offset, op, reg1, reg2 );
            #endif
            
            /* check if registers are valid */
            if( !( IS_DREG( reg1 ) && IS_DREG( reg2 ) ) ) {
                return INVALID_REG;
            }

            /* check if registers are the same */
            if( reg1 == reg2 ) {
                return INVALID_REG;
            }

            /* add */
            temp1 = get_reg( reg1 );
            temp2 = get_reg( reg2 );
            
            result = temp1 + temp2;

            /* set register */
            set_reg( result, reg1 );

            /* check for overflow/underflow */
            if( is_add_overflow( temp1, temp2 ) == 1 ) {
                set_o_flag( ON );
            }
            else {
                /* set flags */
                if( result == 0 ) {
                    /* Z flag */
                    set_z_flag( ON );
                }
                
                if( result < 0 ) {
                    /* N flag */
                    set_n_flag( ON );
                } 
            }

            #if DEBUG_FLAGS
                /* print flag state */
                printf( "\tN flag = %d\n", get_n_flag() );
                printf( "\tZ flag = %d\n", get_z_flag() );
                printf( "\tO flag = %d\n", get_o_flag() );
            #endif

            /* increase pc */
            incr_pc( 2 );

            break;

        case SUB:
            /* reg1, immSRC */
            /* subtract constant value immSRC from value in reg1 (data) and store in reg1 (data) */
            /* set flags */
            /* 4 byte */
            
            /* get registers */
            get_register( &reg1, &reg2, offset + 1 );

            read_block( &imm, offset + 2 );

            #if DEBUG_MEM
                /* print contents */
                printf( "SUB   %2d %4d\n", reg1, imm );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x -- %2x\n", offset, op, reg1, imm );
            #endif

            /* check if registers are valid */
            if( !IS_DREG( reg1 ) ) {
                return INVALID_REG;
            }

            temp1 = get_reg( reg1 );

            result = temp1 - imm;

            /* set register */
            set_reg( result, reg1 );

            /* check for overflow/underflow */
            if( is_sub_overflow( temp1, imm ) == 1 ) {
                set_o_flag( ON );
            }
            else {
                /* check for other flags */

                /* set flags */
                if( result == 0 ) {
                    /* Z flag */
                    set_z_flag( ON );
                }

                if( result < 0 ) {
                    /* N flag */
                    set_n_flag( ON );
                }
            }

            #if DEBUG_FLAGS
                /* print flag state */
                printf( "\tN flag = %d\n", get_n_flag() );
                printf( "\tZ flag = %d\n", get_z_flag() );
                printf( "\tO flag = %d\n", get_o_flag() );
            #endif

            /* increase pc */
            incr_pc( 4 );

            break;

        case SUBR:
            /* reg1, reg2 */
            /* subtract register reg2 (data) from reg1 (data) and store in reg1 (data) */
            /* set flags */
            /* 2 byte */
            
            get_register( &reg1, &reg2, offset + 1 );
            
            #if DEBUG_MEM
                /* print contents */
                printf( "SUBR   %2d %2d\n", reg1, reg2 );
            #endif

            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x %2x %2x ----\n", offset, op, reg1, reg2 );
            #endif
            
            /* check if registers are valid */
            if( !( IS_DREG( reg1 ) && IS_DREG( reg2 ) ) ) {
                return INVALID_REG;
            }

            /* check if registers are the same */
            if( reg1 == reg2 ) {
                return INVALID_REG;
            }

            temp1 = get_reg( reg1 );
            temp2 = get_reg( reg2 );

            result = temp1 - temp2;

            /* store result */
            set_reg( result, reg1 );

            /* check for overflow/underflow */
            if( is_sub_overflow( temp1, temp2 ) == 1 ) {
                set_o_flag( ON );
            }
            else {
                /* set flags */
                if( result == 0 ) {
                    /* Z flag */
                    set_z_flag( ON );
                }

                if( result < 0 ) {
                    /* N flag */
                    set_n_flag( ON );
                } 
            }

            #if DEBUG_FLAGS
                /* print flag state */
                printf( "\tN flag = %d\n", get_n_flag() );
                printf( "\tZ flag = %d\n", get_z_flag() );
                printf( "\tO flag = %d\n", get_o_flag() );
            #endif

            /* increase pc */
            incr_pc( 2 );

            break;

        case OUT:
            /* immSRC */
            /* output constant value immSRC to screen as 16bit signed int */
            /* Note: print int */
            /* 4 byte */

            /* get contents at address */
            read_block( &imm, offset + 2 );
            
            #if DEBUG_MEM
                /* print contents */
                printf( "OUT   %4d\n", imm );
            #endif
    
            #if DEBUG_INST
                /* print instruction */
                printf( "%4d : %2x -- -- %4x\n", offset, op, imm );
            #endif

            #if !(defined DEBUG_INST || defined DEBUG_MEM )
                /* print contents */
                printf( "%d", (signed int)imm );
            #elif defined DEBUG_REG
                printf( "\t\tOUT = %d\n", (signed int)imm );
            #endif
            
            /* increase pc */
            incr_pc( 4 );

            break;

        case OUTC:
            /* immSRC */
            /* output constant value immSRC to screen as 8-bit unsigned character */
            /* note: print ascii character */
            /* 4 byte */

            /* get contents at address */
            read_block( &imm, offset + 2 );

            #if DEBUG_MEM
                /* print contents */
                printf( "OUTC  %4d\n", imm );
            #endif

            #if DEBUG_INST
                /* print intruction */
                printf( "%4d : %2x -- -- %4x\n", offset, op, imm );
            #endif

            #if !(defined DEBUG_INST || defined DEBUG_MEM )
                /* print contents */
                printf( "%c", (unsigned char)imm );
            #elif defined DEBUG_REG
                printf( "\t\tOUT = %c\n", (unsigned char)imm );
            #endif

            /* increase pc */
            incr_pc( 4 );

            break;

        case OUTR:
            /* reg1 */
            /* output value in reg1 (data) as 16 bit signed int */
            /* note: print int */
            /* 2 byte */
            
            /* get contents of register */
            get_register( &reg1, &reg2, offset + 1 );

            #if DEBUG_MEM
                /* print contents */
                printf( "OUTR   %2d\n", reg1 );
            #endif
            
            #if DEBUG_INST
                /* print intruction */
                printf( "%4d : %2x %2x -- ----\n", offset, op, reg1 );
            #endif
            
            /* check if register is valid */
            if( !IS_DREG( reg1 ) ) {
                return INVALID_REG;
            }

            /* get contents of register */
            imm = get_reg( reg1 );
            
            #if !(defined DEBUG_INST || defined DEBUG_MEM )
                /* print contents */
                printf( "%d", (signed int)imm );
            #elif defined DEBUG_REG
                printf( "\t\tOUT = %d\n", (signed int)imm );
            #endif

            /* increase pc */
            incr_pc( 2 );

            break;

        case OUTRC:
            /* reg1 */
            /* output value in reg1 (data) as 8 bit unsigned character */
            /* note: print ascii character */
            /* 2 byte */

            /* get conents of register */
            get_register( &reg1, &reg2, offset + 1 );

            #if DEBUG_MEM
                /* print contents */
                printf( "OUTRC  %2d\n", reg1 );
            #endif
            
            #if DEBUG_INST
                /* print intruction */
                printf( "%4d : %2x %2x -- ----\n", offset, op, reg1 );
            #endif
            
            /* check if register is valid */
            if( !IS_DREG( reg1 ) ) {
                return INVALID_REG;
            }

            /* get contents in reg1 */
            imm = get_reg( reg1 );

            #if !(defined DEBUG_INST || defined DEBUG_MEM )
                /* print contents */
                printf( "%c", (unsigned char)imm );
            #elif defined DEBUG_REG
                printf( "\t\tOUT = %c\n", (unsigned char)imm );
            #endif

            /* increase pc */
            incr_pc( 2 );

            break;

        case OUTI:
            /* reg1 */
            /* output value referenced by reg1 (address) as 16-bit signed int */
            /* note: print int */
            /* 2 byte */

            /* get register */
            get_register( &reg1, &reg2, offset + 1 );

            #if DEBUG_MEM
                /* print contents */
                printf( "OUTI   %2d\n", reg1 );
            #endif
            
            #if DEBUG_INST
                /* print intruction */
                printf( "%4d : %2x %2x -- ----\n", offset, op, reg1 );
            #endif
            
            /* check if register is valid */
            if( !IS_AREG( reg1 ) ) {
                return INVALID_REG;
            }

            /* get address from register */
            temp1 = get_reg( reg1 );

            /* read contents of address */
            read_block( &temp2, temp1 );

            #if !(defined DEBUG_INST || defined DEBUG_MEM )
                /* print contents */
                printf( "%d", (signed int)temp2 );
            #elif defined DEBUG_REG
                printf( "\t\tOUT = %d\n", (signed int)temp2 );
            #endif

            /* increase pc */
            incr_pc( 2 );

            break;

        case OUTIC:
            /* reg1 */
            /* output value referenced by reg1 (address) as 8-bit unsigned int */
            /* note: print ascii char */
            /* 2 byte */

            /* get register */
            get_register( &reg1, &reg2, offset + 1 );
    
            #if DEBUG_MEM
                /* print contents */
                printf( "OUTIC %2d\n", reg1 );
            #endif
            
            #if DEBUG_INST
                /* print intruction */
                printf( "%4d : %2x %2x -- ----\n", offset, op, reg1 );
            #endif
            
            /* check if register is valid */
            if( !IS_AREG( reg1 ) ) {
                return INVALID_REG;
            }

            /* get address */
            imm = get_reg( reg1 );

            /* get contents at address */
            read_block( &temp1, imm );

            #if !(defined DEBUG_INST || defined DEBUG_MEM )
                /* print contents */
                printf( "%d", (unsigned int)temp1 );
            #elif defined DEBUG_REG
                printf( "\t\tOUT = %d\n", (unsigned int)temp1 );
            #endif

            /* increase pc */
            incr_pc( 2 );

            break;

        default:
            /* invalid instruction */
            return INVALID_INSTR;
            break;
    }

    /* cycle completed succesfully */
    return CPU_SUCCESS;
}

void set_reg( short int data, int reg ) {
    
    #if DEBUG_REG
        /* print register and data */
        printf( "\tset_reg : reg  = %d\n", reg );
        printf( "\tset_reg : data = %d\n", data );
    #endif

    /* select register */
    switch( reg ) {
        case A1:
            set_address_reg1( data );
            break;
        case A2:
            set_address_reg2( data );
            break;
        case R1:
            set_data_reg1( data );
            break;
        case R2:
            set_data_reg2( data );
            break;
    }
}

short int get_reg( int reg ) {
    short int data;
    
    /* select register */
    switch( reg ) {
        case A1:
            data = get_address_reg1(); 
            break;
        case A2:
            data = get_address_reg2();
            break;
        case R1:
            data = get_data_reg1();
            break;
        case R2:
            data = get_data_reg2();
            break;
    }

    #if DEBUG_REG
        /* print register */
        printf( "\tget_reg : reg  = %d\n", reg );
        printf( "\tget_reg : data = %d\n", data );
    #endif

    return data;
}

void load_file( char * filename ) {
    int s;          /* holds the state of cpu */
    
    /* read file into memory */
    read_file( filename );
    
    /* initialize the cpu */
    initialize_cpu();
    
    /* start cpu */
    for(;;) {
        /* run a cycle */
        s = cycle();
        
        if( s == CPU_STOP ){
            /* CPU halted normally */
            break;
        }
        else if( s == INVALID_INSTR ){
            /* invalid instruction detected */
            printf( "Invalid instruction!\n" );
            break;
        }
        else if( s == INVALID_REG ) {
            /* invalid register usage */
            printf( "Invalid register usage!\n" );
            break;
        }
    }
}

void load_stdin( void ) {
    int s;          /* holds the state of cpu */
    
    /* read file into memory */
    read_stdin();
    
    #if 0
        print_mem();
    #endif

    /* initialize the cpu */
    initialize_cpu();
    
    /* start cpu */
    for(;;) {
        /* run a cycle */
        s = cycle();
        
        if( s == CPU_STOP ){
            /* CPU halted normally */
            break;
        }
        else if( s == INVALID_INSTR ){
            /* invalid instruction detected */
            printf( "Invalid instruction!\n" );
            break;
        }
        else if( s == INVALID_REG ) {
            /* invalid register usage */
            printf( "Invalid register usage!\n" );
            break;
        }
    }

}


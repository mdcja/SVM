/*
 * Author - Julian Martinez del Campo
 * Descr  - This file contains the documentation and header file for cpu.h
 *          This provides cpu parts, and the main cpu, and loader
 */

#ifndef _CPU_H_
#define _CPU_H_

/* definitions */
#define CPU_SUCCESS 0
#define CPU_STOP 1
#define INVALID_INSTR 2
#define INVALID_REG 3

/***************************************************************
 *
 * Public functions
 *
 **************************************************************/

void initialize_cpu( void );
/*
 * Desc - Initialize cpu
 *  Params - None (modifies cpu structure)
 *  Pre    - None
 *  Post   - CPU is intialized to zero
 */

int get_pc( void );
/*
 * Desc - Get the program counter
 *  Params - None
 *  Pre    - None
 *  Post   - Return the PC 
 */

int get_z_flag( void );
/*
 * Desc - Get z flag
 *  Params - None
 *  Pre    - None
 *  Post   - Return the state of the z flag 
 */

int get_n_flag( void );
/*
 * Desc - Get n flag
 *  Params - None
 *  Pre    - None
 *  Post   - Return the state of n
 */

int get_o_flag( void );
/*
 * Desc - Get the o flag
 *  Params - None
 *  Pre    - None
 *  Post   - Return the state of o flag
 */

int cycle( void );
/*
 * Desc - Performs one cycle in the cpu
 *  Params - None
 *  Pre    - None
 *  Post   - One cycle is performed
 *           Return 0 if operation was successful
 *           Return 1 if halt instruction was read
 *           Return 2 if invalid operation was read
 *           Return 3 if invalid register was read
 */

void set_reg( short int data, int reg );
/*
 * Desc - Save data into the specified register
 *  Params - data to save to register, register number
 *  Pre    - Register is valid 
 *  Post   - reg is set to data
 */

short int get_reg( int reg );
/*
 * Desc - Get data from specified register
 *  Params - register to get data from
 *  Pre    - register is valid
 *  Post   - The contents of reg is returned
 */

void load_file( char * filename );
/*
 * Desc - Load a program in memory, and start execution
 *  Params - Filename to load
 *  Pre    - None
 *  Post   - Object code is loaded, and execution is started
 */

void load_stdin( void );
/*
 * Desc - Load a program in memory, and start execution
 *  Params - Filename to load
 *  Pre    - None
 *  Post   - Object code is loaded, and execution is started
 */


/***************************************************************
 *
 * Private functions
 *
 **************************************************************/

/* THE FUNCTIONS BELOW ARE NOT FOR USE OUTSIDE THIS IMPLEMENTATION */
/* ** ONLY INCLUDED IN HEADER FOR COMPLETENESS */

/* static functions */

#if 0
void set_pc( int byte_offset );
/*
 * Desc - Set the program counter
 *  Params - Set the byte_offset
 *  Pre    - * for non jump type instruction: increase by 1
 *           * for jmp type instructions: increase to JMP address
 *  Post   - The program counter is set
 */

void incr_pc( int n );
/*
 * Desc - Increment the program counter
 *  Params - amount to increase PC
 *  Pre    - None
 *  Post   - Program counter is increased by n
 */

short int get_data_reg1( void );
/*
 * Desc - Get contents of data register 1
 *  Params - None
 *  Pre    - None
 *  Post   - Contents of data register 1 is returned
 */

short int get_data_reg2( void );
/*
 * Desc - Get the contents of data register 2
 *  Params - None
 *  Pre    - None
 *  Post   - Contents of data register 2 is returned
 */

void set_data_reg1( short int data );
/*
 * Desc - Set the contents of data register 1
 *  Params - 2 byte data to set register to
 *  Pre    - None
 *  Post   - Register 1 is set to the conents of data
 */

void set_data_reg2( short int data );
/*
 * Desc - Set the contents of data register 2 
 *  Params - 2 bytes to set to register 2 to
 *  Pre    - None
 *  Post   - Register 2 is set to the contents of data
 */

short int get_address_reg1( void );
/*
 * Desc - Get the contents of R1
 *  Params - None
 *  Pre    - None
 *  Post   - The contents of R1 is returned 
 */

short int get_address_reg2( void );
/*
 * Desc - Get the contents of R2
 *  Params - None
 *  Pre    - None
 *  Post   - The contents of R2 is returned
 */

void set_address_reg1( short int data );
/*
 * Desc - Set the conents of A1
 *  Params - Data (2 bytes) to set to A1
 *  Pre    - None
 *  Post   - The conents of A1 is set to data
 */

void set_address_reg2( short int data );
/*
 * Desc - Set the conents of A2
 *  Params - Data (2 bytes)to set to A2
 *  Pre    - None
 *  Post   - The conents of A2 is set to data
 */

void set_z_flag( int state );
/*
 * Desc - Set z flag
 *  Params - State to set the flag to
 *  Pre    - None
 *  Post   - The state of z is set
 */

void set_n_flag( int state );
/*
 * Desc - Set n flag
 *  Params - State to set the flag to
 *  Pre    - None
 *  Post   - The state of n is set
 */

void set_o_flag( int state );
/*
 * Desc - Set the o flag
 *  Params - State to set the flag to
 *  Pre    - None
 *  Post   - The state of o is set
 */

int is_add_overflow( short int n1, short int n2 );
/*
 * Desc - Check for arithmetic overflow/underflow when adding
 *  Params - number 1, number 2
 *  Pre    - none
 *  Post   - Return 1 if overflow/underflow is detected
 *           Else return 0
 */

int is_sub_overflow( short int n1, short int n2 );
/*
 * Desc - Check for arithmetic overflow/underflow when subtracting
 *  Params - number 1, number 2
 *  Pre    - none
 *  Post   - Return 1 if overflow/underflow is detected
 *           Else return 0
 */

#endif

#endif

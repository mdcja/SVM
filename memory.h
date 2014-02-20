/*
 * Author - Julian Martinez del Campo
 * Descr  - This file contains the documentation and header file for memory.h
 *          Memory.h provides the actual memory and functions to manipulate the SVM memory
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#define MAX_MEM 4096                    /* define the maximum amount of memory in bytes*/

typedef unsigned char Byte;

void read_file( char * filename );
/*
 * Descr  - Read an object file into memory
 *  Params - Filename to read
 *  Pre    - None
 *  Post   - An object file is read into vm memory
 */

void read_stdin( void );
/*
 * Descr - Read an object file into memory from stdin
 *  Params - None
 *  Pre    - None
 *  Post   - An object file is read into vm memory
 */

Byte get_byte( int byte_offset );
/*
 * Descr  - Get the memory value at byte_offset
 *  Params - The address in memory to return
 *  Pre    - Assume that the memory at byte_offset has already been set
 *           If memory has not been set, garbage may be returned
 *  Post   - Return contents of memory at byte_offset
 */

void set_byte( Byte data, int byte_offset );
/*
 * Descr  - Set the memory value at byte_offset
 *  Params - Data to put into memory, address in memory to set
 *  Pre    - None
 *  Post   - The memory at byte_offset has the value of data
 */

void set_register( int r1, int r2, int byte_offset );
/*
 * Descr - Set the registers in a byte
 *  Params - Register 1, register 2, address in memory
 *  Pre    - Byte to set is a register byte
 *  Post   - Register 1 and register 2 are set at memory location at byte_offset
 */

void get_register( int *r1, int *r2, int byte_offset );
/*
 * Descr - Get the registers from a byte
 *  Params - Register 1, register 2, address in memory
 *  Pre    - Byte to get is a register byte
 *  Post   - r1 and r2 are set from the location in memory
 */

int is_address_valid( int address );
/*
 * Descr  - Checks if the address specified is valid
 *  Params - The address of memory to check
 *  Pre    - None
 *  Post   - Return 1 if memory address is valid, Return 0 if memory is invalid
 */

void initialize_mem( void );
/*
 * Descr  - Initialize memory
 *  Params - None
 *  Pre    - None
 *  Post   - VM memory is cleared to 0
 */

int write_block( short int data, int byte_offset );
/*
 * Descr - Write a 2-byte block of data to memory
 *  Params - A 2-byte block of data, starting address in memory to write
 *  Pre    - Memory is available 
 *  Post   - A 2-byte block of memory is written to address starting at byte_offset
 *           Return 1 if block was successfully written
 *           Return 0 if unable to allocate memory
 */

int read_block( short int * reg, int byte_offset );
/*
 * Descr - Read a 2-byte block of data from memory
 *  Params - register to move into, byte_offset
 *  Pre    - Memory is available
 *  Post   - A 2-byte block of memory is read into register
 *           Return 1 if block was succesfully read from
 *           Return 0 if block was unable to read (end of memory)
 */

void print_mem( void );
/*
 * Descr - Print out memory address and contents
 *  Params - None
 *  Pre    - None
 *  Post   - a table with location and contents is printed to stdout
 */

#endif

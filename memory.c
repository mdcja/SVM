/*
 * Author - Julian Martinez del Campo
 * Descr  - This file contains the implementation file for memory.h
 */

#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>    /* for convert mem */

static Byte memory[ MAX_MEM ];           /* declare vm memory */

void read_file( char * filename ) {
    FILE * fp;
    int size;
    int bytes_read;
    
    /* initialize memory */
    initialize_mem();

    /* open filename (readonly) */
    fp = fopen( filename, "rb" );
    
    /* check if filename was opened */
    /* if file could not be opened exit failure */
    if( fp == NULL ) {
        printf( "Failed to open %s!\n", filename );
        exit( EXIT_FAILURE );
    }
    
    /* get file size */
    fseek( fp, 0, SEEK_END );
    size = ftell( fp );
    rewind( fp );
    
    /* read byte by byte */
    bytes_read = fread( memory, 1, size, fp );

    if( bytes_read != size ) {
        printf( "Failed to read file into memory!\n" );
        exit( EXIT_FAILURE );
    }

    #if 0 
        printf( "bytes read = %d\n", bytes_read );
        printf( "size = %d\n", size );
        printf( "sizeof = %lu\n", sizeof( Byte ) );
    #endif

    fclose( fp );
}

void read_stdin( void ) {
    Byte temp;      /* holds a byte */
    int offset = 0; /* holds memory offset */      

    /* initialize memory */
    initialize_mem();

    /* read from stdin */
    while( offset < MAX_MEM ) {
        /* read a character */
        scanf( "%c", &temp );

        /* set value in memory */
        set_byte( temp, offset );
        
        /* increment offset */
        offset++;
    }
}

Byte get_byte( int byte_offset ) {
    return memory[ byte_offset ];
}

void set_byte( Byte data, int byte_offset ) {
    memory[ byte_offset ] = data; 
}

void set_register( int r1, int r2, int byte_offset ) {
    int byte = 0;       /* byte to set, inizialze to 0 */

    /* set register 1 */
    byte = ( byte & 0x3 ) | r1;
    
    /* set register 2 */
    byte = ( ( byte >> 6 ) & 0x3 ) | r2;

    /* set byte in memory */
    set_byte( byte, byte_offset );

}

void get_register( int *r1, int *r2, int byte_offset ) {
    int byte = 0;

    /* get byte */
    byte = get_byte( byte_offset );

    /* get r1 */
    *r1 = ( byte & 0x3 );

    /* get r2 */
    *r2 = ( byte >> 6 ) & 0x3;
}

int is_address_valid( int address ) {
    /* check to see that address is between 0 and MAX_MEM */
    if( ( address < MAX_MEM ) && ( address >= 0 ) ) {
        return 1;
    }
    else {
        return 0;
    }
}

void initialize_mem( void ) {
    int i;

    /* initialize the memory to 0 */
    for( i = 0; i < MAX_MEM; ++i ) {
        memory[ i ] = 0;
    }
}

int write_block( short int data, int byte_offset ) {
    /* note only the byte in data will be set */
    Byte temp;
    
    /* check to see if we will run out of memory */
    if( byte_offset > ( MAX_MEM - 2 ) ) {
        return 0;
    }

    /* set first 1 byte block in memory */
    temp = data & 0xff ;
    set_byte( temp, byte_offset );

    /* set second 1 byte block in memory */
    temp = ( data >> 8 ) & 0xff ;
    set_byte( temp, byte_offset + 1 );
    
    return 1; 

}

int read_block( short int * reg, int byte_offset ) {
    /* note only the first byte in data will be read from */
    int temp1;
    int temp2;
    
    /* check to see if we will go over memory */
    if( byte_offset > ( MAX_MEM - 2 ) ) {
        return 0;
    }

    /* first half of mem */
    temp1 = get_byte( byte_offset );      /* set lower half of register to contents at byte_offset */
    /*temp1 = temp1 << 8;  */                   /* shift register 1 byte left */

    /* second half of mem */
    temp2 = get_byte( byte_offset + 1 );
    temp2 = temp2 << 8;                     /* shift register 1 byte left */

    *reg = temp1 | temp2;                   /* upper and lower halves of memory are merged into reg */

    return 1;
}

void print_mem( void ) {
    int i;
    int temp;
    
    /* print memory */
    for( i = 0; i < MAX_MEM; i++ ) {
        temp = get_byte( i );
        /* print memory address, contents in hex, contents in dec */
        printf( "%3d : %2x\t%16d\n", i, temp, temp );
    }
}


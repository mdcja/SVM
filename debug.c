#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

int main( int argc, char ** argv ){
   
    #if DEBUG_MEM
        printf( "OP      ARGS\n" );
        printf( "-------------------------\n" );
    #endif
    
    #if DEBUG_INST
        printf( "  PC : OP R1 R2 IMM\n" );
        printf( "-------------------------\n" );
    #endif

    /* check if file is passed by command line */
    if( argc == 2 ) {
        /* read from file and start execution */
        load_file( argv[ 1 ] );
    }
    else {
        /* read file from stdin and start execution */
        load_stdin();
    }

    return EXIT_SUCCESS;
}

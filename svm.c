/*
 * Author - Julian Martinez del Campo
 * Descr  - SVM virtual machine
 */


#include <stdlib.h>
#include "cpu.h"

int main( int argc, char ** argv ){
   
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

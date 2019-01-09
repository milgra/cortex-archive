
#include <stdio.h>
#include <stdlib.h>
#include "../../sources/bridge.h"

void bridge_init( void )
{


}

void bridge_free( void )
{


}

void bridge_buy( char* item )
{

    bridge_open( "https://paypal.me/milgra" );

}

void bridge_open( char* url )
{

    char newurl[100];
    snprintf( newurl , 100 , "xdg-open %s", url );
    system( newurl );

}

//
//  bridge.c
//  Termite
//
//  Created by Milan Toth on 2018. 11. 29..
//  Copyright © 2018. milgra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "bridge.h"

void bridge_init( )
{

}

void bridge_free( )
{

}

void bridge_buy( char* item )
{

    bridge_open( "https://paypal.me/milgra" );

}

void bridge_open( char* url )
{

    char newurl[100];
    snprintf( newurl , 100 , "open %s", url );
    system( newurl );

}

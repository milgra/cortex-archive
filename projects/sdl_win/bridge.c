
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ShellApi.h>
#include "../../sources/bridge.h"
#define GLEW_STATIC
#include "GL/glew.h"

void bridge_init( void )
{

    GLint GlewInitResult = glewInit( );

    if ( GLEW_OK != GlewInitResult )
    {

        const GLubyte* error = glewGetErrorString( GlewInitResult );
        printf("ERROR: %s\n",error);

    }

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

    ShellExecute(0,"open",url,NULL,NULL,1);

}

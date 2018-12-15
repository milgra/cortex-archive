#include <jni.h>
#include <android/log.h>
#include "bridge.h"
#include "../sources/defaults.c"
#include "../framework/core/mtbus.c"
#include "../SDL2/src/core/android/SDL_android.h"

JavaVM* javaVM = NULL;
JNIEnv* javaEnv = NULL;

void bridge_init( )
{
    __android_log_print(ANDROID_LOG_INFO, "TERMITE", "bridge_init" );

    defaults.text_scale = 1.4;

//    JNIEnv* env = Android_JNI_GetEnv( );
//    // Make sure thread is attached to JVM/env
//
//    jstring jstr = (*env)->NewStringUTF(env, "This string comes from JNI");
//    // First get the class that contains the method you need to call
//    jclass clazz = (*env)->FindClass(env,"com/milgra/termite/Cortex");
//    // Get the method that you want to call
//    jmethodID messageMe = (*env)->GetMethodID(env,clazz, "messageMe", "(Ljava/lang/String;)V");
//    // Call the method on the object
//    jobject result = (*env)->CallObjectMethod(env,jstr, messageMe);
//    // Get a C-style string
//    const char* str = (*env)->GetStringUTFChars(env,(jstring) result, NULL);
//    printf("%s\n", str);
//    // Clean up
//
//    (*env)->ReleaseStringUTFChars(env,jstr, str);

}


void bridge_free( void )
{


}

void bridge_open( char* url )
{

    javaEnv = Android_JNI_GetEnv();

    jstring jstr = (*javaEnv)->NewStringUTF(javaEnv, url);

    jclass clazz = (*javaEnv)->FindClass(javaEnv,"com/milgra/cortex/Cortex");
    // Get the method that you want to call
    jmethodID openURL = (*javaEnv)->GetStaticMethodID(javaEnv,clazz, "openURL", "(Ljava/lang/String;)V");
    // Call the method on the object
    (*javaEnv)->CallStaticVoidMethod(javaEnv,clazz,openURL,jstr);

}


void bridge_buy( char* item )
{

    int index;

    for ( index = 0 ;
          index < 3 ;
          index++)
    {
        if ( strcmp( defaults.prices[ index ] , item ) == 0 ) break;
    }

    (*javaVM)->AttachCurrentThread(javaVM, &javaEnv, NULL);

    jclass clazz = (*javaEnv)->FindClass(javaEnv,"com/milgra/cortex/Cortex");
    // Get the method that you want to call
    jmethodID buyItem = (*javaEnv)->GetStaticMethodID(javaEnv,clazz, "buyItem", "(I)V");
    // Call the method on the object
    (*javaEnv)->CallStaticVoidMethod(javaEnv,clazz,buyItem,index);

    //(*javaVM)->DetachCurrentThread(javaVM);
}


JNIEXPORT jstring JNICALL Java_com_milgra_termite_Termite_storeitem(
        JNIEnv* env,
        jobject cls,
        jint index,
        jstring name)
{

    javaEnv = env;
    (*env)->GetJavaVM( javaEnv , &javaVM );

    const char *utfname = (*env)->GetStringUTFChars(env, name, NULL);

    __android_log_print(ANDROID_LOG_INFO, "CORTEX", "storeitem %s at %i" , utfname , index );

    strcpy( defaults.prices[ index ], utfname );
}

JNIEXPORT jstring JNICALL Java_com_milgra_termite_Termite_showdonate(
        JNIEnv* env,
        jobject cls,
        jint index,
        jstring name)
{
    __android_log_print(ANDROID_LOG_INFO, "CORTEX", "showdinate" );

    defaults.prices_arrived = 1;

    //mtbus_notify( "CTL", "SHOWDONATION" , NULL );
}

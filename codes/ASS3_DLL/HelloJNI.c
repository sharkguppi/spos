#include <jni.h>       // JNI header
#include <stdio.h>     // For printf
#include "HelloJNI.h"  // Generated header

// Implementation of the native method
JNIEXPORT void JNICALL Java_HelloJNI_sayHello(JNIEnv *env, jobject obj) {
    printf("Hello from JNI on Linux!\n");
}
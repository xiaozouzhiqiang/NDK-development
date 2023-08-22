#include <jni.h>
#include <string>
extern "C"{
    #include "testC.h"
}
#include "android/log.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reflection_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_StaticFun(
        JNIEnv* env,
        jclass /* this */) {
    int result = add_C(4,6);
    __android_log_print(ANDROID_LOG_INFO, "zouzhiqiang", "test-->addC %d",result);
}

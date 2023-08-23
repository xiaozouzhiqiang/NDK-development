#include <jni.h>
#include <string>
extern "C"{
    #include "testC.h"
}
#include "android/log.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reflection_MainActivity_stringFromJNI(JNIEnv* env,jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_StaticFun(JNIEnv* env,jclass /* this */) {
    int result = add_C(4,6);
    __android_log_print(ANDROID_LOG_INFO, "Moonlight", "test-->addC %d",result);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_example_reflection_MainActivity_jni_1add(JNIEnv *env, jobject thiz, jint num) {
    // TODO: implement Jni_add()
    int result = 0;
    for (int i = 0; i < num; ++i) {
        result = result + i ;
    }
    return result;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reflection_MainActivity_testStringApi(JNIEnv *env, jobject thiz,jstring content) {
    // TODO: implement testStringApi()
    const char *a = env->GetStringUTFChars(content, nullptr);
    int contentSize = env->GetStringUTFLength(content);
    if(a != nullptr){
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "GetStringUTFChars:%s, Content Size %d", a,contentSize);
    }
    //防止内存泄漏问题，一般会将内存进行释放，由于这儿会有返回值，所以不释放内存
    //env->ReleaseStringUTFChars(content,a);
    jstring result = env->NewStringUTF("Hello from Jni");
    return result;
}
#include <jni.h>
#include <string>
#include "android/log.h"
extern "C"{
    #include "testC.h"
}
#include "android/log.h"
JavaVM* globalVM = nullptr;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reflection_MainActivity_stringFromJNI(JNIEnv* env,jobject /* this */) {
    env->GetJavaVM(&globalVM);
    JavaVM* JniVM = nullptr;
    env->GetJavaVM(&JniVM);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","GetJavaVM->%p",JniVM);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","JNIEnv->%p",env);
    std::string hello = "测试Java反射 C++ ";
    //jclass FindClass(const char* name)
    jclass MoonLightTestClass = env->FindClass("com/example/reflection/MoonlightTest");
    //jfieldID GetStaticFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID publicStaticField_fieldID = env->GetStaticFieldID(MoonLightTestClass,"publicStaticField","Ljava/lang/String;");
    //jobject GetStaticObjectField(jclass clazz, jfieldID fieldID
    jstring publicStaticField_content = (jstring)env->GetStaticObjectField(MoonLightTestClass,publicStaticField_fieldID);
    const char * content_ptr = env->GetStringUTFChars(publicStaticField_content, nullptr);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","Jni->%s",content_ptr);
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

void *MoonLightThreadTest(void *args){
    for (int i = 0; i < 5; ++i) {
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "JNI->%s,%d","I am from MoonLightThreadTest",i);
    }
    JNIEnv* ThreadTestEnv = nullptr;
    //globalVM 是上面建立的公有指针对象
//    if(globalVM->GetEnv((void**)&ThreadTestEnv,JNI_VERSION_1_6)==JNI_OK){
//        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->GetEnv((void**)&ThreadTestEnv,JNI_VERSION_1_6)->%s","globalVM is Success!");
//    } else{
//        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->GetEnv((void**)&ThreadTestEnv,JNI_VERSION_1_6)->%s","globalVM is failed!");
//    }
//    if(globalVM->AttachCurrentThread(&ThreadTestEnv, nullptr)==JNI_OK){
//        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->AttachCurrentThread(&ThreadTestEnv, nullptr) %s","globalVM is Success!");
//    } else{
//        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->AttachCurrentThread(&ThreadTestEnv, nullptr) %s","globalVM is failed!");
//    }
    pthread_exit(0);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    __android_log_print(ANDROID_LOG_INFO, "Moonlight->Jni_OnLoad", "Jni_OnLoad is Called!");
    jint result = 0;
    result = JNI_VERSION_1_6;
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env,JNI_VERSION_1_6)==JNI_OK){
        __android_log_print(ANDROID_LOG_INFO, "Moonlight->Jni_OnLoad", "JNIEnv is Called Success!");
    }
    pthread_t thread;
    //int pthread_create(pthread_t* __pthread_ptr, pthread_attr_t const* __attr, void* (*__start_routine)(void*), void*);
    pthread_create(&thread, nullptr,MoonLightThreadTest, nullptr);
    pthread_join(thread, nullptr);
    return result;
}

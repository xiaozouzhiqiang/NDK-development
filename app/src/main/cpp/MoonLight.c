//
// Created by ZouZhiQiang on 2023/8/28.
//
#include <stdio.h>
#include <jni.h>
#include <android/log.h>

void*       reserved0;
void*       reserved1;
void*       reserved2;
void*       reserved3;

//jint        (*GetVersion)(JNIEnv *);
//jclass      (*DefineClass)(JNIEnv*, const char*, jobject, const jbyte*, jsize);
//jclass      (*FindClass)(JNIEnv*, const char*);
//jmethodID   (*FromReflectedMethod)(JNIEnv*, jobject);
//jfieldID    (*FromReflectedField)(JNIEnv*, jobject);
///* spec doesn't show jboolean parameter */
//jobject     (*ToReflectedMethod)(JNIEnv*, jclass, jmethodID, jboolean);
//jclass      (*GetSuperclass)(JNIEnv*, jclass);
JNIEXPORT jstring JNICALL
Java_com_example_reflection_MainActivity_stringFromJNIC(JNIEnv* env,jobject thisObj /* this */) {
    const char* hello = "测试Java反射 C";
    //jclass FindClass(const char* name)
    jclass MoonLightTestClass = (*env)->FindClass(env,"com/example/reflection/MoonlightTest");
    //jfieldID GetStaticFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID publicStaticField_fieldID = (*env)->GetStaticFieldID(env,MoonLightTestClass,"publicStaticField","Ljava/lang/String;");
    //jobject GetStaticObjectField(jclass clazz, jfieldID fieldID
    jstring publicStaticField_content = (jstring)(*env)->GetStaticObjectField(env,MoonLightTestClass,publicStaticField_fieldID);
    const char * content_ptr = (*env)->GetStringUTFChars(env,publicStaticField_content, NULL);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->C->Jni","Jni->%s",content_ptr);
    return (*env)->NewStringUTF(env,hello);
}
#include <jni.h>
#include <string>
#include "android/log.h"
extern "C"{
    #include "testC.h"
}
#include "android/log.h"
jobject globalClassloader = nullptr;
jclass findClass = nullptr;
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
Java_com_example_reflection_MainActivity_NewObject(JNIEnv* env,jobject /* this */) {
    //方式一，NewObject方式
    jclass MoonLightTestClass = env->FindClass("com/example/reflection/MoonlightTest"); //获取类
    //public MoonlightTest(String content)
    jmethodID MoonTest_mid = env->GetMethodID(MoonLightTestClass,"<init>", "(Ljava/lang/String;)V"); //获取方法ID
    jstring args1 = env->NewStringUTF("I am from Jni NewObject");
    //jobject NewObject(jclass clazz, jmethodID methodID, ...),第一个是class，第二个参数是MethodID，第三个是参数
    jobject testObject = env->NewObject(MoonLightTestClass,MoonTest_mid,args1);
    if(testObject!= nullptr){
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","NewObject->%s","NewObject Success!");
    }
    //方式二，AllocObject
    jobject testObject2 = env->AllocObject(MoonLightTestClass);
    jstring args2 = env->NewStringUTF("I am from Jni AllocObject");
    env->CallNonvirtualVoidMethod(testObject2,MoonLightTestClass,MoonTest_mid,args2);
    if(testObject!= nullptr){
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","AllocObject->%s","AllocObject Success!");
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_getJavaStatusField(JNIEnv* env,jobject /* this */) {
    //获取java公有静态属性
    jclass MoonLightTestClass = env->FindClass("com/example/reflection/MoonlightTest"); //获取类
    //public static String publicStaticField = "I am public StaticField";  //需要获取的java层公有的静态属性
    //jfieldID GetStaticFieldID(jclass clazz, const char* name, const char* sig) //参数1：class类加载对象，参数2：属性名称，参数3：属性签名
    jfieldID publicStatic_Field = env->GetStaticFieldID(MoonLightTestClass,"publicStaticField","Ljava/lang/String;");
    //static_cast<jstring> 强制类型转换
    jstring publicStaticField_Obj = static_cast<jstring>(env->GetStaticObjectField(MoonLightTestClass, publicStatic_Field));
    //GetStringUTFChars 获取指针类型字符串
    const char* publicStaticField_Content = env->GetStringUTFChars(publicStaticField_Obj, nullptr);
    if(publicStaticField_Content != nullptr){
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","publicStaticField_obj->%s",publicStaticField_Content);
    }
    //修改java 公有静态属性的值
    jstring SetPublicStaticValue = env->NewStringUTF("modify publicStaticValue for jni");
    //SetStaticObjectField设置公有静态属性的值
    env->SetStaticObjectField(MoonLightTestClass,publicStatic_Field,SetPublicStaticValue);
    //static_cast<jstring> 强制类型转换
    jstring publicStaticField_Obj2 = static_cast<jstring>(env->GetStaticObjectField(MoonLightTestClass, publicStatic_Field));
    //GetStringUTFChars 获取指针类型字符串
    const char* SetpublicStaticField_Content2 = env->GetStringUTFChars(publicStaticField_Obj2, nullptr);
    if(publicStaticField_Content != nullptr){
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","SetPublicStaticValue->%s",SetpublicStaticField_Content2);
    }

    //获取java私有静态属性
    //private static String privateStaticField = "I am private StaticField";
    jfieldID privateStatic_Field = env->GetStaticFieldID(MoonLightTestClass,"privateStaticField","Ljava/lang/String;");
    //static_cast<jstring> 强制类型转换
    jstring privateStaticField_obj = static_cast<jstring>(env->GetStaticObjectField(MoonLightTestClass, privateStatic_Field));
    //GetStringUTFChars 获取指针类型字符串
    const char* privateStaticField_content = env->GetStringUTFChars(privateStaticField_obj, nullptr);
    if(privateStaticField_content != nullptr){
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","privateStaticField_obj->%s",privateStaticField_content);
    }

    //获取公有int类型的静态属性
    //public static int publicStaticIntField = 100;
    jfieldID publicStaticInt_Field = env->GetStaticFieldID(MoonLightTestClass,"publicStaticIntField","I");
    //设置公有int类型静态属性的值为200
    env->SetStaticIntField(MoonLightTestClass,publicStaticInt_Field,200);
    //获取公有int类型的静态属性
    jint publicStaticInt_value= env->GetStaticIntField(MoonLightTestClass, publicStaticInt_Field);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","publicStaticInt_value->%d",publicStaticInt_value);

}

extern "C" JNIEXPORT jobject JNICALL
Java_com_example_reflection_MainActivity_CallInit(JNIEnv* env,jobject /* this */) {
    //public MoonlightTest(String content,int arg);
    jclass MoonLightTestClass = env->FindClass("com/example/reflection/MoonlightTest"); //获取类
    jmethodID MoonTest_mid = env->GetMethodID(MoonLightTestClass,"<init>", "(Ljava/lang/String;I)V"); //获取方法ID
    jstring args1 = env->NewStringUTF("I am from Jni CallInit"); //声明一个String字符串
    jobject TestObject = env->NewObject(MoonLightTestClass,MoonTest_mid,args1,1000);
    return TestObject;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_getJavaNotStaticField(JNIEnv* env,jobject obj,jobject MoonObj) {
    //访问私有的非静态属性
    //private  String privateNotStaticField = "I am private Not StaticField";
    jclass MoonLightTestClass = env->FindClass("com/example/reflection/MoonlightTest"); //获取类
    jfieldID privateField_fieldID = env->GetFieldID(MoonLightTestClass,"privateNotStaticField","Ljava/lang/String;");
    //jobject GetObjectField(jobject obj, jfieldID fieldID)
    //这儿的MoonObj可能会提示报错，报错信息-->Inspection 'Inspect JNI SetObjectField calls and its variants' options->Disable inspection即可
    jstring privateField_modifVolue = env->NewStringUTF("modify value for privateNotStaticField");
    env->SetObjectField(MoonObj,privateField_fieldID,privateField_modifVolue);
    jstring privateField_obj = static_cast<jstring>(env->GetObjectField(MoonObj,privateField_fieldID));
    const char* privateField_Content = env->GetStringUTFChars(privateField_obj, nullptr);
    if(privateField_Content != nullptr){
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","privateField_obj->%s",privateField_Content);
    }
    //获取int类型的非静态属性
    //private int privateNonStaticIntField = 400;
    jfieldID privateNonStaticIntField_fielID = env->GetFieldID(MoonLightTestClass,"privateNonStaticIntField","I");
    env->SetIntField(MoonObj,privateNonStaticIntField_fielID,800);
    jint privateNonStaticIntField_value = env->GetIntField(MoonObj,privateNonStaticIntField_fielID);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","privateNonStaticIntField->%d",privateNonStaticIntField_value);

    //获取公有非静态的int数组
    //public int[] intArray = null;
    jfieldID intArray_field = env->GetFieldID(MoonLightTestClass,"intArray","[I");
    jintArray intArray_obj = static_cast<jintArray>(env->GetObjectField(MoonObj, intArray_field));
    int intArray_length = env->GetArrayLength(intArray_obj);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","intArray_length->%d",intArray_length);
    int* array = env->GetIntArrayElements(intArray_obj, nullptr);
    for (int i = 0; i < intArray_length; ++i) {
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","array[%d]->%d",i,array[i]);
    }
    //修改公有非静态属性的int数组
    jint jni_array[intArray_length];
    for (int i = 0; i < intArray_length; ++i) {
        jni_array[i] = 10 - i;
    }
    const jint* ptr = jni_array;
    //void SetIntArrayRegion(jintArray array, jsize start, jsize len,const jint* buf)
    env->SetIntArrayRegion(intArray_obj,0,intArray_length,ptr);
    int* array2 = env->GetIntArrayElements(intArray_obj, nullptr);
    for (int i = 0; i < intArray_length; ++i) {
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","jni_array[%d]->%d",i,array2[i]);
    }
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
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->MoonLightThreadTest","globalVM->%p",globalVM);
    //globalVM 是上面建立的公有指针对象
    if(globalVM->GetEnv((void**)&ThreadTestEnv,JNI_VERSION_1_6)==JNI_OK){
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->GetEnv((void**)&ThreadTestEnv,JNI_VERSION_1_6)->%s","globalVM is Success!");
    } else{
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->GetEnv((void**)&ThreadTestEnv,JNI_VERSION_1_6)->%s","globalVM is failed!");
    }
    //附加到当前线程中
    if(globalVM->AttachCurrentThread(&ThreadTestEnv, nullptr)==JNI_OK){
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->AttachCurrentThread(&ThreadTestEnv, nullptr) %s","globalVM is Success!");
        jstring content = ThreadTestEnv->NewStringUTF("MoonLightThreadTest -> content");
        const char* CharContent = ThreadTestEnv->GetStringUTFChars(content, nullptr);
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "MoonLightThreadTest-> %s",CharContent);
        ThreadTestEnv->ReleaseStringUTFChars(content, CharContent); //释放掉用掉的内存
    } else{
        __android_log_print(ANDROID_LOG_INFO, "Moonlight", "globalVM->AttachCurrentThread(&ThreadTestEnv, nullptr) %s","globalVM is failed!");
    }
    globalVM->DetachCurrentThread();   //退出附加线程
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
    globalVM = vm;
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->JNI_OnLoad","JNIEnv->%p",env);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->JNI_OnLoad","JavaVM->%p",vm);
    //int pthread_create(pthread_t* __pthread_ptr, pthread_attr_t const* __attr, void* (*__start_routine)(void*), void*);
    pthread_create(&thread, nullptr,MoonLightThreadTest, nullptr);
    pthread_join(thread, nullptr);
    return result;
}

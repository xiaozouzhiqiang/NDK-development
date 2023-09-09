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
jclass testClass = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_onCreate(JNIEnv* env,jobject thiz,jobject BundleObj) {
    //实现以下方法
    //第一种是已知类名了，采用Findclass获取父类
    jclass AppCompatActivity_JClass = env->FindClass("androidx/appcompat/app/AppCompatActivity");
    jclass MainActivity_Jclass = env->FindClass("com/example/reflection/MainActivity");
    //第二种获取方式是获取当前Object对象，采用GetSuperclass来获取父类
    jclass MainActivity_Jclss2 = env->GetObjectClass(thiz);
    jclass AppCompatActivity_JClass2 = env->GetSuperclass(MainActivity_Jclss2);

    // protected void onCreate(Bundle savedInstanceState),其中Bundle对象可以查看具体所在类，
    jmethodID onCreate_mid = env->GetMethodID(AppCompatActivity_JClass2,"onCreate","(Landroid/os/Bundle;)V");
    //super.onCreate(savedInstanceState);
    env->CallNonvirtualVoidMethod(thiz,AppCompatActivity_JClass2,onCreate_mid,BundleObj);

    //Log.i("MoonLight->onCreate",MoonObj2.flag);
    jstring arg1 = env->NewStringUTF("MoonLight");
    jstring arg2 = env->NewStringUTF("this is OnCreate");
    jclass log_Class = env->FindClass("android/util/Log");
    jmethodID log_mid = env->GetStaticMethodID(log_Class,"i","(Ljava/lang/String;Ljava/lang/String;)I");
    jint log_result = env->CallStaticIntMethod(log_Class,log_mid,arg1,arg2);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","onCreate->%d",log_result);

    //setContentView(R.layout.activity_main); 这儿代码有些问题，因为版本原因
//    jmethodID  setContentView_mid = env->GetMethodID(MainActivity_Jclss2,"setContentView","(I)V");
//    jclass R_layoutClass = env->FindClass("com/example/reflection/R$layout");
//    jfieldID activity_main_fieldId = env->GetStaticFieldID(R_layoutClass,"activity_main","I");
//    jint activity_main_Value = env->GetStaticIntField(R_layoutClass,activity_main_fieldId);
//    env->CallVoidMethod(thiz,setContentView_mid,activity_main_Value);
//
//    //TextView tv = findViewById(R.id.sample_text);
//    jmethodID  findViewById_mid = env->GetMethodID(MainActivity_Jclss2,"findViewById","(I)Landroid/view/View");
//    jclass R_idClass = env->FindClass("com/example/reflection/R$id");
//    jfieldID sample_text_fieldId = env->GetStaticFieldID(R_idClass,"sample_text","I");
//    jint sample_text_Value = env->GetStaticIntField(R_layoutClass,sample_text_fieldId);
//    env->CallVoidMethod(thiz,findViewById_mid,sample_text_Value);

    //MoonlightTest MoonObj2 = (MoonlightTest)CallInit(); //测试经过JNi层初始化后，构造函数是否初始化
    //Log.i("MoonLight->onCreate",MoonObj2.flag);
    jmethodID CallInit_mid = env->GetMethodID(MainActivity_Jclss2,"CallInit","()Ljava/lang/Object;");
    jobject MoonObj2_field = env->CallObjectMethod(thiz,CallInit_mid);
    jclass MoonlightTest_class = env->FindClass("com/example/reflection/MoonlightTest");
    jfieldID fiag_field = env->GetFieldID(MoonlightTest_class,"flag", "Ljava/lang/String;");
    jstring flag_value = static_cast<jstring>(env->GetObjectField(MoonObj2_field, fiag_field));
    jint flag_result = env->CallStaticIntMethod(log_Class,log_mid,arg1,flag_value);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->onCreate","flag_result->%d",flag_result);

}

/*
extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_onCreate(JNIEnv* env,jobject obj,jobject BundleObj){
    //protected native void onCreate(Bundle savedInstanceState);
    jclass AppCompatActivity_Class = env->FindClass("androidx/appcompat/app/AppCompatActivity");
    jmethodID onCreate_mid = env->GetMethodID(AppCompatActivity_Class,"onCreate","(Landroid/os/Bundle;)V");
    //    void CallNonvirtualVoidMethod(jobject obj, jclass clazz,jmethodID methodID, ...)
    env->CallNonvirtualVoidMethod(obj,AppCompatActivity_Class,onCreate_mid,BundleObj);
    //super.onCreate(savedInstanceState);
    //Log.i("MoonLight","this is OnCreate");
    jstring arg1 = env->NewStringUTF("MoonLight");
    jstring arg2 = env->NewStringUTF("this is OnCreate");
    jclass log_Class = env->FindClass("android/util/Log");
    jmethodID log_mid = env->GetStaticMethodID(log_Class,"i","(Ljava/lang/String;Ljava/lang/String;)I");
    jint log_result = env->CallStaticIntMethod(log_Class,log_mid,arg1,arg2);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","onCreate->%d",log_result);
}*/

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
Java_com_example_reflection_MainActivity_CallJavaStaticFun(JNIEnv* env,jobject /* this */){
    //public static void publicStaticFunction()
    jclass MoonLightClass = env->FindClass("com/example/reflection/MoonlightTest");
    jmethodID JavaPublicStaFun_mid = env->GetStaticMethodID(MoonLightClass,"publicStaticFunction","()V");
    //和java层反射不同的是，JNI反射静态函数时，不需要setAccessible(true)
    //CallStaticVoidMethod用于Void类型的静态函数，除此之外还有CallStaticByteMethod.CallStaticBooleanMethod等
    env->CallStaticVoidMethod(MoonLightClass,JavaPublicStaFun_mid);
    //private static void privateStaticFunction(){
    jmethodID JavaPrivateStaFun_mid = env->GetStaticMethodID(MoonLightClass,"privateStaticFunction","()V");
    env->CallStaticVoidMethod(MoonLightClass,JavaPrivateStaFun_mid);

    //public static int PublicStaticIntFunction(int a)
    jmethodID JavaPublicStaticIntFun_mid = env->GetStaticMethodID(MoonLightClass,"PublicStaticIntFunction","(I)I");
    jint int520 = env->CallStaticIntMethod(MoonLightClass,JavaPublicStaticIntFun_mid,20);
    //public static String PublicStaticStringFunction(String args)
    jmethodID JavaPublicStaticStrinFun_mid = env->GetStaticMethodID(MoonLightClass,"PublicStaticStringFunction","(Ljava/lang/String;)Ljava/lang/String;");
    jstring loveValue = env->NewStringUTF("I love you");
    jstring ZmyString = static_cast<jstring>(env->CallStaticObjectMethod(MoonLightClass,JavaPublicStaticStrinFun_mid,loveValue));
    const char* result_string = env->GetStringUTFChars(ZmyString, nullptr);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","CallJavaStaticFun->%s,%d",result_string,int520);

}

extern "C" JNIEXPORT void JNICALL
Java_com_example_reflection_MainActivity_CallJavaNotStaticFun(JNIEnv* env,jobject /* this */){
    //public void publicNotStaticFunction(){ 调用公有非静态函数
    //通过MoonObj对象去访问公有非静态函数
    jclass MoonLightClass = env->FindClass("com/example/reflection/MoonlightTest");
    jmethodID con_mid = env->GetMethodID(MoonLightClass,"<init>", "(Ljava/lang/String;I)V");
    jstring arg0 = env->NewStringUTF("I am from CallJavaNotStaticFun");
    jobject MoonObj = env->NewObject(MoonLightClass,con_mid,arg0,100);
    jmethodID JavaPublicFun_mid = env->GetMethodID(MoonLightClass,"publicNotStaticFunction","()V");
    env->CallVoidMethod(MoonObj,JavaPublicFun_mid);

    //反射调用私有有参非静态函数
    //private String privateStringNotStaticFunction(String args,int a)
    jmethodID privateNStatic_mid = env->GetMethodID(MoonLightClass,"privateStringNotStaticFunction","(Ljava/lang/String;I)Ljava/lang/String;");
    jstring arg1 = env->NewStringUTF("I am from JNI privateStringNotStaticFunction");
    //jstring privateNStatic_result = static_cast<jstring>(env->CallObjectMethod(MoonObj,privateNStatic_mid,arg1, 200));
    jvalue args[2];
    args[0].l = arg1;
    args[1].i = 200;
    //使用CallObjectMethodA来获取返回值。第三个参数可以为数组指针类型
    jstring privateNStatic_result2 = static_cast<jstring>(env->CallObjectMethodA(MoonObj,privateNStatic_mid,args));
    const char* result_string = env->GetStringUTFChars(privateNStatic_result2, nullptr);
    __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","CallJavaNotStaticFun->%s,%d",result_string,200);

    //反射java层非静态数组 类型的函数
    //private int[] privateIntArrayNotStaticFunction(int num)
    jmethodID privateIntNStatic_mid = env->GetMethodID(MoonLightClass,"privateIntArrayNotStaticFunction","(I)[I");
    jintArray privateIntNStatic_Obj = static_cast<jintArray>(env->CallObjectMethod(MoonObj,privateIntNStatic_mid,10));
    jint* array_ptr = env->GetIntArrayElements(privateIntNStatic_Obj, nullptr);
    jint array_len = env->GetArrayLength(privateIntNStatic_Obj);
    for (int i = 0; i < array_len; ++i) {
        __android_log_print(ANDROID_LOG_INFO,"MoonLight->Jni","array_ptr[%d]->%d",i,array_ptr[i]);
    }
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

jobject MoonTestPushAndPushLocalFrame(JNIEnv* env){
    jobject result = nullptr;
    if(env->PushLocalFrame(20)==0){
        for (int i = 0; i < 18; ++i) {
            jstring MoonTmp = env->NewStringUTF("MoonLight");
        }
        jstring MoonTmp1 = env->NewStringUTF("MoonLight1");
        jstring MoonTmp2 = env->NewStringUTF("MoonLight2");
        result = env->PopLocalFrame(MoonTmp1);
    } else{
        //error
    }
    return result;
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
    jclass MoonLightTestClass = env->FindClass("com/example/reflection/MoonlightTest"); //获取类
    testClass = static_cast<jclass>(env->NewGlobalRef(MoonLightTestClass));
    env->DeleteGlobalRef(testClass);
    int len = 10;
    if(env->EnsureLocalCapacity(10)==0){
        for (int i = 0; i < len; ++i) {
            jstring content = env->NewStringUTF("EnsureLocalCapacity test");
            __android_log_print(ANDROID_LOG_INFO,"MoonLight->JNI_OnLoad","EnsureLocalCapacity->%d",i);
            env->DeleteLocalRef(content);
        }
    }
    jobject resultObj = MoonTestPushAndPushLocalFrame(env);
    //IsSameObject判断一个引用对象的值是否为空
    if(!env->IsSameObject(resultObj,NULL)){
        const char* CharContent = env->GetStringUTFChars(static_cast<jstring>(resultObj), nullptr);
        __android_log_print(ANDROID_LOG_INFO, "oonLight->JNI_OnLoad", "MoonTestPushAndPushLocalFrame-> %s",CharContent);
    }
    return result;
}

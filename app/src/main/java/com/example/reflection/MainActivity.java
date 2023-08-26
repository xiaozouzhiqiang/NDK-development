package com.example.reflection;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.reflection.databinding.ActivityMainBinding;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'reflection' library on application startup.
    static {
        System.loadLibrary("reflection");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());
        //execution_time(); //计算执行java层和jni层相同代码所花费的时间
        testJniAPI(); //测试JNI中的基础数据类型
        //TestJavaField(); // 反射调用java层属性
        TestJavaMethod();
    }

    public void execution_time(){
        long start = System.nanoTime();
        int result = java_add(1000000);
        long end = System.nanoTime();
        Log.i("Java_add","from java_add"+ result + "Java层java_add函数执行所花费的时间：" + (end - start));
        start = System.nanoTime();
        result = jni_add(1000000);
        end = System.nanoTime();
        Log.i("Jni_add","from Jni_add"+ result + "Jni层Jni_add函数执行所花费的时间：" + (end - start));
    }
    public int java_add(int num){
        int result = 0;
        for (int i=0;i<=num;i++){
            result = result + i;
        }
        return  result;
    }
    public void testJniAPI(){
        String resultString = testStringApi("hello from java");
        Log.i("Moonlight","resultString" + resultString);
    }
    public void TestJavaField(){
        try {
            Class testJavaReClazz;
            Class testJavaReClazz2;
            Class testJavaReClazz3;
            MoonlightTest Moon_obj1 = new MoonlightTest();
            MoonlightTest Moon_obj2 = new MoonlightTest("Moonlight");
            MoonlightTest Moon_obj3 = new MoonlightTest("Moonlight",100);
            testJavaReClazz = MainActivity.class.getClassLoader().loadClass("com.example.reflection.MoonlightTest");
            testJavaReClazz2 = Class.forName("com.example.reflection.MoonlightTest");
            testJavaReClazz3 = MoonlightTest.class;
            Field publicStaticField_field = testJavaReClazz3.getDeclaredField("publicStaticField");
            Field publicNotStaticField_field = testJavaReClazz3.getDeclaredField("publicNotStaticField");
            publicStaticField_field.set(null,"publicStaticField->modified");
            publicNotStaticField_field.set(Moon_obj1,"publicNotStaticField->modified");
            String publicStaticField_content = (String)publicStaticField_field.get(null);
            String publicNotStaticField_content = (String)publicNotStaticField_field.get(Moon_obj1);
            Field privateStaticField_field = testJavaReClazz3.getDeclaredField("privateStaticField");
            privateStaticField_field.setAccessible(true); //访问私有属性时，记得加上setAccessible，设置它的权限检查
            privateStaticField_field.set(null,"privateStaticField->modified");
            String privateStaticField_content = (String)privateStaticField_field.get(null);
            /* 现在使用getField方法去访问一个私有的属性，会导致程序崩溃
            Field privateField_field = testJavaReClazz3.getField("privateStaticField");
            Log.i("Moonlight","getField->privateStaticField_field-> "+ privateField_field);
            */
            Field[] fields = testJavaReClazz3.getDeclaredFields();
            Field[] fields_2 = testJavaReClazz3.getFields();
            Log.i("Moonlight","publicStaticField_field-> "+ publicStaticField_field);
            Log.i("Moonlight","privateStaticField_field-> "+ privateStaticField_field);
            Log.i("Moonlight","publicStaticField_content-> "+ publicStaticField_content);
            Log.i("Moonlight","publicNotStaticField_content-> "+ publicNotStaticField_content);
            Log.i("Moonlight","privateStaticField_content-> "+ privateStaticField_content);
            for(Field i:fields){
                Log.i("Moonlight","getDeclaredFields()-> "+i);
            }
            for(Field i:fields_2){
                Log.i("Moonlight","getFields()-> "+i);
            }
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        } catch (Error| NoSuchFieldException e) {
            throw new RuntimeException(e);
        } catch (IllegalAccessException e) {
            throw new RuntimeException(e);
        }
    }
    public void TestJavaMethod(){
        Class MoonlightTestClazz = MoonlightTest.class;
        Method[] getDeclaredMethods = MoonlightTestClazz.getDeclaredMethods();
        for(Method i : getDeclaredMethods){
            Log.i("Moonlight","getDeclaredMethods-> "+i);
        }
        Method[] getMethods = MoonlightTestClazz.getMethods();
        for(Method i : getMethods){
            Log.i("Moonlight","getMethods-> "+i);
        }
    }

    /**
     * A native method that is implemented by the 'reflection' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public static native void StaticFun();
    public native int jni_add(int num);
    public native String testStringApi(String content);
}
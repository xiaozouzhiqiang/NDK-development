package com.example.reflection;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.example.reflection.databinding.ActivityMainBinding;

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
        execution_time(); //计算执行java层和jni层相同代码所花费的时间
        testJniAPI(); //测试JNI中的基础数据类型
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
    /**
     * A native method that is implemented by the 'reflection' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public static native void StaticFun();
    public native int jni_add(int num);
    public native String testStringApi(String content);
}
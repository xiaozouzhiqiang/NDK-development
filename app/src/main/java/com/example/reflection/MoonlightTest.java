package com.example.reflection;

import android.util.Log;

public class MoonlightTest {
    public String flag = null;
    public MoonlightTest(){
        flag = "MoonlightTest()";
    }
    public MoonlightTest(String content){
        flag = "public MoonlightTest(String content)";
        Log.i("Moonlight",content);
    }
    public MoonlightTest(String content,int arg){
        flag = "public MoonlightTest(String content,int arg)";
    }
    public static String publicStaticField = "I am public StaticField";
    public String publicNotStaticField = "I am public Not StaticField";

    private static String privateStaticField = "I am private StaticField";
    private  String privateNotStaticField = "I am private Not StaticField";

    public static void publicStaticFunction(){
        Log.i("Moonlight","I am from publicStaticFunction");
    }
    public void publicNotStaticFunction(){
        Log.i("Moonlight","I am from publicNotStaticFunction");
    }

    private static void privateStaticFunction(){
        Log.i("Moonlight","I am from privateStaticFunction");
    }
    private void privateNotStaticFunction(){
        Log.i("Moonlight","I am from privateNotStaticFunction");
    }
}

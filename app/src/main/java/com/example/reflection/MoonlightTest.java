package com.example.reflection;

import android.util.Log;

public class MoonlightTest {
    public String flag = null;
    public int[] intArray = null;
    public MoonlightTest(){
        flag = "MoonlightTest()";
    }
    public MoonlightTest(String content){
        flag = "public MoonlightTest(String content)";
        intArray = new int[10];
        for (int i = 0; i < 10; i++) {
            intArray[i] = i;
        }
    }
    public MoonlightTest(String content,int arg){
        flag = "public MoonlightTest(String content,int arg)";
    }
    public static String publicStaticField = "I am public StaticField";
    public String publicNotStaticField = "I am public Not StaticField";

    public static int publicStaticIntField = 100;
    public int publicNonStaticIntField = 100;

    private static int privateStaticIntField = 300;
    private int privateNonStaticIntField = 400;

    private static String privateStaticField = "I am private StaticField";
    private String privateNotStaticField = "I am private Not StaticField";

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

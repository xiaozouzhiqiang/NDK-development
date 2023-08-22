package com.example.reflection;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
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
    }

    /**
     * A native method that is implemented by the 'reflection' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public static native void StaticFun();

}
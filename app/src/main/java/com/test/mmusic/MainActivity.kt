package com.test.mmusic

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.test.myplayer.Demo
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    lateinit var demo: Demo

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        demo = Demo()
        sample_text.text=  demo.stringFromJNI()
    }

}

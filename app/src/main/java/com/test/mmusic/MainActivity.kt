package com.test.mmusic

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.test.myplayer.log.MyLog
import com.test.myplayer.player.WlPlayer
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    lateinit var mWlPlayer: WlPlayer

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mWlPlayer = WlPlayer()

        mBtnStart.setOnClickListener {
            begin()
        }
    }

    fun begin() {
        mWlPlayer.setSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3")
        mWlPlayer.parpared()
        mWlPlayer.setmWlOnParparedListener{
            MyLog.d("调用成功")
            mWlPlayer.start()
        }
    }
}

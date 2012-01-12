/*
 *
 */
package com.strom.irrlicht.rabbit;

import java.io.IOException;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

/** */
public class IrrlichtTest extends Activity {

	public static final String SETTING_INFO = "Setting_Info";
	public static final String IS_COPY = "Is_Copy";

	private IrrlichtGLView mGLView;
	private RokonAudio mRokonAudio;
    static SoundFile mExploreSound;
    private SoundFile mExploreSound1;
    private SoundFile mFireSound;
    private AssetManager mAssetManager;

    private SensorManager mSensorManager;
    private Sensor mSensor;

    /*
    以屏幕的左下方为原点（2d编程的时候，是以屏幕左上方为原点的，这个值得注意一下），箭头指向的方向为正。从-10到10，以浮点数为等级单位，想象一下以下情形：
    手机屏幕向上（z轴朝天）水平放置的时侯，（x，y，z）的值分别为（0，0，10）；
    手机屏幕向下（z轴朝地）水平放置的时侯，（x，y，z）的值分别为（0，0，-10）；
    手机屏幕向左侧放（x轴朝天）的时候，（x，y，z）的值分别为（10，0，0）；
    手机竖直（y轴朝天）向上的时候，（x，y，z）的值分别为（0，10，0）；
    其他的如此类推，规律就是：朝天的就是正数，朝地的就是负数。利用x,y,z三个值求三角函数，就可以精确检测手机的运动状态了。
    */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        SharedPreferences setting_info = getSharedPreferences(SETTING_INFO,0);
//        String Is_Copy = setting_info.getString(IS_COPY,"");
//
//        if(Is_Copy != "true"){
//        	 Utils.copyFileFromSdcard(getAssets());
//        	 setting_info.edit().putString(IS_COPY, "true");
//        }

       // RokonMusic.play(this, "sounds/ingame_loog.ogg", false);

        mRokonAudio = new RokonAudio();
        mRokonAudio.setMasterVolume(7);

        mExploreSound = mRokonAudio.createSoundFile(this, "sounds/cannon3.ogg");
//        mExploreSound1 = mRokonAudio.createSoundFile(this,"sounds/cannon2.mp3");
//        mFireSound = mRokonAudio.createSoundFile(this, "sounds/fire.ogg");

        mAssetManager = getAssets();

        mSensorManager = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
        mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        /*TYPE注解
        intTYPE_ACCELEROMETER 加速度
                intTYPE_ALL 所有类型，NexusOne默认为 加速度
                intTYPE_GYROSCOPE 回转仪(这个不太懂)
                intTYPE_LIGHT 光线感应
                intTYPE_MAGNETIC_FIELD 磁场
                intTYPE_ORIENTATION 定向（指北针）和角度
                intTYPE_PRESSUR 压力计
                intTYPE_PROXIMITY 距离
                intTYPE_TEMPERATURE 温度
        */

        Utils.copyFileFromSdcard(getAssets());

        try {
            Utils.unpackOnSdCard(getAssets());
        } catch (IOException e) {
            Log.i("Irrlicht", "Error in unpack");
        }
        nativeEnvJ2C(Environment.getExternalStorageDirectory().getAbsolutePath());
        mGLView = new IrrlichtGLView(this);
        setContentView(mGLView);
        nativeCreateAssetManager(mAssetManager);
        nativeOnCreate();
    }

    public static void playSound(int i){
    	mExploreSound.play();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
        nativeOnPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager.registerListener(mListener, mSensor, SensorManager.SENSOR_DELAY_GAME);
        /*
        SENSOR_DELAY_FASTEST 最灵敏，快的然你无语
        SENSOR_DELAY_GAME 游戏的时候用这个，不过一般用这个就够了，和上一个很难看出区别
        SENSOR_DELAY_NORMAL 比较慢。
        SENSOR_DELAY_UI 最慢的，几乎就是横和纵的区别
        */
        mGLView.onResume();
        nativeOnResume();
    }

    @Override
    public void onDestroy() {
        nativeOnDestroy();
        RokonMusic.end();
        super.onDestroy();
    }

    @Override
	protected void onStop() {
		// TODO Auto-generated method stub
    	//finish();
    	mSensorManager.unregisterListener(mListener);
		super.onStop();
	}

    private final SensorEventListener mListener = new SensorEventListener() {

        private final float[] mScale = new float[] { 2, 2.5f, 0.5f };   // accel
        private final float[] mPrev = new float[3];
        private long mLastGestureTime;

        public void onSensorChanged(SensorEvent event) {
            boolean show = false;
            float[] diff = new float[3];

            diff[0] = event.values[0];
            diff[1] = event.values[1];
            diff[2] = event.values[2];

            nativeSendSensorEvent(diff);


            for (int i = 0; i < 3; i++) {
                diff[i] = Math.round(mScale[i] * (event.values[i] - mPrev[i]) * 0.45f);
                if (Math.abs(diff[i]) > 0) {
                    show = true;
                }
                mPrev[i] = event.values[i];
            }

            if (show) {
                // only shows if we think the delta is big enough, in an attempt
                // to detect "serious" moves left/right or up/down
                Log.i("IrrlichtTest", "sensorChanged " + event.sensor.getName() +
                        " (" + event.values[0] + ", " + event.values[1] + ", " +
                        event.values[2] + ")" + " diff(" + diff[0] +
                        " " + diff[1] + " " + diff[2] + ")");
            }

            long now = android.os.SystemClock.uptimeMillis();
            if (now - mLastGestureTime > 1000) {
                mLastGestureTime = 0;

                float x = diff[0];
                float y = diff[1];
                boolean gestX = Math.abs(x) > 3;
                boolean gestY = Math.abs(y) > 3;

                if ((gestX || gestY) && !(gestX && gestY)) {
                    if (gestX) {
                        if (x < 0) {
                            Log.i("test", "<<<<<<<< LEFT <<<<<<<<<<<<");
                        } else {
                            Log.i("test", ">>>>>>>>> RITE >>>>>>>>>>>");
                        }
                    } else {
                        if (y < -2) {
                            Log.i("test", "<<<<<<<< UP <<<<<<<<<<<<");
                        } else {
                            Log.i("test", ">>>>>>>>> DOWN >>>>>>>>>>>");
                        }
                    }
                    mLastGestureTime = now;
                }
            }
        }

        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        }
    };


    /** load irrlicht.so */
    static {
        System.loadLibrary("irrlicht");
    }

    public native void nativeOnCreate();
    public native void nativeOnPause();
    public native void nativeOnResume();
    public native void nativeOnDestroy();

    public native void nativeInitGL();
    public native void nativeResize(int w, int h);

    public native void nativeGetStatus(IrrlichtStatus status);
    public native void nativeSendEvent(IrrlichtEvent event);
    public native void nativeEnvJ2C(String sdcardPath);
    public native void nativeCreateAssetManager(AssetManager assetManager);
    public native void nativeSendSensorEvent(float[] event);

    public static native void nativeDrawIteration();

}


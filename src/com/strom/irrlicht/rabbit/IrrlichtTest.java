/*
 *
 */
package com.strom.irrlicht.rabbit;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.MotionEvent;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/** */
public class IrrlichtTest extends Activity {

	public static final String SETTING_INFO = "Setting_Info";
	public static final String IS_COPY = "Is_Copy";
	
    @Override
	protected void onStop() {
		// TODO Auto-generated method stub
    	//finish();
		super.onStop();
	}

	private IrrlichtGLView mGLView;
	RokonAudio rokonAudio;
    static SoundFile exploreSound;
    SoundFile exploreSound1;
    SoundFile fireSound;
    AssetManager assetManager;

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
    
        rokonAudio = new RokonAudio();
        rokonAudio.setMasterVolume(7);
        
        exploreSound = rokonAudio.createSoundFile(this, "sounds/cannon3.ogg");
//        exploreSound1 = rokonAudio.createSoundFile(this,"sounds/cannon2.mp3");
//        fireSound = rokonAudio.createSoundFile(this, "sounds/fire.ogg");

        assetManager = getAssets(); 
        Utils.copyFileFromSdcard(getAssets());
        
        try {
            Utils.unpackOnSdCard(getAssets());
        } catch (IOException e) {
            Log.i("Irrlicht", "Error in unpack");
        }
        nativeEnvJ2C(Environment.getExternalStorageDirectory().getAbsolutePath());
        mGLView = new IrrlichtGLView(this);
        setContentView(mGLView);
        nativeCreateAssetManager(assetManager);
        nativeOnCreate();
    }

    public static void playSound(int i){
    	exploreSound.play();
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
        mGLView.onResume();
        nativeOnResume();
    }

    @Override    
    public void onDestroy() {
        nativeOnDestroy();
        RokonMusic.end();
        super.onDestroy();
    }
    
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
    
    public static native void nativeDrawIteration();
    
}


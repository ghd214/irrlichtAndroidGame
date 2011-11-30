package com.strom.irrlicht.rabbit;

import java.util.HashSet;

import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;

/**
 * @author Richard Taylor
 * This is optimized for short sound files, which must be played quickly
 * such as sound effects in games. This is not suitable for music, and a 
 * relatively low file size limit exists.
 * 
 * The RokonAudio class uses SoundPool to manage its sounds.
 *
 */
public class RokonAudio {

	public static final int MAX_SOUNDS = 50;
	public static final int MAX_STREAMS = 5;
	
	public static RokonAudio singleton;
	private int i, j;
	
	private float _masterVolume;
	private SoundPool _soundPool;
	private SoundFile[] soundArr = new SoundFile[MAX_SOUNDS];
	
	public RokonAudio() {
		RokonAudio.singleton = this;
		_soundPool = new SoundPool(MAX_STREAMS, AudioManager.STREAM_MUSIC, 0);
		_masterVolume = 1;
	}
	
	/**
	 * Frees up some memory, this should be called when you are finished.
	 */
	public void destroy() {
		try {
			for(i = 0; i < MAX_STREAMS; i++)
				soundArr[i].unload();
			_soundPool.release();
		} catch (Exception e) { }
		_soundPool = null;
	}
	
	/**
	 * @return a HashSet of all current SoundFile's 
	 */
	public SoundFile[] getSounds() {
		return soundArr;
	}
	
	/**
	 * @return the SoundPool object currently being used
	 */
	public SoundPool getSoundPool() {
		return _soundPool;
	}
	
	/**
	 * Loads a file from the /assets/ folder in your APK, ready to be played
	 * @param filename
	 * @return 
	 */
	public SoundFile createSoundFile(Context activity, String filename) {
		try {
			int id = _soundPool.load(activity.getAssets().openFd(filename), 0);
			SoundFile soundFile = new SoundFile(id);
			//Debug.print("SoundFile loaded as id=" + id);
			j = -1;
			for(i = 0; i < MAX_SOUNDS; i++)
				if(soundArr[i] == null)
					j = i;
			if(j == -1) {
			//	Debug.print("TOO MANY SOUNDS");
				System.exit(0);
			}
			soundArr[j] = soundFile;
			return soundFile;
		} catch (Exception e) {
			//Debug.print("CANNOT FIND " + filename + " IN ASSETS");
			e.printStackTrace();
			return null;
		}
	}
	
	/**
	 * @param soundFile SoundFile to be removed from the memory
	 */
	public void removeSoundFile(SoundFile soundFile) {
		_soundPool.unload(soundFile.getId());
		for(i = 0; i < MAX_SOUNDS; i++)
			if(soundArr[i] != null)
				if(soundArr[i].equals(soundFile))
					soundArr[i] = null;
	}
	
	/**
	 * Removes all SoundFile's from the memory
	 */
	public void removeAllSoundFiles() {
		for(i = 0; i < MAX_SOUNDS; i++)
			soundArr[i] = null;
		_soundPool.release();
	}
	
	/**
	 * @param masterVolume the volume at which all future AudioStream's will play
	 */
	public void setMasterVolume(float masterVolume) {
		_masterVolume = masterVolume;
	}
	
	/**
	 * @return the current volume at which AudioStream's will play at
	 */
	public float getMasterVolume() {
		return _masterVolume;
	}

}

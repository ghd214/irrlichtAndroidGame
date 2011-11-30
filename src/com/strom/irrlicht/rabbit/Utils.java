package com.strom.irrlicht.rabbit;

import java.io.*;
import java.math.BigInteger;
import java.net.HttpURLConnection;
import java.net.URL;
import java.net.URLConnection;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.zip.GZIPInputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.os.Environment;


/** Utils functions. */
public class Utils {

	public static void copyFileFromSdcard(AssetManager assetManager){
       // AssetManager assetManager = getAssets();  
        String[] files = null;  
        try 
        {  
             files = assetManager.list("");  
        }catch (IOException e) 
        {  
             Log.e("", e.getMessage());  
        }  
        String strSdcardPath = "/sdcard/irrlicht/"; 
        String strDestFile; 
        int iFileCnt = files.length; 
        for( int i = 0; i < iFileCnt; i++) 
        { 
           String strFileNameSrc = files[i]; 
           if( strFileNameSrc.contentEquals("images"  ) || 
                 strFileNameSrc.contentEquals("sounds" ) || 
                 strFileNameSrc.contentEquals("skins" ) || 
                 strFileNameSrc.contentEquals("webkit" ) ) 
           { 
               
              // dont copy.. not really a asset file 
              continue; 
           } 
           strDestFile = strSdcardPath + strFileNameSrc; 
           VxCopyAsssetToSdcard(strFileNameSrc, strDestFile, assetManager,true);   
        
        }
	}
	
	/*////////////////////////////
	private AssetManager getAssets() {
		// TODO Auto-generated method stub
		return null;
	}
	*/
	static int VxCopyAsssetToSdcard( String strFileSrc, String strFileDest,
												AssetManager assetmanager,boolean bDontCopyIfExistsAndSameSize ) 
	{ 
	  int iSrcFileSize = 0; 
	  int iDestFileSize = 0; 
	  InputStream oInputStream = null; 
	  OutputStream oOutputStream = null; 
	  // get src file size and stream 
	  try 
	  { 
	     oInputStream = assetmanager.open(strFileSrc); 
	       if( null != oInputStream) 
	       { 
	          iSrcFileSize = oInputStream.available(); 
	       } 
	       else 
	       { 
	            Log.e("", "ERROR: VxCopyResourceToSdcard could not open file " + strFileSrc );  
	          return -1; 
	       } 
	  } 
	  catch (IOException e) 
	  { 
	        Log.e("", "ERROR: VxCopyResourceToSdcard could not open file " + strFileSrc );  
	     // TODO Auto-generated catch block 
	     e.printStackTrace(); 
	      return -1; 
	  } 
	  if( bDontCopyIfExistsAndSameSize ) 
	  { 
	     // get dest file size and stream 
	     try 
	     { 
	        InputStream oTmpStream = null; 
	        File readFile = new File( strFileDest); 
	        oTmpStream = new FileInputStream(readFile);  ; 
	          if( null != oTmpStream) 
	          { 
	             iDestFileSize = oTmpStream.available(); 
	             oTmpStream.close(); 
	          } 
	          else 
	          { 
	               Log.e("", "ERROR: VxCopyAsssetToSdcard could not get size of file " + strFileDest );  
	             return -1; 
	          } 
	     } 
	     catch (IOException e) 
	     { 
	           Log.e("", "VxCopyAsssetToSdcard dest file doesnt exist.. will copy " + strFileDest );  
	     } 
	  } 
	  if( bDontCopyIfExistsAndSameSize && ( iSrcFileSize == iDestFileSize)) 
	  { 
	        Log.i("", "VxCopyAsssetToSdcard file exists " + strFileDest ); 
	        return 0; 
	  } 
	    // Read the entire asset into a local byte buffer. 
	    byte[] buffer = new byte[iSrcFileSize]; 
	    try 
	    { 
	     oInputStream.read(buffer); 
	       oInputStream.close(); 
	  } 
	    catch (IOException e) 
	    { 
	        Log.e("", "ERROR: VxCopyAsssetToSdcard could not read file " + strFileSrc );  
	     // TODO Auto-generated catch block 
	     e.printStackTrace(); 
	  } 
	  // write buffer to destination file 
	  try 
	  { 
	     File oWriteFile = new File( strFileDest); 
	     try 
	     {  
	        oWriteFile.createNewFile();  
	     } 
	     catch (IOException e) 
	     {  
	        Log.e("", "error creating file " + strFileDest, e); 
	           e.printStackTrace(); 
	        return -1; 
	     } 
	     oOutputStream = new FileOutputStream(oWriteFile);  
	     //Don’t forget to close stream 
	     if (oOutputStream != null) 
	     {  
	         try 
	         {  
	           oOutputStream.write(buffer); 
	            oOutputStream.flush();  
	            oOutputStream.close();  
	            Log.i("", "Success copying file " + strFileDest + " size " + iSrcFileSize ); 
	         } 
	         catch (IOException e) 
	         {  
	               Log.e("", "ERROR: VxCopyResourceToSdcard could not write file " + strFileDest );  
	            e.printStackTrace(); 
	             return -1; 
	         }  
	     } 
	  } 
	  catch (IOException e) 
	  { 
	     Log.e("", "ERROR: VxCopyAsssetToSdcard could not read file " + strFileSrc );  
	     // TODO Auto-generated catch block 
	     e.printStackTrace(); 
	     return -1; 
	  } 
	  return 0; 
	} 
	
	
    /** open file */
    public static FileInputStream tryToGetLocalFile(Context ctx, String filename) {
        try {
            FileInputStream fin = ctx.openFileInput(filename);
            return fin;
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }

    /** svg image on sdcard */
    public static void unpackOnSdCard(AssetManager assetManager) throws IOException {
        if (Environment.getExternalStorageState().compareTo(Environment.MEDIA_MOUNTED)==0) {
            File sdcard = Environment.getExternalStorageDirectory();
            String irrlichtPath = sdcard.getAbsoluteFile() + "/Irrlicht/";
            File irrlichtDir = new File(irrlichtPath);
            if (irrlichtDir.exists() && !irrlichtDir.isDirectory()) {
                throw new IOException("Irrlicht exists and is not a directory on SD Card");
            } else if (!irrlichtDir.exists()) {
                irrlichtDir.mkdirs();
            }
            // Note: /sdcard/irrlicht dir exists
            String[] filenames = assetManager.list("data");
            for(String filename:filenames) {
                InputStream inputStream = assetManager.open("data/" + filename);
                OutputStream outputStream = new FileOutputStream(irrlichtPath + "/" + filename);
                // copy
                byte[] buffer = new byte[4096];
                int length;
                while ( (length = inputStream.read(buffer)) > 0 ) {
                    outputStream.write(buffer, 0, length);
                }
                outputStream.flush();
                outputStream.close();
                inputStream.close();
            }           
        } else {
            throw new IOException("SD Card not available");
        }

    }

    public static boolean isSdCardAvailable() {
        if (Environment.getExternalStorageState().compareTo(Environment.MEDIA_MOUNTED)==0) {
            return true;
        } else {
            return false;
        }
    }


    /** compute md5 */
    public static String getMd5Hash(String input) {
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            byte[] messageDigest = md.digest(input.getBytes());
            BigInteger number = new BigInteger(1, messageDigest);
            String md5 = number.toString(16);
            while (md5.length() < 32) {
                md5 = "0" + md5;
            }
            return md5;
        } catch (NoSuchAlgorithmException e) {
            Log.e("MD5", e.getMessage());
            return null;
        }
    }

    /** read binary input stream */
    private static byte[] readBinaryInput(InputStream in) throws IOException {
        ByteArrayOutputStream bo = new ByteArrayOutputStream();
        byte[] tmpStorage = new byte[4096];
        int pos = 0;
        for (int n; (n = in.read(tmpStorage)) != -1;) {
            bo.write(tmpStorage, 0, n);
            pos += n;
        }
        return bo.toByteArray();
    }

    /** read string input stream */
    private static String readStringInput(InputStream in) throws IOException {
        StringBuilder sb = new StringBuilder();

        byte[] buffer = new byte[4096];
        for (int n; (n = in.read(buffer)) != -1;) {
            sb.append(new String(buffer, 0, n));
        }
        return sb.toString();
    }

}

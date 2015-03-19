package com.pegas.pacman;

import android.app.NativeActivity;

public class MyNativeActivity extends NativeActivity {
	static {
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("game");
	}
}

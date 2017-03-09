package com.example.jack.android_client;

import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Point;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

public class DrawActivity extends AppCompatActivity {

   private String addr;
   private int   port;

   private TextView xCrd;
   private TextView yCrd;
   private TextView LED;
   private TextView gradient;
   private SeekBar seekBar;
   private Button buttonBack;

   private boolean skip;
   private int demo;


   public int getDisplayContentHeight() {
      final WindowManager windowManager = getWindowManager();
      final Point size = new Point();
      int screenHeight = 0, actionBarHeight = 0;
      if (getActionBar() != null) {
         actionBarHeight = getActionBar().getHeight();
      }
      int contentTop = ((ViewGroup) findViewById(android.R.id.content)).getTop();
      if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
         windowManager.getDefaultDisplay().getSize(size);
         screenHeight = size.y;
      } else {
         Display d = windowManager.getDefaultDisplay();
         screenHeight = d.getHeight();
      }
      return screenHeight - contentTop - actionBarHeight;
   }

   @Override
   protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_draw);

      xCrd = (TextView)findViewById(R.id.xCrd);
      yCrd = (TextView)findViewById(R.id.yCrd);
      LED  = (TextView)findViewById(R.id.LED);
      gradient = (TextView)findViewById(R.id.gradient);
      buttonBack = (Button)findViewById(R.id.buttonBack);

      Intent intent = getIntent();
      addr = intent.getStringExtra("addr");
      port = intent.getIntExtra("port", 20);
      demo = intent.getIntExtra("demo", -1);
      skip = intent.getBooleanExtra("skip", true);

      System.out.println(addr + ' ' + port + ' ' + demo + ' ' + skip);

      buttonBack.setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            Intent intent = new Intent(DrawActivity.this, ConnectActivity.class);
            startActivity(intent);
         }
      });

      final View touchView = findViewById(R.id.activity_draw);
      touchView.setOnTouchListener(new View.OnTouchListener() {
         @Override
         public boolean onTouch(View v, MotionEvent event) {
            final int action = event.getAction();
            UDPClient myClient;

            int width  = Resources.getSystem().getDisplayMetrics().widthPixels;
            //int height = Resources.getSystem().getDisplayMetrics().heightPixels;
            int height = getDisplayContentHeight();

            int currXCrd = (int)event.getX();
            int currYCrd = (int)event.getY();

            switch (action) {
               case MotionEvent.ACTION_DOWN:
                  xCrd.setText(String.valueOf(currXCrd));
                  yCrd.setText(String.valueOf(currYCrd));
                  LED.setText("LED: ON");
                  if(!skip) { // && currXCrd - prevXCrd > DIFFERENCE || prevYCrd - currYCrd > DIFFERENCE) {
                     myClient = new UDPClient(addr,
                                              port,
                                              width,
                                              height,
                                              (int) event.getX() < 0 ? 0 : (int) event.getX(),
                                              (int) event.getY() < 0 ? 0 : (int) event.getY(),
                                              1,
                                              seekBar.getProgress());
                     myClient.execute();
                  }
                  break;
               case MotionEvent.ACTION_MOVE:
                  xCrd.setText(String.valueOf((int) event.getX()));
                  yCrd.setText(String.valueOf((int) event.getY()));
                  if(!skip) { // && currXCrd - prevXCrd > DIFFERENCE || prevYCrd - currYCrd > DIFFERENCE) {
                     myClient = new UDPClient(addr,
                                              port,
                                              width,
                                              height,
                                              (int) event.getX() < 0 ? 0 : (int) event.getX(),
                                              (int) event.getY() < 0 ? 0 : (int) event.getY(),
                                              1,
                                              seekBar.getProgress());
                     myClient.execute();
                  }
                  break;
               case MotionEvent.ACTION_UP:
                  xCrd.setText(String.valueOf((int) event.getX()));
                  yCrd.setText(String.valueOf((int) event.getY()));
                  LED.setText("LED: OFF");
                  if(!skip) { //&& currXCrd - prevXCrd > DIFFERENCE || prevYCrd - currYCrd > DIFFERENCE) {
                     myClient = new UDPClient(addr,
                                              port,
                                              width,
                                              height,
                                              (int) event.getX() < 0 ? 0 : (int) event.getX(),
                                              (int) event.getY() < 0 ? 0 : (int) event.getY(),
                                              0,
                                              seekBar.getProgress());
                     myClient.execute();
                  }
                  break;
            } // end-switch()
            return true;
         }
      });

      seekBar = (SeekBar)findViewById(R.id.seekBar1);
      seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
         @Override
         public void onStopTrackingTouch(SeekBar seekBar) {
         }

         @Override
         public void onStartTrackingTouch(SeekBar seekBar) {
         }

         @Override
         public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            int value = seekBar.getProgress();
            gradient.setText(String.valueOf(value));
         }
      });
   }
}
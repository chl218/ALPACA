package com.example.jack.android_client;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class ConnectActivity extends AppCompatActivity {

   private EditText editTextAddr;
   private EditText editTextPort;

   private Button buttonConnect;
   private Button buttonSkip;

   private Button buttonDemo1;
   private Button buttonDemo2;
   private Button buttonDemo3;

   private EditText editTextDemo1;
   private EditText editTextDemo2;
   private EditText editTextDemo3;


   private String getAddress() {
      String addr;
      try {
         addr = editTextAddr.getText().toString();
      }
      catch(Exception e) {
         addr = "127.0.0.1";
      }

      return addr;
   }

   private int getPort() {
      int port;
      try {
         port = Integer.parseInt(editTextPort.getText().toString());
      }
      catch (Exception e) {
         port = 22;
      }
      if(port < 0) port = 22;

      return port;
   }

   private int getDuration(EditText et) {
      int duration;
      try {
         duration =  Integer.parseInt(et.getText().toString());
      }
      catch (Exception e) {
         duration = 1;
      }
      if(duration < 0) duration = 1;

      return duration;
   }

   void promptError() {
      Context context = getApplicationContext();
      CharSequence text = "Network Error";
      int duration = Toast.LENGTH_SHORT;

      Toast toast = Toast.makeText(context, text, duration);
      toast.show();
   }

   @Override
   protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_connect);

      editTextAddr = (EditText)findViewById(R.id.editTextAddr);
      editTextPort = (EditText)findViewById(R.id.editTextPort);

      buttonConnect   = (Button)findViewById(R.id.buttonConnect);
      buttonSkip      = (Button)findViewById(R.id.buttonSkip);

      buttonDemo1 = (Button)findViewById(R.id.buttonDemo1);
      buttonDemo2 = (Button)findViewById(R.id.buttonDemo2);
      buttonDemo3 = (Button)findViewById(R.id.buttonDemo3);

      editTextDemo1 = (EditText)findViewById(R.id.editTextDemo1);
      editTextDemo2 = (EditText)findViewById(R.id.editTextDemo2);
      editTextDemo3 = (EditText)findViewById(R.id.editTextDemo3);

      // start draw activity
      buttonConnect.setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            String addr = getAddress();
            int    port = getPort();

            Intent intent = new Intent(ConnectActivity.this, DrawActivity.class);
            intent.putExtra("skip", false);
            intent.putExtra("addr", addr);
            intent.putExtra("port", port);
            startActivity(intent);
         }
      });

      // demo without internet
      buttonSkip.setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            Intent intent = new Intent(ConnectActivity.this, DrawActivity.class);
            intent.putExtra("skip", true);
            startActivity(intent);
         }
      });

      // {address, port, width, height, xCrd, yCrd, LEDStatus, LEDGradient, skipConnect, demoNumber, demoAmount}
      buttonDemo1.setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            String addr = getAddress();
            int    port = getPort();

            int duration = getDuration(editTextDemo1);

            try {
               UDPClient client = new UDPClient(addr, port, -1, -1, -1, -1, -1, -1, 1, duration);
               client.execute();
            }
            catch(Exception e) {
               promptError();
            }
         }
      });
      buttonDemo2.setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            String addr = getAddress();
            int    port = getPort();

            int duration = getDuration(editTextDemo2);

            try {
               UDPClient client = new UDPClient(addr, port, -1, -1, -1, -1, -1, -1, 2, duration);
               client.execute();
            }
            catch(Exception e) {
               promptError();
            }
         }
      });
      buttonDemo3.setOnClickListener(new View.OnClickListener() {
         @Override
         public void onClick(View v) {
            String addr = getAddress();
            int    port = getPort();

            int duration = getDuration(editTextDemo3);

            try {
               UDPClient client = new UDPClient(addr, port, -1, -1, -1, -1, -1, -1, 3, duration);
               client.execute();
            }
            catch(Exception e) {
               promptError();
            }
         }
      });
   }

}
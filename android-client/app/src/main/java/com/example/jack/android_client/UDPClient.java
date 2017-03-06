package com.example.jack.android_client;

import android.os.AsyncTask;

import java.net.*;
import java.io.*;

public class UDPClient extends AsyncTask<Void, Void, Void> {

   private String dstAddress;
   private int dstPort;
   private int width;
   private int height;
   private int xCrd;
   private int yCrd;
   private int LED;
   private int LEDGradient;
   private int demo;
   private int amount;

   UDPClient(String addr, int port, int width, int height, int xCrd, int yCrd, int LED, int LEDGradient) {
      this(addr, port, width, height, xCrd, yCrd, LED, LEDGradient, -1, -1);
   }
   UDPClient(String addr, int port, int width, int height, int xCrd, int yCrd, int LED, int LEDGradient,
             int demo, int amount) {
      this.dstAddress  = addr.replaceAll(" ", ""); // remove all white space
      this.dstPort     = port;
      this.width       = width;
      this.height      = height;
      this.xCrd        = xCrd;
      this.yCrd        = yCrd;
      this.LED         = LED;
      this.LEDGradient = LEDGradient;
      this.demo        = demo;
      this.amount      = amount;
   }

   @Override
   protected Void doInBackground(Void... arg0) {

      DatagramSocket socket = null;
      try {
         // Create UDP socket
         socket = new DatagramSocket();
         // Create the message
         String msg = "" + width + " " + height + " " + xCrd + " " + yCrd + " "
                         + LED + " " + LEDGradient + " " + demo + " " + amount;
         // Transform message to byte array due to parameter type
         byte[] send = msg.getBytes();

         // Transform String address type to InetAddress type
         InetAddress inetAddress = InetAddress.getByName(dstAddress);
         // Create UDP packet
         DatagramPacket sendPacket = new DatagramPacket(send, send.length, inetAddress, dstPort);
         // Send the packet
         socket.send(sendPacket);

      }
      catch (Exception e) {
         e.printStackTrace();
      }
      finally {
         if (socket != null) {
            try {
               socket.close();
            } catch (Exception e) {
               e.printStackTrace();
            }
         }
      }
      return null;
   }

   @Override
   protected void onPostExecute(Void result) {
      super.onPostExecute(result);
   }

}
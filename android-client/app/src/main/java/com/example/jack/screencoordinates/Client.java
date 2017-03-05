package com.example.jack.screencoordinates;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import java.net.*;
import java.io.*;

public class Client extends AsyncTask<Void, Void, Void> {

    private final int UNKNOWNHOST = 1;
    private final int UNKNOWNPORT = 2;
    private final String SENDING = "Sending msg";

    private String dstAddress;
    private int dstPort;
    private String response = "";
    private int width;
    private int height;
    private int xCrd;
    private int yCrd;
    private int LED;
    private int LEDGradient;
    private int demo;
    private int amount;

    Client(String addr, int port, int width, int height, int xCrd, int yCrd, int LED, int LEDGradient) {
        this(addr, port, width, height, xCrd, yCrd, LED, LEDGradient, -1, -1);
    }
    Client(String addr, int port, int width, int height, int xCrd, int yCrd, int LED, int LEDGradient,
           int demo, int amount) {
        this.dstAddress  = addr;
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

        Socket socket = null;

        try {
//            System.out.println("Connecting to " + dstAddress + " on port " + dstPort);
            Socket client = new Socket(dstAddress.replaceAll(" ", ""), dstPort);

//            Log.d(CONNECTING, "Just connected to " + client.getRemoteSocketAddress());

            PrintWriter outToServer = new PrintWriter(new OutputStreamWriter(client.getOutputStream()));

//            Log.d(SENDING, "" + width + " " + height + " " + xCrd + " " + yCrd + " "
//                                 + LED + " " + LEDGradient + " " + demo + " " + amount);

            outToServer.println("" + width + " " + height + " " + xCrd + " " + yCrd + " "
                                + LED + " " + LEDGradient + " " + demo + " " + amount);
            outToServer.flush();

//            BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
//            System.out.println("Server Says : " + in.readLine());

        } catch (UnknownHostException e) {
            System.out.println("unknown host");
            e.printStackTrace();
        } catch (IOException e) {
            System.out.println("io");
            e.printStackTrace();
        } finally {
            if (socket != null) {
                try {
                    socket.close();
                } catch (IOException e) {
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
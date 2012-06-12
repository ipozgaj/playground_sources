/* HttpServer.java
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

import java.io.*;
import java.net.*;

public class HTTPServer {
	private static int port;
	private static boolean verbosity;
	private static String basedir;
	private static ServerSocket serverSocket;
	
	public static void printStatus () {
		System.out.println ("Host name: " + serverSocket.getInetAddress().getHostName());
		System.out.println ("Host address: " + serverSocket.getInetAddress());
		System.out.println ("Service port: " + port);
		System.out.println ("Base directory: " + basedir);
		System.out.println ("Listening for client connections...");
	}
		
	public static void main (String[] args) {
		HTTPServerThread serverThread;
			
		/* parse command line parameters */
		if (args.length < 2) {
			System.out.println ("Usage: java HTTPServer [-v] port basedir");
			System.exit (1);
		}

		if (args[0].compareTo ("-v") == 0)
			verbosity = true;

		try {
			if (verbosity) {
				port = new Integer (args[1]).intValue();
				basedir = args[2];
			} else {
				port = new Integer (args[0]).intValue();
				basedir = args[1];
			}
		} catch (Exception e) {
			System.out.println ("Invalid parameters");
			System.out.println ("Usage: java HTTPServer [-v] port basedir");
			System.exit (1);
		}

		try {
			serverSocket = new ServerSocket (port);

			if (verbosity)
				printStatus();

			while (true) {
				serverThread = new HTTPServerThread (serverSocket.accept(), basedir, verbosity);
				serverThread.run();
			}
			
		} catch (BindException e) {
			System.out.println ("You are not root and you are trying to use the privileged port");
			System.exit (1);
		} catch (IOException e) {
			System.out.println (e.toString());
			System.exit (1);
		}
	}
}

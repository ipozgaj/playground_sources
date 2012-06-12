/* implementacija HTTP servera prema RFC1945 
 *
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

import java.io.*;
import java.net.*;
import java.util.*;

public class HTTPServerThread extends Thread {
	private boolean verbose, validRequest;
	private String basedir, requestedFile;
	private Socket clientSocket;
	private BufferedReader inputStream;
	private PrintWriter outputStream;
	private Date d;
	
	public HTTPServerThread (Socket s, String basedir, boolean verbose) {
		this.verbose = verbose;
		this.basedir = basedir;
		clientSocket = s;
		requestedFile = basedir;

		d = new Date (System.currentTimeMillis());

		try {
			inputStream = new BufferedReader (new InputStreamReader (clientSocket.getInputStream()));
			outputStream = new PrintWriter (clientSocket.getOutputStream());
		} catch (IOException e) {
			System.out.println (e.toString());
		}
	}

	public void printStatus() {
		System.out.println ("\nConnect from: " + clientSocket.getInetAddress());
		System.out.println ("Time: " + d);
		System.out.println ("Thread ID: " + this.getName());
		System.out.println ("Requested document: " + requestedFile);
	}

	public void run() {
		String token = "", clientRequest = "";
		StringTokenizer st;
		StringBuffer sb;
		File f;

		validRequest = true;
		
		try {
			clientRequest = inputStream.readLine();
		} catch (IOException e) {
			System.out.println (e.toString());
		}

		st = new StringTokenizer (clientRequest, " ");
		sb = new StringBuffer();

		sb.append (clientRequest);

		try {
			token = st.nextToken();
		} catch (NoSuchElementException e) {
			validRequest = false;
		}
		
		if (token.compareTo ("GET") != 0)
			validRequest = false;
		
		try {
			token = st.nextToken();
		} catch (NoSuchElementException e) {
			validRequest = false;
		}
		
		requestedFile += token;
		token = token.substring (token.indexOf ("/", 0) + 1);

		f = new File (requestedFile);
		
		try {
			token = st.nextToken();
		} catch (NoSuchElementException e) {
			validRequest = false;
		}

		if (!token.startsWith ("HTTP"))
			validRequest = false;

		if (verbose)
			printStatus();

		if (validRequest) {
			try {
				String contentType = null;
				
				BufferedReader br = new BufferedReader (new FileReader (f));
				outputStream.println ("HTTP/1.0 200 OK");
	
				st = new StringTokenizer (f.getName(),".");
				st.nextToken();
				token = st.nextToken();
	
				if (token.compareTo ("html") == 0) {
					outputStream.println ("Content-type: text/html");
					contentType = "Content-type: text/html";
				} else if (token.compareTo ("txt") == 0) {
					outputStream.println ("Content-type: text/plain");
					contentType = "Content-type: text/plain";
				} else if (token.compareTo ("gif") == 0) {
					outputStream.println ("Content-type: image/gif");
					contentType = "Content-type: image/gif";
				} else if (token.compareTo ("jpg") == 0) {
					outputStream.println ("Content-type: image/jpg");
					contentType = "Content-type: image/jpg";
				} else {
					outputStream.println ("Content-type: application/bin");
					contentType = "Content-type: application/bin";
				}
				
				if (verbose) {
					System.out.println (contentType);
					System.out.println ("Status: 200 OK");
				}
				
				StringBuffer buffer = new StringBuffer();
	
				while (br.ready()) {
					token = br.readLine() + "\n";
					buffer.append (token);
				}
				
				outputStream.println ("Connection: Close");
				outputStream.println ("Content-length: " + buffer.length() + "\n");
				outputStream.println (buffer.toString());
	
				br.close();
			} catch (NoSuchElementException e) {
				System.out.println (e.toString());
			} catch (FileNotFoundException e) {
				String htmlBuffer = new String ("<html><head><title>404: Page Not Found</head><body>404: Page Not Found</body></html>");
				outputStream.println ("HTTP/1.0 404 File Not Found ");
				outputStream.println ("Content-type: text/html");
				outputStream.println ("Connection: Close");
				outputStream.println ("Content-length: " + htmlBuffer.length());
				outputStream.println (htmlBuffer);

				if (verbose) {
					System.out.println ("Content-type: text/html");
					System.out.println ("Status: 404 Page Not Found");
				}
			} catch (IOException e) {
				System.out.println ("Socket communication error");
			}
		} else {
			System.out.println ("Content-type: text/html");
			System.out.println ("Status: 400 Bad Request");
		}
		
		try {
			outputStream.flush();
			inputStream.close();
			outputStream.close();
			clientSocket.close();
		} catch (Exception e) {
			System.out.println (e.toString());
		}
	}
}

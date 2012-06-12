/* Labos iz Otvorenog racunarstva
 * (C) 2005  Igor Pozgaj <ipozgaj@fly.srk.fer.hr>
 */

import java.io.*;
import java.awt.Graphics;
import java.applet.Applet;
import fer.apr.rasip.dms.*;
import fer.apr.rasip.gui.*;

public class DataAnalyser extends Applet {
	public static MainFrame mf;
	public static DataSourceSim dss;
	public static MeasuredData md;

	public void init() {
		mf = new MainFrame();
		dss = new DataSourceSim();
	}

	public void start() {
		int i;

		try {
			dss.open();
		} catch (DataSourceException e) {
			System.out.println (e.toString());
			System.exit (1);
		}

    	while (true) {
			try {
				Thread.sleep (2000);
			} catch (Exception e) {
			}

			for (i = 0; i < 16; ++i) {
        		try {
					MeasuredData dsData = dss.getData (i);
					mf.setValue (i, md.getValue());
				} catch (Exception e) {
					System.out.println (e.toString());
				}
			}
		}
	}

	public void stop() {
		try {
			dss.close();
		} catch (Exception e) {
		}
	}

	public void destroy() {
		try {
			dss.close();
		} catch (Exception e) {
		}
	}

	public static void main (String[] args) {
		int i;
		
		MainFrame mf = new MainFrame();
		DataSourceSim dss = new DataSourceSim();
		
		try {
			dss.open();

			while (true) {
				try {
					Thread.sleep (2000);
				} catch (Exception e) {
				}

				for (i = 0; i < 16; ++i) {
					try {
						MeasuredData md = dss.getData(i);
	        			mf.setValue (i, md.getValue());
					} catch (Exception e) {
						System.out.println (e.toString());
					}
				}
			}
		} catch (DataSourceException e) {
				System.out.println (e.toString());
				System.exit (1);
		}
	}
}

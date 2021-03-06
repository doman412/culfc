package com.arnerdl;

import java.awt.Color;
import java.awt.EventQueue;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;

import org.math.plot.Plot2DPanel;

public class App {
	private final static String version = "2.0";
	private static StreamConnection con;
	private static BufferedWriter writer;
	private static BufferedReader reader;
	//private static boolean lineCapture = false;
	
	private static File f;
	private static FileWriter fr;
	private static BufferedWriter br;
	
	final static JCheckBox checkSave = new JCheckBox("Save?");
	final static Plot2DPanel plot = new Plot2DPanel();
	
	private JFrame frmAutobotsControlPanel;
	
	private static AutobotsLog log;
	
	boolean goToggle = true;
	/**
	 * Launch the application.
	 * 
	 * 
	 */
	
	
	static {
		try {
		    for (LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()) {
		    	if ("Windows".equals(info.getName())) {
		            UIManager.setLookAndFeel(info.getClassName());
		            break;
		        }
		    }
		} catch (Exception e) {
		    // If Nimbus is not available, you can set the GUI to another look and feel.
		}	}
	
	public static void main(String[] args) {
			
		try {
			con = (StreamConnection)Connector.open("btspp://000666006C08:1");
			writer = new BufferedWriter(new OutputStreamWriter(con.openOutputStream()));
			reader = new BufferedReader(new InputStreamReader(con.openInputStream()));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
//			reader = con.openDataInputStream();

		f = new File("line_capture.csv");
		if(f.exists()){
			f.delete();
		}
		
		try {
			f.createNewFile();
			fr = new FileWriter(f);
		} catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		br = new BufferedWriter(fr);
		
		
/*		(new Thread(new Runnable(){
			public void run() {
				// TODO Auto-generated method stub
				while(true){
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					try {
						con = (StreamConnection)Connector.open("btspp://000666006C08:1");
						writer = new BufferedWriter(new OutputStreamWriter(con.openOutputStream()));
						reader = new BufferedReader(new InputStreamReader(con.openInputStream()));
						log.print("New connection\n");
					} catch (IOException e) {
						// TODO Auto-generated catch block
						//System.err.println("Already Connected");
					}
					
				}	
			}				
		})).start();*/
			
		(new Thread(new Runnable() {
			public void run() {
				DoubleBuffer db = DoubleBuffer.allocate(128);
				CharBuffer cb = CharBuffer.allocate(128);
				byte error = 0;
				String line = null;
				plot.addLinePlot("camera data", new double[128]);
				while(true){
					try {
						if(reader.ready()) {
							line = reader.readLine();
							//if (line == null) {
							//	break;
							//}
							if(line.equals("l")){
								//log.print("inside!\n");
								//log.print(line+"\n");
								cb.clear();
								while(cb.length()>0){
									reader.read(cb);
								}
								//log.print("send done\n");
								db.clear();
								for(char c : cb.array()){
									db.put(c*2);
									if(checkSave.isSelected())
										br.write((double)(c*2)+",");
								}
//								if(checkSave.isSelected())
//									br.write("\n");
								
								plot.removeAllPlots();
								plot.addLinePlot("camera data", db.array());
								plot.setFixedBounds(0, 0, 128);
								plot.setFixedBounds(1, 0, 255);	
								//plot.changePlotData(0, db.array());
								
							}
							else if(line.equals("e")){
								while(!reader.ready());
								error = (byte)reader.read();
								error +=63;
								if(checkSave.isSelected())
									br.write(error+"\n");
								//System.out.println(error);
								plot.addLinePlot("error", Color.RED, new double[][]{{error,0},{error,255}});
								plot.setFixedBounds(0, 0, 128);
								plot.setFixedBounds(1, 0, 255);
							}
							else{
								//log.print(String.valueOf(line.length())+"\n");
								//if(line.length()>1){
								//String sub = line.substring(0, 2);
								//for(char c: sub.toCharArray()){
								//	log.print( String.format("%d",Character.getNumericValue(c)) );
								//}
								//log.print("\n");
								//}
								log.print(line+"\n");
							}
						}
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
		})).start();
			
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					App window = new App();
					window.frmAutobotsControlPanel.setVisible(true);
					((JPanel)window.frmAutobotsControlPanel.getContentPane()).grabFocus();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public App() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frmAutobotsControlPanel = new JFrame();
		frmAutobotsControlPanel.setTitle("Optimus Prime's Control Panel");
		frmAutobotsControlPanel.setBounds(100, 100, 480, 640);
		frmAutobotsControlPanel.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmAutobotsControlPanel.getContentPane().setLayout(new GridLayout(3, 1, 0, 0));
		frmAutobotsControlPanel.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e) {
				try {
					fr.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		});
		
		JPanel panel = new JPanel();
		frmAutobotsControlPanel.getContentPane().add(panel);
		panel.setLayout(new GridLayout(0, 2, 0, 0));
		((JPanel)frmAutobotsControlPanel.getContentPane()).grabFocus();
		
		JPanel left_col = new JPanel();
		JPanel right_col = new JPanel();
		panel.add(left_col);
		panel.add(right_col);
		left_col.setLayout(new GridLayout(0, 2, 0, 0));
		right_col.setLayout(new GridLayout(0, 2, 0, 0));
		
		final JTextField servoText = new JTextField();
		final JTextField txtDrive = new JTextField();
		final JTextField txtLMotor = new JTextField();
		final JTextField txtRMotor = new JTextField();
		//final JTextField txtMode = new JTextField();
		final JTextField txtGet = new JTextField();
		final JTextField txtParm1 = new JTextField("kp");
		final JTextField txtData1 = new JTextField();
		final JTextField txtParm2 = new JTextField("ki");
		final JTextField txtData2 = new JTextField();
		final JTextField txtParm3 = new JTextField("kd");
		final JTextField txtData3 = new JTextField();
		final JTextField txtCmd = new JTextField();
		final JTextField txtArgs = new JTextField();
		
		JButton btnServo = new JButton("Servo");
		JButton btnDrive = new JButton("Drive");
		JButton btnLMotor = new JButton("Left Motor");
		JButton btnRMotor = new JButton("Right Motor");
		final JButton btnGo = new JButton("Go");
		final JButton btnStop = new JButton("Stop");
		JButton btnMode = new JButton("Mode");
		JButton btnGet = new JButton("Get");
		JButton btnSet = new JButton("Set");
		JButton btnCmd = new JButton("Custom Commad");
		
		left_col.add(btnServo);
		left_col.add(servoText);
		left_col.add(btnDrive);
		left_col.add(txtDrive);
		left_col.add(btnLMotor);
		left_col.add(txtLMotor);
		left_col.add(btnRMotor);
		left_col.add(txtRMotor);
		left_col.add(btnCmd);
		left_col.add(new JPanel());
		left_col.add(txtCmd);
		left_col.add(txtArgs);
		right_col.add(btnGo);
		right_col.add(btnStop);
		right_col.add(btnMode);
		//right_col.add(new JPanel());
		right_col.add(checkSave);
		//right_col.add(new JPanel());
		right_col.add(btnGet);
		right_col.add(txtGet);
		right_col.add(btnSet);
		right_col.add(new JPanel());
		right_col.add(txtParm1);
		right_col.add(txtData1);
		right_col.add(txtParm2);
		right_col.add(txtData2);
		right_col.add(txtParm3);
		right_col.add(txtData3);
		
		btnServo.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				command("s", servoText.getText());
			}
			
		});
		btnDrive.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("m", txtDrive.getText());
			}
			
		});
		btnLMotor.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("l", txtLMotor.getText());
			}
			
		});
		btnRMotor.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("r", txtRMotor.getText());
				
			}
			
		});
		btnGo.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("go");
				goToggle = false;
			}
			
		});
		btnStop.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("stop");
				goToggle = true;
			}
			
		});
		btnMode.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("mode");
			}
			
		});
		btnGet.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("get", txtGet.getText());
			}
			
		});
		btnSet.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command("set", txtParm1.getText().toLowerCase()+" "+txtData1.getText());
				command("set", txtParm2.getText().toLowerCase()+" "+txtData2.getText());
				command("set", txtParm3.getText().toLowerCase()+" "+txtData3.getText());
			}
			
		});
		btnCmd.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command(txtCmd.getText(), txtArgs.getText());
			}
			
		});
		
		
		JTextArea textArea = new JTextArea("PrimeOS v"+version+"\nNow Logged into Optimus Prime\nWelcome to the Optimus Prime Configuration System.\n");
		App.log = new AutobotsLog(textArea);
		
		JScrollPane scrollPane = new JScrollPane(textArea, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setAutoscrolls(true);
		frmAutobotsControlPanel.getContentPane().add(scrollPane);
		frmAutobotsControlPanel.getContentPane().add(plot);
		frmAutobotsControlPanel.setFocusable(true);
		frmAutobotsControlPanel.getContentPane().addKeyListener(new KeyListener(){


			
			//boolean driveDown = false;
			
			@Override
			public void keyPressed(KeyEvent e) {
				if(e.getKeyCode()==KeyEvent.VK_SPACE){
					if(goToggle)
						btnGo.doClick();
					else
						btnStop.doClick();
				}
				if(e.getKeyCode()==KeyEvent.VK_ENTER){
					command("stop");
				}
				if(e.getKeyCode()==KeyEvent.VK_M){
					command("mode");
				}
				
				//if(goToggle){
					if(e.getKeyCode()==KeyEvent.VK_UP){
						//driveDown = true;
						command("m", "60");
					}
					if(e.getKeyCode()==KeyEvent.VK_DOWN){
						command("m", "-60");
					}
					if(e.getKeyCode()==KeyEvent.VK_LEFT){
						command("s", "-1");
					}
					if(e.getKeyCode()==KeyEvent.VK_RIGHT){
						command("s", "1");
					}
				//}
				if(e.getKeyCode()==KeyEvent.VK_L){
					//if(!lineCapture){
						//lineCapture=true;
						command("set line_capture 1");

						//log.print("linecapture true\n");
					//}
				}
				
				
			}

			@Override
			public void keyReleased(KeyEvent e) {
				//if(goToggle){
					if(e.getKeyCode()==KeyEvent.VK_UP){
						//driveDown = false;
						command("m", "255");
					}
					if(e.getKeyCode()==KeyEvent.VK_DOWN){
						command("m", "255");
					}
					if(e.getKeyCode()==KeyEvent.VK_RIGHT || e.getKeyCode()==KeyEvent.VK_LEFT){
						command("s", "0");
					}
				//}
				//if(e.getKeyCode()==KeyEvent.VK_ENTER){
				//	command('d', "0");
				//}
				if(e.getKeyCode()==KeyEvent.VK_L){
				//	lineCapture = false;
					command("set line_capture 0");
					//log.print("linecapture false\n");
				}
			}

			@Override
			public void keyTyped(KeyEvent e) {
				
			}
			
		});
	}
	
	private void command(String cmd, String arg){
		try {
			if(writer!=null){
				writer.write(""+cmd+" "+arg+"\r");
				writer.flush();
			}
			
			((JPanel)frmAutobotsControlPanel.getContentPane()).grabFocus();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	private void command(String cmd){
		try {
			if(writer!=null){
				writer.write(""+cmd+"\r");
				writer.flush();
			}
			
			((JPanel)frmAutobotsControlPanel.getContentPane()).grabFocus();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

}

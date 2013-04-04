package com.arnerdl;

import java.awt.EventQueue;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import javax.bluetooth.BluetoothStateException;
import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;

public class App {

	private JFrame frmAutobotsControlPanel;
	private JTextField txtData;
	private JTextField txtStop;
	
	private static StreamConnection con;
	private static BufferedWriter writer;
	private static DataInputStream reader;
	private JTextField goText;
	private JTextField revText;
	private JTextField varText;
	private JTextField varSelectText;
	private JTextField opGoText;
	
	private static  AutobotsLog log;
	
	boolean goToggle = true;
	/**
	 * Launch the application.
	 * 
	 * 
	 */
	
	
	static {
		try {
		    for (LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()) {
		        if ("Nimbus".equals(info.getName())) {
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
//			reader = new BufferedReader(new InputStreamReader(con.openInputStream()));
			reader = con.openDataInputStream();

			(new Thread(new Runnable(){
				@Override
				public void run() {
					int counter = 0;
					long thresh = 500;
					long time = System.currentTimeMillis();
					int data;
					char diagBuf[] = new char[15];
					System.out.println("****NEW RUN*****");
					while(true){
						try{
//							if(reader.ready()){
								if(counter<10)
									diagBuf[counter++] = (char)reader.readByte();
								else {
									data = (int)reader.readByte();
									counter = 0;
//									for(int i=0; i<10; i++){
//										System.out.print(diagBuf[i]);
//									}
//									System.out.println(data);
									log.print("Car Data: " + new String(diagBuf) + (data & 0xFF) + "\n");
								}
//							}
							
						} catch( IOException e){
							System.err.println("IOException in reader thread");
						}
					}
				}
			})).start();
		} catch (BluetoothStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
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
		frmAutobotsControlPanel.setBounds(100, 100, 450, 300);
		frmAutobotsControlPanel.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmAutobotsControlPanel.getContentPane().setLayout(new GridLayout(2, 1, 0, 0));
		
		
		JPanel panel = new JPanel();
		frmAutobotsControlPanel.getContentPane().add(panel);
		panel.setLayout(new GridLayout(0, 2, 0, 0));
		((JPanel)frmAutobotsControlPanel.getContentPane()).grabFocus();
		JPanel left_col = new JPanel();
		panel.add(left_col);
		left_col.setLayout(new GridLayout(0, 2, 0, 0));
		
		JButton btnServo = new JButton("Servo");
		left_col.add(btnServo);
		
		txtData = new JTextField();
		txtData.setText("data");
		left_col.add(txtData);
		txtData.setColumns(10);
		goText = new JTextField();
		JButton btnGo = new JButton("Go");
		btnGo.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command('g', goText.getText());
			}
			
		});
		left_col.add(btnGo);
		
		
		left_col.add(goText);
		goText.setColumns(10);
		
		JButton btnVarselect = new JButton("varSelect");
		left_col.add(btnVarselect);
		
		varSelectText = new JTextField();
		btnVarselect.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command('x', varSelectText.getText());
			}
			
		});
		left_col.add(varSelectText);
		varSelectText.setColumns(10);
		
		JPanel right_col = new JPanel();
		panel.add(right_col);
		right_col.setLayout(new GridLayout(0, 2, 0, 0));
		
		final JButton btnStop = new JButton("Stop!");
		btnStop.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command('b', "1");
				goToggle = true;
			}
			
		});
		right_col.add(btnStop);
		
		txtStop = new JTextField();
		txtStop.setText("STOP");
		right_col.add(txtStop);
		txtStop.setColumns(10);
		revText = new JTextField();
		JButton btnReverse = new JButton("Reverse");
		btnReverse.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent arg0) {
				command('r', revText.getText());
			}
			
		});
		right_col.add(btnReverse);
		
		
		right_col.add(revText);
		revText.setColumns(10);
		
		JButton varBtn = new JButton("varBtn");
		right_col.add(varBtn);
		
		varText = new JTextField();
		varBtn.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					if(writer!=null){
						writer.write("set " + varText.getText());
						writer.flush();
					}
					((JPanel)frmAutobotsControlPanel.getContentPane()).grabFocus();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				//command('f', varText.getText());
			}
		});
		right_col.add(varText);
		varText.setText("varTxt");
		varText.setColumns(10);
		
		final JButton btnOpgo = new JButton("opGo");
		right_col.add(btnOpgo);
		
		opGoText = new JTextField();
		btnOpgo.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				command('o', opGoText.getText());
				goToggle = false;
				
			}
		});
		right_col.add(opGoText);
		opGoText.setColumns(10);
		
		btnServo.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				command('s', txtData.getText());
			}
			
		});
		
		JTextArea textArea = new JTextArea("PrimeOS v1.1\nNow Logged into Optimus Prime\nWelcome to the Optimus Prime Configuration System.\n");
		this.log = new AutobotsLog(textArea);
		
		JScrollPane scrollPane = new JScrollPane(textArea, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
				JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setAutoscrolls(true);
		frmAutobotsControlPanel.getContentPane().add(scrollPane);
		
		
		
		frmAutobotsControlPanel.setFocusable(true);
		frmAutobotsControlPanel.getContentPane().addKeyListener(new KeyListener(){


			
			boolean driveDown = false;
			
			@Override
			public void keyPressed(KeyEvent e) {
				if(e.getKeyCode()==KeyEvent.VK_SPACE){
					if(goToggle)
						btnOpgo.doClick();
					else
						btnStop.doClick();
				}
				if(e.getKeyCode()==KeyEvent.VK_ENTER){
					command('d', "1");
				}
				
				if(goToggle){
					if(e.getKeyCode()==KeyEvent.VK_UP){
						driveDown = true;
						command('m', "60");
					}
					if(e.getKeyCode()==KeyEvent.VK_DOWN){
						command('m', "-60");
					}
					if(e.getKeyCode()==KeyEvent.VK_LEFT){
						command('s', "-1");
					}
					if(e.getKeyCode()==KeyEvent.VK_RIGHT){
						command('s', "1");
					}
				}
				
			}

			@Override
			public void keyReleased(KeyEvent e) {
				if(goToggle){
					if(e.getKeyCode()==KeyEvent.VK_UP){
						driveDown = false;
						command('m', "1");
					}
					if(e.getKeyCode()==KeyEvent.VK_DOWN){
						command('m', "1");
					}
					if(e.getKeyCode()==KeyEvent.VK_RIGHT || e.getKeyCode()==KeyEvent.VK_LEFT){
						command('s', "0");
					}
				}
				if(e.getKeyCode()==KeyEvent.VK_ENTER){
					command('d', "0");
				}
			}

			@Override
			public void keyTyped(KeyEvent e) {
				
			}
			
		});
	}
	
	private void command(char cmd, String data){
		try {
			if(writer!=null){
				writer.write(""+cmd+" "+data+"\r");
				writer.flush();
			}
			if(cmd=='f'){
//				log.print("Sending: "+cmd+" "+data+"\n");
				log.print("Updated: "+varSelectText.getText()+ " with " + data+"\n");
			} else {
				log.print("Sending: "+cmd+" "+data+"\n");
			}
			
			((JPanel)frmAutobotsControlPanel.getContentPane()).grabFocus();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

}

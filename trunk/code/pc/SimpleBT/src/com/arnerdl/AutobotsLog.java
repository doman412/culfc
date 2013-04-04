package com.arnerdl;

import javax.swing.JTextArea;

public class AutobotsLog {

	
	
	private JTextArea area;
	private StringBuilder builder;

	public AutobotsLog(JTextArea area){
		this.area = area;
		this.builder = new StringBuilder(this.area.getText());
		
		
	}
	
	public void print(String s){
		builder.append(s);
		this.area.setText(builder.toString());
	}
	
	
	

}

#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\keypad_new.c"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);    
}

void loop() {
  Serial.println("test");
  // put your main code here, to run repeatedly:
  read_ID();
  Serial.println("test9");
  Serial.println(array_ID[0]);
  Serial.println(array_ID[1]);
  Serial.println(array_ID[2]);
  Serial.println(array_ID[3]);
  
}

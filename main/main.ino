#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\keypad_new.c"
#include "D:\AUC\Semester10(Spring2021)\Embedded\Project\repo\Embedded_Systems_Project\database_json.h"




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  
while (!Serial) continue;
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);
  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }  

  uint8_t temp = doc["ID"][49];
  Serial.println(temp);
  
}

void loop() {
//   int temp = doc["ID"][2];
//  Serial.println("test");
//   put your main code here, to run repeatedly:
//   read_ID();
//   Serial.println("test9");
//   Serial.println(array_ID[0]);
//  Serial.println(array_ID[1]);
//  Serial.println(array_ID[2]);
//   Serial.println(array_ID[3]);
 
}

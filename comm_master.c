char mystr[5] = "Hello"; //String data

void setup() {
  // Begin the Serial at 9600 Baud
  Serial.begin(9600);
}

void loop() {
  Serial.write(mystr,5); //Write the serial data
  delay(1000);
}

  Serial.begin(9600);
  char *str1; 
  char *str2;
  str1 = "Hello";
  str2 = "x";
  char *str3 = (char*)malloc(1 + strlen(str1) + strlen(str2));
  strcpy(str3, str2);
  strcat(str3, str1); 
  Serial.println(str3);

  free(str3);   
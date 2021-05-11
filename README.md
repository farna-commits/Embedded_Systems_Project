# Embedded Systems Project
Repo for ECNG4504 Project 

**Description**
A secure door system consisting of two Arduino microcontrollers and a keypad. The keypad and the first controller represent the door side and the other Arduino represents the database server. First, a key exchange between Tx and Rx are done using Diffie Hellman algorithm. Then a PIN is entered from the keypad to the first Arduino, then it's encrypted using AES128 algorithm. This encrypted PIN is sent using UART communication embedded in a layer called TinyProto that advances the communication between both controllers. In the database side, the PIN is then decrypted, hashed using SHA256 algorithm, then compared using Linear Search with a static hashed JSON database stored in the controllers ROM. Finally, the database send the response encrypted again using AES128, then decrypted in the door's side to act upon with an action to either open the door or not.

**Code Structure**
The code hierarchy consists of:
- Two main files, one for the door and the other for the database. 
- One header file, methods.h that has functions declerations, variables and constants. 
- methods.c file, the implemenation file for the functions used. 
- hashing.c file, contains the SHA256 hashing algorithm used. 
- excel_to_json.py: a Python script that reads the PIN codes from an excel file and transform it to a hashed list that is then copied to JSON structure in methods.h. 

//------------------------------------L I B R A R I E S ------------------------------------------------------
#include <M5Atom.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <EEPROM.h>
//-----------------------------------P R E D E F I N E D -----------------------------------------------------
const char* ssid = "ATOM_AP_TEST";   //AP name (customize to your preference)
const char* password = "12345678";   //AP password (customize to your preference)
int x = 0;                           //variable for switching between AP and STA 
String input1Value = "";             //box input1 string
String input2Value = "";             //box input2 string
#define EEPROM_SIZE 32               //EEPROM size in bytes
WiFiServer server(80);               //defined port

void setup() {
  M5.begin();                        //initialization
//------------------------------ R E A D - E P P R O M - V A L U E S -----------------------------------------
  if (!EEPROM.begin(EEPROM_SIZE)) {  //request for space
      Serial.println(
      "\nFailed to initialise EEPROM!");  //fail
      delay(1000000); 
  }

  String readString1 = "";               
  for (int i = 0; i < EEPROM_SIZE; ++i) {
    char c = EEPROM.read(i);                  //Read the first value written to EEPROM
    if (c == '\0') {
      break;  // Reached the end of string1
    }
    readString1 += c;
  }

  // Read string2 from EEPROM
  String readString2 = "";
  int offset = readString1.length() + 1;    //offset to the start of the next value
  for (int i = offset; i < EEPROM_SIZE; ++i) {
    char c = EEPROM.read(i);                //read the second value written to EEPROM
    if (c == '\0') {
      break;  // Reached the end of string2
    }
    readString2 += c;
  }
  Serial.println("Input 1: " + readString1);
  Serial.println("Input 2: " + readString2);  //display values from EEPROM


//---------------- C H E C K - S T A - S T A T U S ---------------------------
  Serial.println("Connecting to Wi-Fi...");
  WiFi.mode(WIFI_STA);                       //turn on STA mode
  WiFi.begin(readString1.c_str(), readString2.c_str()); //the read values from EEPROMA are used to connect
  int timeout = 10;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {//attempt to connect to existing Wi-Fi for given ssid and password values
    delay(1000);
    Serial.print(".");
    timeout--;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to Wi-Fi network");         //successful connection attempt
    x = 0;
  } else {
    Serial.println("\nFailed to connect to Wi-Fi network"); //failed connection attempt
    x = 1;
  }
//---------------E N A B L E - A P ------------------------------------------
if (x == 1){
  WiFi.disconnect();                                        //disable previously switched on STA
  Serial.println("\nWIFI ACCESS POINT");
  Serial.printf("Please connect to: %s\nThen access:", ssid);
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();                        //gives user the ip address to connect to on the browser
  Serial.println(myIP);
  server.begin();
  }
}

void loop() {
//-------------------------------A-P---------------------------------------------
if(x == 1){
  WiFiClient client = server.available();

  if (client) {                                           //when a visitor visits the website, the visitor's details are displayed
    Serial.println("New Client:");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
                                                      
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");         //HTML to display the web page
            client.println("Content-type:text/html");
            client.println();

            client.println("<html><body>");
            client.println("<h2>To connect Atom Echo please provide your SSID and password</h2>");
            client.println("<form method='POST'>");
            client.println("SSID: <input type='text' name='input1'><br>");
            client.println("Password: <input type='text' name='input2'><br>");
            client.println("<input type='submit' value='Submit'>");
            client.println("</form>");
            client.println("</body></html>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.startsWith("POST")) { //checks if there is a new command on the website
          String postData = "";
          while (client.available()) {
            postData += client.readStringUntil('\n');
          }

          input1Value = getValue(postData, "input1"); //gets values from input boxes
          input2Value = getValue(postData, "input2");

          Serial.print("Input 1: ");
          Serial.println(input1Value);
          Serial.print("Input 2: ");
          Serial.println(input2Value);


          client.println("HTTP/1.1 200 OK");     //HTML to display the web page
          client.println("Content-type:text/html");
          client.println();
          client.println("<html><body>");
          client.print("<h2>Submitted Values:</h2>");
          client.print("</p>");
          client.print("<p>SSID: ");
          client.print(input1Value);
          client.print("</p>");
          client.print("<p>Password: ");
          client.print(input2Value);
          client.println("</p>");
          client.println("</body></html>");
          client.println();

                                              
          for (int i = 0; i < input1Value.length(); ++i) {    //writes both entered values to EEPROM
            EEPROM.write(i, input1Value[i]);
          }
          EEPROM.write(input1Value.length(), '\0'); 

          int offset = input1Value.length() + 1;
          for (int i = 0; i < input2Value.length(); ++i) {
            EEPROM.write(offset + i, input2Value[i]);
          }
          EEPROM.write(offset + input2Value.length(), '\0');

          EEPROM.commit(); //final save to memory

          break;
        }
      }
    }
    client.stop();
  }
 }
 //----------------------------------- S-T-A -----------------------------------------
 if(x==0){
   Serial.println("Connected to Wi-Fi network");    //displays on the serial monitor that the connection to the home Wi-Fi network has been successfully established
   delay(5000);
 }

}

//---------------G E T - I N P U T - V A L U E S -------------------------------
String getValue(String data, String key) {
  String value = "";
  int startIndex = data.indexOf(key + "=");
  if (startIndex != -1) {
    startIndex += key.length() + 1;
    int endIndex = data.indexOf("&", startIndex);
    if (endIndex == -1) {
      endIndex = data.length();
    }
    value = data.substring(startIndex, endIndex);
  }
  return value;
}

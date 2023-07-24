#include <SoftwareSerial.h>

SoftwareSerial sim(11, 10);
SoftwareSerial ss(13, 12);




const unsigned char UBLOX_INIT[] PROGMEM = {
  //  0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12, //(10Hz)
//   0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A, //(5Hz)
//   0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39, //(1Hz)
  
//   0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24, // GxGGA
   0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B, // GxGLL
   0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32, // GxGSA
   0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39, // GxGSV
   0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40, // GxRMC
   0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47 // GxVTG
};


void setup() {
  Serial.begin(19200);
  Serial.println("Starting the program");
  
  ss.begin(9600);
  for(unsigned int i = 0; i < sizeof(UBLOX_INIT); i++) {        
    ss.listen();                
    ss.write(pgm_read_byte(UBLOX_INIT+i));
  }

  Serial.println("Wrote to gps");
  
  sim.begin(19200);
  sim.listen();
  sendATCommand("AT");
  sendATCommand("AT+CPIN?");
  sendATCommand("AT+CGATT?");
  sendATCommand("AT+CSTT=\"internet.megacom.kg\",\"\",\"\"");
  sendATCommand("AT+CIPSHUT");
  sendATCommand("AT+CMEE=0");
  sendATCommand("ATE0");
  sendATCommand("ATV0");
  sendATCommand("ATI");
  sendATCommand("AT+CIPSTART=\"UDP\",\"206.189.156.143\",\"3333\"");
  
}

void loop() {
  ss.listen();
  String str = "111,";
  while(ss.available()) {
    str += (char)ss.read();
  }

  
  if (str.length() > 15) {
    sim.listen();
    Serial.println(str);
//    str += "";  
    int stringlen = str.length() + 1;
    String command = "AT+CIPSEND=" + (String)stringlen;
//    Serial.println(stringlen);
//    Serial.print("COMMAND: ");Serial.println(command);
    sendATCommand(command);
    sendATCommand(str);   
  }




 
  
//  bool newData = false;
//  ss.listen();
//  for (unsigned long start = millis(); millis() - start < 1000;) {
//    while (ss.available()) {
//      char c = ss.read();
//      Serial.write(c);
//      if (gps.encode(c)) newData = true;
//    }
//  }
//  if (newData) {
//    float latitude, longitude;
//    unsigned long epoch;
//    gps.f_get_position(&latitude, &longitude, &epoch);
//
//    char location[20];
//    long longlat = (long)(latitude * 1000000);
//    long longlng = (long)(longitude * 1000000);
//    sprintf(location, "%ld||%ld", longlat, longlng);
//    
//    Serial.print("Sending: "); Serial.println(location); Serial.print("Size: "); Serial.println(sizeof(location));
//    sim.listen();
//    sendATCommand("AT+CIPSEND=20");
//    sendATCommand(location);
//  }
}

String sendATCommand(String command) {
  
  String resp = "";
  Serial.print("Command you sent: ");Serial.println(command);
  sim.println(command);
  resp = waitResponse();
  if (resp.startsWith(command)) resp = resp.substring(resp.indexOf("\r", command.length()) + 2);
  Serial.print("Response: ");Serial.println(resp);
  return resp;
}

String waitResponse() {
  String resp = "";
  unsigned long timeout = millis() + 10000;
  while (!sim.available() && millis() < timeout) {}
  if (sim.available()) resp = sim.readString();
  else Serial.println("Timeout...");
  return resp;
   
}

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 2, TXPin = 3, CTPin = 4;
static const uint32_t GPSBaud = 4800;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
static unsigned long lastTime = 0;
char cc;

void setup()
{
  Serial.begin(GPSBaud);
  ss.begin(1200);
  ss.print(F("Starting"));
  lastTime = millis();

  pinMode(CTPin, OUTPUT);
  
  ///// setup SIRF III GPS to be more battery friendly ; will work fine without it if device does not support it, but will have drastically reduced battery life.
  byte message3[] = {0xA0, 0xA2, 0x00, 0x18, 0x81, 0x02, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x05, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x12, 0xC0, 0x01, 0x67, 0xB0, 0xB3 }; //0xA0A200188102010100010101050101010001000100010001000112C00167B0B3
  Serial.write(message3, sizeof(message3));
  delay(500);
  Serial.write("$PSRF100,0,4800,8,1,0*0F\r\n");
  delay(500);
  byte message1[] = {0xA0, 0xA2, 0x00, 0x09, 0x97, 0x00, 0x01, 0x00, 0x0A, 0x00, 0x00, 0x00, 0xC8, 0x01, 0x6A, 0xB0, 0xB3 }; // 0xA0A20009970001000A000000C8016AB0B3
  Serial.write(message1, sizeof(message1));
  // set device to undergo a power saving 180 second interval
  byte message2[] = {0xA0, 0xA2, 0x00, 0x0F, 0xA7, 0x00, 0x00, 0x75, 0x30, 0x00, 0x04, 0x93, 0xE0, 0x00, 0x00, 0x00, 0xB4, 0x00, 0x00, 0x03, 0x77, 0xB0, 0xB3 }; // 0xA0A2000FA700007530000493E0000000B400000377B0B3
  Serial.write(message2, sizeof(message2));
  Serial.write(message3, sizeof(message3));
  delay(500);
  /////
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  if (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      if (millis() - lastTime > 10000) { // important to note the FCC guide lines state no less than 10 second intervals
        displayInfo(); // http://www.edn.com/electronics-blogs/eye-on-iot-/4437311/Using-433-MHz-for-wireless-connectivity-in-the-Internet-of-Things
        lastTime = millis();
      }
    }
  } else if (millis() - lastTime > 30000) { // assuming each transmission might be up to a second, but likely a fractional second,  we're going to play it safe and make most transmissions 30 intervals; FCC
    displayInfo();
    lastTime = millis();
  } else {
    delay(250);
  }
}

void displayInfo()
{
  // brings transmitter out of sleep mode
  digitalWrite(4, LOW); // enter AT command mode ; wake up from sleep mode
  delay(250);
  digitalWrite(4, HIGH); // enter transparent mode
  delay(100);
  
  ss.print(F(":"));
  if (gps.location.isValid())
  {
    ss.print(gps.location.lat(),5);  // 5 decimal place accuracy on degrees; roughly a foot added error
    ss.print(F(","));
    ss.print(gps.location.lng(),5);  // reduced data being sent over connection is less power and elss error
    if (gps.location.age()>300000){  // if older than 5 minutes
      ss.print(F("#"));
      ss.print(gps.location.age()/180000); // how many updates is it behind
    }
  }
  else
  {
    ss.print(F("!"));
    ss.print(cc);
    // maybe its stuck in the wrong mode; change modes if possible
    byte message3[] = {0xA0, 0xA2, 0x00, 0x18, 0x81, 0x02, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x05, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x12, 0xC0, 0x01, 0x67, 0xB0, 0xB3 }; //0xA0A200188102010100010101050101010001000100010001000112C00167B0B3
    Serial.write(message3, sizeof(message3));
  }
  delay(1000); // just give it time to send
  
  digitalWrite(4, LOW); // enter AT command mode
  delay(250);
  ss.print(F("AT+SLEEP\r\n")); // set transmitter for sleep mode
  ss.print("AT+SLEEP\r\n"); // set transmitter for sleep mode
  ss.println("AT+SLEEP"); // set transmitter for sleep mode
  delay(250);
  digitalWrite(4, HIGH); // enter transparent mode ; enter sleep mode
  delay(100);
  
}

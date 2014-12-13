/*
based on XBeeAnalogReceiveSeries1 by Michael Margolis
 Read an analog value from an XBee API frame and set the brightness
 of an LED accordingly. */
 
const int ledPin = 11;
int hi = 1023;
int lo = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  configureRadio(); // check the return value if you need error handling
}

boolean configureRadio() {
  // put the radio in command mode: 
  Serial.flush(); 
  Serial.print("+++");
  delay(100);
  String ok_response = "OK\r"; // the response we expect.
  // Read the text of the response into the response variable 
  String response = String("");
  while (response.length() < ok_response.length()) {
    if (Serial.available() > 0) {
      response += (char) Serial.read(); 
    }
  }
  // If we got the right response, configure the radio and return true. 
  if (response.equals(ok_response)) {
    Serial.print("ATAP1\r"); // Enter API mode 
    delay(100);
    Serial.print("ATCN\r"); // back to data mode return true;
  } 
  else {
    return false; // This indicates the response was incorrect. 
  }
}
void loop() {
  if (Serial.available() >= 14) { // Wait until we have a mouthful of data 
    if (Serial.read() == 0x7E) { // Start delimiter of a frame
      // Skip over the bytes in the API frame we don't care about 
      for (int i = 0; i < 10; i++) {
        Serial.read(); 
      }
      // The next two bytes are the high and low bytes of the sensor reading 
      int analogHigh = Serial.read();
      int analogLow = Serial.read();
      int analogValue = analogLow + (analogHigh * 256);
      // Scale the brightness to the Arduino PWM range 
      int brightnessC = constrain(analogValue, lo, hi);
      int brightness = map(brightnessC, lo, hi, 0, 255);
      // Light the LED
      analogWrite(ledPin, brightness);
      
     Serial.print(analogValue); 
     Serial.print("  "); 
     Serial.print(brightnessC); 
     Serial.print("  "); 
     Serial.println(brightness); 
    }
  } 
}


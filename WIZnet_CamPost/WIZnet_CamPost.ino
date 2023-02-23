/*
  Telnet client
  This sketch connects to a a telnet server (http://www.google.com)
  using an Arduino Wiznet Ethernet shield.  You'll need a telnet server
  to test this with.
  Processing's ChatServer example (part of the network library) works well,
  running on port 10002. It can be found as part of the examples
  in the Processing application, available at
  http://processing.org/
  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13
  created 14 Sep 2010
  modified 9 Apr 2012
  by Tom Igoe
*/

#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <SPI.h>
// MQTT [[
#include <PubSubClient.h>
// ]]
#include "ArduCAM_OV2640.h"
#include "memorysaver.h"


const int BUTTON = 6; // Arduino pin connected to button's pin
int buzzerPin = 8;
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0x77, 0xF7
};
IPAddress ip(192, 168, 0, 77);
//IPAddress ip(10, 5, 15, 109);

// Enter the IP address of the server you're connecting to:
//IPAddress server(192, 168, 100, 2);
//IPAddress myDns(192, 168, 100, 1);
//IPAddress server(10, 5, 15, 78);
IPAddress server(192, 168, 0, 107);
IPAddress myDns(192, 168, 0, 1);
uint16_t port = 1066;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use port 10002):
#define max_transfer 1024
#define max_buffer  (20 * max_transfer)

EthernetClient client;
byte img_buf[max_buffer];

int count = 0;
char c = 0;
bool is_capture = false;

void ethernet_transfer(byte *bptr, size_t len) {
  size_t sent = 0;
  for (; sent + max_transfer < len; ) {
    client.write(bptr, max_transfer);
    sent += max_transfer;
    bptr += max_transfer;
    //Serial.println(max_transfer);
  }
  client.write(bptr, len - sent);

  Serial.print("Sent: "); Serial.println(len);
}

void http_postData(byte *buf, uint32_t length) {
  count = 0;

  // if the server's disconnected, reconnect the client:
  while (!client.connected()) {
    Serial.println();
    Serial.println("disconnected. Reconnecting...");
    if (client.connect(server, port)) {
      Serial.println("connected");
      break;
    } else {
      // if you didn't get a connection to the server:
      if (++count > 5) {  // Retry 5 times.
        Serial.println("HTTP Post failed. Give up.");
        return;
      }
      Serial.println("connection failed");
      delay(1000);
    }
  }

  delay(200);

  client.write("POST / HTTP/1.1\n");
  client.write("Host: 192.168.0.118\n");
  client.write("User-Agent: Arduino/1.0\n");
  client.write("Connection: close\n");
  client.write("Content-Disposition: form-data; name=\"photo\"; filename=\"capture.jpeg\"\n");
  client.write("Content-Type: image/jpeg\n");
  client.write("Content-Length: ");
  char lenbuf[20];
  sprintf(lenbuf, "%d\n\n", length);
  client.write(lenbuf);
  ethernet_transfer(buf, length);
  client.write("\n\n");
  client.flush();
  delay(100);

  int len = client.available();
  if (len > 0) {
    byte buffer[200];
    if (len > 200) len = 200;
    client.read(buffer, len);
    Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    Serial.println("");
    //byteCount = byteCount + len;
  }

  client.stop();
}

//=========================================================
void ArduCam_setup();
void ArduCam_sendImg();

//const int CS = 5; //GPIO-5 for SPI
const int CS = 13;  //GPIO-13 for SPI1
uint8_t vid, pid;
ArduCAM myCAM( OV2640, CS );

void ArduCam_setup() {
  uint8_t temp;

  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);

  Wire.setSDA(8);
  Wire.setSCL(9);
  // This example will use I2C0 on GPIO4 (SDA) and GPIO5 (SCL)

  // Using SPI
  //SPI.setSCK(2);
  //SPI.setTX(3);
  //SPI.setRX(4);
  //SPI.setCS(CS);
  //myCAM.Arducam_init(&SPI, &Wire);

  // Using SPI1
  SPI1.setSCK(10);
  SPI1.setTX(11);
  SPI1.setRX(12);
  SPI1.setCS(CS);
  myCAM.Arducam_init(&SPI1, &Wire);

  //Reset the CPLD
  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);

  while (1) {
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    if (temp != 0x55) {
      Serial.println(temp);
      Serial.println("ACK CMD SPI interface Error! END");
      delay(1000);
      continue;
    } else {
      Serial.println("ACK CMD SPI interface OK. END");
      break;
    }
  }

  while (1) {
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))) {
      Serial.println("ACK CMD Can't find OV2640 module! END");
      delay(1000); continue;
    }
    else {
      Serial.println("ACK CMD OV2640 detected. END");
      break;
    }
  }

  //Change to JPEG capture mode and initialize the OV2640 module
  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  //myCAM.OV2640_set_JPEG_size(OV2640_176x144);
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  //myCAM.OV2640_set_JPEG_size(OV2640_640x480);
  delay(50);
  myCAM.OV2640_set_Light_Mode(Auto);
  delay(50);
  //myCAM.OV2640_set_Color_Saturation(Saturation0);
  myCAM.OV2640_set_Color_Saturation(Saturation0);
  delay(50);
  //myCAM.OV2640_set_Brightness(Brightness0);
  myCAM.OV2640_set_Brightness(Brightness0);
  delay(50);
  //myCAM.OV2640_set_Contrast(Contrast0);
  myCAM.OV2640_set_Contrast(Contrast0);
  delay(50);
  myCAM.OV2640_set_Special_effects(Bluish);
  //myCAM.OV2640_set_Special_effects(Greenish);
  //myCAM.OV2640_set_Special_effects(Reddish);
  //myCAM.OV2640_set_Special_effects(BW);
  //myCAM.OV2640_set_Special_effects(Negative);
  //myCAM.OV2640_set_Special_effects(BWnegative);
  //myCAM.OV2640_set_Special_effects(Antique);
  myCAM.OV2640_set_Special_effects(Normal);
  myCAM.OV2640_set_Light_Mode(Office);
  delay(1000);
  myCAM.clear_fifo_flag();
  delay(50);
}

int ArduCam_capture() {
  uint32_t length = 0;
  uint32_t read_len = 0;
  byte *bptr;

  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  //Start capture
  myCAM.start_capture();

  Serial.println("Wait CAP_DONE_MASK...");
  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK))
    delay(1);

  Serial.print("CAM Image Ready.");
  delay(1);

  //read_fifo_burst(myCAM);
  length = myCAM.read_fifo_length();
  Serial.println(length);

  myCAM.CS_LOW();
  myCAM.set_fifo_burst();//Set fifo burst mode

  bptr = img_buf;

  // Read image from ArduCam
  while (1) {
    if (length > max_transfer) {
      read_len = myCAM.transferBytes((uint8_t*)NULL, (uint8_t*)bptr, (uint32_t)max_transfer);
      //Serial.println(read_len);
      //ethernet_transfer(buf, read_len);
      length -= read_len;
      bptr += read_len;
    } else {
      read_len = myCAM.transferBytes((uint8_t*)NULL, (uint8_t*)bptr, (uint32_t)length);
      //Serial.println(read_len);
      //ethernet_transfer(buf, read_len);
      break;
    }
    //delayMicroseconds(15);
  }

  myCAM.CS_HIGH();
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
  Serial.println("Copied to buffer");

  // Find end of image
  bptr = img_buf + 2;
  for (length = 2; length < max_buffer ; ++length, ++bptr) {
    if (*bptr == 0xFF) {
      if (*(bptr + 1) == 0xD9) {
        Serial.print("End of image. New length = ");
        length += 2;  // Include 0xFF 0xD9
        Serial.println(length);
        break;
      }
    }
  }

  return length;
}

/*/
  void ArduCam_sendImg() {
  size_t length = 0;

  length = ArduCam_capture();
  // Sned image to server.
  ethernet_transfer(img_buf, length);
  }
  /*/
void ArduCam_sendImg() {
  size_t length = 0;

  length = ArduCam_capture();
  // Sned image to server.
  http_postData(img_buf, length);
}
//*/

//===================================================================================================
// MQTT [[

IPAddress mqtt_server(54, 87, 92, 106);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(">>>>>>>>>>> Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

EthernetClient ethClient;
PubSubClient mqtt_client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      mqtt_client.publish("WIZ-outTopic", "hello world");
      // ... and resubscribe
      mqtt_client.subscribe("WIZ-inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// ]]

//===================================================================================================

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  //Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
  Ethernet.init(17);  // WIZnet EVB-Pico (GPIO-17)

  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP); /* assigning PULL_UP mode to push button as an INPUT*/
  currentButtonState = digitalRead(BUTTON);
  pinMode(buzzerPin, OUTPUT);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}



  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  /*
    //SPIClassRP2040 SPI1(spi1, PIN_SPI1_MISO, PIN_SPI1_SS, PIN_SPI1_SCK, PIN_SPI1_MOSI);
    Serial.print("MISO="); Serial.println(PIN_SPI1_MISO);
    Serial.print("SS="); Serial.println(PIN_SPI1_SS);
    Serial.print("SCLK="); Serial.println(PIN_SPI1_SCK);
    Serial.print("MOSI="); Serial.println(PIN_SPI1_MOSI);
  */

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.println("connected");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  // MQTT [[
  mqtt_client.setServer(mqtt_server, 1883);
  mqtt_client.setCallback(callback);
  // ]]

  // Initial setup for ArduCam
  ArduCam_setup();

  // give the Ethernet shield a second to initialize:
  delay(1500);

}

unsigned long prevmillis = 0;
bool is_pressed = false;

void loop() {

  //  // MQTT [[
  //  if (!mqtt_client.connected()) {
  //    reconnect();
  //  }
  //  mqtt_client.loop();
  //  // ]]

  lastButtonState    = currentButtonState;      // save the last state
  currentButtonState = digitalRead(BUTTON); // read new state

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("Hello");
    ArduCam_sendImg();
    digitalWrite(buzzerPin, HIGH);
    count++;
    Serial.println(count);
    delay(5000);
  }
}

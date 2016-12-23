//Basic script to post readings from A0 to Hasvi database
//Requires an Ardunio with ethernet shield (or built-in)

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

EthernetClient client;

// The Hasvi server and token to the datastream
char IOTServer[] = "data.snorlax.hasvi.com";
char token[] = "token_id";

// last time you connected to the server, in milliseconds
unsigned long lastConnectionTime = 0;    
// delay between updates, in milliseconds (300sec = 5 min)
const unsigned long postingInterval = 5L * 60L * 1000L;  

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Booted!");

  // start the Ethernet connection and the server:
  // using DHCP get get IP Address
  Ethernet.begin(mac);
  Serial.print("Arduino is at ");
  Serial.println(Ethernet.localIP());

}

void loop() {

  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(IOTServer, 80)) {
    Serial.println("connecting...");
    // send the HTTP GET request to Hasvi:
    client.print("GET ");
    client.print("/insertData?token=");
    client.print(token);
    client.print("&data=");
    client.print(analogRead(A0));
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(IOTServer);
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}


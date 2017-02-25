#include "WiFiEsp.h" //Librería https://github.com/bportaluri/WiFiEsp
#include "SoftwareSerial.h" 
#include <Twitter.h> //http://arduino-tweet.appspot.com/

SoftwareSerial Serial1(10, 11); // (RX, TX) En placa Arduino  
char ssid[] = "red-wifi";        
char pass[] = "password";        
int status = WL_IDLE_STATUS;

int ledPin = 3;
byte sensorPin = 7; // Pin Sensor de movimiento 1
byte sensorPin2 = 6; // Pin Sensor de movimiento 2
byte movimiento = 0; // Variable para activar alarma
int Numerorandom =0; // Número ramdom para enviar por Twitter
byte Numerorandom2 =0; // Número ramdom para enviar por Twitter
int analogZero; // Número ramdom generado por Pin A0 para enviar por Twitter
char msg[140]; // Mensaje para Twitter
Twitter twitter("tokes-para-twitter"); // Token desde http://arduino-tweet.appspot.com/

WiFiEspClient client; 

void setup()
{
  pinMode(sensorPin,INPUT);
  pinMode(sensorPin2,INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);  //Monitor serial
  Serial1.begin(9600); //ESP2286-01
  WiFi.init(&Serial1);

  //Se inicia módulo ESP8266
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("Error al inciar el modulo. Reiniciar.)");
    digitalWrite(ledPin, HIGH);
    delay(2000);
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(5000);
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    //Loop infinito
    while (true);
  }
 
  //Se inicia conexión a red WiFi
  while ( status != WL_CONNECTED) {
    Serial.print("Conectando a la red WiFi: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(1000);
    
 printWifiStatus();
 delay(15000);
 Serial.println();
 
 }

	if (status == WL_CONNECTED){
   		digitalWrite(ledPin, HIGH);
  		Serial.println("LED ON");
	} 	
	else {
   		digitalWrite(ledPin, LOW);
    
	}

}


void loop()
{
  
 byte state = digitalRead(sensorPin);
 byte state2 = digitalRead(sensorPin2);

 
    if(state == 1) {
     Serial.println("Se ha detectado movimiento!");
      movimiento++;
      delay(1000);
      Serial.println(movimiento);
    }
    else if (state == 0) {
      delay(1000);
     Serial.println("Sin movimiento!");
    }

    if(state2 == 1) {
     Serial.println("Se ha detectado movimiento!");
      movimiento++;
      delay(1000);
      Serial.println(movimiento);
    }
    else if (state2 == 0) {
      delay(1000);
     Serial.println("Sin movimiento!");
    }

// Se activa la alarma y se envía el mensaje a Twitter 
   
    while (movimiento >= 5) {   
	analogZero=analogRead(1);
	Numerorandom = random(101,11565);
	Numerorandom2 = random(1,254);
	Serial.println(Numerorandom);
	Serial.println(Numerorandom2);
        
	// Se envía el tweet con 3 números generados al azar para no duplicar 
        sprintf(msg, "Alarma spamloco activada %d - %d - %d.", Numerorandom,Numerorandom2,analogZero);

        // Muestra el mensaje en el Puerto Serie
        Serial.println(msg);
        Serial.println("Conectando a Twitter");
client.stop();
  if (twitter.post(msg)) {
     int status = twitter.wait(&Serial);
      if (status == 200) {
        Serial.println("200 OK");
       delay(60001); // Si el mensaje se envía se espera 1 minuto para evitar spam
     } else {
          Serial.print("Error : code ");
        Serial.println(status);
        
     }
    } else {
      Serial.println("Fallo en la conexión.");
  }

movimiento = 0; // Variable de movimientos vuelve a cero
 } 
    
 
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

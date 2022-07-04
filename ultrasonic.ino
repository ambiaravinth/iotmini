#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define echoPin1 16
#define trigPin1 05
#define echoPin2 04
#define trigPin2 00
#define echoPin3 14
#define trigPin3 12
#define echoPin4 13
#define trigPin4 15

long duration1, duration2, duration3, duration4;
int distance1, distance2, distance3, distance4;

// Replace with your network credentials
const char* ssid = "ambi";
const char* password = "abcd1234";

// Initialize Telegram BOT
#define BOTtoken "5509658621:AAE-gjP1_nMhPky-2cB7Z3rcJQ-7L2nbDyw"
#define CHAT_ID "5156129207"

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup() {
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  client.setTrustAnchors(&cert);    // Add root certificate for api.telegram.org
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  //Serial.begin(9600);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  bot.sendMessage(CHAT_ID, "Smart_BIN_0001 is activated...", "");
}

void loop() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.print(" cm ");

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;
  Serial.print(" Distance2: ");
  Serial.print(distance2);
  Serial.print(" cm ");

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distance3 = duration3 * 0.034 / 2;
  Serial.print("Distance3: ");
  Serial.print(distance3);
  Serial.print(" cm ");

  digitalWrite(trigPin4, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin4, LOW);
  duration4 = pulseIn(echoPin4, HIGH);
  distance4 = duration4 * 0.034 / 2;
  Serial.print("Distance4: ");
  Serial.print(distance4);
  Serial.println(" cm ");

  if ((distance1 >= 1000 || distance1 <= 3)&&(distance2 >= 1000 || distance2 <= 3)&&(distance3 >= 1000 || distance3 <= 3)&&(distance4 >= 1000 || distance4 <= 3)){
      bot.sendMessage(CHAT_ID, "Smart_BIN_0001 is full...", "");
      Serial.println("Bin is full.........."); 
  }

  delay(800);
}

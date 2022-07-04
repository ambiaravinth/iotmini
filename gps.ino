#include <TinyGPS++.h> // library for GPS module
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device
#include "HX711.h"
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "ambi"; //ssid of your wifi
const char* password = "abcd1234"; //password of your wifi

// Initialize Telegram BOT
#define BOTtoken "5509658621:AAE-gjP1_nMhPky-2cB7Z3rcJQ-7L2nbDyw"
// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "5156129207"

double latitude , longitude;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str,msg="";
int pm;
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 12;
const int LOADCELL_SCK_PIN = 13;
float iniw,presw;

HX711 scale;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); //connecting to wifi
  while (WiFi.status() != WL_CONNECTED)// while wifi not connected
  {
    delay(500);
    Serial.print("."); //print "...."
  }
  Serial.println("");
  Serial.println("WiFi connected");

  scale.set_scale(140.687);
  scale.tare();
  //Serial.println(scale.read());
  //Serial.println(scale.read_average(20));
  //Serial.println(scale.get_value(5));
  iniw = scale.get_units(10);
  Serial.println(scale.get_units(5), 1);

  bot.sendMessage(CHAT_ID, "Monitoring started", "");
}


void loop()
{
  while (ss.available() > 0) //while data is available
    if (gps.encode(ss.read())) //read gps data
    {
      if (gps.location.isValid()) //check whether gps location is valid
      {
        latitude = gps.location.lat();
        lat_str=String(latitude);
        longitude = gps.location.lng();
        lng_str=String(longitude);
      }
      if (gps.date.isValid()) //check whether gps date is valid
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();
        if (date < 10)
          date_str = '0';
        date_str += String(date);// values of date,month and year are stored in a string
        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month); // values of date,month and year are stored in a string
        date_str += " / ";
        if (year < 10)
          date_str += '0';
        date_str += String(year); // values of date,month and year are stored in a string
      }
      if (gps.time.isValid())  //check whether gps time is valid
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();
        minute = (minute + 30); // converting to IST
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;   // converting to IST
        if (hour >= 12)  // checking whether AM or PM
          pm = 1;
        else
          pm = 0;
        hour = hour % 12;
        if (hour < 10)
          time_str = '0';
        time_str += String(hour); //values of hour,minute and time are stored in a string
        time_str += " : ";
        if (minute < 10)
          time_str += '0';
        time_str += String(minute); //values of hour,minute and time are stored in a string
        time_str += " : ";
        if (second < 10)
          time_str += '0';
        time_str += String(second); //values of hour,minute and time are stored in a string
        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";
      }
    }

  Serial.print("Long= ");
  Serial.print(longitude);
  msg+=lng_str;
  Serial.print(" Lati= ");
  Serial.print(latitude);
  msg+=lat_str;
  Serial.print(" date= ");
  Serial.print(date_str);
  msg+=date_str;
  Serial.print(" time= ");
  Serial.println(time_str);
  msg+=time_str;

  presw = scale.get_units(5);
  scale.power_down(); // put the ADC in sleep mode
  Serial.print("iniw= ");
  Serial.print(iniw);
  Serial.print(" presw= ");
  Serial.println(presw);
  if (presw <= (iniw - 10)) {
      bot.sendMessage(CHAT_ID,"Msg= "+msg, "");
  }
  msg=" ";

  //  s += lat_str;
  //  s += lng_str;
  //  s += date_str;
  //  s += time_str;
  delay(100);
  scale.power_up();
}

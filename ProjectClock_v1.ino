#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <TimeLib.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   14
#define DATA_PIN  13
#define CS_PIN_TIME    5
#define CS_PIN_DATE    15

const char* ssid = "Enigma";
const char* password = "saifullah2992";
const char* ntpServerName = "pool.ntp.org";
const long timezoneOffset = 6 * 3600; // 6 hours converted to seconds
const char* monthNames[] = {"", "b", "j", "a", "g", "d", "e", "k", "o", "p", "m", "f", "c"};
const char* weekNames[] = {"", "$", "%", "&", "(", ")", "*", "#"};

MD_Parola P_time = MD_Parola(HARDWARE_TYPE, CS_PIN_TIME, MAX_DEVICES);
MD_Parola P_date = MD_Parola(HARDWARE_TYPE, CS_PIN_DATE, MAX_DEVICES);

int conv2bangla(int date, int mash, int bochor, int konta)
{

  if (mash >= 4 && mash <= 12)
  {
    bochor = bochor - 593;
  }
  else
  {
    bochor = bochor - 1 - 593;
  }

  if (mash == 4)
  {

    if (bochor < 14)
    {
      mash = 12;
      //name="Chaitra";
      date = date + 14 + 3;
    }
    else if (date >= 14)
    {
      mash = 1;
      //name="Boishakh";
      date = date - 14 + 1;
    }

  }
  else if (mash == 5)
  {

    if (date < 15)
    {
      mash = 1;
      //name="Boishakh";
      date = date + 15 + 2;
    }
    else if (date >= 15)
    {
      mash = 2;
      //name="Joistha";
      date = date - 15 + 1;
    }

  }
  else if (mash == 6)
  {

    if (date < 15)
    {
      mash = 2;
      //name="Joistha";
      date = date + 15 + 2;
    }
    else if (date >= 15)
    {
      mash = 3;
      //name="Ashar";
      date = date - 15 + 1;
    }
  }
  else if (mash == 7)
  {

    if (date < 16)
    {
      mash = 3;
      //name="Ashar";
      date = date + 16;
    }
    else if (date >= 16)
    {
      mash = 4;
      //name="Srabon";
      date = date - 16 + 1;
    }
  }
  else if (mash == 8)
  {

    if (date < 16)
    {
      mash = 4;
      //name="Srabon";
      date = date + 16;
    }
    else if (date >= 16)
    {
      mash = 5;
      //name="Vadro";
      date = date - 16 + 1;
    }
  }
  else if (mash == 9)
  {

    if (date < 16)
    {
      mash = 5;
      //name="Vadro";
      date = date + 16;
    }
    else if (date >= 16)
    {
      mash = 6;
      //name="Ashwin";
      date = date - 16 + 1;
    }
  }
  else if (mash == 10)
  {

    if (date < 16)
    {
      mash = 6;
      //name="Aswin";
      date = date + 15;
    }
    else if (date >= 16)
    {
      mash = 7;
      //name="Kartik";
      date = date - 16 + 1;
    }
  }
  else if (mash == 11)
  {

    if (date < 15)
    {
      mash = 7;
      //name="Kartik";
      date = date + 15 + 1;
    }
    else if (date >= 15)
    {
      mash = 8;
      //name="Agrahoyon";
      date = date - 15 + 1;
    }

  }
  else if (mash == 12)
  {

    if (date < 15)
    {
      mash = 8;
      //name="Agrahoyon";
      date = date + 15 + 1;
    }
    else if (date >= 15)
    {
      mash = 9;
      //name="Poush";
      date = date - 15 + 1;
    }
  }
  else if (mash == 1)
  {

    if (date < 14)
    {
      mash = 9;
      //name="Poush";
      date = date + 14 + 3;
    }
    else if (date >= 14)
    {
      mash = 10;
      //name="Magh";
      date = date - 14 + 1;
    }

  }
  else if (mash == 2)
  {

    if (date < 13)
    {
      mash = 10;
      //name="Magh";
      date = date + 13 + 5;
    }
    else if (date >= 13)
    {
      mash = 11;
      //name="Falgun";
      date = date - 13 + 1;
    }
  }
  else if (mash == 3)
  {

    if (date < 15)
    {
      mash = 11;
      //name="Falgun";
      date = date + 15 + 2;
    }
    else if (date >= 15)
    {
      mash = 12;
      //name="Choitra";
      date = date - 15 + 1;
    }
  }
  /*Serial.println("Function er vitore");
    Serial.println(date);
    Serial.println(mash);
    Serial.println(bochor);*/
  if (!konta) return date;
  else if (konta == 1) return mash;
  else return bochor;
}

void setup() {
  Serial.begin(9600);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  configTime(timezoneOffset, 0, ntpServerName);  // Configure the time and NTP server
  while (!time(nullptr)) {  // Wait until time is set by syncing to the NTP server
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nTime set!");

  P_time.begin();
  P_date.begin();

  P_time.setIntensity(0);  // Set the brightness to a visible level
  P_date.setIntensity(0);  // Set the brightness to a visible level
}

void loop() {
  time_t now = time(nullptr)+timezoneOffset;
  int hours = hour(now);
  if(hours > 12) {
    hours = hours - 12;
  }
  int minutes = minute(now);
  int seconds = second(now);

  char timeStr[9];
  if(seconds % 2 == 1) {
      if(minutes%10==1||minutes%10==2||minutes%10==0||minutes/10==1||minutes/10==2||minutes/10==0)
        sprintf(timeStr, "%d:%02d", hours, minutes);

      else
        sprintf(timeStr, " %d:%02d", hours, minutes);
  }
  else {
      if(minutes%10==1||minutes%10==2||minutes%10==0||minutes/10==1||minutes/10==2||minutes/10==0)
        sprintf(timeStr, "%d %02d", hours, minutes);

      else
        sprintf(timeStr, " %d %02d", hours, minutes);
  }

  Serial.println(timeStr);

  P_time.displayClear();
  P_time.displayZoneText(0, timeStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P_time.displayAnimate();

int gDay = day(now);
int gMonth = month(now);
int gYear = year(now);
int weekDay = weekday(now);

int bDay = conv2bangla(gDay, gMonth, gYear, 0);
int bMonth = conv2bangla(gDay, gMonth, gYear, 1);
int bYear = conv2bangla(gDay, gMonth, gYear, 2);

char dateStr[15]; // Enough space for day, month abbreviation, and year
sprintf(dateStr, "%d%s", bDay, monthNames[bMonth]);


    Serial.println(dateStr);

    if(seconds%4==0||seconds%4==1){
    P_date.displayClear();
    P_date.displayZoneText(0, weekNames[weekDay], PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    P_date.displayAnimate(); 
    }

    else{
    P_date.displayClear();
    P_date.displayZoneText(0, dateStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    P_date.displayAnimate(); 
    }
}

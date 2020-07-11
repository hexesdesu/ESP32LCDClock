// ESP8266 ESP-01 Internet real time clock
 
 
#include <WiFi.h>
#include <WiFiUdp.h>
 
#include <NTPClient.h>               // Include NTPClient library
#include <TimeLib.h>                 // Include Arduino time library
#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library
 
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows
 
const char *ssid     = "DynamicOP";
const char *password = "DynamicOP";
 
WiFiUDP ntpUDP;
 
// 'time.nist.gov' is used (default server) with +1 hour offset (3600 seconds) 60 seconds (60000 milliseconds) update interval
NTPClient timeClient(ntpUDP, "cn.ntp.org.cn", 8*3600, 60000);
 
char Time[] = "TIME:00:00:00";
char Date[] = "DATE:2020/00/00";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;
 
void setup() {
 
  Serial.begin(115200);
 
  lcd.begin(25, 21);// initialize the lcd with SDA and SCL pins
  lcd.backlight();                   // Turn backlight ON
  lcd.setCursor(0, 0);
  lcd.print(Time);
  lcd.setCursor(0, 1);
  lcd.print(Date);
 
  WiFi.begin(ssid, password);
 
  Serial.print("Connecting.");
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
 
  timeClient.begin();
}
 
void loop() {
 
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server
 
  second_ = second(unix_epoch);
  if (last_second != second_) {
 
    minute_ = minute(unix_epoch);
    hour_   = hour(unix_epoch);
    day_    = day(unix_epoch);
    month_  = month(unix_epoch);
    year_   = year(unix_epoch);
 
    Time[12] = second_ % 10 + 48;
    Time[11] = second_ / 10 + 48;
    Time[9]  = minute_ % 10 + 48;
    Time[8]  = minute_ / 10 + 48;
    Time[6]  = hour_   % 10 + 48;
    Time[5]  = hour_   / 10 + 48;
 
    Date[13]  = day_   / 10 + 48;
    Date[14]  = day_   % 10 + 48;
    Date[10]  = month_  / 10 + 48;
    Date[11]  = month_  % 10 + 48;
    Date[7] = (year_   / 10) % 10 + 48;
    Date[8] = year_   % 10 % 10 + 48;
 
    // Send time and date to serial monitor
    Serial.println(Time);
    Serial.println(Date);
 
    // Display time and date on the 16x2 LCD
    lcd.setCursor(0, 0);
    lcd.print(Time);
    lcd.setCursor(0, 1);
    lcd.print(Date);
 
    last_second = second_;
  }
 
  delay(1000);
 
}

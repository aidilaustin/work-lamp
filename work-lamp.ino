#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "Arasso_2.4G";
const char* password = "67022210";

#define BOTtoken "6736278645:AAHEFAMhi_hLRJ5lm8YSZZvsCeUjV53NR_Q"
#define CHAT_ID "5554080268"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

const int led = 13;

void setup() {
  Serial.begin(115200);
  configTime(0, 0, "pool.ntp.org");
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  pinMode(led, OUTPUT);

  WiFi.mode(WIFI_STA);            
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(500);
  while(WiFi.waitForConnectResult() != WL_CONNECTED){Serial.print(".");}
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());   

  bot.sendMessage(CHAT_ID, "Started", "");  
}

void handleTelegramMessages() {
  int NewMessages = bot.getUpdates(bot.last_message_received + 1);

  for (int i=0; i<NewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;

    if (text == "/help") {
      String info = "Hi, " + from_name + ".\n\n";
      info += "Please choose to control the Light.\n";
      info += "/1 to turn ON Light\n";
      info += "/2 to turn OFF Light\n";
      info += "/status to request current Light Status \n";
      bot.sendMessage(chat_id, info, "");
    }
    if (text == "/1") {
      bot.sendMessage(chat_id, "Light is turned ON", "");
      digitalWrite(led, HIGH);
    }
    if (text == "/2") {
      bot.sendMessage(chat_id, "Light is turned OFF", "");
      digitalWrite(led, LOW);
    }
    if (text == "/status") {
      if (digitalRead(led)){
        bot.sendMessage(chat_id, "Light is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "Light is OFF", "");
      }
    }
  }
}

void loop() {
  handleTelegramMessages();
  delay(1000); // Delay to avoid flooding Telegram with requests
}

//Library yang digunakan
#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//defini pin yang digunakan
#define pin_lampu1  5  //D1
#define pin_lampu2  4  //D2
#define pin_lampu3  12 //D6
#define pin_kipas   14 //D4
#define pin_pintu   2  //D5

//token yang didapat dari telegram
#define botToken "704085459:AAEJde6lDBnanUI92anw9tTPEihGd6cQPeE"  // Token dari BotFather

//variabel yang digunakan untuk proses komunikasi telegram
WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
long checkTelegramDueTime;
int checkTelegramDelay = 1000; //  1000 (1 second)
String defaultChatId = "-128380507"; //This can be got by using a bot called "myIdBot"

//variabel untuk koneksi wifi
char ssid[] = "OTHER";       // your network SSID (name)
char password[] = "12341234";  // your network key

void setup() {      //inisialisasi Serial, Wifi, dan Pin
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  pinMode(pin_lampu1, OUTPUT);
  pinMode(pin_lampu2, OUTPUT);
  pinMode(pin_lampu3, OUTPUT);
  pinMode(pin_kipas, OUTPUT);
  pinMode(pin_pintu, OUTPUT);
  digitalWrite(pin_lampu1, LOW);
  digitalWrite(pin_lampu2, LOW);
  digitalWrite(pin_lampu3, LOW);
  digitalWrite(pin_kipas, LOW);
  digitalWrite(pin_pintu, LOW);
  Serial.println("Bismillah");
}

// fungsi untuk menerima input dari telegram
void handleNewMessages(int numNewMessages) {
  
  for (int i=0; i<numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";
  
  if (text == "/start" || text == "/help") {
      String welcome = "Welcome to SmartHome.\n" + from_name + ".\n";
      welcome += "The following options are available.\n";
      welcome += "/options is to show the shortcut button.\n";
      welcome += "Use shortcut of button to control your home\n";
      bot.sendMessage(chat_id, welcome);
    }
    else if (text == "Lampu 1 ON") {
      digitalWrite(pin_lampu1, HIGH);
      bot.sendMessage(chat_id, "Lampu 1 Nyala", "");
    }
    else if (text == "Lampu 1 OFF") { 
      digitalWrite(pin_lampu1, LOW);
      bot.sendMessage(chat_id, "Lampu 1 Mati", "");
    }
    else if (text == "Lampu 2 ON") {
      digitalWrite(pin_lampu2, HIGH);
      bot.sendMessage(chat_id, "Lampu 2 Nyala", "");
    }
    else if (text == "Lampu 2 OFF") {
      digitalWrite(pin_lampu2, LOW);
      bot.sendMessage(chat_id, "Lampu 2 Mati", "");
    }
    else if (text == "Lampu 3 ON") {
      digitalWrite(pin_lampu3, HIGH);      
      bot.sendMessage(chat_id, "Lampu 3 Nyala", "");
    }    
    else if (text == "Lampu 3 OFF") {
      digitalWrite(pin_lampu3, LOW);
      bot.sendMessage(chat_id, "Lampu 3 Mati", "");
    }
    else if (text == "Buka Pintu") {
      digitalWrite(pin_pintu, HIGH);
      bot.sendMessage(chat_id, "Pintu Telah Terbuka", "");
    }
    else if (text == "Tutup Pintu") {
      digitalWrite(pin_pintu, LOW);
      bot.sendMessage(chat_id, "Pintu Telah di Tutup", "");
    }
    else if (text == "Kipas ON") {
      digitalWrite(pin_kipas, HIGH);
      bot.sendMessage(chat_id, "Kipas Dinyalakan", "");
    }
    else if (text == "Kipas OFF") {
      digitalWrite(pin_kipas, LOW);
      bot.sendMessage(chat_id, "Kipas Dimatikan", "");
    }
    else if (text == "/options") {
      String keyboardJson = "[[\"Lampu 1 ON\", \"Lampu 1 OFF\"],[\"Lampu 2 ON\", \"Lampu 2 OFF\"] ,[\"Lampu 3 ON\", \"Lampu 3 OFF\"] ,[\"Kipas ON\", \"Kipas OFF\"] ,[\"Buka Pintu\",\"Tutup Pintu\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson, true);
    }
    else{
      }
  }
}

//fungsi utama menunggu inputan dari telegram
void loop() {
  long noww = millis();
  Serial.println("Cek");
  if(noww >= checkTelegramDueTime) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    checkTelegramDueTime = noww + checkTelegramDelay;
  }
  noww = millis();
}

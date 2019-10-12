// Library yang digunakan
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// definisi pin untuk RFID
#define SS_PIN  53  
#define RST_PIN 8

//variabel untuk RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;

//Variabel Servo
Servo servo;

//Variabel untuk membaca status input
int State_mode;
int State1, State2, State3, State4, State5;
int State11, State22, State33, State44, State55;

//deklarasi pin untuk masukkan dari nodemcu
const int lampu1_in = 29;
const int lampu2_in = 31;
const int lampu3_in = 33;
const int kipas_in = 35;
const int pintu_in = 37;

//deklarasi pin saklar / manual
const int pin_mode = 47;
const int lampu1_in_m = 39;
const int lampu2_in_m = 41;
const int lampu3_in_m = 43;
const int kipas_in_m = 45;

//deklarasi untuk keluaran ke lampu pintu dan kipas
const int lampu1_out =  19;
const int lampu2_out =  20;
const int lampu3_out =  21;
const int kipas_out =  3;
const int buzzer_out =  10;

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  servo.attach(2); 
  servo.write(0);
  pinMode(pin_mode, INPUT_PULLUP);
  pinMode(lampu1_in, INPUT_PULLUP);
  pinMode(lampu2_in, INPUT_PULLUP);
  pinMode(lampu3_in, INPUT_PULLUP);
  pinMode(kipas_in, INPUT_PULLUP);
  pinMode(pintu_in, INPUT_PULLUP);
  
  pinMode(lampu1_in_m, INPUT_PULLUP);
  pinMode(lampu2_in_m, INPUT_PULLUP);
  pinMode(lampu3_in_m, INPUT_PULLUP);
  pinMode(kipas_in_m, INPUT_PULLUP);
  
  pinMode(lampu1_out, OUTPUT);
  pinMode(lampu2_out, OUTPUT);
  pinMode(lampu3_out, OUTPUT);
  pinMode(kipas_out, OUTPUT);
  pinMode(buzzer_out, OUTPUT);

  digitalWrite(lampu1_out, LOW);
  digitalWrite(lampu2_out, LOW);
  digitalWrite(lampu3_out, LOW);
  digitalWrite(kipas_out, HIGH);
  digitalWrite(buzzer_out, LOW);
  digitalWrite(buzzer_out, HIGH);
  delay(1000);;
  digitalWrite(buzzer_out, LOW);

  
}

void cek_rfid() 
{
  //Serial.println("Cek RFID...");
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  if (content.substring(1) == "83 3E 5F D3") // ID TAG yang sesuai
  {
    digitalWrite(buzzer_out,HIGH);
    delay(100);
    digitalWrite(buzzer_out,LOW);
    delay(100);
    digitalWrite(buzzer_out,HIGH);
    delay(100);
    digitalWrite(buzzer_out,LOW);
    
    servo.write(90);
    delay(1000);
    servo.write(0);
    statuss = 1;
  }else{
    digitalWrite(buzzer_out,HIGH);
    delay(500);
    digitalWrite(buzzer_out,LOW);
  }
} 

void cek_servo(){
  servo.write(90);
  delay(1000);
  servo.write(0);
  delay(1000);
}

void buka(){
  servo.write(90);
}

void tutup(){
  servo.write(0);
}

void cek_kipas(){
  digitalWrite(kipas_out, HIGH);
  delay(5000);
  //digitalWrite(kipas_out, LOW);
  //delay(10000);
}

void cek_lampu(){
  digitalWrite(lampu1_out,HIGH);
  digitalWrite(lampu2_out,HIGH);
  digitalWrite(lampu3_out,HIGH);
  delay(1000);
  digitalWrite(lampu1_out,LOW);
  digitalWrite(lampu2_out,LOW);
  digitalWrite(lampu3_out,LOW);
  delay(2000);
}
void mode_(){
  State_mode = digitalRead(pin_mode);

  if(State_mode == HIGH){
    Serial.println("Auto"); 
    coba_auto();   
  }else if(State_mode == LOW){
    Serial.println("Manual");
    coba_manual();
    cek_rfid();
  }
}

void coba_manual(){
    State11 = digitalRead(lampu1_in_m);
    State22 = digitalRead(lampu2_in_m);
    State33 = digitalRead(lampu3_in_m);
    State44 = digitalRead(kipas_in_m);
    //State55 = digitalRead(pintu_in_m);

    if(State11 == LOW){digitalWrite(lampu1_out, HIGH);}else{digitalWrite(lampu1_out, LOW);}
    if(State22 == LOW){digitalWrite(lampu2_out, HIGH);}else{digitalWrite(lampu2_out, LOW);}
    if(State33 == LOW){digitalWrite(lampu3_out, HIGH);}else{digitalWrite(lampu3_out, LOW);}
    if(State44 == LOW){digitalWrite(kipas_out, LOW);}else{digitalWrite(kipas_out, HIGH);}
    //if(State55 == LOW){baca_rfid();}
}

void coba_auto(){
    State1 = digitalRead(lampu1_in);
    State2 = digitalRead(lampu2_in);
    State3 = digitalRead(lampu3_in);
    State4 = digitalRead(kipas_in);
    State5 = digitalRead(pintu_in);

    if(State1 == HIGH){digitalWrite(lampu1_out, HIGH);}else{digitalWrite(lampu1_out, LOW);}
    if(State2 == HIGH){digitalWrite(lampu2_out, HIGH);}else{digitalWrite(lampu2_out, LOW);}
    if(State3 == HIGH){digitalWrite(lampu3_out, HIGH);}else{digitalWrite(lampu3_out, LOW);}
    if(State4 == HIGH){digitalWrite(kipas_out, LOW);}else{digitalWrite(kipas_out, HIGH);}
    if(State5 == HIGH){buka();}else{tutup();}

}
void loop(){
  //mode();
  //cek_kipas();
  //cek_servo();
  //cek_rfid();
  //cek_lampu();
  //coba_manual();
  //coba_auto();
  mode_();
}

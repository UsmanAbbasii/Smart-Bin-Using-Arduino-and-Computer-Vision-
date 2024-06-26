#include<LiquidCrystal_I2C.h>
#include<Servo.h>
#include <MQ135.h>

#define PIN_MQ135 A0

MQ135 mq135_sensor(PIN_MQ135);

float temperature = 21.0; // Assume current temperature. Recommended to measure with DHT22
float humidity = 25.0; // Assume current humidity. Recommended to measure with DHT22


int air_quality;

Servo myservo;

int myservopin = 2;
int rtrig = 3;
int recho = 4;
int nrtrig = 5;
int nrecho = 6;
int rl = 7;
int pushbutton = 12;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int a;

long rdistance, rduration;
long nrdistance, nrduration;

void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  myservo.attach(myservopin);
  pinMode(rtrig, OUTPUT);
  pinMode(recho, INPUT);
  pinMode(nrtrig, OUTPUT);
  pinMode(nrecho, INPUT);
  pinMode(pushbutton, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(rl, OUTPUT);

  myservo.write(60);

    /*lcd.setCursor(0, 0);
    lcd.print("Computer Vision");
    lcd.setCursor(0, 1);
    lcd.print("Based Waste ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Management");
    lcd.setCursor(0, 1);
    lcd.print("Project ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Super Vised By:");
    lcd.setCursor(0, 1);
    lcd.print("Sir Sikandar");
    delay(3000);
    lcd.clear();
    lcd.print("Made By:");
    lcd.setCursor(0, 1);
    lcd.print("Usman Abbasi");
    delay(2000);
    lcd.clear();
    lcd.print("Muhammad Danish");
    delay(2000);
    lcd.clear();*/


}

void loop() {

  lcd.setCursor(0, 0);
  lcd.print("Use Me for Waste");

  int push = digitalRead(pushbutton);
  //Serial.println(push);

  if (push == 1) {

    Serial.print(1);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Predicting");
    lcd.setCursor(0, 1);
    lcd.print("Waste");

    while (!Serial.available());

    String index = Serial.readString();

    if (index == "0") {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("There's no");
      lcd.setCursor(0, 1);
      lcd.print("Waste");

      delay(2000);
    }

    else if (index == "1") {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Non-Recyclabe");
      lcd.setCursor(0, 1);
      lcd.print("Waste Received");

      for(int i = 60; i <= 110; i++){

        myservo.write(i);
        delay(10);
      }
      
      delay(2000);
      
      for(int i = 110; i >= 60; i--){

        myservo.write(i);
        delay(10);
      }
    }

    else if (index == "2") {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Recyclabe Waste");
      lcd.setCursor(0, 1);
      lcd.print("Received");

      for(int i = 60; i >= 0; i--){

        myservo.write(i);
        delay(10);
      }
      
      delay(2000);
      
      for(int i = 0; i <= 60; i++){

        myservo.write(i);
        delay(10);
      }
    }

    delay(500);
  }

  measureDistance();
  
  airquality();
  
  applyCondition();

  delay(50);
}


void measureDistance() {
  
  digitalWrite(rtrig, HIGH);
  delay(0.001);
  digitalWrite(rtrig, LOW);
  rduration = pulseIn(recho, HIGH);
  
  digitalWrite(nrtrig, HIGH);
  delay(0.001);
  digitalWrite(nrtrig, LOW);
  nrduration = pulseIn(nrecho, HIGH);
  
  rdistance = (rduration / 2) * 0.0343;
  nrdistance = (nrduration / 2) * 0.0343;
  
  /*Serial.print(" RDistance = ");
  Serial.print(rdistance);
  Serial.print("cm");
  Serial.print("    || NRDistance = ");
  Serial.print(nrdistance);
  Serial.println("cm");*/
}
void airquality(){
  
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  /*Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");*/
  
  lcd.setCursor(0, 1);
  lcd.print("Air:");
  lcd.print(correctedPPM);
  lcd.print("PPM   ");
  /*Serial.print("Air Quality is ");
  Serial.print(air_quality);
  Serial.println(" ppm ");*/
  delay(500);
  
  }

void applyCondition() {

  here:

  int flag = 0;

  if (rdistance <= 5 && nrdistance <= 5) {

    lcd.setCursor(0, 0);
    lcd.print("Both Portions    ");
    lcd.setCursor(0, 1);
    lcd.print("are Full.          ");
    digitalWrite(rl, HIGH);
    flag = 1;
  }

  else if (rdistance <= 5) {

    lcd.setCursor(0, 0);
    lcd.print("Recycling Side   ");
    lcd.setCursor(0, 1);
    lcd.print("is Full.           ");
    digitalWrite(rl, HIGH);
    flag = 1;
  }
  
  else if (nrdistance <= 5) {

    lcd.setCursor(0, 0);
    lcd.print("Non Recycling    ");
    lcd.setCursor(0, 1);
    lcd.print("Side is Full.        ");
    digitalWrite(rl, HIGH);
    flag = 1;
  }

  if(flag == 1){

    measureDistance();
    goto here;
  }

  else{

    flag = 0;
  }

  digitalWrite(rl, LOW);
}

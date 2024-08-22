#include <IRremote.h>
#include <LCD-I2C.h>
LCD_I2C lcd(0x27, 16, 2);
#define Power 69
#define SensorOn 70
#define Mute 71
#define PauseandResume 68
#define Left 22
#define Right 67
#define EQ 7
#define Down 21
#define Up 9
#define Zero 22
#define One 12
#define Two 24
#define Three 94
#define Four 8
#define Five 28
#define Six 90
#define Seven 66
#define Eight 82
#define Nine 74
#define Reverse 25
#define Misc 13
int fwd1 = 9;
int back1 = 10;
int fwd2 = 3;
int back2 = 6;
int speed = 11;
int speed2 = 13;
int speedStat;
int echoPin=5;
int trigPin=2;

bool checker = true;

#define IR_RECEIVE_PIN 12
void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);
  pinMode(fwd1, OUTPUT);
  pinMode(back1, OUTPUT);
  pinMode(fwd2, OUTPUT);
  pinMode(back2, OUTPUT);
  pinMode(speed, OUTPUT);
  pinMode(speed2, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  lcd.begin();
  lcd.display();
  lcd.backlight();
  
}
float readDistance() {
  digitalWrite(trigPin, LOW);   // Set trig pin to low to ensure a clean pulse
  delayMicroseconds(2);         // Delay for 2 microseconds
  digitalWrite(trigPin, HIGH);  // Send a 10 microsecond pulse by setting trig pin to high
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Set trig pin back to low

  // Measure the pulse width of the echo pin and calculate the distance value
  float distance = pulseIn(echoPin, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
  return distance;
}
void loop() {
  
  
    
  
  while (IrReceiver.decode()==0) {
    digitalWrite(trigPin, LOW);   // Set trig pin to low to ensure a clean pulse
    delayMicroseconds(2);         // Delay for 2 microseconds
    digitalWrite(trigPin, HIGH);  // Send a 10 microsecond pulse by setting trig pin to high
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    float distance = readDistance();
    //Serial.println(distance);
    if (distance<70){
      
      lcd.clear();
      lcd.print("Obstacle Detected!");
      digitalWrite(fwd1, LOW);
      digitalWrite(back1, LOW);
      digitalWrite(fwd2, LOW);
      digitalWrite(back2, LOW);
      checker=true;
    }
    else{
      
    }
  }
  delay(500);
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    Serial.println(command);
    //delay(500);
    switch (command) {
      case PauseandResume: {
        if (checker==true){
          lcd.clear();
          lcd.print("Go!");
          digitalWrite(fwd1, HIGH);
          digitalWrite(back1, LOW);
          digitalWrite(fwd2, HIGH);
          digitalWrite(back2, LOW);
          speedStat=255;
          analogWrite(speed, speedStat);
          analogWrite(speed2, speedStat);
          checker=false;
          break;
        }
        else{
          lcd.clear();
          lcd.print("Stop!");
          digitalWrite(fwd1, LOW);
          digitalWrite(back1, LOW);
          digitalWrite(fwd2, LOW);
          digitalWrite(back2, LOW);
          checker=true;
          break;
        }
      }
      case Reverse: {
        lcd.clear();
        lcd.print("Reversing!");
        digitalWrite(fwd1, !digitalRead(fwd1));
        digitalWrite(back1, !digitalRead(back1));
        digitalWrite(fwd2, !digitalRead(fwd2));
        digitalWrite(back2, !digitalRead(back2));
        break;
      }
      
      case Up: {
        lcd.clear();
        lcd.print("Speeed!");
        speedStat=speedStat+42;
        analogWrite(speed, speedStat);
        analogWrite(speed2, speedStat);
        Serial.println("Speed: ");
        Serial.println(speedStat);
        break;
      }
      case Down: {
        lcd.clear();
        lcd.print("Slooow..");
        speedStat=speedStat-42;
        analogWrite(speed, speedStat);
        analogWrite(speed2, speedStat);
        Serial.println("Speed: ");
        Serial.println(speedStat);
        break;
      }
      case Left: {
        lcd.clear();
        lcd.print("Left!");
        digitalWrite(fwd1, HIGH);
        digitalWrite(back1, LOW);
        digitalWrite(fwd2, LOW);
        digitalWrite(back2, LOW);
        delay(1600);
        break;
      }

      case Right: {
        lcd.clear();
        lcd.print("Right!");
        digitalWrite(fwd1, LOW);
        digitalWrite(back1, LOW);
        digitalWrite(fwd2, HIGH);
        digitalWrite(back2, LOW);
        delay(1600);
        break;
      }
      
      default: {
        // do nothing
      }
    }
    
  
}
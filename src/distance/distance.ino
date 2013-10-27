// HC-SR04 proof of concept
// Reference:  http://arduinobasics.blogspot.com/2013/01/arduino-basics-sonar-project-tutorial.html

#include <avr/sleep.h>

// http://playground.arduino.cc/Learning/arduinoSleepCode


int echoPin = 12;
int trigPin = 8;

int greenLed   = 3; //pwm
int yellowLed  = 4;
int redLed     = 5; // pwm


// Potentiometer setup
int greenSetting  = A0;
int yellowSetting = A1;
int redSetting    = A2;

// Default tolerance distances
int greenTolerance  = 140;
int yellowTolerance = 80;
int redTolerance    = 30;

long duration;

void setup() {
  // pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  
  digitalWrite(echoPin, LOW);
  
  // Distances in centimeters
  greenTolerance  = map(analogRead(greenSetting), 0, 1023, 0, 500);
  yellowTolerance = map(analogRead(yellowSetting), 0, 1023, 0, 500);  
  redTolerance    = map(analogRead(redSetting), 0, 1023, 0, 500);    
  
  Serial.println("Green : " + String(greenTolerance) + "cm");
  Serial.println("Yellow: " + String(yellowTolerance) + "cm");  
  Serial.println("Red   : " + String(redTolerance) + "cm");  
  Serial.println("-------------------------");  
    
  
  Serial.begin(9600);
  
  flashLeds();
   
}

void flashLeds() {
  digitalWrite(greenLed, HIGH);
  delay(250);
  digitalWrite(greenLed, LOW);
  
  digitalWrite(yellowLed, HIGH);
  delay(250);  
  digitalWrite(yellowLed, LOW);
  
  digitalWrite(redLed, HIGH);
  delay(250); 
  digitalWrite(redLed, LOW);  
}

void showGreen() {
  digitalWrite(greenLed, HIGH);
  digitalWrite(yellowLed, LOW);  
  digitalWrite(redLed, LOW);    
}

void showYellow() {
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, HIGH);  
  digitalWrite(redLed, LOW);      
}

void showRed() {
  digitalWrite(greenLed, LOW);
  digitalWrite(yellowLed, LOW);  
  digitalWrite(redLed, HIGH);  
}


void loop() {
//  Serial.println("v1.5");
  double distance = getDistance();
  
  if (distance > 500) {
    return;
  }
  
  if (distance < redTolerance) {
    showRed();
  } else if(distance < yellowTolerance) {
    showYellow();
  } else if (distance < greenTolerance) {
    showGreen();
  } else {
    flashGreen();
  }
  
  delay(200);
}


void flashGreen() {
  digitalWrite(greenLed, LOW);  
  digitalWrite(yellowLed, LOW);  
  digitalWrite(redLed, LOW);   
  delay(200);
  showGreen();
}

  
double getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
//  Serial.println("durationOfEcho:");
//  Serial.println(duration);    
  
  // Distance = ((Duration of high level)*(Sonic :340m/s))/2
  double distance = (duration/58.2);  
  if (distance < 500) {
    Serial.print(distance);
    Serial.println("cm");
  }
 
  return distance;
}


void wakeUpNow() {
}

void sleepNow() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(0, wakeUpNow, LOW);
  sleep_mode();
  sleep_disable();
  detachInterrupt(0);
}
  

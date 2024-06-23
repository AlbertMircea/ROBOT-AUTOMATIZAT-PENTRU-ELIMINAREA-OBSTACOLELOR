#define newInfra 13
#define manualOnOff A2

const byte LeftRight = 5;
const byte UpDown = 9;

int in1 = 2;
int in2 = 4;
int enA = 3;

int in3 = 8;
int in4 = 7;
int enB = 6;

int speedA = 90;
int speedB = speedA + 13;

int speedInvarteA = 80;
int speedInvarteB = 90;

const int trigPin = 10;
const int echoPin = 11;

const int ledPin = 12;

int infr;
int newINFR = 0;
long cm;
int entry = 0;
byte manualFunction = 0;

void setup() {

  Serial.begin(9600);
  
  pinMode(LeftRight, INPUT);
  pinMode(UpDown, INPUT);
  pinMode(manualOnOff, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(newInfra, INPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(ledPin, OUTPUT);

  delay(3000);
}

void loop() {
  invarteFata();
  cm = readUltraSonicSensor();
  if (cm <= 40) {
    stop();
    do
    {
      invarteSpate();
      delay(50);
      cm = readUltraSonicSensor();
    }while(cm>40);
    mergeFata();
    bool ok = false;
    while (ok == false) {
      manualFunction = GetPWM(manualOnOff);
      if(manualFunction == 10){
        manualON();
      }
      newINFR = digitalRead(newInfra);
      if (newINFR == LOW) { 
        stop();
    	  delay(1000); 
        entry++;
        invarteFata();
        if(entry == 3){
          stop();
          digitalWrite(ledPin, HIGH);
          while(1){
            manualFunction = GetPWM(manualOnOff);
            Serial.println(manualFunction);
            if(manualFunction == 10){
              manualON();
            }
          }
        }
        mergeSpate();
        delay(500);
        ok = true;
      }
    }
  }
}


void manualON()
{
  while(1)
  {
    byte leftRight = GetPWM(LeftRight);
    byte upDown = GetPWM(UpDown);
    byte onOff = GetPWM(manualOnOff);
    
    if(onOff == 5)
      break;
    
    if((leftRight==7)&&(upDown==7))
    {
      stop();
    }
    
    else if(upDown>7)
    {
      mergeFata();
    }
    else if(upDown<7)
    {
      mergeSpate();
    }
    else if(leftRight>7)
    {
      invarteSpateManual();
    }
    else if(leftRight<7)
    {
      invarteFataManual();
    }
    else
    {
      stop();
    }
    delay(5);
  }
}

byte GetPWM(byte pin)
{
  unsigned long highTime = pulseIn(pin, HIGH, 50000UL);  
  unsigned long lowTime = pulseIn(pin, LOW, 50000UL);  

  if (highTime == 0 || lowTime == 0)
    return digitalRead(pin) ? 100 : 0;  

  return (100 * highTime) / (highTime + lowTime);  
}


void mergeFata() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, speedA);
  analogWrite(enB, speedB);
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void invarteFata() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, speedInvarteA);
  analogWrite(enB, speedInvarteB);
}
void invarteFataManual() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 85);
  analogWrite(enB, 85);
}

void invarteSpate() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, speedInvarteA);
  analogWrite(enB, speedInvarteB);
}
void invarteSpateManual() {

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 85);
  analogWrite(enB, 85);
}

void mergeSpate() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, speedA);
  analogWrite(enB, speedB);
}

long readUltraSonicSensor() {
  long duration, inches, cm;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  return cm;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

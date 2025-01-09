#define newInfra 13
const int trigPin = 10;
const int echoPin = 11;

int in1 = 2;
int in2 = 4;
int enA = 3;

int in3 = 8;
int in4 = 7;
int enB = 6;

int speedA = 90;
int speedB = speedA + 13;

int speedInvarteA = 85;  // Define missing variables
int speedInvarteB = 85;

int newINFR;
long cm;
int entry = 0;
byte autoDone = 0;

int manual = 0;

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(newInfra, INPUT);

    // Initialize motor pins as output
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(enB, OUTPUT);

    while (!Serial) {
        ; // Wait for serial port to connect. Needed for native USB
    }
}

void loop() {
  if(autoDone == 0)
  {
    invarteSpate();
    cm = readUltraSonicSensor();
    if (cm <= 40 && cm != -1) {
      stop();
      delay(500);
      do
      {
        invarteFata();
        delay(50);
        cm = readUltraSonicSensor();
      }while(cm>40 || cm == -1);
      mergeFata();
      bool ok = false;
      while (ok == false) {
        newINFR = digitalRead(newInfra);
      Serial.print(newINFR);
      Serial.print(",");
      Serial.println(cm);
        if (newINFR == 0) { 
          stop();

    	    delay(1000); 
          entry++;
          //invarteSpate();
          if(entry == 3){
            stop();
            autoDone = 1;
            ok= true;
          }
          else
          {
          
            mergeSpate();
            delay(500);
            ok = true;
          }
          }
        }
      }
    }
    else{
  cm = readUltraSonicSensor();
        newINFR = digitalRead(newInfra);
      Serial.print(newINFR);
      Serial.print(",");
      Serial.println(cm);
      delay(1000);

  // Check if a command is available and process it immediately
  if (Serial.available() > 0) {
      String command = Serial.readStringUntil('\n');  // Read command from serial
      command.trim();  // Remove any extra whitespace or newline characters

      // Process the command
      if (command == "FORWARD") {
          mergeFata();
          Serial.println("ACK: FORWARD");
      } else if (command == "BACKWARD") {
          mergeSpate();
          Serial.println("ACK: BACKWARD");
      } else if (command == "LEFT") {
          invarteFata();
          Serial.println("ACK: LEFT");
      } else if (command == "RIGHT") {
          invarteSpate();
          Serial.println("ACK: RIGHT");
      } else if (command == "STOP") {
          stop();
          Serial.println("ACK: STOP");
      } else if (command == "RESET") {
          stop();
          autoDone = 0;
          entry = 0;
          Serial.println("ACK: RESET");
      } else {
          Serial.println("ERROR: UNKNOWN COMMAND");
      }
  }
    }
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

void invarteSpate() {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enA, speedInvarteA);
    analogWrite(enB, speedInvarteB);
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
    long duration, cm;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout
    if (duration == 0) {
        // Timeout occurred, return a default value
        return -1;
    }
    cm = microsecondsToCentimeters(duration);

    return cm;
}

long microsecondsToCentimeters(long microseconds) {
    return microseconds / 29 / 2;
}

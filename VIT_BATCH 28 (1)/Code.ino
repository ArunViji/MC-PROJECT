int PIR = 4;
int Bulb = 12;
int currentPeople = 0;
#define trigPin1 5
#define echoPin1 6
#define trigPin2 8
#define echoPin2 9
String sequence = "";
int timeoutCounter = 0;
unsigned long previousMillis = 0;
const long interval = 1000;
unsigned long ultrasonicInterval = 100;

void setup()
{
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  pinMode(Bulb, OUTPUT);
  digitalWrite(Bulb, LOW);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

void loop()
{
	 unsigned long currentMillis = millis();

	// Read ultrasonic sensors at a fixed interval
     if (currentMillis - previousMillis >= ultrasonicInterval) {
    		previousMillis = currentMillis;

   		 int sensor1Val = measureDistance(trigPin1, echoPin1);
   		 int sensor2Val = measureDistance(trigPin2, echoPin2);

    		// Process the data
   	     if (sensor1Val < 150 && sequence.charAt(0) != '1') {
      		sequence += "1";
   		 }
   		 else if (sensor2Val < 150 && sequence.charAt(0) != '2') {
     		 sequence += "2";
		 }

   	 	if (sequence.equals("12")) {
      		currentPeople++;
      		sequence = "";
   		 }
   		 else if (sequence.equals("21") && currentPeople > 0) {
      		currentPeople--;
     		 sequence = "";
		 }

    		// Resets the sequence if it is invalid or timeouts
   		if (sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200) {
      		sequence = "";
    		}

    		if (sequence.length() == 1) {
      		timeoutCounter++;
   		 }
   		 else {
     		 timeoutCounter = 0;
	   	 }

    // Print values to serial
    Serial.print("Seq: ");
    Serial.print(sequence);
    Serial.print("No. of People : ");
    Serial.println(currentPeople);
  }

  if (currentPeople > 0) {
    if (digitalRead(PIR) == HIGH) {
      digitalWrite(Bulb, HIGH);
      previousMillis = currentMillis;
    }
    else {
      if (currentMillis - previousMillis <= 120000) {
        digitalWrite(Bulb, HIGH);
      }
      else {
        digitalWrite(Bulb, LOW);
      }
    }
  }
  else {
    digitalWrite(Bulb, LOW);
  }
}

// Returns the distance of the ultrasonic sensor that is passed in
int measureDistance(int tp, int ep) {
  digitalWrite(tp, LOW);
  delayMicroseconds(2);
  digitalWrite(tp, HIGH);
  delayMicroseconds(10);
  digitalWrite(tp, LOW);
  int duration = pulseIn(ep, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

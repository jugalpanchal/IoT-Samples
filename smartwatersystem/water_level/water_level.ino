
/* Ultrasonic Sensor */
int trigPin = 12;
int echoPin = 11;

/* Motor */
int transistorMiddlePin = 8;

void setup()
{
  Serial.begin(9600);

  /* Ultrasonic Sensor */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); 

  /* Motor */
  pinMode(transistorMiddlePin, OUTPUT);
}

void loop()
{
  
  long distance = getDistance();

  long heightTank =15;
  long waterLevel = heightTank-distance;

  if(waterLevel <= 3)
    setMotorState(false);
  else
    setMotorState(true);
}

void setMotorState(bool state)
{
  if(state)
    digitalWrite(transistorMiddlePin, HIGH);
  else
    digitalWrite(transistorMiddlePin, LOW);
}

long getDistance()
{
  long t = 0, h = 0, hp = 0;
  
  // Transmitting pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Waiting for pulse
  t = pulseIn(echoPin, HIGH);// t is length of pulse in microsecond
  //works on pulses from 10 microsecond to 3 min length
  
  // Calculating distance 
  h = t / 58;//h is hight in CM //if need hight in inch then divide by 148
  
  //h = h - 6;  // offset correction
  //h = 50 - h;  // water height, 0 - 50 cm
  //hp = 2 * h;  // distance in %, 0-100 %

  
  // Sending to computer
  Serial.print(h);
  //Serial.print(hp);
  // Serial.print(" cm\n");
  Serial.print("\n");
  
  delay(1000);

  return h;
}





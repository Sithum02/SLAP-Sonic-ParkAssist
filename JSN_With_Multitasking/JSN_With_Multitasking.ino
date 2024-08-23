// Define the class
const int MAX_DIST = 4665;
const int MAX_DELAY = 750;
unsigned long current_time_sensor = 0;
unsigned long current_time_buzzer = 0;


class Sensor {
public:
  int id;
  int trigPin;
  int echoPin;
  long duration;
  int distance;
  unsigned long previous_time_sensor = 0;

  // Constructor
  Sensor(int id, int trig, int echo) {
    this->id = id;
    this->trigPin = trig;
    this->echoPin = echo;
  }

  // Method to get distance
  int getDistance() {
    return this->distance;
  }

  void setPins() {
    pinMode(this->trigPin, OUTPUT);
    pinMode(this->echoPin, INPUT);
  }

  // Method to trigger sensor
  void trigger() {
    // Clears the trigPin
    digitalWrite(this->trigPin, LOW);
    if (current_time_sensor - this->previous_time_sensor > 2) {
      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(this->trigPin, HIGH);
      this->previous_time_sensor = current_time_sensor;
      if (current_time_sensor - this->previous_time_sensor > 10) {
        digitalWrite(this->trigPin, LOW);
      }
    }


    // Reads a pulse (either HIGH or LOW) on a pin. For example,
    // if value is HIGH, pulseIn() waits for the pin to go from LOW to HIGH, starts timing, then waits for the pin to go LOW and stops timing.
    // Returns the length of the pulse in microseconds or gives up and returns 0 if no complete pulse was received within the timeout.
    this->duration = pulseIn(this->echoPin, HIGH, 30000);

    // Constraining the values within the required limit
    //this->duration = constrain(this->duration, 1068, MAX_DIST);

    // Mapping each duration to an BuzzerLevel, while inverting it
    // BuzzerLevel = map(duration, 250, 1000, 255, 0);
    this->distance = this->duration / 2 * 343 / 10000;
    if (this->distance == 0) {
      this->distance = 80;
    }

    // this->display();
  }

  // Method to display distance and activate speakers
  // void display() {
  //   if (this->distance != 183) {
  //     Serial.print(this->id);
  //     Serial.print(": ");
  //     Serial.println(this->distance);
  //   }
  // }
};

class Buzzer {
public:
  int id;
  int pin;
  unsigned long buzzer_delay;
  unsigned long previous_time_buzzer = 0;

  //Constructor
  Buzzer(int id, int pin) {
    this->id = id;
    this->pin = pin;
  } 

  void setPins() {
    pinMode(this->pin, OUTPUT);
  }

  void setDelay(int sensor_1_dist, int sensor_2_dist) {
    // Select the closest  distance
    int distance = min(sensor_1_dist, sensor_2_dist);

    // Constrain the distane
    distance = constrain(distance, 20, 80);
    // Serial.print(distance);
    // Serial.print(" ");
    // Serial.println(this->buzzer_delay);
    this->buzzer_delay = map(distance, 20, 80, 150, 750)*1000;
  }

  void trigger() {
    if (this->buzzer_delay < 750000){
      if (current_time_buzzer - this->previous_time_buzzer >this->buzzer_delay) {
        this->previous_time_buzzer = current_time_buzzer;
        digitalWrite(this->pin, HIGH);
      }
      if (current_time_buzzer - this->previous_time_buzzer > this->buzzer_delay/2) {
        digitalWrite(this->pin, LOW);
      }
    }

    else{
      digitalWrite(this->pin, LOW);
    }
  }

};

// Instantiate an object of the class

// id trig echo
Sensor sensor1(1, 4, 5);
Sensor sensor2(2, 6, 7);
Sensor sensor3(3, 8, 9);
Sensor sensor4(4, 10, 11);
Sensor sensor5(5, 12, 13);
Sensor sensor6(6, 14, 15);
Sensor sensor7(7, 16, 17);
Sensor sensor8(8, 18, 19); // Analog pins designated digital pin names. 

// id, pin
Buzzer Buzzer1(1, 0);
Buzzer Buzzer2(2, 1);
Buzzer Buzzer3(3, 2);
Buzzer Buzzer4(4, 3);


void setup() {
  
  sensor1.setPins();
  sensor2.setPins();
  sensor3.setPins();
  sensor4.setPins();
  sensor5.setPins();
  sensor6.setPins();
  sensor7.setPins();
  sensor8.setPins();

  Buzzer1.setPins();
  Buzzer2.setPins();
  Buzzer3.setPins();
  Buzzer4.setPins();
}

void loop() {
  current_time_sensor = micros();
  current_time_buzzer = micros();
  sensor1.trigger();
  sensor2.trigger();
  sensor3.trigger();
  sensor4.trigger();
  sensor5.trigger();
  sensor6.trigger();
  sensor7.trigger();
  sensor8.trigger();

  Buzzer1.setDelay(sensor1.distance, sensor2.distance);
  Buzzer2.setDelay(sensor3.distance, sensor4.distance);
  Buzzer3.setDelay(sensor5.distance, sensor6.distance);
  Buzzer4.setDelay(sensor7.distance, sensor8.distance);

  Buzzer1.trigger();
  Buzzer2.trigger();
  Buzzer3.trigger();
  Buzzer4.trigger();

}

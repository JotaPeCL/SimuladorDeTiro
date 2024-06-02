#include <Wire.h>
#include <MPU6050.h>

MPU6050 sensor;

float initial_ax, initial_ay, initial_az;
bool isInitialized = false;

const int buttonPin1 = 12; // Pin del primer botón
const int buttonPin2 = 11; // Pin del segundo botón
const int buttonPin3 = 9; // Pin del tercer botón
const int buttonPin4 = 8; // Pin del cuarto botón

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sensor.initialize();

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);

  // Esperamos a que el sensor esté listo
}

void loop() {
  int16_t ax, ay, az;
  sensor.getAcceleration(&ax, &ay, &az);

  // Inicializamos los valores iniciales la primera vez
  if (!isInitialized) {
    initial_ax = ax;
    initial_ay = ay;
    initial_az = az;
    isInitialized = true;
  }

  float denom_y = sqrt(pow(ax, 2) + pow(az, 2));
  float accel_ang_y = (denom_y == 0) ? 0 : atan(ay / denom_y) * (180.0 / 3.14);

  float relative_ax = ax - initial_ax;
  float relative_ay = ay - initial_ay;
  float relative_az = az - initial_az;

  float denom_x = sqrt(pow(relative_ay, 2) + pow(relative_az, 2));
  float accel_ang_x = (denom_x == 0) ? 0 : atan(relative_ax / denom_x) * (180.0 / 3.14);

  float denom_z = sqrt(pow(ax, 2) + pow(az, 2));
  float accel_ang_z = (denom_z == 0) ? 0 : atan(az / denom_z) * (180.0 / 3.14);

  // Lectura de los botones
  bool buttonState1 = digitalRead(buttonPin1) == LOW ? 1 : 0;
  bool buttonState2 = digitalRead(buttonPin2) == LOW ? 1 : 0;
  int buttonState3 = digitalRead(buttonPin3) == LOW ? -1 : 0;
  int buttonState4 = digitalRead(buttonPin4) == LOW ? 1 : 0;

  // Enviar datos del MPU6050 y botones
  Serial.print(accel_ang_y);
  Serial.print(", ");
  Serial.print(accel_ang_x);
  Serial.print(", ");
  Serial.print(accel_ang_z);
  Serial.print(", ");
  Serial.print(buttonState1);
  Serial.print(", ");
  Serial.print(buttonState2);
  Serial.print(", ");
  Serial.print(buttonState3);
  Serial.print(", ");
  Serial.println(buttonState4);

  delay(100);
}

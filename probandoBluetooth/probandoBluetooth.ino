#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define IN1 12
#define IN2 13
#define IN3 27
#define IN4 14

#define echo 25
#define trig 17

int delayAv = 200;
int delayRe = 400;
int delayDe = 650;
int delayIz = 650;
int delayDet = 500;

int brightness = 100;
int motorSpeed = 100;

int count = 0;
bool modoAutomatico = false;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  SerialBT.begin("ESP32_BT");
  Serial.println("Bluetooth iniciado. Esperando comandos...");
}

// Función de movimiento
void moverMotores(int m1a, int m1b, int m2a, int m2b, int duracion = 300) {
  analogWrite(IN1, m1a);
  analogWrite(IN2, m1b);
  analogWrite(IN3, m2a);
  analogWrite(IN4, m2b);
  delay(duracion);
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Sensor ultrasónico
float medirDistancia() {
  long duration, distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 58;
  if (distance < 2 || distance > 400) {
    distance = -1;
  }
  return distance;
}

void loop() {
  // Modo automático con sensor
  if (modoAutomatico) {
    sensorChoque();
  }

  // Comando por Bluetooth
  if (SerialBT.available()) {
    char comando = SerialBT.read();
    Serial.println("Comando recibido: " + String(comando));

    switch (comando) {
      case 'w': avanzar(); break;
      case 's': retroceder(); break;
      case 'a': girarIzq(); break;
      case 'd': girarDer(); break;
      case 'x': detener(); break;
      case 'm': modoAutomatico = !modoAutomatico;
                SerialBT.println(modoAutomatico ? "Modo AUTO activado" : "Modo MANUAL activado");
                break;
      default: SerialBT.println("Comando no reconocido");
    }
  }
}

// Movimientos
void avanzar() {
  Serial.println("Avanzando");
  moverMotores(motorSpeed, 0, 0, motorSpeed, delayAv);
}

void detener() {
  Serial.println("Deteniendo");
  moverMotores(0, 0, 0, 0, delayDet);
}

void retroceder() {
  Serial.println("Retrocediendo");
  moverMotores(0, motorSpeed, motorSpeed, 0, delayRe);
}

void girarDer() {
  Serial.println("Girando derecha");
  moverMotores(motorSpeed, 0, 0, 0, delayDe);
}

void girarIzq() {
  Serial.println("Girando izquierda");
  moverMotores(0, 0, 0, motorSpeed, delayIz);
}

void inversoGirarIzq() {
  Serial.println("Girando izquierda inverso");
  moverMotores(0, 0, 0, motorSpeed, delayIz);
}

void sensorChoque() {
  float distancia = medirDistancia();
  if (distancia > 20 && distancia != -1) {
    count = 0;
    avanzar();
  } else if (distancia <= 20 && distancia != -1) {
    count++;
    if (count > 2) {
      evitarChoque();
      Serial.println("Evitando choque");
    } else {
      detener();
      girarDer();
      detener();
    }
  }
}

void evitarChoque() {
  retroceder();
  detener();
  inversoGirarIzq();
  detener();
}

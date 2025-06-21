// Pines de control para el puente H

#define IN1 12  // Motor A // Rueda Izquierda Ade
#define IN2 13             // 
#define IN3 27  // Motor B // 
#define IN4 14             // Rueda derecha Ade

#define echo 25 // Amarillo
#define trig 17 // Verde

int delayAv = 200;
int delayRe = 400;
int delayDe = 650;
int delayIz = 650;

int brightness = 100;
int motorSpeed = 100;

int count = 0;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

}

// Funcion de distancia
float medirDistancia() {
  long duration, distance;
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  duration = pulseIn(echo, HIGH);

  distance = duration / 58;

  if (distance < 2  || distance > 400) {
    distance = -1;
  }
  
  return distance;

}

void loop() {
  sensorChoque();

}

void detener() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  delay(500);

}

void avanzar() {
  analogWrite(IN1, motorSpeed);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, motorSpeed);
  delay(delayAv);

}

void retroceder() {
  analogWrite(IN1, 0);
  analogWrite(IN2, motorSpeed);
  analogWrite(IN3, motorSpeed);
  analogWrite(IN4, 0);
  delay(delayRe);

}

void girarDer() {
  analogWrite(IN1, motorSpeed);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
  delay(delayDe);

}

void girarIzq() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, motorSpeed);
  delay(delayIz);
}

void inversoGirarIzq() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, motorSpeed);
  analogWrite(IN4, 0);
  delay(delayIz);
}

void sensorChoque(){
  float distancia = medirDistancia();

  if(distancia > 20 && distancia != -1) {
    count=0;
    avanzar();
  } else if (distancia <= 20 && distancia != -1) {
    count++;
    if(count>2){
      evitarChoque();
    } else{
      detener();
      girarDer();
      detener();
    }
  }
}

void evitarChoque(){
  retroceder();
  detener();
  inversoGirarIzq();
  detener();
}
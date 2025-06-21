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
int delayDet = 500;

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

  Serial.begin(9600);

}

// Funciones básicas de movimiento del robot
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

void avanzar() {
  try {
  Serial.println("Avanzando");
  moverMotores(motorSpeed, 0, 0, motorSpeed, delayAv);
  }
    return true;
  } catch (...) {
    Serial.println("Error al avanzar");
    return false;
  }
  

void detener() {
  try {
  Serial.println("Deteniendo");
  moverMotores(0, 0, 0, 0, delayDet);
  }
    return true;
  } catch (...) {
    Serial.println("Error al detener");
    return false;
  }
}

void retroceder() {
  try {
  Serial.println("Retrocediendo");
  moverMotores(0, motorSpeed, motorSpeed, 0, delayRe);
  }
    return true;
  } catch (...) {
    Serial.println("Error al retroceder");
    return false;
  }
}

void girarDer() {
  try {
  Serial.println("Girando derecha");
  moverMotores(motorSpeed, 0, 0, 0, delayDe);
  }
    return true;
  } catch (...) {
    Serial.println("Error al girar derecha");
    return false;
  }
}

void girarIzq() {
  try {
  Serial.println("Girando izquierda");
  moverMotores(0, 0, 0, motorSpeed, delayIz);
  }
    return true;
  } catch (...) {
    Serial.println("Error al girar izquierda");
    return false;
  }
}

void inversoGirarIzq() {
  try {
  Serial.println("Girando izquierda inverso");
  moverMotores(0, 0, 0, motorSpeed, delayIz);
  }
    return true;
  } catch (...) {
    Serial.println("Error al girar izquierda inverso");
    return false;
  }
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
      Serial.println("Evitando choque");
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
#include <ESP8266WiFi.h>

// WiFi
const char* ssid = "LAB. ROBOTICA";
const char* password = "LabRoboticaUAI";
WiFiServer server(80);

// Pines motores
#define IN1 D1  // GPIO12
#define IN2 D2  // GPIO13
#define IN3 D5  // GPIO14
#define IN4 D6  // GPIO15

// Sensor ultrasónico
#define echo D2  // GPIO4
#define trig D1  // GPIO5

int delayAv = 200;
int delayRe = 400;
int delayDe = 650;
int delayIz = 650;
int delayDet = 500;

int motorSpeed = 180;  // Usaremos HIGH/LOW en lugar de PWM

int count = 0;
bool modoAutomatico = false;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a WiFi");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {

  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    String request = client.readStringUntil('\r');
    Serial.println("Petición: " + request);
    client.flush();

    if (request.indexOf("GET /w") != -1) avanzar();
    else if (request.indexOf("GET /s") != -1) retroceder();
    else if (request.indexOf("GET /a") != -1) girarIzq();
    else if (request.indexOf("GET /d") != -1) girarDer();
    else if (request.indexOf("GET /x") != -1) detener();
    else if (request.indexOf("GET /m") != -1) {
      modoAutomatico = !modoAutomatico;
      detener();
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    
    // Página web con botones y AJAX
    client.println(R"rawliteral(
      <!DOCTYPE html>
      <html>
      <head>
        <meta charset="UTF-8">
        <title>Robot ESP8266</title>
        <style>
          button {
            width: 100px;
            height: 50px;
            font-size: 16px;
            margin: 5px;
          }
        </style>
      </head>
      <body>
        <h2>Control del Robot</h2>
        <button onclick="enviar('w')">Avanzar</button><br>
        <button onclick="enviar('a')">Derecha</button>
        <button onclick="enviar('x')">Detener</button>
        <button onclick="enviar('d')">Izquierda</button><br>
        <button onclick="enviar('s')">Retroceder</button><br>

        <script>
          function enviar(cmd) {
            fetch('/' + cmd)
              .then(response => console.log("Comando enviado: " + cmd))
              .catch(err => console.error("Error:", err));
          }
        </script>
      </body>
      </html>
    )rawliteral");

    delay(1);
    Serial.println("Cliente desconectado");
  }
}

// ---------- Movimiento ----------
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

void avanzar() {
  Serial.println("Avanzando");
  moverMotores(motorSpeed, 0, motorSpeed, 0, delayAv);
}

void retroceder() {
  Serial.println("Retrocediendo");
  moverMotores(0, motorSpeed, 0, motorSpeed, delayRe);
}

void detener() {
  Serial.println("Deteniendo");
  moverMotores(0, 0, 0, 0, delayDet);
}



void girarIzq() {
  Serial.println("Girando derecha");
  moverMotores(motorSpeed, 0, 0, 0, delayDe);
}

void girarDer() {
  Serial.println("Girando izquierda");
  moverMotores(0, 0, motorSpeed, 0, delayIz);
}
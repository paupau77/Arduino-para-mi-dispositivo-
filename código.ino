#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----- Configuración pantalla OLED -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ----- Pines -----
const int botonPin = 2;       // Botón conectado al pin digital 2
const int sensorPin = A0;     // Sensor conectado al pin analógico A0

// ----- Variables -----
int sensorValue = 0;
bool pausa = false;
bool estadoBoton = false;
bool estadoBotonAnterior = HIGH;
unsigned long tiempoUltimoCambio = 0;
const unsigned long debounceDelay = 200;  // Delay antirrebote

const float maxConductivity = 50.0;  // mS/cm (valor de ejemplo)

// ----- SETUP -----
void setup() {
  pinMode(botonPin, INPUT_PULLUP);
  pinMode(sensorPin, INPUT);
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error al inicializar la pantalla OLED"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Medidor Salinidad"));
  display.display();
  delay(2000);
}

// ----- LOOP PRINCIPAL -----
void loop() {
  estadoBoton = digitalRead(botonPin);

  // --- Antirrebote y detección de flanco de bajada (pulsación) ---
  if (estadoBoton == LOW && estadoBotonAnterior == HIGH &&
      (millis() - tiempoUltimoCambio) > debounceDelay) {
    pausa = !pausa;
    tiempoUltimoCambio = millis();
  }
  estadoBotonAnterior = estadoBoton;

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(F("Medidor Salinidad"));

  if (!pausa) {
    sensorValue = analogRead(sensorPin);
    float voltaje = sensorValue * (5.0 / 1023.0);
    float conductividad = convertirAConductividad(sensorValue);

    display.print(F("Lectura: "));
    display.println(sensorValue);

    display.print(F("Voltaje: "));
    display.print(voltaje, 2);
    display.println(F(" V"));

    display.print(F("Cond: "));
    display.print(conductividad, 2);
    display.println(F(" mS/cm"));

    Serial.print("ADC: ");
    Serial.print(sensorValue);
    Serial.print(" | Voltaje: ");
    Serial.println(voltaje);
  } else {
    display.println(F("Estado: PAUSA"));
  }

  display.display();
  delay(300);  // Tiempo de refresco
}

// ----- Función para convertir ADC a conductividad (lineal) -----
float convertirAConductividad(int val) {
  return (val / 1023.0) * maxConductivity;
}

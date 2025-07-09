/*
  Medidor de Salinidad con Arduino y Pantalla OLED
  Autor: Paulina Juich
  Licencia: © 2025 Paulina Juich. Todos los derechos reservados.

Este diseño, documentación y código están protegidos por la legislación de propiedad intelectual. 

🧠 El uso personal, académico o educativo sin fines de lucro está permitido con atribución.
💰 Cualquier uso comercial, distribución, modificación o integración en productos requiere una licencia paga o autorización expresa.

Contacto para licencias: paulinajuich4@gmail.com

  Mi código va mejorado con:
  - Calibración ajustable con soluciones patrón
  - Comentarios detallados en el código
  - Manejo básico de errores en sensor y pantalla
  - Antirrebote optimizado con temporización
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definiciones OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pines
const int sensorPin = A0;      // Pin analógico para sensor de conductividad
const int buttonPin = 2;       // Pin para pulsador

// Variables de calibración
float maxConductivity = 50.0;  // Conductividad máxima (mS/cm), puede calibrarse

// Variables para botón con antirrebote
bool medirActivo = true;       // Estado para medir o pausar
bool ultimoEstadoBoton = HIGH;
unsigned long ultimoTiempoCambio = 0;
const unsigned long debounceDelay = 50; // ms para antirrebote

// Variables para lectura
int valorADC = 0;
float voltaje = 0.0;
float conductividad = 0.0;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Error: No se encontró pantalla OLED"));
    while(true); // Detener ejecución si falla pantalla
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(F("Medidor de Salinidad"));
  display.println(F("Iniciando..."));
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  leerBotonConDebounce();

  if(medirActivo) {
    valorADC = analogRead(sensorPin);

    if(valorADC < 0 || valorADC > 1023) {
      Serial.println(F("Error: lectura ADC fuera de rango"));
      mostrarError("Error lectura ADC");
      return;
    }

    voltaje = valorADC * (5.0 / 1023.0);
    conductividad = convertirAConductividad(valorADC);
    mostrarLecturas(valorADC, voltaje, conductividad);

    Serial.print("ADC: "); Serial.print(valorADC);
    Serial.print("  Voltaje: "); Serial.print(voltaje, 2);
    Serial.print(" V  Conductividad: "); Serial.print(conductividad, 2);
    Serial.println(" mS/cm");
  } else {
    mostrarPausa();
  }

  delay(200);
}

void leerBotonConDebounce() {
  bool estadoBoton = digitalRead(buttonPin);

  if(estadoBoton != ultimoEstadoBoton) {
    ultimoTiempoCambio = millis();
  }

  if((millis() - ultimoTiempoCambio) > debounceDelay) {
    if(estadoBoton == LOW && ultimoEstadoBoton == HIGH) {
      medirActivo = !medirActivo;
    }
  }

  ultimoEstadoBoton = estadoBoton;
}

float convertirAConductividad(int val) {
  return (val / 1023.0) * maxConductivity;
}

void mostrarLecturas(int adc, float volt, float cond) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(F("Medidor de Salinidad"));
  display.println();
  display.print(F("ADC: ")); display.println(adc);
  display.print(F("Voltaje: ")); display.print(volt, 2); display.println(F(" V"));
  display.print(F("Conductividad: ")); display.print(cond, 2); display.println(F(" mS/cm"));
  display.display();
}

void mostrarPausa() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(F("Medicion pausada"));
  display.println(F("Presione boton"));
  display.println(F("para continuar"));
  display.display();
}

void mostrarError(const char* mensaje) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(F("! ERROR !"));
  display.println(mensaje);
  display.display();
}

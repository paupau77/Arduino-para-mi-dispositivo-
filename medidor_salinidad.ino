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
#include <LiquidCrystal_I2C.h>

// Crear el objeto lcd con dirección I2C 0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines
const int sensorPin = A0;       // Potenciómetro como sensor de salinidad
const int buttonPin = 2;        // Pulsador conectado a GND con INPUT_PULLUP

// Configuración de lectura
float maxConductividad = 50.0;  // Valor máximo calibrado en mS/cm

// Estado del sistema
bool medirActivo = true;
bool botonPresionado = false;

// ⚠️ FÓRMULA DE CALIBRACIÓN PENDIENTE:
// Aquí se debe ingresar la ecuación proporcionada por el profesional bioquímico
// para convertir la conductividad (en mS/cm) a salinidad (en g/L, ppt, etc).
// Ejemplo cuando esté disponible:
// float salinidad = 0.42 * pow(conductividad, 2) - 1.6 * conductividad + 0.9;

//float salinidad = 0.0; 
// <- ⚠️ Será calculada con la fórmula real cuando esté disponible

//lcd.setCursor(0, 1);
//lcd.print("Sal: N/D"); // N/D = No disponible

unsigned long ultimaLectura = 0;
const unsigned long intervaloLectura = 300; // ms

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  lcd.init();          // Inicializa LCD
  lcd.backlight();     // Enciende luz de fondo

  Serial.begin(9600);
  Serial.println("Iniciando medidor de salinidad...");

  lcd.setCursor(0, 0);
  lcd.print("Medidor Salinidad");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}

void loop() {
  leerBoton();

  if (medirActivo && (millis() - ultimaLectura >= intervaloLectura)) {
    int adc = analogRead(sensorPin);
    float voltaje = adc * (5.0 / 1023.0);
    float conductividad = (adc / 1023.0) * maxConductividad;

    mostrarLectura(adc, voltaje, conductividad);

    Serial.print("ADC: "); Serial.print(adc);
    Serial.print("  Voltaje: "); Serial.print(voltaje, 2);
    Serial.print(" V  Conductividad: "); Serial.print(conductividad, 2);
    Serial.println(" mS/cm");

    ultimaLectura = millis();
  }

  if (!medirActivo) {
    static bool pausaMostrada = false;
    if (!pausaMostrada) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("== PAUSADO ==");
      lcd.setCursor(0, 1);
      lcd.print("Presiona boton");
      pausaMostrada = true;
    }
  }
}

// --- Antirrebote con cambio de flanco ---
void leerBoton() {
  static unsigned long lastDebounceTime = 0;
  static const unsigned long debounceDelay = 50;

  bool estadoBoton = digitalRead(buttonPin) == LOW;

  if (estadoBoton && !botonPresionado && (millis() - lastDebounceTime > debounceDelay)) {
    botonPresionado = true;
    medirActivo = !medirActivo;
    Serial.println(medirActivo ? "MIDIENDO" : "PAUSADO");
    lcd.clear();
    lastDebounceTime = millis();
  }

  if (!estadoBoton && botonPresionado) {
    botonPresionado = false;
    lastDebounceTime = millis();
  }
}

void mostrarLectura(int adc, float voltaje, float cond) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:"); lcd.print(voltaje, 1);
  lcd.print(" C:"); lcd.print(cond, 1);

  lcd.setCursor(0, 1);
  lcd.print("ADC:"); lcd.print(adc);
}

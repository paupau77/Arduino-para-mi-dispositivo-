/*
  Medidor de Salinidad Básico con Arduino UNO
  -------------------------------------------
  Dispositivo básico para medir la conductividad eléctrica de una muestra líquida 
  (simulada con un potenciómetro), mostrando en pantalla LCD 16x2 I2C el voltaje, 
  valor ADC y una estimación simple de la conductividad. 
  Está preparado para incorporar una fórmula profesional que convierta la 
  conductividad a salinidad real (g/L, ppt, etc) cuando se disponga.

  Autora: Paulina Juich
  Licencia: 
  Este proyecto fue desarrollado por Paulina Juich y registrado en la DNDA (Argentina) 
  bajo el número de expediente EX-2025-78014687- el 18 de Julio de 2025.

  Todo el contenido de este repositorio (código fuente, diseño electrónico, documentación) 
  se encuentra protegido por derechos de autor.

  ⚠️ El incumplimiento de estas condiciones podrá derivar en acciones legales 
  conforme a la Ley 11.723 de Propiedad Intelectual.

  © 2025 Paulina Juich. Todos los derechos reservados.

  🧠 El uso personal, académico o educativo sin fines de lucro está permitido con atribución.
  💰 Cualquier uso comercial, distribución, modificación o integración en productos requiere 
     una licencia paga o autorización expresa.

  Contacto para licencias: paulinajuich4@gmail.com

  Mejoras en este código:
  - Comentarios detallados en cada bloque
  - Antirrebote optimizado con temporización
  - Preparado para probar fórmulas lineal, cuadrática o cúbica
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Crear el objeto lcd con dirección I2C 0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines
const int sensorPin = A0;       // Potenciómetro como sensor de salinidad
const int buttonPin = 2;        // Pulsador conectado a GND con INPUT_PULLUP

// Configuración de lectura
float maxConductividad = 50.0;  // Valor máximo calibrado en mS/cm (ajustable según calibración real)

// Estado del sistema
bool medirActivo = true;
bool botonPresionado = false;

// ⚠️ FÓRMULAS DE CALIBRACIÓN (de ejemplo, se deben reemplazar por datos reales)
// Fórmula LINEAL: salinidad = a1 * conductividad + b1
float a1 = 0.5, b1 = 0.8;

// Fórmula CUADRÁTICA: salinidad = a2 * cond^2 + b2 * cond + c2
float a2 = 0.02, b2 = 0.4, c2 = 0.7;

// Fórmula CÚBICA: salinidad = a3 * cond^3 + b3 * cond^2 + c3 * cond + d3
float a3 = 0.001, b3 = -0.02, c3 = 0.5, d3 = 0.6;

// Selección de fórmula activa: 1 = lineal, 2 = cuadrática, 3 = cúbica
int tipoFormula = 3;  

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

  //  Solo lee cada "intervaloLectura" ms
  if (medirActivo && (millis() - ultimaLectura >= intervaloLectura)) {
    int adc = analogRead(sensorPin);  // Lee el sensor (0–1023)
    float voltaje = adc * (5.0 / 1023.0);  // Conversión a voltaje (0–5V)
    float conductividad = (adc / 1023.0) * maxConductividad; // Escalado a mS/cm

    // --- Cálculo de salinidad según fórmula elegida ---
    float salinidad = 0.0;
    if (tipoFormula == 1) {
      salinidad = a1 * conductividad + b1;
    } else if (tipoFormula == 2) {
      salinidad = a2 * pow(conductividad, 2) + b2 * conductividad + c2;
    } else if (tipoFormula == 3) {
      salinidad = a3 * pow(conductividad, 3) + b3 * pow(conductividad, 2) + c3 * conductividad + d3;
    }

    mostrarLectura(adc, voltaje, conductividad, salinidad);

    // --- Datos también por Serial ---
    Serial.print("ADC: "); Serial.print(adc);
    Serial.print("  Voltaje: "); Serial.print(voltaje, 2);
    Serial.print(" V  Conductividad: "); Serial.print(conductividad, 2);
    Serial.print(" mS/cm  Salinidad: "); Serial.print(salinidad, 2);
    Serial.println(" g/L");
    ultimaLectura = millis();
  }

  //  Muestra pantalla de pausa
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

// --- Mostrar datos en pantalla ---
void mostrarLectura(int adc, float voltaje, float cond, float sal) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:"); lcd.print(voltaje, 1);
  lcd.print(" C:"); lcd.print(cond, 1);

  lcd.setCursor(0, 1);
  lcd.print("S:"); 
  lcd.print(sal, 1);   // valor de salinidad
  lcd.print("g/L ");   // <- aquí agregamos la unidad
  lcd.print("ADC:");
  lcd.print(adc);
}
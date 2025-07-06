# Medidor de Salinidad con Arduino y Pantalla OLED

Dispositivo portátil para medir la conductividad eléctrica de soluciones líquidas, mostrando los valores en tiempo real en una pantalla OLED. Cuenta con un pulsador para pausar o reanudar la lectura.

---

## 📑 Índice

- 🔧 Componentes  
- 🔌 Esquema de conexión  
- ⚙️ Instalación y dependencias  
- 📺 Pantalla OLED SSD1306 (I2C) (o La LCD 16*2)- Información importante  
- 🧠 Explicación del código  
- 🔧 Calibración del sensor  
- ▶️ Modo de uso  
- 🧪 Notas técnicas  
- 📄 Licencia  
- ✍️ Autor  

---

## 🔧 Componentes

| Componente              | Cantidad             |
|------------------------|----------------------|
| Arduino UNO / Nano / compatible | 1                    |
| Sensor de conductividad (analógico) | 1                    |
| Pantalla OLED SSD1306 (I2C) | 1                    |
| Pulsador                | 1                    |
| Protoboard o placa perforada | 1                    |
| Cables dupont           | Varios               |

---

## 🔌 Esquema de conexión

- **Sensor de conductividad:**  
  - Salida analógica → Pin `A0` del Arduino

- **Pantalla OLED SSD1306 (I2C):**  
  - SDA → Pin `A4` (Arduino UNO/Nano)  
  - SCL → Pin `A5` (Arduino UNO/Nano)  
  - VCC → 3.3V o 5V (según especificación del módulo, ver sección OLED)  
  - GND → GND  

- **Pulsador:**  
  - Un extremo a pin digital `2`  
  - Otro extremo a GND  
  - Pin configurado con `INPUT_PULLUP` (no requiere resistencia externa)

- **Alimentación:**  
  - Fuente estable de 5V o conexión USB  

---

## 📷 Esquema de conexión del circuito

![Esquema de conexión](esquema_conexion.png)

*Figura 1: Diagrama de conexión entre Arduino, sensor de conductividad, pantalla OLED SSD1306 y pulsador.*

---

## ⚙️ Instalación y dependencias

1. Abrí el archivo `medidor_salinidad_mejorado.ino` en el IDE de Arduino.  
2. Instalá las siguientes librerías desde el Gestor de Librerías:  
   - `Adafruit SSD1306`  
   - `Adafruit GFX`  
3. Conectá los componentes según el esquema.  
4. Cargá el sketch en tu placa Arduino.  

---

## 📺 Pantalla OLED SSD1306 (I2C) (o La LCD 16*2) - Información importante

### 🔋 Alimentación y niveles lógicos

- Las pantallas OLED SSD1306 pueden funcionar a **3.3 V o 5 V**, dependiendo del modelo. La LCD 16*2 también funciona a 5v 
- **Antes de conectar, verificá el voltaje de operación de tu pantalla.**  
- Si tu pantalla es de **3.3 V y Arduino funciona a 5 V**, recomendamos usar un **convertidor de nivel lógico** en las líneas SDA y SCL para evitar daños o problemas de comunicación.  
- Algunos módulos ya tienen reguladores y adaptadores de nivel integrados, pero no todos, así que ¡cuidado!

### 🔌 Conexión recomendada

| Pantalla OLED          | Arduino UNO/Nano         |
|-----------------------|-------------------------|
| VCC                   | 3.3 V o 5 V (según modelo) |
| GND                   | GND                     |
| SDA                   | A4 (SDA)                |
| SCL                   | A5 (SCL)                |

### 🎯 Dirección I2C

- La dirección I2C estándar suele ser `0x3C`.  
- Si tu pantalla tiene otra dirección, cambiá esta línea en el código:  

```cpp
#define OLED_I2C_ADDRESS 0x3C  // Cambiar a 0x3D si corresponde
```

### 💻 Código para inicializar pantalla con manejo de errores

```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_I2C_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("❌ Error al inicializar pantalla OLED. Verifique conexiones y dirección I2C."));
    while(true); // Detener ejecución para evitar continuar sin pantalla
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Medidor Salinidad"));
  display.display();
  delay(2000);
}
```

### ⚠️ Problemas comunes y soluciones

| Problema                        | Posible causa                        | Solución recomendada                           |
|--------------------------------|------------------------------------|-----------------------------------------------|
| Pantalla no enciende           | Alimentación incorrecta             | Verificar voltaje VCC (3.3V o 5V)             |
| No se muestra nada en pantalla | Dirección I2C incorrecta            | Confirmar dirección, cambiar a 0x3C o 0x3D    |
| Pantalla parpadea o muestra basura | Niveles lógicos incompatibles      | Usar convertidor de nivel lógico para SDA/SCL|
| Error al inicializar pantalla  | Librerías faltantes o mal instaladas | Instalar librerías Adafruit SSD1306 y GFX    |

---

## 🧠 Explicación del código

- **setup()**: Configura pines, inicia comunicación serie, inicializa pantalla OLED y muestra mensaje de bienvenida. Si la pantalla no se inicializa, detiene la ejecución mostrando error.  
- **loop()**:  
  - Lee el estado del botón con antirrebote optimizado para alternar entre medición activa y pausa.  
  - En medición activa: lee valor analógico del sensor, calcula voltaje y conductividad, muestra valores en pantalla y monitor serie.  
  - En pausa: muestra mensaje indicando estado pausado.  
  - Incluye manejo básico de errores para lecturas fuera de rango.  
- **convertirAConductividad(int val)**: Convierte linealmente el valor ADC a conductividad usando una constante de calibración.

---

## 🔧 Calibración del sensor

Para mejorar la precisión:

1. Prepará una solución con conductividad conocida (ejemplo: 10 mS/cm).  
2. Medí el valor ADC que produce esa solución con el sensor conectado.  
3. Ajustá la variable `maxConductivity` en el código con la fórmula:  

   ```cpp
   maxConductivity = (conductividad_conocida * 1023.0) / valorADC_medido;
   ```

4. Guardá y cargá de nuevo el sketch en Arduino.  
5. Probá con diferentes soluciones para verificar la precisión.  

---

## ▶️ Modo de uso

- Al encender el dispositivo, se muestra un mensaje de bienvenida.  
- Presioná el botón para alternar entre:  
  - **Medición activa:** actualiza valores en pantalla.  
  - **Pausa:** mantiene la última lectura y deja de actualizar.  

Los valores mostrados incluyen:  
- Lectura ADC (0–1023)  
- Voltaje (0–5 V)  
- Conductividad estimada (mS/cm)  

---

## 🧪 Notas técnicas

- El botón usa `INPUT_PULLUP` para simplicidad y menor hardware.  
- El antirrebote está implementado con temporización para evitar lecturas falsas.  
- La pantalla OLED debe estar correctamente conectada y configurada con dirección I2C `0x3C`.  
- La fórmula de conversión es lineal, ajustable con la constante `maxConductivity`.  
- Se recomienda usar fuente de alimentación estable para evitar fluctuaciones en lectura ADC.

---

## 📄 Licencia

© 2025 Paulina Juich. Todos los derechos reservados.

- Uso personal, académico o educativo sin fines de lucro permitido con atribución.  
- Uso comercial, distribución, modificación o integración en productos requiere licencia paga o autorización expresa.  

Contacto para licencias: [paulinajuich4@gmail.com](mailto:paulinajuich4@gmail.com)

---

## ✍️ Autor

Paulina Juich  
Julio 2025

---

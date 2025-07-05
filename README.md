# Medidor de Salinidad con Arduino y Pantalla OLED

Medidor de salinidad basado en Arduino que permite visualizar la conductividad eléctrica de soluciones líquidas. El sistema utiliza una pantalla OLED para mostrar valores en tiempo real y un pulsador para pausar o reanudar las lecturas.

---

## 📑 Índice

- [Componentes](#-componentes)
- [Esquema de conexión](#-esquema-de-conexión)
- [Instalación y dependencias](#-instalación-y-dependencias)
- [Explicación del código](#-explicación-del-código)
- [Calibración del sensor](#-calibración-del-sensor)
- [Modo de uso](#-modo-de-uso)
- [Notas técnicas](#-notas-técnicas)
- [Licencia](#-licencia)
- [Autor](#-autor)

---

## 🧰 Componentes

| Componente                     | Cantidad |
|-------------------------------|----------|
| Arduino UNO / Nano / compatible| 1        |
| Sensor de conductividad (analógico) | 1        |
| Pantalla OLED SSD1306 (I2C)   | 1        |
| Pulsador                      | 1        |
| Protoboard o placa perforada  | 1        |
| Cables dupont                 | varios   |

---

## 🔌 Esquema de conexión

- **Sensor de conductividad:**  
  Conectar la salida analógica del sensor al pin **A0** del Arduino.

- **Pantalla OLED SSD1306 (I2C):**  
  - SDA -> A4 (Arduino UNO/Nano)  
  - SCL -> A5 (Arduino UNO/Nano)  
  - VCC -> 3.3V o 5V (según especificación de la pantalla)  
  - GND -> GND

- **Pulsador:**  
  - Un extremo conectado a pin digital **2**  
  - Otro extremo conectado a **GND**  
  - El pin 2 está configurado con `INPUT_PULLUP`, por lo que **no requiere resistencia externa**.

- **Alimentación:**  
  Arduino alimentado con fuente estable de 5V o USB.

![Esquema de conexión](esquema_conexion.png)

---

## ⚙️ Instalación y dependencias

1. Abrí el proyecto `código.ino` en el IDE de Arduino.
2. Instalá las siguientes librerías desde el Gestor de Librerías:  
   - Adafruit SSD1306  
   - Adafruit GFX
3. Cargá el sketch en tu placa Arduino.
4. Conectá los componentes según el esquema.

---

## 🧠 Explicación del código

El firmware está desarrollado en C++ utilizando el entorno de Arduino.

### 1. Función `setup()`

- Configura los pines.
- Inicia la comunicación serie (`Serial.begin`).
- Inicializa la pantalla OLED y muestra un mensaje de bienvenida.
- Si falla la pantalla, detiene la ejecución mostrando error.

### 2. Función `loop()`

- Lee el estado del botón con antirrebote optimizado.
- Alterna entre estados de medición activa y pausa.
- En medición activa:  
  - Lee el valor del sensor (ADC).  
  - Calcula el voltaje.  
  - Convierte el valor a conductividad usando calibración.  
  - Muestra los valores en pantalla y monitor serie.  
- En pausa: muestra mensaje indicando el estado.
- Incluye manejo básico de errores para lecturas fuera de rango.

### 3. Función `convertirAConductividad(int val)`

- Convierte linealmente el valor ADC (0-1023) a conductividad en mS/cm.
- Usa variable `maxConductivity` para calibración.

---

## 🔧 Calibración del sensor

Para mejorar la precisión, es recomendable calibrar el sensor con soluciones patrón:

1. Prepará una solución con conductividad conocida (por ejemplo, 10 mS/cm).
2. Conectá el sensor y medí el valor ADC que produce esa solución.
3. Ajustá la variable `maxConductivity` en el código con la fórmula:

   ```
   maxConductivity = (conductividad_conocida * 1023.0) / valorADC_medido;
   ```

4. Guardá y cargá el sketch en tu Arduino.
5. Probá con diferentes soluciones para verificar precisión.

> Para calibraciones avanzadas, podrías modificar el código para usar interpolación no lineal con múltiples puntos.

---

## ▶️ Modo de uso

- Al encender el dispositivo, se muestra un mensaje de bienvenida.
- Presioná el botón para alternar entre:  
  - **Medición activa:** se actualizan los valores en pantalla.  
  - **Pausa:** se mantiene la última lectura y deja de actualizar.
- Los valores mostrados incluyen:  
  - Lectura ADC (0–1023)  
  - Voltaje (0–5 V)  
  - Conductividad estimada (mS/cm)

---

## 🧪 Notas técnicas

- El botón está conectado a GND y usa `INPUT_PULLUP` para simplicidad y menor hardware.
- El antirrebote está implementado con temporización para evitar lecturas falsas.
- La pantalla OLED debe estar correctamente conectada y configurada con la dirección I2C 0x3C.
- La fórmula de conversión es lineal, ajustable con la constante `maxConductivity`.
- Se recomienda usar una fuente de alimentación estable para evitar fluctuaciones en la lectura ADC.

---

## 📄 Licencia

© 2025 Paulina Juich. Todos los derechos reservados.

Este diseño, documentación y código están protegidos por la legislación de propiedad intelectual. 

🧠 El uso personal, académico o educativo sin fines de lucro está permitido con atribución.
💰 Cualquier uso comercial, distribución, modificación o integración en productos requiere una licencia paga o autorización expresa.

Contacto para licencias: paulinajuich4@gmail.com

---

## ✍️ Autor

Paulina Juich  
Julio 2025

---

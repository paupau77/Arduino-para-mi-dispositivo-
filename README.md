# Medidor de Salinidad con Arduino y Pantalla OLED o LCD 16x2 I2C

Dispositivo portátil para medir la conductividad eléctrica de soluciones líquidas, mostrando los valores en tiempo real en una pantalla OLED o LCD 16x2 I2C. Cuenta con un pulsador para pausar o reanudar la lectura.

---

## 📑 Índice

- 🔧 Componentes  
- 🔌 Esquema de conexión  
- ⚙️ Instalación y dependencias  
- 📺 Pantalla OLED SSD1306 (I2C) o LCD 16x2 I2C - Información importante  
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
| Pantalla OLED SSD1306 (I2C) o LCD 16x2 I2C | 1              |
| Pulsador                | 1                    |
| Protoboard o placa perforada | 1                    |
| Cables dupont           | Varios               |

---

## 🔌 Esquema de conexión

- **Sensor de conductividad:**  
  - Salida analógica → Pin `A0` del Arduino

- **Pantalla OLED SSD1306 (I2C) o LCD 16x2 I2C:**  
  - SDA → Pin `A4` (Arduino UNO/Nano)  
  - SCL → Pin `A5` (Arduino UNO/Nano)  
  - VCC → 5V (ambas son compatibles con 5V en la mayoría de los módulos)  
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

*Figura 1: Diagrama de conexión entre Arduino, sensor de conductividad, pantalla y pulsador.*

---

## ⚙️ Instalación y dependencias

1. Abrí el archivo `medidor_salinidad_mejorado.ino` en el IDE de Arduino.  
2. Instalá las siguientes librerías desde el Gestor de Librerías:  
   - `Adafruit SSD1306`  
   - `Adafruit GFX`  
   - `LiquidCrystal_I2C` (si usás LCD)  
3. Conectá los componentes según el esquema.  
4. Cargá el sketch en tu placa Arduino.  

---

## 📺 Pantalla OLED SSD1306 (I2C) o LCD 16x2 I2C - Información importante

### 🔋 Alimentación y niveles lógicos

- Las pantallas OLED SSD1306 y LCD 16x2 I2C funcionan ambas a **5V** en sus versiones comunes.
- Si usás un OLED que opera a 3.3V, se recomienda un convertidor de nivel lógico para SDA y SCL.
- Algunos módulos ya tienen adaptadores de nivel integrados.

### 🔌 Conexión recomendada

| Pantalla               | Arduino UNO/Nano         |
|------------------------|--------------------------|
| VCC                   | 5 V                      |
| GND                   | GND                      |
| SDA                   | A4 (SDA)                 |
| SCL                   | A5 (SCL)                 |

### 🎯 Dirección I2C

- OLED: comúnmente `0x3C`
- LCD 16x2 I2C: comúnmente `0x27` o `0x3F`

> Cambiá la dirección en el código si no coincide.

---

## 🧠 Explicación del código

- **setup()**: Configura pines, inicia comunicación serie, inicializa la pantalla y muestra mensaje de bienvenida.  
- **loop()**:  
  - Lee el estado del botón (con antirrebote).  
  - En modo medición: lee el sensor, calcula voltaje y conductividad, muestra valores.  
  - En modo pausa: muestra mensaje fijo.  
- **convertirAConductividad()**: realiza la conversión lineal ADC → mS/cm.

---

## 🔧 Calibración del sensor

1. Preparar solución conocida (ejemplo: 10 mS/cm).  
2. Medir valor ADC.  
3. Ajustar `maxConductivity` con:

```cpp
maxConductivity = (conductividad_conocida * 1023.0) / valorADC_medido;
```

---

## ▶️ Modo de uso

- Al encender, muestra mensaje de bienvenida.  
- Pulsá el botón para alternar entre:  
  - **Medición activa:** valores actualizados  
  - **Pausa:** valores congelados  

---

## 🧪 Notas técnicas

- Botón con `INPUT_PULLUP` no necesita resistencia externa.  
- Antirrebote por software incluido.  
- Compatible con OLED o LCD I2C.  
- Fuente de alimentación debe ser estable.

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

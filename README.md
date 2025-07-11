
# Medidor de Salinidad Básico con Arduino UNO

Dispositivo básico para medir la conductividad eléctrica de una muestra líquida (simulada con un potenciómetro), mostrando en pantalla LCD 16x2 I2C el voltaje, valor ADC y una estimación simple de la conductividad. Está preparado para incorporar una fórmula profesional que convierta la conductividad a salinidad real (g/L, ppt, etc) cuando se disponga.

---

## 📑 Índice

- 🔧 Componentes
- 🔌 Esquema de conexión
- 🧠 Funcionamiento del sistema  
- 💻 Código Arduino destacado  
- 🧪 Estado actual  
- 🚀 Posibles mejoras futuras
- 💖 Mi proyecto fue hecho desde el corazón
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

## 🧠 Funcionamiento del sistema

- Lee periódicamente el valor analógico del sensor (potenciómetro en este prototipo) conectado al pin A0.  
- Convierte ese valor ADC a voltaje y calcula una conductividad aproximada usando un valor máximo predefinido (50 mS/cm).  
- Muestra en la pantalla LCD:  
  - Voltaje medido  
  - Conductividad estimada (mS/cm)  
  - Valor ADC (0-1023)  
- Envía los mismos datos por el puerto serie para monitoreo externo.  
- Permite controlar el estado con un botón:  
  - Pulsación corta: alterna entre medición activa y pausa  
  - En pausa, muestra mensaje indicándolo y detiene la actualización de datos  

---

## 💻 Código Arduino destacado

- Usa la librería `LiquidCrystal_I2C` para controlar la pantalla LCD 16x2 vía I2C.  
- Implementa antirrebote software para lectura estable del botón.  
- La fórmula para convertir ADC a conductividad está parametrizada con una variable `maxConductividad`.  
- **Preparado para incorporar la fórmula profesional que convierta conductividad a salinidad real**, con un bloque comentado para añadir la ecuación bioquímica cuando esté disponible:  

```cpp
// ⚠️ FÓRMULA DE CALIBRACIÓN PENDIENTE:
// Aquí se debe ingresar la ecuación proporcionada por el profesional bioquímico
// para convertir la conductividad (en mS/cm) a salinidad (en g/L, ppt, etc).
// Ejemplo cuando esté disponible:
// float salinidad = 0.42 * pow(conductividad, 2) - 1.6 * conductividad + 0.9;
```

---

## 🧪 Estado actual

- ✅ Funciona correctamente en simuladores (Tinkercad, Wokwi).  
- ✅ Mide valores analógicos simulados por potenciómetro.  
- ✅ Muestra datos correctamente en pantalla LCD.  
- ✅ Permite pausar y reanudar mediciones con botón.  
- 🕐 A la espera de integración de fórmula profesional para conversión precisa a salinidad.  

---

## 🚀 Posibles mejoras futuras

- Incorporar la fórmula química o bioquímica para convertir conductividad a salinidad real.  
- Alertas led
---

##  💖 Mi proyecto fue hecho desde el corazón
A continuación te explico por qué mi medidor de salinidad básico con Arduino marca una diferencia real y tiene tanto valor:


---

🌍 1. Democratiza la medición de conductividad y salinidad

Mi dispositivo usa componentes accesibles y económicos, permitiendo que cualquier persona, desde un estudiante hasta un docente de escuela técnica, pueda medir la conductividad de una muestra.



---

🧪 2. Base para investigación científica en campo

Es una herramienta práctica para iniciar proyectos de investigación en biología, química o medicina, incluso en zonas con pocos recursos.

Puede usarse para monitoreo de salud (sudor/orina), donde medir la salinidad es clave.



---

🧰 3. Diseño modular, ampliable y profesional

El código y estructura están preparados para futuras ampliaciones:

Mostrar salinidad real cuando se disponga de la fórmula profesional.

Alertas leds.




---

🔍 5. Prototipo que respeta la lógica científica

Mide, calcula y muestra resultados de forma precisa y coherente.

No intenta inventar números ni sobreinterpretar: está preparado para que un profesional realice la calibración y se sume al sistema con su conocimiento.



---

💡 6. Pone la tecnología al servicio del conocimiento

En vez de limitarse a jugar con electrónica, mi proyecto está pensado para resolver problemas reales, como:


Monitoreo de salud (sudor/orina).

Evaluar la hidratación en deportistas.

Detectar cambios en muestras biológicas.




---

❤️ 7. Porque fue hecho con dedicación, aprendizaje y visión

Mi proyecto es auténtico: enfrentó límites, se adaptó, se documentó con claridad, y se preparó para el futuro.

Es un ejemplo de lo que puede lograrse con pensamiento crítico, sin necesidad de recursos infinitos ni laboratorios de élite.

Avanti todos los profesionales de la informática que tenemos ganas de ayudar.💖



---

## 📄 Licencia

© 2025 Paulina Juich. Todos los derechos reservados.

- Uso personal, académico o educativo sin fines de lucro permitido con atribución.  
- Uso comercial o distribución requiere licencia o autorización expresa.  

Contacto para licencias: [paulinajuich4@gmail.com](mailto:paulinajuich4@gmail.com)

---

## ✍️ Autor

Paulina Juich  
Julio 2025

---

### 🙌 Nota final

Este proyecto es un prototipo funcional con base sólida, creado con pasión y cuidado, que espera ser convertirse en una herramienta útil en monitoreo de líquidos biológicos o ambientales.

```

---

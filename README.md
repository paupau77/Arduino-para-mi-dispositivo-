# Medidor de Salinidad con Arduino y Pantalla OLED

Medidor de salinidad basado en Arduino que permite visualizar la conductividad eléctrica de soluciones líquidas. El sistema utiliza una pantalla OLED para mostrar valores en tiempo real y un pulsador para pausar o reanudar las lecturas.

---

## 📑 Índice

- [Componentes](#componentes)
- [Esquema de conexión](#esquema-de-conexión)
- [Instalación y dependencias](#instalación-y-dependencias)
- [Explicación del código](#explicación-del-código)
  - [setup()](#1-función-setup)
  - [loop()](#2-función-loop)
  - [convertirAConductividad()](#3-función-convertiraconductividad)
- [Modo de uso](#modo-de-uso)
- [Notas técnicas](#notas-técnicas)
- [Licencia](#licencia)
- [Autor](#autor)

---

## 🧰 Componentes

| Componente                          | Cantidad |
|-------------------------------------|----------|
| Arduino UNO / Nano / compatible     | 1        |
| Sensor de conductividad (analógico) | 1        |
| Pantalla OLED SSD1306 (I2C)         | 1        |
| Pulsador                            | 1        |
| Protoboard o placa perforada        | 1        |
| Cables dupont                       | varios   |

---

## 🔌 Esquema de conexión

A continuación se presenta el circuito electrónico del dispositivo:

![Esquema de conexión](esquema_conexion.png)

---

## ⚙️ Instalación y dependencias

1. Abrí el proyecto `medidor_salinidad.ino` en el IDE de Arduino.
2. Instalá las siguientes librerías desde el Gestor de Librerías:
   - `Adafruit SSD1306`
   - `Adafruit GFX`
3. Cargá el sketch en tu placa Arduino.
4. Conectá los componentes según el esquema anterior.

---

## 🧠 Explicación del código

El firmware está desarrollado en C++ utilizando el entorno de Arduino.

### 1. Función `setup()`

- Configura los pines.
- Inicia la comunicación serie (`Serial.begin`).
- Inicializa la pantalla OLED.
- Muestra un mensaje inicial durante 2 segundos.

### 2. Función `loop()`

Contiene la lógica principal del sistema:

- Lee el estado del botón.
- Detecta un cambio de estado (flanco descendente) con antirrebote.
- Alterna entre los estados de pausa y medición.
- Si no está en pausa:
  - Lee el valor del sensor (ADC).
  - Calcula el voltaje.
  - Convierte el valor a una estimación de conductividad.
  - Muestra los valores en pantalla y monitor serie.
- Si está en pausa, muestra el mensaje correspondiente.

### 3. Función `convertirAConductividad()`

```cpp
float convertirAConductividad(int val) {
  return (val / 1023.0) * maxConductivity;
}
```

- Realiza una conversión lineal desde el valor del ADC a un valor estimado de conductividad en miliSiemens por centímetro (mS/cm).
- El parámetro `maxConductivity` puede ajustarse para calibrar el sensor.
  ⚠️Nota: El código utiliza INPUT_PULLUP, por lo que no requiere resistencia externa en el botón.
---

## ▶️ Modo de uso

1. Al encender el dispositivo, se muestra un mensaje de bienvenida.
2. Presioná el botón para alternar entre:
   - Medición activa: se actualizan los valores.
   - Pausa: se mantiene la última lectura.
3. Los valores mostrados incluyen:
   - Lectura ADC (0–1023)
   - Voltaje (0–5 V)
   - Conductividad estimada (mS/cm)

---

## 🧪 Notas técnicas

- El valor por defecto de `maxConductivity` es 50.0 mS/cm.
- La fórmula de conversión es lineal y puede personalizarse según el sensor.
- Para mayor precisión, calibrar con soluciones patrón.
- El botón está conectado a GND y usa `INPUT_PULLUP` para simplicidad.

---

## 📄 Licencia

Este proyecto está licenciado bajo la **GNU General Public License v3.0 (GPL-3.0)**.  
Podés usarlo, modificarlo y redistribuirlo bajo los términos de esta licencia.

---

## ✍️ Autor

**Paulina Juich**  
Julio 2025

---

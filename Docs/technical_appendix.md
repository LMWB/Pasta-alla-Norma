# ⚙️ AuroraLamp — Technical Appendix

This section provides essential hardware and configuration information for users, installers, and maintainers.

---

## 🧭 Operating Modes

| Jumper Position | Mode Name          | Description |
|-----------------|--------------------|--------------|
| **Left (1)**    | **Rainbow Fade**   | Smooth continuous transition through all colors of the visible spectrum. |
| **Center (2)**  | **Sunstorm**       | Dynamic effect simulating solar flares — deep orange to near-white flashes. |
| **Right (3)**   | **Christmas Glow** | Gentle fade between festive colors: greens, reds, and golden tones. |

> The jumper selects the active color cycle at power-up.  
> The lamp automatically loops through its pattern until powered off.

---

## 🔌 Power Supply

| Parameter | Specification |
|------------|----------------|
| **Input Voltage** | 5 V DC via USB-C |
| **Typical Current** | 50 – 200 mA (depending on brightness and color) |
| **Connector** | USB-C, reversible plug |
| **Polarity Protection** | Built-in |

> Use only standard 5 V USB power sources (e.g., phone charger, PC, or power bank).

---

## 💡 LED System

| Component | Description |
|------------|-------------|
| **LED Type** | WS2812B (addressable RGB) |
| **Control Logic Voltage** | 5 V (TTL compatible) |
| **Color Depth** | 24-bit (8 bits per channel) |
| **Gamma Correction** | Applied in firmware for natural light transitions |
| **PWM Frequency** | ~800 kHz internal (per WS2812B spec) |

---

## 🧠 Firmware

- Fully self-contained — no external controller required.  
- Gamma-corrected brightness control ensures visual consistency across all color modes.  
- Smooth blending between predefined palettes (rainbow, sunstorm, and festive).

> Firmware auto-starts and runs continuously after power-up.

---

## ⚠️ Notes & Safety

- Operate in dry indoor environments only.  
- Do not short the USB-C pins or modify the LED wiring while powered.  
- Avoid direct viewing of full-brightness white light from close distance.  
- Disconnect power before opening or modifying the enclosure.

---

### Version
Firmware: **v1.0.0**  
Hardware Revision: **A1**  
Documentation: **October 2025**

---

### © 2025 calogizer electronic engineering  
Crafted with care — engineered for light.

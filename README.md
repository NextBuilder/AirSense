<div align="center">

<br/>

<img src="Code/AS.png" width="900"/>

<br/><br/>

# AirSense

**The Smartest DIY Air Purifier — Engineered from Scratch**

<br/>

<!-- ════════ REPO STATS ════════ -->
![Stars](https://img.shields.io/github/stars/NextBuilder/AirSense?style=flat-square&logo=github&logoColor=white&label=Stars&color=0d1117&labelColor=0d1117)
![Forks](https://img.shields.io/github/forks/NextBuilder/AirSense?style=flat-square&logo=github&logoColor=white&label=Forks&color=0d1117&labelColor=0d1117)
![Issues](https://img.shields.io/github/issues/NextBuilder/AirSense?style=flat-square&logo=github&logoColor=white&label=Issues&color=0d1117&labelColor=0d1117)
![License](https://img.shields.io/badge/Non--Commercial-0d1117?style=flat-square&logo=opensourceinitiative&logoColor=white&labelColor=0d1117)

<br/>

<!-- ════════ TECH STACK ════════ -->
![ESP32-S3](https://img.shields.io/badge/ESP32--S3-1a0a09?style=for-the-badge&logo=espressif&logoColor=f3a99a&labelColor=1a0a09&color=1a0a09)
![Arduino IoT Cloud](https://img.shields.io/badge/Arduino_IoT_Cloud-091a12?style=for-the-badge&logo=arduino&logoColor=86e0b8&labelColor=091a12&color=091a12)
![LVGL](https://img.shields.io/badge/LVGL_UI-0e0a1a?style=for-the-badge&logo=c&logoColor=b9aaff&labelColor=0e0a1a&color=0e0a1a)
![HEPA](https://img.shields.io/badge/HEPA_Filtration-0a1a14?style=for-the-badge&logoColor=7dddb5&labelColor=0a1a14&color=0a1a14)
![Fusion360](https://img.shields.io/badge/Fusion_360-1a110a?style=for-the-badge&logo=autodesk&logoColor=f5c97a&labelColor=1a110a&color=1a110a)

<br/>

<!-- ════════ LIBRARIES ════════ -->
[![TFT_eSPI](https://img.shields.io/badge/TFT__eSPI-0d1117?style=flat-square&logo=github&logoColor=7bcfee&labelColor=0d1117&color=0d1117)](https://github.com/Bodmer/TFT_eSPI)
[![LVGL](https://img.shields.io/badge/LVGL-0d1117?style=flat-square&logo=github&logoColor=b09bff&labelColor=0d1117&color=0d1117)](https://github.com/lvgl/lvgl)
[![Adafruit BME680](https://img.shields.io/badge/Adafruit_BME680-0d1117?style=flat-square&logo=github&logoColor=f5a0c5&labelColor=0d1117&color=0d1117)](https://github.com/adafruit/Adafruit_BME680)
[![ArduinoIoTCloud](https://img.shields.io/badge/ArduinoIoTCloud-0d1117?style=flat-square&logo=github&logoColor=7dddb5&labelColor=0d1117&color=0d1117)](https://github.com/arduino-libraries/ArduinoIoTCloud)
[![Arduino_ConnectionHandler](https://img.shields.io/badge/Arduino__ConnectionHandler-0d1117?style=flat-square&logo=github&logoColor=7dddb5&labelColor=0d1117&color=0d1117)](https://github.com/arduino-libraries/Arduino_ConnectionHandler)

<br/>

<!-- ════════ SOCIALS ════════ -->
[![YouTube](https://img.shields.io/badge/YouTube-1a0505?style=for-the-badge&logo=youtube&logoColor=ff7b7b&labelColor=1a0505&color=1a0505)](https://youtube.com/@next.builder)
[![Instagram](https://img.shields.io/badge/Instagram-1a0a12?style=for-the-badge&logo=instagram&logoColor=f5a0c5&labelColor=1a0a12&color=1a0a12)](https://instagram.com/next_builder)
[![Twitter](https://img.shields.io/badge/Twitter-0d0d0d?style=for-the-badge&logo=x&logoColor=d0d0d0&labelColor=0d0d0d&color=0d0d0d)](https://x.com/NEXTBUILDERDIY)
[![Instructables](https://img.shields.io/badge/Instructables-1a1200?style=for-the-badge&logo=instructables&logoColor=ffc84a&labelColor=1a1200&color=1a1200)](https://www.instructables.com/member/Next%20Builder%20DIY/)
[![Hackster](https://img.shields.io/badge/Hackster.io-051218?style=for-the-badge&logo=hackster&logoColor=6fcfee&labelColor=051218&color=051218)](https://hackster.io/NEXTBUILDER)
[![Hackaday](https://img.shields.io/badge/Hackaday-111111?style=for-the-badge&logo=hackaday&logoColor=cccccc&labelColor=111111&color=111111)](https://hackaday.io/NextBuilder)

<br/>

</div>

---

<br/>

## Overview

Indoor air pollution is invisible — yet it shapes your sleep, focus, and long-term health. **AirSense** is a fully open-source smart air purifier that continuously monitors your indoor environment and automatically purifies it in real time.

Built around the **Waveshare ESP32-S3**, it measures PM2.5 dust concentration, temperature, humidity, atmospheric pressure, and indoor air quality using dedicated environmental sensors. All readings render live on a smooth **LVGL TFT dashboard** set into a custom 3D-printed cylindrical enclosure, and sync to **Arduino IoT Cloud** for remote monitoring and one-tap fan control from your phone.

Every part — firmware, schematic, CAD files, and cloud setup — is documented and fully reproducible.

<br/>

---

<br/>

## Live Display

<div align="center">
<img src="Images/62.png" width="740"/>
<br/><br/>
<sub>LVGL dashboard running live — PM2.5 · AQI · Temperature · Humidity · Feed Rate · Filter Life · WiFi status</sub>
</div>

<br/>

---

<br/>

## Demo

<div align="center">
<a href="https://www.youtube.com/watch?v=aSXS77vslJU">
<img src="https://img.youtube.com/vi/aSXS77vslJU/maxresdefault.jpg" width="800"/>
</a>
<br/><br/>
<sub>▶ Click to watch the full build and demo on YouTube</sub>
</div>

<br/>

---

<br/>

## Features

| | Feature | Detail |
|:---:|:---|:---|
| 🌫️ | **PM2.5 Dust Monitoring** | Real-time particulate concentration — Waveshare dust sensor |
| 📊 | **Air Quality Index** | AQI calculated live with colour-coded Good / Moderate / Unhealthy states |
| 🌡️ | **Temperature** | Live °C readout via BME680 |
| 💧 | **Humidity** | Relative humidity displayed and synced to cloud |
| 🌍 | **Atmospheric Pressure** | Barometric pressure monitoring |
| 🫁 | **Indoor Air Quality** | VOC and gas resistance via BME680 |
| ☁️ | **Arduino IoT Cloud** | Full two-way sync — monitor and control from anywhere |
| 📱 | **Mobile Control** | Fan toggle via Arduino IoT Remote app |
| ⚡ | **Auto Fan Control** | Relay activates fan automatically when AQI degrades |
| 🎨 | **LVGL Dashboard** | Smooth embedded UI on 2.4″ ILI9341 TFT (240 × 320) |
| 🌿 | **HEPA Filtration** | Xiaomi Air Purifier 4 Lite filter — captures particles to 0.3 µm |
| 🖨️ | **Custom Enclosure** | Cylindrical body designed in Autodesk Fusion 360 |
| 🔧 | **Modular** | Easy to assemble, maintain, and upgrade |
| 🚀 | **Open Source** | Full hardware and software — fork it, build it, improve it |

<br/>

---

<br/>

## Enclosure Design

<div align="center">
<img src="Images/CAD.png" width="640"/>
<br/><br/>
<sub>Fusion 360 CAD — cylindrical body · hexagonal intake grille · side-mounted TFT cutout · Next Builder branding</sub>
</div>

<br/>

The enclosure wraps around the Xiaomi 4 Lite HEPA filter cylinder. The 150mm fan mounts on top, pulling air upward through the filter. The TFT display sits flush in a precision side cutout. A stainless steel 18L open-top dustbin forms the structural body — rigid, heat-resistant, and easy to source.

<br/>

---
<br/>

## Bill of Materials

<div align="center">

<img src="Images/22.png" width="860"/>
<br/>
<sub>All electronic components — Dust Sensor · 2.4″ LCD · BME680 · Relay · ESP32-S3 · 5V PSU</sub>

<br/><br/>

<table>
<tr>
<td align="center" width="33%">
<img src="Images/24.png" width="260"/><br/><br/>
<b>Xiaomi Air Purifier 4 Lite</b><br/>
<sub>True HEPA Filter — 0.3 µm</sub>
</td>
<td align="center" width="33%">
<img src="Images/23.png" width="260"/><br/><br/>
<b>18L Open-Top Steel Dustbin</b><br/>
<sub>Structural enclosure body</sub>
</td>
<td align="center" width="33%">
<img src="Images/25.png" width="260"/><br/><br/>
<b>Anchor 150mm Exhaust Fan</b><br/>
<sub>220V Smart Air — circulation</sub>
</td>
</tr>
</table>

</div>

<br/>

> Full BOM with purchase links is in the [Instructables tutorial →](https://www.instructables.com/AirSense-the-Smartest-DIY-Air-Purifier/)

<br/>

---

<br/>

## Internal Assembly

<div align="center">
<img src="Images/54.png" width="820"/>
<br/><br/>
<sub>Inside view after complete wiring and assembly — ESP32-S3 · Display · BME680 · Dust Sensor · Relay · PSU · 220V Fan</sub>
</div>

<br/>

**Wiring notes:**

- The relay switches the **220V fan line** — never touch the high-voltage side while powered
- The dust sensor uses a timed LED pulse pin — follow the sampling interval in the firmware exactly
- BME680 is on I²C; SDA and SCL are shared without conflicts on ESP32-S3
- Display is on SPI — CS, DC, RST must match the `#define` values in `User_Setup.h` (TFT_eSPI)

<br/>

---

<br/>

## How It Works

1. Sensors sample every 2 seconds — PM2.5, temperature, humidity, pressure, VOC
2. ESP32-S3 computes AQI and classifies air quality level
3. LVGL renders all values live with colour-coded AQI ring
4. All data syncs over Wi-Fi to Arduino IoT Cloud
5. If AQI crosses the threshold, relay fires and fan starts automatically
6. Fan can also be toggled manually from the Arduino IoT Remote app

<br/>

---

<br/>

## Getting Started

### 1 — Set Up Arduino IDE

1. Install [Arduino IDE 2](https://www.arduino.cc/en/software)
2. **File → Preferences** → paste into *Additional Boards Manager URLs*:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. **Tools → Board → Boards Manager** → search `esp32` → install **Espressif Systems ESP32**

### 2 — Install Libraries

Go to **Tools → Manage Libraries** and install each one:

| Library | GitHub |
|:---|:---|
| TFT_eSPI | [Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) |
| LVGL | [lvgl/lvgl](https://github.com/lvgl/lvgl) |
| Adafruit BME680 | [adafruit/Adafruit_BME680](https://github.com/adafruit/Adafruit_BME680) |
| ArduinoIoTCloud | [arduino-libraries/ArduinoIoTCloud](https://github.com/arduino-libraries/ArduinoIoTCloud) |
| Arduino_ConnectionHandler | [arduino-libraries/Arduino_ConnectionHandler](https://github.com/arduino-libraries/Arduino_ConnectionHandler) |

> After installing TFT_eSPI, edit `User_Setup.h` inside the library folder to match your display pins and select the ILI9341 driver.

### 3 — Configure the Firmware

Open `Code/AirSense.ino` and fill in your credentials:

```cpp
// Wi-Fi
const char SSID[]        = "YOUR_WIFI_SSID";
const char PASS[]        = "YOUR_WIFI_PASSWORD";

// Arduino IoT Cloud
const char DEVICE_ID[]   = "YOUR_DEVICE_ID";
const char SECRET_KEY[]  = "YOUR_SECRET_KEY";
```

Select board: **Tools → Board → ESP32S3 Dev Module** · choose your COM port · click **Upload**

### 4 — Arduino IoT Cloud Setup

1. Sign in at [create.arduino.cc/iot](https://create.arduino.cc/iot)
2. Create a new **Thing** and add these variables:

| Variable | Type | Direction |
|:---|:---|:---|
| `pm25` | `float` | Read |
| `temperature` | `float` | Read |
| `humidity` | `float` | Read |
| `pressure` | `float` | Read |
| `aqi` | `int` | Read |
| `fanControl` | `bool` | Read / Write |

3. Copy **Device ID** + **Secret Key** into the firmware
4. Install **Arduino IoT Remote** on your phone and connect to your Thing

### 5 — Assemble

Follow the wiring diagram and the full step-by-step assembly guide:

**→ [instructables.com/AirSense-the-Smartest-DIY-Air-Purifier](https://www.instructables.com/AirSense-the-Smartest-DIY-Air-Purifier/)**

<br/>

---

<br/>

## License

Open for **personal and educational use only.**
Commercial use, resale, or redistribution in any product requires explicit written permission from the author.

<br/>

---

<br/>

<div align="center">

Built with ❤️ by **[Next Builder](https://youtube.com/@next.builder)** ·

*Built one? Share it. Open an issue, tag us, drop a photo — the community makes this worth building.*

<br/>

[![YouTube](https://img.shields.io/badge/YouTube-1a0505?style=for-the-badge&logo=youtube&logoColor=ff7b7b&labelColor=1a0505&color=1a0505)](https://youtube.com/@next.builder)
[![Instagram](https://img.shields.io/badge/Instagram-1a0a12?style=for-the-badge&logo=instagram&logoColor=f5a0c5&labelColor=1a0a12&color=1a0a12)](https://instagram.com/next_builder)
[![Twitter](https://img.shields.io/badge/Twitter-0d0d0d?style=for-the-badge&logo=x&logoColor=d0d0d0&labelColor=0d0d0d&color=0d0d0d)](https://x.com/NEXTBUILDERDIY)
[![Instructables](https://img.shields.io/badge/Instructables-1a1200?style=for-the-badge&logo=instructables&logoColor=ffc84a&labelColor=1a1200&color=1a1200)](https://www.instructables.com/member/Next%20Builder%20DIY/)
[![Hackster](https://img.shields.io/badge/Hackster.io-051218?style=for-the-badge&logo=hackster&logoColor=6fcfee&labelColor=051218&color=051218)](https://hackster.io/NEXTBUILDER)
[![Hackaday](https://img.shields.io/badge/Hackaday-111111?style=for-the-badge&logo=hackaday&logoColor=cccccc&labelColor=111111&color=111111)](https://hackaday.io/NextBuilder)

<br/>

**⭐ Star this repo if it helped you build something awesome ⭐**

</div>

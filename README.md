<div align="center">

<br/>

<img src="Code/AS.png" width="900"/>

<br/><br/>

# Air Sense

**The Smartest DIY Air Purifier — Engineered from Scratch**

<br/>

<!-- ════════ REPO STATS ════════ -->
![Stars](https://img.shields.io/github/stars/NextBuilder/AirSense?style=flat-square&logo=github&logoColor=white&label=Stars&color=0d1117&labelColor=0d1117)
![Forks](https://img.shields.io/github/forks/NextBuilder/AirSense?style=flat-square&logo=github&logoColor=white&label=Forks&color=0d1117&labelColor=0d1117)
![Issues](https://img.shields.io/github/issues/NextBuilder/AirSense?style=flat-square&logo=github&logoColor=white&label=Issues&color=0d1117&labelColor=0d1117)
![License](https://img.shields.io/badge/Non--Commercial-0d1117?style=flat-square&logo=opensourceinitiative&logoColor=white&labelColor=0d1117)

</div>

---
<br/>

## 🌿 Overview

Indoor air pollution is often invisible, yet it can significantly impact our health and comfort. **AirSense** is an open-source smart air purifier that combines real-time environmental monitoring, intelligent purification, and cloud connectivity into a single device.

Powered by the **Waveshare ESP32-S3**, AirSense continuously measures **PM2.5 dust levels, temperature, humidity, atmospheric pressure, and indoor air quality** using dedicated sensors. Live data is displayed on a modern **LVGL-powered TFT dashboard** and synchronized with **Arduino IoT Cloud**, allowing remote monitoring and fan control from anywhere.

Designed in **Autodesk Fusion**, the system features a custom 3D-printed enclosure, HEPA filtration, automatic fan control, and a product-like user experience. This repository includes everything needed to build your own AirSense, including source code, CAD files, circuit diagrams, and setup instructions.
<br/>

---
<br/>

## Live Display

<div align="center">
<img src="Images/62.png" width="740"/>
<br/><br/><sub>AirSense Live Dashboard — PM2.5 Dust · AQI · Temperature · Humidity · Air Quality · Feed Rate · Filter Health · Wi-Fi Status</sub>
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

## ✨ Features

**AirSense** combines intelligent air purification, environmental monitoring, and cloud connectivity into a single product-grade device. It continuously monitors **PM2.5 dust levels, AQI, temperature, humidity, atmospheric pressure, and indoor air quality** in real time, presenting all information through a beautiful **LVGL-powered dashboard**. The system uses a **Xiaomi Air Purifier 4 Lite HEPA filter**, capable of capturing up to **99.97% of airborne particles as small as 0.3 μm**, helping create a cleaner and healthier indoor environment.

Built around the **ESP32-S3**, AirSense synchronizes live data with **Arduino IoT Cloud**, enabling remote monitoring and control from anywhere through a smartphone, tablet, or web browser. The custom enclosure, designed in **Autodesk Fusion 360**, delivers a clean product-like appearance while maintaining easy assembly and maintenance. With complete source code, CAD files, circuit diagrams, and documentation included, AirSense is a fully open-source platform designed for learning, customization, and real-world everyday use.
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

The enclosure is designed around the **Xiaomi Air Purifier 4 Lite HEPA filter**, creating a compact and efficient airflow path. A **150mm high-airflow fan** is mounted at the top, drawing contaminated air through the filter and exhausting clean air upward. The **2.4-inch TFT display** is seamlessly integrated into a precision-cut side panel for a clean, product-like appearance. An **18L stainless steel dustbin** serves as the main structural body, providing excellent durability, a premium finish, and easy availability for replication.

<br/>

---
<br/>

## Bill of Materials

<div align="center">

<img src="Images/22.png" width="860"/>
<br/>
<sub>All electronic components</sub>

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
<sub>Inside view after complete wiring and assembly</sub>
</div>

<br/>

**Wiring Notes**

* The relay switches the **220V AC fan**, so always disconnect mains power before making any wiring changes or performing maintenance.
* The dust sensor relies on a precise LED pulse timing sequence for accurate measurements. The sampling logic used in the firmware should remain unchanged.
* The BME680 communicates via the **I²C interface**, using dedicated SDA and SCL connections on the ESP32-S3.
* The TFT display operates over **SPI**. Ensure all display pins, including CS, DC, and RST, match the assignments defined in the provided `User_Setup.h` file.
* Before powering the system, verify all power, signal, and ground connections to ensure safe and reliable operation.

<br/>

---

<br/>

## How It Works

AirSense continuously monitors the surrounding environment using dedicated air quality and environmental sensors. Every few seconds, the system measures **PM2.5 dust concentration, temperature, humidity, atmospheric pressure, and indoor air quality**, providing a real-time view of indoor environmental conditions.

The collected data is processed by the **ESP32-S3**, which calculates the Air Quality Index (AQI) and updates all measurements on the built-in **LVGL-powered dashboard**. Users can instantly view key air quality metrics through a clean and intuitive graphical interface. At the same time, sensor readings are synchronized with **Arduino IoT Cloud** over Wi-Fi, allowing remote monitoring from anywhere using a smartphone, tablet, or web browser.

The purification fan operates continuously to draw air through the **HEPA filtration system**, where dust and airborne particles are captured before clean air is released back into the environment. Users can manually turn the purifier on or off at any time using the **Arduino IoT Remote App**, providing convenient control from anywhere. By combining real-time monitoring, cloud connectivity, and continuous air purification, AirSense helps users better understand and improve the quality of the air they breathe every day.

<br/>

---

<br/>

## 📚 Build Guide & Documentation

Want to build your own **AirSense**? Complete step-by-step instructions, cloud configuration, wiring details, assembly photos, and testing procedures are available in the full written tutorials below.

Whether you're a beginner exploring IoT and air quality monitoring or an experienced maker looking to customize the design, these guides will walk you through the entire build process from start to finish.

### Tutorials & Project Documentation

* **Instructables**
  https://www.instructables.com/AirSense-the-Smartest-DIY-Air-Purifier/

* **Hackster.io**
  https://www.hackster.io/NEXTBUILDER/airsense-the-smartest-diy-air-purifier-f775f8

* **Hackaday.io**
  https://hackaday.io/project/205928-airsense-the-smartest-diy-air-purifier

* **ElectronicWings**
  https://www.electronicwings.com/users/NextBuilder/projects/6592/airsense---the-smartest-diy-air-purifier

All project resources, including firmware, CAD models, library configuration files, and circuit diagrams, are available in this GitHub repository.

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

[![youtube](https://www.readmecodegen.com/api/social-icon?name=youtube&size=32&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&link=https%3A%2F%2Fyoutube.com%2F%40Next.Builder)](https://youtube.com/@Next.Builder)
[![instagram](https://www.readmecodegen.com/api/social-icon?name=instagram&size=32&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&link=https%3A%2F%2Finstagram.com%2Fnext_builder)](https://instagram.com/next_builder)
[![x](https://www.readmecodegen.com/api/social-icon?name=x&size=34&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&link=https%3A%2F%2Fx.com%2FNEXTBUILDERDIY)](https://x.com/NEXTBUILDERDIY)
[![linkedin](https://www.readmecodegen.com/api/social-icon?name=linkedin&size=32&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&textColor=%23ffffff&link=linkedin.com%2Fcompany%2Fnextbuilderdiy)](linkedin.com/company/nextbuilderdiy)
[![instructables](https://www.readmecodegen.com/api/social-icon?name=instructables&size=30&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&link=https%3A%2F%2Fwww.instructables.com%2Fmember%2FNext%2520Builder%2520DIY%2F)](https://www.instructables.com/member/Next%20Builder%20DIY/)
[![hackster](https://www.readmecodegen.com/api/social-icon?name=hackster&size=30&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&link=https%3A%2F%2Fhackster.io%2FNEXTBUILDER)](https://hackster.io/NEXTBUILDER)
[![hackaday](https://www.readmecodegen.com/api/social-icon?name=hackaday&size=32&shape=circle&reverseBackground=true&textAlignment=horizontal&showText=true&link=https%3A%2F%2Fhackaday.io%2FNextBuilder)](https://hackaday.io/NextBuilder)

<br/>

*⭐ Star this repo if it helped you build something awesome ⭐*

</div>

// * lv_conf.h — LVGL v9 configuration
// * ───────────────────────────────────
// * COPY THIS FILE TO:
// *   The folder NEXT TO (not inside) your lvgl library folder.
// *
// *   CORRECT location:
// *   Arduino/libraries/lv_conf.h        ← HERE
// *   Arduino/libraries/lvgl/            ← library folder
// *
// * Optimised for ESP32-S3-Zero + 320x240 TFT + Air Quality Dashboard
// */

#if 1   /* ← must be 1 to enable this config. If 0, nothing works. */

#ifndef LV_CONF_H
#define LV_CONF_H

// ─── CRITICAL FIX: disable ARM Helium assembly (not compatible with ESP32) ─
#define LV_USE_NATIVE_HELIUM_ASM 0

// ─── COLOUR DEPTH ────────────────────────────────────────────
#define LV_COLOR_DEPTH 16   // 16-bit RGB565 matches ILI9341

// ─── MEMORY ──────────────────────────────────────────────────
#define LV_MEM_SIZE (48U * 1024U)   // 48KB internal LVGL heap

// ─── REFRESH ─────────────────────────────────────────────────
#define LV_DEF_REFR_PERIOD   10     // ms display refresh period
#define LV_INDEV_DEF_READ_PERIOD 10 // ms input read period

// ─── RENDERING ───────────────────────────────────────────────
#define LV_DRAW_COMPLEX      1      // enables arcs, gradients, shadows
#define LV_SHADOW_CACHE_SIZE 0
#define LV_CIRCLE_CACHE_SIZE 4
#define LV_IMG_CACHE_DEF_SIZE 0

// ─── FONTS ───────────────────────────────────────────────────
// Only enable what this project uses — saves Flash memory
#define LV_FONT_MONTSERRAT_10  1
#define LV_FONT_MONTSERRAT_12  1
#define LV_FONT_MONTSERRAT_14  1
#define LV_FONT_MONTSERRAT_16  1
#define LV_FONT_MONTSERRAT_22  1
#define LV_FONT_MONTSERRAT_28  1
#define LV_FONT_MONTSERRAT_48  1   // Large AQI number

// Disable unused sizes to save Flash
#define LV_FONT_MONTSERRAT_8   0
#define LV_FONT_MONTSERRAT_18  0
#define LV_FONT_MONTSERRAT_20  0
#define LV_FONT_MONTSERRAT_24  0
#define LV_FONT_MONTSERRAT_26  0
#define LV_FONT_MONTSERRAT_30  0
#define LV_FONT_MONTSERRAT_32  0
#define LV_FONT_MONTSERRAT_34  0
#define LV_FONT_MONTSERRAT_36  0
#define LV_FONT_MONTSERRAT_38  0
#define LV_FONT_MONTSERRAT_40  0
#define LV_FONT_MONTSERRAT_42  0
#define LV_FONT_MONTSERRAT_44  0
#define LV_FONT_MONTSERRAT_46  0

#define LV_FONT_DEFAULT &lv_font_montserrat_14

// ─── WIDGETS USED ────────────────────────────────────────────
#define LV_USE_ARC      1   // Neon AQI ring
#define LV_USE_LABEL    1   // All text labels
#define LV_USE_IMG      1   // Icons
#define LV_USE_LINE     1
#define LV_USE_BTN      0
#define LV_USE_CHART    0
#define LV_USE_TABLE    0
#define LV_USE_SLIDER   0
#define LV_USE_SWITCH   0
#define LV_USE_BAR      0

// ─── ANIMATIONS ──────────────────────────────────────────────
#define LV_USE_ANIMATION 1

// ─── LOGGING (disable in production) ─────────────────────────
#define LV_USE_LOG       0

// ─── PERFORMANCE MONITOR (set 1 to show FPS while debugging) ─
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR  0

// ─── TICK SOURCE ─────────────────────────────────────────────
// Using hardware timer + lv_tick_inc() manually in firmware
#define LV_TICK_CUSTOM     1
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

#endif  // LV_CONF_H
#endif  // enable guard
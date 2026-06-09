/**
 * User_Setup.h — TFT_eSPI configuration
 * ─────────────────────────────────────
 * COPY THIS FILE TO:
 *   Arduino/libraries/TFT_eSPI/User_Setup.h
 *   (overwrite/replace the existing one)
 *
 * Hardware : ESP32-S3-Zero + Waveshare 2.4" ILI9341
 * Pins     : CONFIRMED WORKING
 */

#define USER_SETUP_ID 70
#define USER_SETUP_LOADED

// ─── DRIVER ──────────────────────────────────────────────────
#define ILI9341_DRIVER

// ─── DISPLAY SIZE (portrait native, rotated in code) ─────────
#define TFT_WIDTH   240
#define TFT_HEIGHT  320

// ─── HSPI PORT (required for ESP32-S3) ───────────────────────
#define USE_HSPI_PORT

// ─── PIN DEFINITIONS ─────────────────────────────────────────
#define TFT_MOSI  11    // DIN
#define TFT_SCLK  12    // CLK
#define TFT_CS    10    // CS
#define TFT_DC     9    // DC
#define TFT_RST    8    // RST
#define TFT_MISO  -1    // Not connected
#define TFT_BL    -1    // Backlight hard-wired to 3V3

// ─── SPI SPEED ───────────────────────────────────────────────
#define SPI_FREQUENCY        40000000
#define SPI_READ_FREQUENCY    6000000
#define SPI_TOUCH_FREQUENCY   2500000

// ─── FONTS ───────────────────────────────────────────────────
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// ─── REQUIRED ────────────────────────────────────────────────
#define SUPPORT_TRANSACTIONS
/**
 * ============================================================
 *  Air Sense: The Smartest DIY Air Purifier
 *  Author   : Kishan Pratap Singh
 *  Cloud    : Arduino IoT Cloud (Air Sense)
 * ============================================================
 */

// ===== INCLUDES =====
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <SPI.h>
#include <Adafruit_BME680.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

// ═══════════════════════════════════════════════════════════════
//  CONFIGURATION
// ═══════════════════════════════════════════════════════════════

// ── WiFi ──────────────────────────────────────────────────────
const char SSID[] = "Airtel_Nirmala Maa";
const char PASS[] = "Air@8090";

// ── IoT Cloud device credentials ───────────────────
const char THING_ID[]      = "Your_Client_ID";
const char DEVICE_LOGIN[]  = "your_device_name";              
const char DEVICE_KEY[]    = "Your_Client_Secret";

// ── Pin map ───────────────────────────────────────────────────
#define BME_SDA        43   // BME680 I2C SDA
#define BME_SCL        44   // BME680 I2C SCL

#define DUST_ILED_PIN   5   // Waveshare dust sensor — LED trigger
#define DUST_AOUT_PIN   6   // Waveshare dust sensor — analog output

#define RELAY_PIN       7   // Fan relay

// ── Polarity configuration ───────────────────────────────────
#define DUST_LED_ACTIVE_LOW 1
#if DUST_LED_ACTIVE_LOW
  #define DUST_LED_ON  LOW
  #define DUST_LED_OFF HIGH
#else
  #define DUST_LED_ON  HIGH
  #define DUST_LED_OFF LOW
#endif

#define RELAY_ACTIVE_LOW 0
#if RELAY_ACTIVE_LOW
  #define RELAY_ON  LOW
  #define RELAY_OFF HIGH
#else
  #define RELAY_ON  HIGH
  #define RELAY_OFF LOW
#endif

// ── Dust sensor calibration ──────────────────────────────────
#define DUST_SAMPLES                  15      
#define DUST_SENSITIVITY              200.0f  
#define DUST_SMOOTHING_ALPHA          0.3f    
#define DUST_DEFAULT_BASELINE         0.60f   
#define DUST_FLOOR_ADAPT_RATE         0.01f   
#define DUST_BOARD_VOLTAGE_MULTIPLIER 10.0f  
#define DUST_DEBUG                    1       

// ── BME680 gas sensor ─────────────────────────────────────────
#define BME_WARMUP_MS       300000UL  
#define IAQ_SMOOTHING_ALPHA 0.1f      

// ── Font fallback (if Inter fonts aren't generated) ──────────
#ifndef INTER_FONTS_AVAILABLE
  #define inter_bold_32    lv_font_montserrat_28
  #define inter_medium_14  lv_font_montserrat_10
  #define inter_regular_12 lv_font_montserrat_12
  #define inter_regular_16 lv_font_montserrat_14
#endif

// ═══════════════════════════════════════════════════════════════
//  DATA TYPES & AQI LOGIC
// ═══════════════════════════════════════════════════════════════

typedef struct {
  float pm25, temperature, humidity, pressure, iaq, filterLife;
  int   aqi;
  bool  wifiConnected;
} SensorData;

typedef struct { const char *label; lv_color_t color; } AQIStatus;

// EPA AQI categories, label text shortened to fit the dashboard circle.
static inline AQIStatus getAQIStatus(int aqi) {
  if (aqi <=  50) return {"Good",          lv_color_make(  0,228,  0)};   // Green
  if (aqi <= 100) return {"Moderate",      lv_color_make(255,255,  0)};   // Yellow
  if (aqi <= 150) return {"Unhealthy for\nSensitive Groups",
                                          lv_color_make(255,126,  0)};    // Orange
  if (aqi <= 200) return {"Unhealthy",     lv_color_make(255,  0,  0)};   // Red
  if (aqi <= 300) return {"Very Unhealthy",lv_color_make(143, 63,151)};   // Purple
  return              {"Hazardous",         lv_color_make(126,  0, 35)};    // Maroon
}

// EPA PM2.5 breakpoint table, extended through the Hazardous band
// (AQI 500) so severe pollution days display correctly.
static inline int calcAQI(float pm25, float iaq) {
  int p = 0;
  if      (pm25 <= 12.0f)   p = (int)(pm25 / 12.0f * 50);
  else if (pm25 <= 35.4f)   p = 51  + (int)((pm25 - 12.1f)  / 23.3f  * 49);
  else if (pm25 <= 55.4f)   p = 101 + (int)((pm25 - 35.5f)  / 19.9f  * 49);
  else if (pm25 <= 150.4f)  p = 151 + (int)((pm25 - 55.5f)  / 94.9f  * 49);
  else if (pm25 <= 250.4f)  p = 201 + (int)((pm25 - 150.5f) / 99.9f  * 99);
  else if (pm25 <= 500.4f)  p = 301 + (int)((pm25 - 250.5f) / 249.9f * 199);
  else                      p = 500;
  return (int)(p*0.7f + (100.f-iaq)*1.5f*0.3f);
}

// ═══════════════════════════════════════════════════════════════
//  UI THEME & LAYOUT CONSTANTS
// ═══════════════════════════════════════════════════════════════

#define CB    lv_color_hex(0x050505)
#define CC    lv_color_hex(0x111111)
#define CD    lv_color_hex(0x232323)
#define CW    lv_color_hex(0xFFFFFF)
#define CG    lv_color_hex(0x00DF50)
#define CGD1  lv_color_hex(0x003D18)
#define CGD2  lv_color_hex(0x001E0C)
#define CGD3  lv_color_hex(0x000D05)
#define CDIM  lv_color_hex(0x757575)
#define CR    10

#define CSW   96
#define CH    75
#define XL     2
#define XR   222
#define XC   101
#define WC   118
#define Y1     2
#define Y2    80
#define Y3   158

#define AQI_CX  160
#define AQI_CY   86
#define AQI_R   109

#define SCREEN_W 320
#define SCREEN_H 240
#define BUF_SIZE (SCREEN_W * 24)

// ═══════════════════════════════════════════════════════════════
//  DISPLAY & LVGL CORE
// ═══════════════════════════════════════════════════════════════

static lv_color_t buf1[BUF_SIZE];
static lv_color_t buf2[BUF_SIZE];
TFT_eSPI tft = TFT_eSPI();

void lvgl_flush_cb(lv_display_t *d, const lv_area_t *a, uint8_t *px) {
  tft.startWrite();
  tft.setAddrWindow(a->x1, a->y1, a->x2-a->x1+1, a->y2-a->y1+1);
  tft.pushColors((uint16_t*)px, (a->x2-a->x1+1)*(a->y2-a->y1+1), true);
  tft.endWrite();
  lv_display_flush_ready(d);
}

void IRAM_ATTR lvgl_tick() { lv_tick_inc(1); }

void initDisplay() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void initLVGL() {
  lv_init();
  lv_display_t *disp = lv_display_create(SCREEN_W, SCREEN_H);

  lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(disp, lvgl_flush_cb);

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
  hw_timer_t *tmr = timerBegin(1000000);          // 1 MHz
  timerAttachInterrupt(tmr, &lvgl_tick);
  timerAlarm(tmr, 1000, true, 0);                 // 1 ms tick
#else
  hw_timer_t *tmr = timerBegin(0, 80, true);      // 80MHz 
  timerAttachInterrupt(tmr, &lvgl_tick, true);
  timerAlarmWrite(tmr, 1000, true);
  timerAlarmEnable(tmr);
#endif
}

// ═══════════════════════════════════════════════════════════════
//  UI WIDGET HANDLES
// ═══════════════════════════════════════════════════════════════

lv_obj_t *ui_glow_arc   = NULL;
lv_obj_t *ui_aqi_arc    = NULL;
lv_obj_t *ui_aqi_value  = NULL;
lv_obj_t *ui_aqi_status = NULL;
lv_obj_t *ui_aqi_label  = NULL;

lv_obj_t *ui_pm25_val   = NULL;
lv_obj_t *ui_temp_val   = NULL;
lv_obj_t *ui_hum_val    = NULL;
lv_obj_t *ui_pres_val   = NULL;
lv_obj_t *ui_iaq_val    = NULL;
lv_obj_t *ui_filter_val = NULL;
lv_obj_t *ui_filter_status_label = NULL;
lv_obj_t *ui_wifi_label = NULL;
lv_obj_t *ui_wifi_dot   = NULL;

// ═══════════════════════════════════════════════════════════════
//  UI BUILDER HELPERS
// ═══════════════════════════════════════════════════════════════

static lv_obj_t* mkCard(lv_obj_t *p, lv_coord_t x, lv_coord_t y,
                          lv_coord_t w, lv_coord_t h) {
  lv_obj_t *c = lv_obj_create(p);
  lv_obj_set_pos(c,x,y); lv_obj_set_size(c,w,h);
  lv_obj_clear_flag(c, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scrollbar_mode(c, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_bg_color(c,     CC,           0);
  lv_obj_set_style_bg_opa(c,       LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(c, CD,           0);
  lv_obj_set_style_border_width(c, 1,            0);
  lv_obj_set_style_border_opa(c,   85,           0);
  lv_obj_set_style_radius(c,       CR,           0);
  lv_obj_set_style_pad_all(c,      0,            0);
  lv_obj_set_style_shadow_color(c, lv_color_hex(0x000000), 0);
  lv_obj_set_style_shadow_width(c, 8,            0);
  lv_obj_set_style_shadow_opa(c,   70,           0);
  return c;
}

static void mkDot(lv_obj_t *c, lv_coord_t cw, lv_color_t col) {
  lv_obj_t *d = lv_obj_create(c);
  lv_obj_set_size(d,6,6); lv_obj_set_pos(d, cw-11, 9);
  lv_obj_set_style_bg_color(d,    col,              0);
  lv_obj_set_style_bg_opa(d,      LV_OPA_COVER,     0);
  lv_obj_set_style_radius(d,      LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(d, 0,               0);
  lv_obj_set_style_pad_all(d,     0,                0);
}

static void mkTitle(lv_obj_t *c, const char *t, lv_coord_t x) {
  lv_obj_t *l = lv_label_create(c);
  lv_label_set_text(l, t);
  lv_obj_set_style_text_color(l, CDIM,                   0);
  lv_obj_set_style_text_font(l,  &inter_medium_14,       0);
  lv_obj_set_pos(l, x, 11);
}

static lv_obj_t* mkVal(lv_obj_t *c, const char *v, const char *u,
                         const lv_font_t *f,
                         const lv_font_t *unit_font,
                         lv_coord_t vx, lv_coord_t vy,
                         lv_coord_t unit_off_x = 6) {
  lv_obj_t *vl = lv_label_create(c);
  lv_label_set_text(vl, v);
  lv_obj_set_style_text_color(vl, CW, 0);
  lv_obj_set_style_text_font(vl,  f,  0);
  lv_obj_set_pos(vl, vx, vy);

  lv_obj_t *ul = lv_label_create(c);
  lv_label_set_text(ul, u);
  lv_obj_set_style_text_color(ul, CDIM,                   0);
  lv_obj_set_style_text_font(ul,  unit_font,              0);
  lv_obj_align_to(ul, vl, LV_ALIGN_OUT_RIGHT_BOTTOM, unit_off_x, -3);
  return vl;
}

static lv_obj_t* metCard(lv_obj_t *p,
                           lv_coord_t x, lv_coord_t y, lv_coord_t w,
                           const char *ttl,
                           const char *val, const char *unit,
                           const lv_font_t *fnt,
                           const lv_font_t *unit_fnt,
                           lv_coord_t vx, lv_coord_t vy,
                           lv_color_t dotCol,
                           lv_coord_t title_x,
                           lv_coord_t unit_off_x = 6) {
  lv_obj_t *c = mkCard(p, x, y, w, CH);
  mkDot(c, w, dotCol);
  mkTitle(c, ttl, title_x);
  return mkVal(c, val, unit, fnt, unit_fnt, vx, vy, unit_off_x);
}

// ═══════════════════════════════════════════════════════════════
//  DASHBOARD WIDGETS
// ═══════════════════════════════════════════════════════════════

static void buildAQI(lv_obj_t *scr, int aqi) {
  AQIStatus st = getAQIStatus(aqi);
  const lv_coord_t cx=AQI_CX, cy=AQI_CY, r=AQI_R;

  auto glowArc = [&](lv_coord_t dia, lv_color_t col,
                      lv_opa_t opa, int w) -> lv_obj_t* {
    lv_obj_t *a = lv_arc_create(scr);
    lv_obj_set_size(a, dia, dia);
    lv_obj_set_pos(a, cx-dia/2, cy-dia/2);
    lv_arc_set_angles(a,    0, 360);
    lv_arc_set_bg_angles(a, 0, 360);
    lv_obj_set_style_arc_color(a, col, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(a, col, LV_PART_MAIN);
    lv_obj_set_style_arc_width(a, w,   LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(a, w,   LV_PART_MAIN);
    lv_obj_set_style_arc_opa(a,  opa,  LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(a,  opa,  LV_PART_MAIN);
    lv_obj_remove_style(a, NULL, LV_PART_KNOB);
    lv_obj_set_style_bg_opa(a,   LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(a, 0, 0);
    return a;
  };

  ui_glow_arc = glowArc(r+2, CGD1, 130, 10);

  lv_coord_t fi = r - 10;
  lv_obj_t *f = lv_obj_create(scr);
  lv_obj_set_size(f, fi, fi);
  lv_obj_set_pos(f, cx-fi/2, cy-fi/2);
  lv_obj_set_style_bg_color(f,    lv_color_hex(0x060606), 0);
  lv_obj_set_style_bg_opa(f,      LV_OPA_COVER,           0);
  lv_obj_set_style_radius(f,      LV_RADIUS_CIRCLE,       0);
  lv_obj_set_style_border_width(f, 0, 0);
  lv_obj_set_style_pad_all(f,     0, 0);
  lv_obj_clear_flag(f, LV_OBJ_FLAG_SCROLLABLE);

  ui_aqi_arc = lv_arc_create(scr);
  lv_obj_set_size(ui_aqi_arc, r, r);
  lv_obj_set_pos(ui_aqi_arc, cx-r/2, cy-r/2);
  lv_arc_set_angles(ui_aqi_arc,    0, 360);
  lv_arc_set_bg_angles(ui_aqi_arc, 0, 360);
  lv_obj_set_style_arc_color(ui_aqi_arc, st.color,                LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(ui_aqi_arc, lv_color_hex(0x181818),  LV_PART_MAIN);
  lv_obj_set_style_arc_width(ui_aqi_arc, 4, LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(ui_aqi_arc, 4, LV_PART_MAIN);
  lv_obj_remove_style(ui_aqi_arc, NULL, LV_PART_KNOB);
  lv_obj_set_style_bg_opa(ui_aqi_arc, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(ui_aqi_arc, 0, 0);

  ui_aqi_label = lv_label_create(scr);
  lv_label_set_text(ui_aqi_label, "AQI");
  lv_obj_set_style_text_color(ui_aqi_label, CDIM,                   0);
  lv_obj_set_style_text_font(ui_aqi_label,  &inter_regular_12,      0);
  lv_obj_set_width(ui_aqi_label, WC);
  lv_obj_set_style_text_align(ui_aqi_label, LV_TEXT_ALIGN_CENTER,   0);
  lv_obj_set_pos(ui_aqi_label, XC, cy-40);

  ui_aqi_value = lv_label_create(scr);
  char nb[8]; snprintf(nb, 8, "%d", aqi);
  lv_label_set_text(ui_aqi_value, nb);
  lv_obj_set_style_text_color(ui_aqi_value, CW,                    0);
  lv_obj_set_style_text_font(ui_aqi_value,  &inter_bold_32,        0);
  lv_obj_set_width(ui_aqi_value, WC);
  lv_obj_set_style_text_align(ui_aqi_value, LV_TEXT_ALIGN_CENTER,  0);
  lv_obj_set_pos(ui_aqi_value, XC, cy-21);

  ui_aqi_status = lv_label_create(scr);
  lv_label_set_text(ui_aqi_status, st.label);
  lv_obj_set_style_text_color(ui_aqi_status, st.color,               0);
  lv_obj_set_style_text_font(ui_aqi_status,  &inter_regular_12,      0);
  lv_obj_set_width(ui_aqi_status, WC);
  lv_label_set_long_mode(ui_aqi_status, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_align(ui_aqi_status, LV_TEXT_ALIGN_CENTER,   0);
  lv_obj_set_pos(ui_aqi_status, XC, cy+15);
}

static void buildWifi(lv_obj_t *scr, bool conn) {
  lv_obj_t *c = mkCard(scr, XC, Y3, WC, CH);

  ui_wifi_dot = lv_obj_create(c);
  lv_obj_set_size(ui_wifi_dot, 6, 6);
  lv_obj_set_pos(ui_wifi_dot, WC-13, 9);
  lv_obj_set_style_bg_color(ui_wifi_dot, conn?CG:lv_color_hex(0xFF4444), 0);
  lv_obj_set_style_bg_opa(ui_wifi_dot,   LV_OPA_COVER,     0);
  lv_obj_set_style_radius(ui_wifi_dot,   LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(ui_wifi_dot, 0,            0);
  lv_obj_set_style_pad_all(ui_wifi_dot,  0,                0);

  lv_obj_t *sub = lv_label_create(c);
  lv_label_set_text(sub, "WIFI");
  lv_obj_set_style_text_color(sub, CDIM,                   0);
  lv_obj_set_style_text_font(sub,  &inter_medium_14,       0);
  lv_obj_set_pos(sub, 12, 10);

  ui_wifi_label = lv_label_create(c);
  lv_label_set_text(ui_wifi_label, conn ? "CONNECTED" : "OFFLINE");
  lv_obj_set_style_text_color(ui_wifi_label,
    conn ? CW : lv_color_hex(0xFF4444), 0);
  lv_obj_set_style_text_font(ui_wifi_label, &inter_regular_12, 0);
  lv_obj_set_pos(ui_wifi_label, 5, 44);
  lv_obj_set_width(ui_wifi_label, WC-11);
  lv_obj_set_style_text_align(ui_wifi_label, LV_TEXT_ALIGN_CENTER, 0);
}

void createDashboard(const SensorData *d) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, CB,           0);
  lv_obj_set_style_bg_opa(scr,   LV_OPA_COVER, 0);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  char b[16];

  snprintf(b,16,"%.0f",d->pm25);
  ui_pm25_val = metCard(scr,XL,Y1,CSW,
    "PM2.5",b,"ug/m3",
    &inter_bold_32, &inter_medium_14,
    9,32,lv_color_hex(0x00DF50),
    9, 26);

  snprintf(b,16,"%.0f",d->temperature);
  ui_temp_val = metCard(scr,XL,Y2,CSW,
    "TEMPERATURE",b,"\xC2\xB0""C",
    &inter_bold_32, &lv_font_montserrat_16,
    9,32,lv_color_hex(0xFFAA33),
    4, 26);

  snprintf(b,16,"%.0f",d->humidity);
  ui_hum_val = metCard(scr,XL,Y3,CSW,
    "HUMIDITY",b,"%",
    &inter_bold_32, &lv_font_montserrat_16,
    9,32,lv_color_hex(0x3399FF),
    9, 26);

  snprintf(b,16,"%.0f",d->pressure);
  ui_pres_val = metCard(scr,XR,Y1,CSW,
    "PRESSURE",b,"hPa",
    &inter_bold_32, &inter_medium_14,
    7,34,lv_color_hex(0x9977FF),
    9, 37);

  snprintf(b,16,"%.0f",d->iaq);
  ui_iaq_val = metCard(scr,XR,Y2,CSW,
    "IAQ / GAS",b,"%",
    &inter_bold_32, &lv_font_montserrat_16,
    9,32,lv_color_hex(0x3399FF),
    9);

  snprintf(b,16,"%.0f",d->filterLife);
  ui_filter_val = metCard(scr,XR,Y3,CSW,
    "FILTER LIFE",b,"%",
    &inter_bold_32, &lv_font_montserrat_16,
    9,32,lv_color_hex(0x00DF50),
    9);

  lv_obj_t *filterCard = lv_obj_get_parent(ui_filter_val);
  ui_filter_status_label = lv_label_create(filterCard);
  lv_label_set_text(ui_filter_status_label, "");
  lv_obj_set_style_text_font(ui_filter_status_label, &inter_regular_12, 0);
  lv_obj_set_style_text_color(ui_filter_status_label, lv_color_hex(0xFF4444), 0);
  lv_obj_align_to(ui_filter_status_label, ui_filter_val, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 3);

  buildAQI(scr, d->aqi);
  buildWifi(scr, d->wifiConnected);
}

void updateSensorValues(const SensorData *d) {
  char b[16];
  AQIStatus st = getAQIStatus(d->aqi);

  auto upd = [](lv_obj_t *l, const char *t){ if(l) lv_label_set_text(l,t); };
  snprintf(b,16,"%.0f",d->pm25);        upd(ui_pm25_val,   b);
  snprintf(b,16,"%.0f",d->temperature); upd(ui_temp_val,   b);
  snprintf(b,16,"%.0f",d->humidity);    upd(ui_hum_val,    b);
  snprintf(b,16,"%.0f",d->pressure);    upd(ui_pres_val,   b);
  snprintf(b,16,"%.0f",d->iaq);         upd(ui_iaq_val,    b);
  snprintf(b,16,"%.0f",d->filterLife);  upd(ui_filter_val, b);

  const char* filterMsg = "";
  if (d->filterLife <= 10)      filterMsg = "Replace Filter";
  else if (d->filterLife <= 20) filterMsg = "Replace Soon";
  if (ui_filter_status_label) lv_label_set_text(ui_filter_status_label, filterMsg);

  if (ui_aqi_value)  { snprintf(b,16,"%d",d->aqi); lv_label_set_text(ui_aqi_value,b); }
  if (ui_aqi_status) { lv_label_set_text(ui_aqi_status, st.label);
                       lv_obj_set_style_text_color(ui_aqi_status, st.color, 0); }
  if (ui_aqi_arc)
    lv_obj_set_style_arc_color(ui_aqi_arc, st.color, LV_PART_INDICATOR);
  if (ui_wifi_label) {
    lv_label_set_text(ui_wifi_label, d->wifiConnected?"CONNECTED":"OFFLINE");
    lv_obj_set_style_text_color(ui_wifi_label,
      d->wifiConnected?CW:lv_color_hex(0xFF4444), 0);
  }
  if (ui_wifi_dot)
    lv_obj_set_style_bg_color(ui_wifi_dot,
      d->wifiConnected?CG:lv_color_hex(0xFF4444), 0);
}

// ═══════════════════════════════════════════════════════════════
//  UI ANIMATIONS
// ═══════════════════════════════════════════════════════════════

static uint16_t s_glowPhase = 0;
static uint16_t s_wifiPhase = 0;

static uint8_t smoothPulse(uint16_t phase, uint16_t total) {
  uint32_t p = (uint32_t)phase * 512 / total;
  if (p > 255) p = 511 - p;
  uint32_t x = p;
  uint32_t y = x*x*(767 - 2*x) / 65536;
  return (uint8_t)(y > 255 ? 255 : y);
}

void bootAnimation() {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x050505), 0);
  lv_obj_set_style_bg_opa(scr,   LV_OPA_COVER,           0);

  lv_obj_t *glow = lv_arc_create(scr);
  lv_obj_set_size(glow, 120, 120);
  lv_obj_align(glow, LV_ALIGN_CENTER, 0, -10);
  lv_arc_set_bg_angles(glow, 0, 360);
  lv_obj_set_style_arc_color(glow, lv_color_hex(0x003A15), LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(glow, lv_color_hex(0x050505), LV_PART_MAIN);
  lv_obj_set_style_arc_width(glow, 18, LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(glow, 18, LV_PART_MAIN);
  lv_obj_set_style_arc_opa(glow,   0,  LV_PART_INDICATOR);
  lv_obj_remove_style(glow, NULL, LV_PART_KNOB);
  lv_obj_set_style_bg_opa(glow, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(glow, 0, 0);

  lv_obj_t *sweep = lv_arc_create(scr);
  lv_obj_set_size(sweep, 100, 100);
  lv_obj_align(sweep, LV_ALIGN_CENTER, 0, -10);
  lv_arc_set_bg_angles(sweep, 0, 360);
  lv_arc_set_angles(sweep, 0, 0);
  lv_obj_set_style_arc_color(sweep, lv_color_hex(0x00DF50), LV_PART_INDICATOR);
  lv_obj_set_style_arc_color(sweep, lv_color_hex(0x0D0D0D), LV_PART_MAIN);
  lv_obj_set_style_arc_width(sweep, 3, LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(sweep, 3, LV_PART_MAIN);
  lv_obj_remove_style(sweep, NULL, LV_PART_KNOB);
  lv_obj_set_style_bg_opa(sweep, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(sweep, 0, 0);

  for (int a = 0; a <= 360; a += 4) {
    lv_arc_set_angles(sweep, 0, a);
    uint8_t glowOpa = (uint8_t)((uint32_t)a * 110 / 360);
    lv_obj_set_style_arc_opa(glow, glowOpa, LV_PART_INDICATOR);
    lv_timer_handler();
    delay(10);
  }
  delay(300);
  for (int op = 255; op >= 0; op -= 8) {
    lv_obj_set_style_arc_opa(sweep, op, LV_PART_INDICATOR);
    lv_obj_set_style_arc_opa(glow,  (uint8_t)(op*110/255), LV_PART_INDICATOR);
    lv_timer_handler();
    delay(8);
  }
  lv_obj_del(sweep);
  lv_obj_del(glow);
  lv_timer_handler();
}

void animateGlow() {
  if (!ui_glow_arc) return;
  s_glowPhase++;
  if (s_glowPhase >= 480) s_glowPhase = 0;
  uint8_t pulse  = smoothPulse(s_glowPhase, 480);
  uint8_t glowOpa = 55 + (uint8_t)((uint32_t)pulse * 145 / 255);
  lv_obj_set_style_arc_opa(ui_glow_arc, glowOpa, LV_PART_INDICATOR);
  lv_obj_set_style_arc_opa(ui_glow_arc, 0,        LV_PART_MAIN);
  if (ui_aqi_arc) {
    uint8_t ringOpa = 210 + (uint8_t)((uint32_t)pulse * 45 / 255);
    lv_obj_set_style_arc_opa(ui_aqi_arc, ringOpa, LV_PART_INDICATOR);
  }
}

void animateWiFi() {
  if (!ui_wifi_dot) return;
  s_wifiPhase++;
  if (s_wifiPhase >= 90) s_wifiPhase = 0;
  uint8_t pulse = smoothPulse(s_wifiPhase, 90);
  uint8_t opa   = 120 + (uint8_t)((uint32_t)pulse * 135 / 255);
  lv_obj_set_style_bg_opa(ui_wifi_dot, opa, 0);
}

// ═══════════════════════════════════════════════════════════════
//  SENSORS
// ═══════════════════════════════════════════════════════════════

Adafruit_BME680 bme;
bool bme680_ok = false;

float readDustSensor() {
  uint32_t mvSum = 0;

  for (int i = 0; i < DUST_SAMPLES; i++) {
    digitalWrite(DUST_ILED_PIN, DUST_LED_ON);
    delayMicroseconds(280);
    mvSum += analogReadMilliVolts(DUST_AOUT_PIN);
    delayMicroseconds(40);
    digitalWrite(DUST_ILED_PIN, DUST_LED_OFF);
    delayMicroseconds(9680);   // complete the 10ms sensing cycle
  }

  float voltage = (mvSum / (float)DUST_SAMPLES) / 1000.0f * DUST_BOARD_VOLTAGE_MULTIPLIER;

  static float baseline = DUST_DEFAULT_BASELINE;
  if (voltage < baseline) {
    baseline += DUST_FLOOR_ADAPT_RATE * (voltage - baseline);
  }

  float dust_ug = (voltage - baseline) * DUST_SENSITIVITY;
  if (dust_ug < 0)   dust_ug = 0;
  if (dust_ug > 500) dust_ug = 500;

  static float filtered = -1;
  if (filtered < 0) filtered = dust_ug;
  filtered = filtered + DUST_SMOOTHING_ALPHA * (dust_ug - filtered);

#if DUST_DEBUG
  {
    const int barWidth = 20;
    int filled = (int)((filtered / 500.0f) * barWidth);
    if (filled < 0)         filled = 0;
    if (filled > barWidth)  filled = barWidth;
    char bar[barWidth + 1];
    for (int i = 0; i < barWidth; i++) bar[i] = (i < filled) ? '#' : '-';
    bar[barWidth] = '\0';

    Serial.printf("[Dust]  V=%5.3fV  Base=%5.3fV  PM2.5=%6.1f ug/m3  [%s]\n",
                  voltage, baseline, filtered, bar);
  }
#endif

  return filtered;
}

// ── HEPA filter life tracking ────────────────────────────────

unsigned long fanRuntimeSeconds = 0;
unsigned long lastFanSecond     = 0;
bool fanIsOn                    = true;

void updateFanRuntime() {
  if (millis() - lastFanSecond >= 1000) {
    lastFanSecond += 1000;
    if (fanIsOn) {
      fanRuntimeSeconds++;
    }
  }
}

float calculateFilterLife() {
  float life = 100.0f - (fanRuntimeSeconds / 360000.0f);
  if (life < 0.0f) life = 0.0f;
  return life;
}

// ═══════════════════════════════════════════════════════════════
//  ARDUINO IOT CLOUD
// ═══════════════════════════════════════════════════════════════

float DUST;          // PM2.5
float Humidity;
float Temperature;
int   AQI;
bool  Power;          // fan control

void onPowerChange();

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_LOGIN);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.setThingId(THING_ID);

  ArduinoCloud.addProperty(DUST,        READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(Humidity,    READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(Temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(AQI,         READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(Power,       READWRITE, ON_CHANGE, onPowerChange);
}

void onPowerChange() {
  if (Power) {
    digitalWrite(RELAY_PIN, RELAY_ON);
    fanIsOn = true;
  } else {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    fanIsOn = false;
  }
}

// ═══════════════════════════════════════════════════════════════
//  MAIN PROGRAM
// ═══════════════════════════════════════════════════════════════

SensorData g_data = {
  .pm25=0, .temperature=0, .humidity=0,
  .pressure=0, .iaq=50, .filterLife=100,
  .aqi=0, .wifiConnected=false
};

unsigned long tSensor=0, tAnim=0;

void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.println();
  Serial.println("========================================================");
  Serial.println("   AirSense — The Smartest DIY Air Purifier");
  Serial.println("========================================================");

  // Display self-test
  initDisplay();
  tft.fillScreen(TFT_RED);
  delay(1500);
  tft.fillScreen(TFT_BLACK);
  Serial.println(" [OK]   Display self-test complete");

  // BME680
  Wire.begin(BME_SDA, BME_SCL);
  if (bme.begin()) {
    bme680_ok = true;
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);
    Serial.println(" [OK]   BME680 initialized");
  } else {
    Serial.println(" [WARN] BME680 not found - using fallback data");
    bme680_ok = false;
  }

  // Dust sensor
  pinMode(DUST_ILED_PIN, OUTPUT);
  digitalWrite(DUST_ILED_PIN, DUST_LED_OFF);
  Serial.println(" [OK]   Dust sensor pins ready");

  // Fan relay - on at startup
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_ON);
  fanIsOn = true;
  Serial.println(" [OK]   Fan relay ON");

  // Display + UI
  initLVGL();
  bootAnimation();
  createDashboard(&g_data);
  Serial.println(" [OK]   Dashboard UI ready");

  // Arduino IoT Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  Serial.println("========================================================");
  Serial.println(" Boot complete — entering main loop");
  Serial.println("========================================================");
  Serial.println();
}

void loop() {
  unsigned long now = millis();

  ArduinoCloud.update();
  lv_timer_handler();

  updateFanRuntime();
  g_data.filterLife = calculateFilterLife();

  if (now - tAnim >= 16) {
    tAnim = now;
    animateGlow();
    animateWiFi();
  }

  if (now - tSensor >= 2000) {
    tSensor = now;

    if (bme680_ok && bme.performReading()) {
      g_data.temperature = bme.temperature;
      g_data.humidity    = bme.humidity;
      g_data.pressure    = bme.pressure / 100.0f;

      float gas = bme.gas_resistance / 1000.0f;
      float iaqPercent = 0;
      if (gas > 200) iaqPercent = 100;
      else if (gas < 10) iaqPercent = 0;
      else iaqPercent = (gas - 10) * 100.0f / (200.0f - 10.0f);
      iaqPercent = constrain(iaqPercent, 0, 100);

      if (now < BME_WARMUP_MS) {
        
        g_data.iaq = 50;
      } else {
        static float iaqFiltered = -1;
        if (iaqFiltered < 0) iaqFiltered = iaqPercent;
        iaqFiltered += IAQ_SMOOTHING_ALPHA * (iaqPercent - iaqFiltered);
        g_data.iaq = iaqFiltered;
      }
    } else if (!bme680_ok) {
      g_data.temperature = 0;
      g_data.humidity    = 0;
      g_data.pressure    = 0;
      g_data.iaq         = 50;
    }

    g_data.pm25          = readDustSensor();
    g_data.wifiConnected = (WiFi.status() == WL_CONNECTED);
    g_data.aqi            = calcAQI(g_data.pm25, g_data.iaq);

    updateSensorValues(&g_data);

    DUST        = g_data.pm25;
    Temperature = g_data.temperature;
    Humidity    = g_data.humidity;
    AQI         = g_data.aqi;

  }

  delay(4);
}

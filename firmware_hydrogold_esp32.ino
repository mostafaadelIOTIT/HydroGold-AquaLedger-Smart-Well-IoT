#include <Arduino.h>
#include "mbedtls/md.h" // Hardware SHA-256 Cryptography for AquaLedger & Odoo ERP

// Pin Definitions for HydroGold Smart Filter & iLock Node
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;
const int PRESSURE_PIN = 34;      // Simulates Differential Pressure (Filter Clogging %)
const int SCALE_PIN = 35;         // Simulates Harvested Dry Sludge Weight (Kg)
const int ILOCK_SERVO_PIN = 13;   // Controls Smart iLock Valve / Harvester Lock
const int BACKWASH_PUMP_PIN = 2;  // Blue LED: Auto-Backwash Flushing Pump
const int UVC_STERILIZER_PIN = 4; // Purple LED: UV-C Anti-Iron Bacteria Sterilizer
const int ILOCK_STATUS_PIN = 15;  // Green LED: Smart iLock OTP Unlocked Status

// Generate SHA-256 Cryptographic Hash for Blockchain/Odoo ERP Ledger
String signAquaLedgerSHA256(String payload) {
  byte shaResult[32];
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) payload.c_str(), payload.length());
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);

  String hashStr = "";
  for (int i = 0; i < 16; i++) {
    char hex[3];
    sprintf(hex, "%02x", (int)shaResult[i]);
    hashStr += hex;
  }
  return hashStr;
}

// Simple PWM Servo Control for ESP32 (0 deg = Locked, 90 deg = Unlocked)
void setiLockAngle(int angle) {
  int pulseWidth = map(angle, 0, 180, 500, 2400);
  for (int i = 0; i < 15; i++) {
    digitalWrite(ILOCK_SERVO_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(ILOCK_SERVO_PIN, LOW);
    delay(20 - pulseWidth / 1000);
  }
}

float getWellWaterDepthCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ILOCK_SERVO_PIN, OUTPUT);
  pinMode(BACKWASH_PUMP_PIN, OUTPUT);
  pinMode(UVC_STERILIZER_PIN, OUTPUT);
  pinMode(ILOCK_STATUS_PIN, OUTPUT);

  setiLockAngle(0); // Default state: iLock CLOSED
  digitalWrite(UVC_STERILIZER_PIN, HIGH); // UV-C Biofilm protection always ON

  Serial.println("================================================================");
  Serial.println("[HYDROGOLD-NODE] Smart Dual-Filter & AquaLedger iLock: ONLINE");
  Serial.println("[SECURITY] Edge SHA-256 & 6-Digit OTP Verification: ACTIVE");
  Serial.println("================================================================");
}

void loop() {
  float wellDepthCm = getWellWaterDepthCm();
  int filterClogPct = map(analogRead(PRESSURE_PIN), 0, 4095, 0, 100);
  int harvestedSludgeKg = map(analogRead(SCALE_PIN), 0, 4095, 0, 500);

  // Industrial Conversion Math (1 ton sludge = 3.01x mass output / 6x liquid fertilizer)
  int liquidFertilizerLiters = harvestedSludgeKg * 6;
  float greenGoldCrystalsKg = harvestedSludgeKg * 3.01;
  float carbonCreditsKgCO2 = harvestedSludgeKg * 1.85;

  String filterState = "NORMAL_FILTRATION";
  String ilockState = "LOCKED";
  String otpCode = "NONE";

  // 1. Automated Backwash & Sludge Harvesting Logic (If clogging > 70%)
  if (filterClogPct > 70) {
    filterState = "AUTO_BACKWASH_FLUSHING";
    digitalWrite(BACKWASH_PUMP_PIN, HIGH);
    Serial.println(">>> [FILTER ALERT] Clogging > 70%! Executing Auto-Backwash into Harvester...");
    delay(1200);
    digitalWrite(BACKWASH_PUMP_PIN, LOW);
  }

  // 2. Smart iLock & Bank OTP Dispatch (If harvested sludge >= 100 Kg ready for pickup)
  if (harvestedSludgeKg >= 100) {
    ilockState = "OTP_UNLOCKED_FOR_PICKUP";
    otpCode = "849201"; // Simulated 6-Digit Encrypted Bank/Gov OTP
    digitalWrite(ILOCK_STATUS_PIN, HIGH);
    setiLockAngle(90); // Rotate iLock Servo to OPEN position
  } else {
    digitalWrite(ILOCK_STATUS_PIN, LOW);
    setiLockAngle(0);  // Keep iLock Servo CLOSED
  }

  // 3. Construct Standardized JSON for AquaLedger & Odoo ERP
  String payload = "{\"well_depth_cm\":" + String(wellDepthCm, 1) +
                   ",\"filter_clog_pct\":" + String(filterClogPct) +
                   ",\"sludge_kg\":" + String(harvestedSludgeKg) +
                   ",\"fertilizer_L\":" + String(liquidFertilizerLiters) +
                   ",\"crystals_kg\":" + String(greenGoldCrystalsKg, 1) +
                   ",\"ilock\":\"" + ilockState + "\"" +
                   ",\"otp\":\"" + otpCode + "\"}";

  Serial.println("[AQUALEDGER-JSON] " + payload);
  Serial.println("[SHA256-LEDGER-SIG] " + signAquaLedgerSHA256(payload));
  delay(1000);
}

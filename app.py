import streamlit as st
import pandas as pd
import numpy as np
import hashlib
import json

st.set_page_config(page_title="HydroGold & AquaLedger ERP", layout="wide")

st.title("💧 HydroGold Dynamics & AquaLedger™ – Smart Well & Zero-Waste ERP Console")
st.caption("Lead Architect: Mostafa Adel | Automated Filter Backwash, iLock OTP Security & Circular Upcycling")

# Sidebar: Simulate ESP32 Well & Harvester Telemetry
st.sidebar.header("📡 Smart Well & Harvester Input")
well_id = st.sidebar.selectbox("Select Well Node", ["WELL-NEW-VALLEY-01", "WELL-DELTA-04", "WELL-QASSIM-SA-02", "WELL-BONN-DE-01"])
filter_clog_pct = st.sidebar.slider("Dual-Filter Differential Pressure / Clog (%)", 0, 100, 78)
sludge_kg = st.sidebar.slider("Harvested Dry Fe/Mn Sludge Weight (Kg)", 0, 1000, 250, step=10)
well_depth_cm = st.sidebar.slider("Ultrasonic Well Water Depth (cm)", 50, 400, 140)

# Industrial Upcycling Calculations (Based on HydroGold Stoichiometry)
# 1 Ton Dry Sludge -> 3.01 Tons Final Products | 100 Kg Sludge -> 600 Liters Liquid Fertilizer
mass_multiplied_kg = round(sludge_kg * 3.01, 1)
liquid_fertilizer_liters = sludge_kg * 6
fe2o3_yield_kg = round(sludge_kg * 0.58, 1)  # Avg 58% Iron Oxide
mno2_yield_kg = round(sludge_kg * 0.17, 1)   # Avg 17% Manganese Dioxide
silica_yield_kg = round(sludge_kg * 0.25, 1) # Avg 25% Colloidal Silica
cbam_carbon_credits_kg = round(sludge_kg * 1.85, 1)

# Financial & Barter Valuation (EGP & EUR Equivalents)
market_value_egp = liquid_fertilizer_liters * 35  # 600L = 21,000 EGP
farmer_barter_credit_egp = round(market_value_egp * 0.25, 0)
market_value_eur = round(market_value_egp / 52, 1)

# Automation & iLock Security Logic
backwash_status = "AUTO_BACKWASH_ACTIVE (Flushing to Harvester)" if filter_clog_pct > 70 else "NORMAL_FILTRATION (UV-C Active)"
ilock_status = "UNLOCKED (OTP: 849201 - Ready for Bank/Gov Pickup)" if sludge_kg >= 100 else "LOCKED (Accumulating Sludge)"
status_color = "green" if sludge_kg >= 100 else "orange"

# Top KPI Metrics
col1, col2, col3, col4 = st.columns(4)
col1.metric("Harvested Dry Sludge", f"{sludge_kg} Kg", f"3.01x Output: {mass_multiplied_kg} Kg")
col2.metric("Aqua Ferro-Mang Liquid", f"{liquid_fertilizer_liters} Liters", "+22% Crop Yield")
col3.metric("Industrial Output Value", f"{market_value_egp:,.0f} EGP", f"~ €{market_value_eur} EUR")
col4.metric("EU CBAM Carbon Offset", f"{cbam_carbon_credits_kg} Kg CO₂", "Verified Green Credit")

st.markdown(f"### 🔐 Smart iLock & Valve Status: :{status_color}[{ilock_status}]")
st.info(f"**Filter SCADA State:** `{backwash_status}` | **Farmer Barter Wallet Credit:** `{farmer_barter_credit_egp:,.0f} EGP` (Redeemable for Fertilizers / Healthcare)")

# Upcycling Breakdown Chart
st.subheader("🏭 Zero-Waste Stoichiometric Output Breakdown (Fe₂O₃ / MnO₂ / Silica)")
df_chem = pd.DataFrame({
    "Product Stream": [
        "Green-Gold Soluble Crystals (3.01x Mass)",
        "Iron Oxide (Fe₂O₃ - Cement/Ferrites)",
        "Manganese Dioxide (MnO₂ - Batteries/Agri)",
        "Colloidal Silica (Radiative Cooling/Paints)"
    ],
    "Yield (Kg)": [mass_multiplied_kg, fe2o3_yield_kg, mno2_yield_kg, silica_yield_kg]
}).set_index("Product Stream")

st.bar_chart(df_chem)

# Cryptographic Audit Trail for Odoo ERP & Agricultural Bank
payload_dict = {
    "well_id": well_id,
    "clog_pct": filter_clog_pct,
    "sludge_kg": sludge_kg,
    "fertilizer_L": liquid_fertilizer_liters,
    "ilock": ilock_status[:8]
}
payload_str = json.dumps(payload_dict, separators=(',', ':'))
sha256_sig = hashlib.sha256(payload_str.encode()).hexdigest()[:32]

st.subheader("🔗 AquaLedger Blockchain & Odoo ERP Synchronization")
st.code(f"SCADA Telemetry JSON : {payload_str}\nEdge SHA-256 Ledger  : {sha256_sig}\nOdoo ERP Inventory   : SYNCED (Agricultural Bank & Governorate Tripartite Ledger)", language="yaml")

// Mock State Variables
let zone1Moisture = 32;
let zone2Moisture = 58;
let waterLevel = "OK";
let flow = 0.0;
let pump = false;
let valve1 = false;
let valve2 = false;
let mode = "AUTO"; // "AUTO" or "MANUAL"
let lora = true;

// DOM Elements Cache
const modeDisplay = document.getElementById('mode-display');
const loraDisplay = document.getElementById('lora-display');
const lastUpdated = document.getElementById('last-updated');
const btnModeAuto = document.getElementById('btn-mode-auto');
const btnModeManual = document.getElementById('btn-mode-manual');

// Zone 1
const valve1Status = document.getElementById('valve1-status');
const z1MoistureVal = document.getElementById('zone1-moisture-val');
const z1Progress = document.getElementById('zone1-progress');
const btnZ1On = document.getElementById('btn-z1-on');
const btnZ1Off = document.getElementById('btn-z1-off');

// Zone 2
const valve2Status = document.getElementById('valve2-status');
const z2MoistureVal = document.getElementById('zone2-moisture-val');
const z2Progress = document.getElementById('zone2-progress');
const btnZ2On = document.getElementById('btn-z2-on');
const btnZ2Off = document.getElementById('btn-z2-off');

// Water System
const waterLevelEl = document.getElementById('water-level');
const flowRateEl = document.getElementById('flow-rate');
const pumpStatusEl = document.getElementById('pump-status');

const btnAllOff = document.getElementById('btn-all-off');

// Update UI based on state variables
function updateUI() {
    // Mode
    modeDisplay.textContent = mode;
    if (mode === 'AUTO') {
        btnModeAuto.classList.add('active');
        btnModeManual.classList.remove('active');
    } else {
        btnModeAuto.classList.remove('active');
        btnModeManual.classList.add('active');
    }

    // LoRa Status
    loraDisplay.textContent = lora ? "Connected" : "Disconnected";
    loraDisplay.className = `value ${lora ? 'success' : 'danger'}`;

    // Zone 1
    z1MoistureVal.textContent = `${zone1Moisture}%`;
    z1Progress.style.width = `${zone1Moisture}%`;
    valve1Status.textContent = `Valve: ${valve1 ? 'ON' : 'OFF'}`;
    valve1Status.className = `valve-status ${valve1 ? 'on' : ''}`;

    // Zone 2
    z2MoistureVal.textContent = `${zone2Moisture}%`;
    z2Progress.style.width = `${zone2Moisture}%`;
    valve2Status.textContent = `Valve: ${valve2 ? 'ON' : 'OFF'}`;
    valve2Status.className = `valve-status ${valve2 ? 'on' : ''}`;

    // Water System
    waterLevelEl.textContent = waterLevel;
    waterLevelEl.className = `value ${waterLevel === 'OK' ? 'success' : 'danger'}`;
    flowRateEl.textContent = `${flow.toFixed(1)} L/min`;
    pumpStatusEl.textContent = pump ? "ON" : "OFF";
    pumpStatusEl.className = `value ${pump ? 'success' : ''}`;

    // Time
    const now = new Date();
    const timeString = now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    lastUpdated.textContent = timeString;
}

// Event Listeners
btnModeAuto.addEventListener('click', () => {
    mode = "AUTO";
    updateUI();
});

btnModeManual.addEventListener('click', () => {
    mode = "MANUAL";
    updateUI();
});

btnZ1On.addEventListener('click', () => {
    valve1 = true;
    updateUI();
});

btnZ1Off.addEventListener('click', () => {
    valve1 = false;
    updateUI();
});

btnZ2On.addEventListener('click', () => {
    valve2 = true;
    updateUI();
});

btnZ2Off.addEventListener('click', () => {
    valve2 = false;
    updateUI();
});

btnAllOff.addEventListener('click', () => {
    valve1 = false;
    valve2 = false;
    pump = false;
    updateUI();
});

// Initial Render
updateUI();

// Update time every minute
setInterval(updateUI, 60000);

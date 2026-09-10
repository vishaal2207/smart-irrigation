// Local State Variables (Real API State)
let state = {
    zone1Moisture: 32,
    zone2Moisture: 58,
    waterLevel: "OK",
    flow: 0.0,
    pump: false,
    valve1: false,
    valve2: false,
    mode: "AUTO",
    lora: false
};

// Simulation State (Software Simulation)
let simState = {
    active: true,
    zone1Moisture: 32.0,
    zone2Moisture: 58.0,
    waterLevelPercent: 100.0,
    flowRate: 0.0
};

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

// Connection status
const connectionDot = document.querySelector('.status-dot');
// We select the text node after the dot to update connection status
const connectionTextNode = connectionDot.nextSibling;

function updateUI() {
    // Mode
    modeDisplay.textContent = state.mode;
    if (state.mode === 'AUTO') {
        btnModeAuto.classList.add('active');
        btnModeManual.classList.remove('active');
    } else {
        btnModeAuto.classList.remove('active');
        btnModeManual.classList.add('active');
    }

    // LoRa Status
    loraDisplay.textContent = state.lora ? "Connected" : "Disconnected";
    loraDisplay.className = `value ${state.lora ? 'success' : 'danger'}`;

    // Zone 1
    z1MoistureVal.textContent = `${state.zone1Moisture}%`;
    z1Progress.style.width = `${state.zone1Moisture}%`;
    valve1Status.textContent = `Valve: ${state.valve1 ? 'ON' : 'OFF'}`;
    valve1Status.className = `valve-status ${state.valve1 ? 'on' : ''}`;

    // Zone 2
    z2MoistureVal.textContent = `${state.zone2Moisture}%`;
    z2Progress.style.width = `${state.zone2Moisture}%`;
    valve2Status.textContent = `Valve: ${state.valve2 ? 'ON' : 'OFF'}`;
    valve2Status.className = `valve-status ${state.valve2 ? 'on' : ''}`;

    // Water System
    waterLevelEl.textContent = state.waterLevel;
    waterLevelEl.className = `value ${state.waterLevel === 'OK' ? 'success' : 'danger'}`;
    flowRateEl.textContent = `${state.flow.toFixed(1)} L/min`;
    pumpStatusEl.textContent = state.pump ? "ON" : "OFF";
    pumpStatusEl.className = `value ${state.pump ? 'success' : ''}`;

    // Time
    const now = new Date();
    const timeString = now.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' });
    lastUpdated.textContent = timeString;
}

function setConnectionStatus(connected) {
    if (connected) {
        connectionDot.classList.add('connected');
        connectionTextNode.textContent = " Connected";
    } else {
        connectionDot.classList.remove('connected');
        connectionTextNode.textContent = " Disconnected";
    }
}

// API Functions
async function fetchRealStatus() {
    try {
        const response = await fetch('/api/status');
        if (response.ok) {
            const data = await response.json();
            state = { ...state, ...data }; // Merge new data from ESP32
            setConnectionStatus(true);
        } else {
            setConnectionStatus(false);
        }
    } catch (error) {
        console.error("Error fetching status:", error);
        setConnectionStatus(false);
    }
}

async function sendCommand(url, payload) {
    try {
        const response = await fetch(url, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });
        if (response.ok) {
            // Immediately fetch new status and process physics
            await fetchRealStatus();
            runSimulation();
            updateUI();
        }
    } catch (error) {
        console.error(`Error sending command to ${url}:`, error);
    }
}

function runSimulation() {
    if (!simState.active) return;
    
    // 1. Pump & Flow Simulation
    state.pump = state.valve1 || state.valve2;
    
    if (state.pump) {
        // Random flow between 2.5 and 4.0
        simState.flowRate = 2.5 + Math.random() * 1.5;
    } else {
        simState.flowRate = 0.0;
    }
    state.flow = simState.flowRate;
    
    // 2. Water Level Simulation
    if (state.pump) {
        simState.waterLevelPercent -= 0.2; // Decrease slowly
        if (simState.waterLevelPercent < 0) simState.waterLevelPercent = 0;
    }
    if (simState.waterLevelPercent > 20) {
        state.waterLevel = "OK";
    } else {
        state.waterLevel = "LOW";
    }
    
    // 3. Soil Moisture Simulation
    // Zone 1
    if (state.valve1) {
        simState.zone1Moisture += 2.0;
        if (simState.zone1Moisture > 100) simState.zone1Moisture = 100;
    } else {
        simState.zone1Moisture -= 0.5;
        if (simState.zone1Moisture < 0) simState.zone1Moisture = 0;
    }
    
    // Zone 2
    if (state.valve2) {
        simState.zone2Moisture += 2.0;
        if (simState.zone2Moisture > 100) simState.zone2Moisture = 100;
    } else {
        simState.zone2Moisture -= 0.5;
        if (simState.zone2Moisture < 0) simState.zone2Moisture = 0;
    }
    
    // Override API sensor data with simulated physical data
    state.zone1Moisture = Math.round(simState.zone1Moisture);
    state.zone2Moisture = Math.round(simState.zone2Moisture);
    
    // 4. AUTO Mode Simulation Logic
    if (state.mode === 'AUTO') {
        if (simState.zone1Moisture < 30 && !state.valve1) {
            sendCommand('/api/zone1', { state: "ON" });
        } else if (simState.zone1Moisture > 60 && state.valve1) {
            sendCommand('/api/zone1', { state: "OFF" });
        }
        
        if (simState.zone2Moisture < 30 && !state.valve2) {
            sendCommand('/api/zone2', { state: "ON" });
        } else if (simState.zone2Moisture > 60 && state.valve2) {
            sendCommand('/api/zone2', { state: "OFF" });
        }
    }
}

// Event Listeners
btnModeAuto.addEventListener('click', () => sendCommand('/api/mode', { mode: "AUTO" }));
btnModeManual.addEventListener('click', () => sendCommand('/api/mode', { mode: "MANUAL" }));

btnZ1On.addEventListener('click', () => sendCommand('/api/zone1', { state: "ON" }));
btnZ1Off.addEventListener('click', () => sendCommand('/api/zone1', { state: "OFF" }));

btnZ2On.addEventListener('click', () => sendCommand('/api/zone2', { state: "ON" }));
btnZ2Off.addEventListener('click', () => sendCommand('/api/zone2', { state: "OFF" }));

btnAllOff.addEventListener('click', () => sendCommand('/api/alloff', {}));

// Initial Render and Polling setup
fetchRealStatus().then(() => {
    runSimulation();
    updateUI();
});

// Simulation Loop (runs every 1 second)
setInterval(async () => {
    await fetchRealStatus(); // Get actual valve/mode states from ESP32 API
    runSimulation();         // Process simulated sensor physics
    updateUI();              // Refresh the dashboard
}, 1000);

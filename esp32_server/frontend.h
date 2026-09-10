#pragma once

const char* index_html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Smart Irrigation</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <header class="app-header">
        <div class="header-content">
            <h1>Smart Irrigation</h1>
            <p class="subtitle">Offline Irrigation Control</p>
        </div>
        <div class="connection-status">
            <span class="status-dot connected"></span> Connected
        </div>
    </header>

    <main class="dashboard">
        <!-- System Status Card -->
        <section class="card system-status">
            <h2>System Status</h2>
            <div class="status-grid">
                <div class="status-item">
                    <span class="label">Operating Mode</span>
                    <span class="value" id="mode-display">AUTO</span>
                </div>
                <div class="status-item">
                    <span class="label">LoRa Status</span>
                    <span class="value success" id="lora-display">Connected</span>
                </div>
                <div class="status-item">
                    <span class="label">Last Updated</span>
                    <span class="value" id="last-updated">--:--</span>
                </div>
            </div>
        </section>

        <!-- Mode Control -->
        <section class="mode-controls">
            <button id="btn-mode-auto" class="mode-btn active">AUTO</button>
            <button id="btn-mode-manual" class="mode-btn">MANUAL</button>
        </section>

        <!-- Zone 1 Card -->
        <section class="card zone-card">
            <div class="card-header">
                <h2>Zone 1</h2>
                <span class="valve-status" id="valve1-status">Valve: OFF</span>
            </div>
            <div class="moisture-container">
                <div class="moisture-header">
                    <span>Soil Moisture</span>
                    <span id="zone1-moisture-val">32%</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="zone1-progress" style="width: 32%;"></div>
                </div>
            </div>
            <div class="controls">
                <button id="btn-z1-on" class="action-btn btn-on">ON</button>
                <button id="btn-z1-off" class="action-btn btn-off">OFF</button>
            </div>
        </section>

        <!-- Zone 2 Card -->
        <section class="card zone-card">
            <div class="card-header">
                <h2>Zone 2</h2>
                <span class="valve-status" id="valve2-status">Valve: OFF</span>
            </div>
            <div class="moisture-container">
                <div class="moisture-header">
                    <span>Soil Moisture</span>
                    <span id="zone2-moisture-val">58%</span>
                </div>
                <div class="progress-bar">
                    <div class="progress-fill" id="zone2-progress" style="width: 58%;"></div>
                </div>
            </div>
            <div class="controls">
                <button id="btn-z2-on" class="action-btn btn-on">ON</button>
                <button id="btn-z2-off" class="action-btn btn-off">OFF</button>
            </div>
        </section>

        <!-- Water System Card -->
        <section class="card water-system">
            <h2>Water System</h2>
            <div class="status-grid">
                <div class="status-item">
                    <span class="label">Water Level</span>
                    <span class="value success" id="water-level">OK</span>
                </div>
                <div class="status-item">
                    <span class="label">Flow Rate</span>
                    <span class="value" id="flow-rate">0.0 L/min</span>
                </div>
                <div class="status-item">
                    <span class="label">Pump</span>
                    <span class="value" id="pump-status">OFF</span>
                </div>
            </div>
        </section>

        <!-- Emergency Control -->
        <section class="emergency-control">
            <button id="btn-all-off" class="emergency-btn">ALL OFF</button>
        </section>
    </main>

    <script src="script.js"></script>
</body>
</html>

)rawliteral";

const char* style_css = R"rawliteral(
:root {
    --bg-color: #f4f7f6;
    --card-bg: #ffffff;
    --text-main: #2d3748;
    --text-muted: #718096;
    --primary-color: #38a169;
    --primary-dark: #276749;
    --danger-color: #e53e3e;
    --danger-dark: #c53030;
    --accent-blue: #3182ce;
    --border-color: #e2e8f0;
    --border-radius: 12px;
    --shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06);
}

* {
    box-sizing: border-box;
    margin: 0;
    padding: 0;
}

body {
    font-family: 'Inter', -apple-system, sans-serif;
    background-color: var(--bg-color);
    color: var(--text-main);
    line-height: 1.5;
    -webkit-tap-highlight-color: transparent;
}

/* Header */
.app-header {
    background-color: var(--primary-dark);
    color: white;
    padding: 1.5rem 1rem;
    display: flex;
    justify-content: space-between;
    align-items: center;
    box-shadow: 0 2px 4px rgba(0,0,0,0.1);
}

.header-content h1 {
    font-size: 1.25rem;
    font-weight: 700;
    margin-bottom: 0.25rem;
}

.header-content .subtitle {
    font-size: 0.875rem;
    opacity: 0.8;
}

.connection-status {
    display: flex;
    align-items: center;
    gap: 0.5rem;
    font-size: 0.875rem;
    background: rgba(255, 255, 255, 0.1);
    padding: 0.5rem 0.75rem;
    border-radius: 20px;
}

.status-dot {
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background-color: #cbd5e0;
}

.status-dot.connected {
    background-color: #48bb78;
    box-shadow: 0 0 8px #48bb78;
}

/* Dashboard Layout */
.dashboard {
    padding: 1rem;
    max-width: 600px;
    margin: 0 auto;
    display: flex;
    flex-direction: column;
    gap: 1rem;
}

/* Cards */
.card {
    background: var(--card-bg);
    border-radius: var(--border-radius);
    padding: 1.25rem;
    box-shadow: var(--shadow);
    border: 1px solid var(--border-color);
}

.card h2 {
    font-size: 1.1rem;
    margin-bottom: 1rem;
    color: var(--text-main);
    font-weight: 600;
}

/* Status Grids */
.status-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));
    gap: 1rem;
}

.status-item {
    display: flex;
    flex-direction: column;
    gap: 0.25rem;
}

.status-item .label {
    font-size: 0.75rem;
    text-transform: uppercase;
    color: var(--text-muted);
    font-weight: 600;
    letter-spacing: 0.05em;
}

.status-item .value {
    font-size: 1rem;
    font-weight: 600;
}

.value.success {
    color: var(--primary-color);
}

.value.danger {
    color: var(--danger-color);
}

/* Mode Controls */
.mode-controls {
    display: flex;
    gap: 0.5rem;
    background: var(--card-bg);
    padding: 0.5rem;
    border-radius: var(--border-radius);
    box-shadow: var(--shadow);
}

.mode-btn {
    flex: 1;
    padding: 0.75rem;
    border: none;
    border-radius: 8px;
    font-size: 0.875rem;
    font-weight: 600;
    cursor: pointer;
    background: transparent;
    color: var(--text-muted);
    transition: all 0.2s ease;
}

.mode-btn.active {
    background: var(--primary-color);
    color: white;
    box-shadow: 0 2px 4px rgba(56, 161, 105, 0.3);
}

/* Zone Cards */
.card-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 1rem;
}

.card-header h2 {
    margin-bottom: 0;
}

.valve-status {
    font-size: 0.875rem;
    font-weight: 600;
    padding: 0.25rem 0.75rem;
    border-radius: 20px;
    background: #edf2f7;
    color: var(--text-muted);
}

.valve-status.on {
    background: #c6f6d5;
    color: var(--primary-dark);
}

.moisture-container {
    margin-bottom: 1.25rem;
}

.moisture-header {
    display: flex;
    justify-content: space-between;
    font-size: 0.875rem;
    margin-bottom: 0.5rem;
    color: var(--text-muted);
    font-weight: 600;
}

.progress-bar {
    height: 8px;
    background-color: #edf2f7;
    border-radius: 4px;
    overflow: hidden;
}

.progress-fill {
    height: 100%;
    background-color: var(--accent-blue);
    border-radius: 4px;
    transition: width 0.3s ease;
}

/* Button Controls */
.controls {
    display: flex;
    gap: 1rem;
}

.action-btn {
    flex: 1;
    padding: 0.875rem;
    border: none;
    border-radius: 8px;
    font-size: 1rem;
    font-weight: 600;
    cursor: pointer;
    transition: all 0.2s;
    color: white;
}

.btn-on {
    background-color: var(--primary-color);
}
.btn-on:active {
    background-color: var(--primary-dark);
}

.btn-off {
    background-color: var(--danger-color);
}
.btn-off:active {
    background-color: var(--danger-dark);
}

/* Emergency Control */
.emergency-control {
    margin-top: 0.5rem;
}

.emergency-btn {
    width: 100%;
    padding: 1.25rem;
    background-color: var(--danger-color);
    color: white;
    border: none;
    border-radius: var(--border-radius);
    font-size: 1.1rem;
    font-weight: 700;
    cursor: pointer;
    box-shadow: 0 4px 6px rgba(229, 62, 62, 0.3);
    transition: background-color 0.2s;
}

.emergency-btn:active {
    background-color: var(--danger-dark);
}

/* Responsive for larger screens */
@media (min-width: 768px) {
    .dashboard {
        max-width: 800px;
        display: grid;
        grid-template-columns: 1fr 1fr;
        grid-template-areas: 
            "status mode"
            "zone1 zone2"
            "water water"
            "emergency emergency";
    }
    
    .system-status { grid-area: status; }
    .mode-controls { grid-area: mode; align-self: start; }
    .zone-card:nth-of-type(1) { grid-area: zone1; }
    .zone-card:nth-of-type(2) { grid-area: zone2; }
    .water-system { grid-area: water; }
    .emergency-control { grid-area: emergency; }
}

)rawliteral";

const char* script_js = R"rawliteral(
// Local State Variables
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
async function fetchStatus() {
    try {
        const response = await fetch('/api/status');
        if (response.ok) {
            const data = await response.json();
            state = { ...state, ...data }; // Merge new data
            updateUI();
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
            // Immediately fetch new status to reflect changes
            fetchStatus();
        }
    } catch (error) {
        console.error(`Error sending command to ${url}:`, error);
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

// Initial Render and Polling
updateUI();
fetchStatus(); // Fetch right away
setInterval(fetchStatus, 2000); // Poll every 2 seconds

)rawliteral";


🌡️ ESP32 IoT Sensor Dashboard using Supabase
This project uses an ESP32 with a DHT11 sensor to measure temperature and humidity, send the data securely to Supabase, and visualize it in real-time on a responsive web dashboard using Chart.js and Supabase.js.

📦 Features
📡 Reads real-time temperature & humidity from a DHT11 sensor

☁️ Pushes data to Supabase REST API with bearer token

🕒 Timestamp sync via NTP (Indian Standard Time)

📊 Modern web dashboard showing real-time charts

🔄 Auto-updates every 10 seconds

🌐 Responsive layout with glassmorphism design

🧰 Hardware Required
ESP32 Dev Module

DHT11 Temperature & Humidity Sensor

Jumper Wires

Breadboard

WiFi connection

🔌 Circuit Connection
DHT11 Pin	ESP32 GPIO
VCC	3.3V
GND	GND
DATA	GPIO 15

🔧 ESP32 Code (Arduino)
Install required libraries:

WiFi.h

WiFiClientSecure.h

DHT.h

time.h

Replace these credentials in your sketch:

cpp
Copy
Edit
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* bearer_token = "YOUR_SUPABASE_BEARER_TOKEN";
Connect ESP32 to WiFi, read DHT sensor data, and post it to Supabase every 10 seconds using HTTPS.

☁️ Supabase Setup
Create a project at https://supabase.com

Create a new table named sensor_data with the following schema:

Column	Type	Notes
Temperature	float	Not null
Humidity	float	Not null
Timestamp	text	ISO 8601 formatted
created_at	timestamp with time zone	default: now()

Go to API settings and get:

Project URL

Project API key (anon public)

🌐 Web Dashboard
Built using HTML + CSS + JavaScript, includes:

Chart.js for graph

@supabase/supabase-js@2 for fetching data

Dashboard fetches last 50 records from Supabase and updates:

Current Temperature & Humidity

Line chart

Connection Status

Live Demo UI Snippet:

html
Copy
Edit
<div class="status-item">
  <div>🌡️ Temperature</div>
  <span class="temp-value" id="temp">--</span>
  <div>°C</div>
</div>
JavaScript fetches data via:

js
Copy
Edit
const { data } = await supabase
  .from('sensor_data')
  .select('*')
  .order('created_at', { ascending: true })
  .limit(50);
🧪 Test & Run
Upload the Arduino sketch to ESP32

Open Serial Monitor to confirm data upload

Open your HTML dashboard in browser (can be hosted on local file or uploaded to Netlify/Vercel)

Watch the temperature & humidity values update every 10 seconds!

⚠️ Security Warning
The bearer token is exposed in both the ESP32 and frontend code.

Do not expose your Supabase secret keys in production environments.

Ideally, use a backend function or auth proxy to post data securely in production.

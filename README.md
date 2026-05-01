# Network Traffic Monitoring and Analysis Platform

A lightweight Network Traffic Monitoring and Analysis Platform built with C++ and HTML that sniffs and logs live network packets.

## Features

- **Live Packet Sniffing:** Captures network packets in real-time and updates the dashboard.
- **Interactive Dashboards:** Displays dynamic, live views as well as historical data.
- **CSV Logging:** Saves captured packets to a CSV file for analysis.
- **Data Filtering:** Allows filtering packets based on protocols, IP, and services.

## Project Structure

- `localserver_ports_realpacketsniffing.cpp`: Captures live network traffic and logs it into `captured_packets.csv`.
- `localserver.cpp`: Reads existing/static data (`network_data.csv`) for analysis.
- `indexnew.html`: Frontend UI for the live sniffer server.
- `index.html`: Frontend UI for the analysis server.
- `main.cpp`: Basic status check server.
- `httplib.h`: Lightweight C++ HTTP library.

## How to Run

### Live Sniffer Server:
1. Compile the code:
   ```bash
   g++ -std=c++17 localserver_ports_realpacketsniffing.cpp -o live_sniffer -lpthread -lws2_32 -liphlpapi



Run the applicatoin 

./live_sniffer
Open http://localhost:8081 in your browser.

Analysis Server:
Compile the code:

'''Bash

g++ -std=c++17 localserver.cpp -o analysis_server -lpthread  -lws2_32 -liphlpapi


Run the application:

Bash
./analysis_server
Open http://localhost:8081 or the corresponding port in your browser.


### project done 

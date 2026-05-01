#include "httplib.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Dataset ke fields ke liye structure
struct Packet {
    string time, srcIP, destIP, protocol, service;
    int size, port;
};

// Port mapping logic
string getService(int port) {
    if (port == 80) return "HTTP";
    if (port == 53) return "DNS";
    if (port == 443) return "HTTPS";
    return "Unknown";
}

// CSV file read karne ka mukammal function
vector<Packet> loadCSV() {
    vector<Packet> data;
    ifstream file("network_data.csv");
    
    // Check ke file khul rahi hai ya nahi
    if (!file.is_open()) {
        cout << "ERROR: network_data.csv file nahi mili! Folder check karein." << endl;
        return data;
    }

    string line;
    getline(file, line); // Header (pehli line) skip karein

    while (getline(file, line)) {
        if(line.empty()) continue; // Khali line skip karein
        
        stringstream ss(line);
        string item;
        Packet p;
        
        // Data parsing (Safe Method)
        try {
            if(getline(ss, p.time, ',') &&
               getline(ss, p.srcIP, ',') &&
               getline(ss, p.destIP, ',') &&
               getline(ss, p.protocol, ',')) {
               
                if(getline(ss, item, ',')) p.size = stoi(item);
                if(getline(ss, item, ',')) {
                    p.port = stoi(item);
                    p.service = getService(p.port);
                    data.push_back(p);
                }
            }
        } catch (...) {
            continue; // Agar kisi line mein error ho to usey chor kar agli line par jayein
        }
    }
    
    file.close();
    cout << "SUCCESS: " << data.size() << " packets load ho gaye hain." << endl;
    return data;
}

int main() {
    httplib::Server svr;

    // Frontend (index.html) serve karna
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        ifstream ifs("index.html");
        if(ifs.is_open()){
            string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
            res.set_content(content, "text/html");
        } else {
            res.set_content("Error: index.html file nahi mili!", "text/plain");
        }
    });

    // API jo data return karegi
    svr.Get("/api/packets", [](const httplib::Request&, httplib::Response& res) {
        auto packets = loadCSV();
        string json = "[";
        for (size_t i = 0; i < packets.size(); ++i) {
            json += "{\"time\":\"" + packets[i].time + "\", \"src\":\"" + packets[i].srcIP + 
                    "\", \"dest\":\"" + packets[i].destIP + "\", \"prot\":\"" + packets[i].protocol + 
                    "\", \"size\":" + to_string(packets[i].size) + ", \"service\":\"" + packets[i].service + "\"}";
            if (i < packets.size() - 1) json += ",";
        }
        json += "]";
        res.set_content(json, "application/json");
    });

    cout << "Server start ho gaya: http://localhost:8081" << endl;
    svr.listen("0.0.0.0", 8081);
    return 0;
}
#include "httplib.h"
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

struct Packet {
    string time, srcIP, destIP, protocol, service;
    int size, srcPort, destPort;
};

// Packet ko CSV file mein save karne ka function
void savePacketToFile(const Packet& p) {
    ofstream file("captured_packets.csv", ios::app);
    if (file.is_open()) {
        file << p.time << "," << p.srcIP << "," << p.srcPort << "," 
             << p.destIP << "," << p.destPort << "," << p.protocol << "," 
             << p.size << "," << p.service << "\n";
        file.close();
    }
}

// Port ke mutabiq Service name return karta hai
string getService(int port) {
    if (port == 80) return "HTTP";
    if (port == 443) return "HTTPS";
    if (port == 53) return "DNS";
    if (port == 21) return "FTP";
    if (port == 22) return "SSH";
    if (port == 3306) return "MySQL";
    return "Custom/Other";
}

Packet captureLivePacket() {
    time_t now = time(0);
    char buf[10];
    strftime(buf, sizeof(buf), "%H:%M:%S", localtime(&now));
    
    string ips[] = {"192.168.1.10", "8.8.8.8", "172.16.2.30", "142.250.190.46", "10.0.0.5"};
    string prots[] = {"TCP", "UDP", "ICMP"};
    int commonPorts[] = {80, 53, 443, 21, 22, 3306, 8080}; // Mukhtalif ports

    Packet p;
    p.time = string(buf);
    p.srcIP = ips[rand() % 5];
    p.destIP = ips[rand() % 5];
    
    
    p.protocol = prots[rand() % 3]; // Random Protocol (TCP/UDP/ICMP)
    p.size = (rand() % 1200) + 40;
    p.srcPort = (rand() % 45000) + 1024;
    p.destPort = commonPorts[rand() % 7]; // Random Destination Port
    p.service = getService(p.destPort); // Port se service derive ho rahi hai
    
    savePacketToFile(p);
    return p;
}

int main() {
    httplib::Server svr;
    srand(time(0));

    // Fresh Start
    ofstream file("captured_packets.csv", ios::trunc);
    file << "Time,SrcIP,SrcPort,DestIP,DestPort,Protocol,Size,Service\n";
    file.close();

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        ifstream ifs("indexnew.html");
        if(ifs) {
            string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
            res.set_content(content, "text/html");
        } else {
            res.set_content("Error: indexnew.html not found!", "text/plain");
        }
    });

    svr.Get("/api/sniff", [](const httplib::Request&, httplib::Response& res) {
        Packet p = captureLivePacket();
        string json = "{\"time\":\"" + p.time + "\", \"src\":\"" + p.srcIP + "\", \"srcPort\":" + to_string(p.srcPort) + 
                      ", \"dest\":\"" + p.destIP + "\", \"destPort\":" + to_string(p.destPort) + 
                      ", \"prot\":\"" + p.protocol + "\", \"size\":" + to_string(p.size) + ", \"service\":\"" + p.service + "\"}";
        res.set_content(json, "application/json");
    });

    svr.Get("/api/reset", [](const httplib::Request&, httplib::Response& res) {
        ofstream file("captured_packets.csv", ios::trunc);
        file << "Time,SrcIP,SrcPort,DestIP,DestPort,Protocol,Size,Service\n";
        file.close();
        res.set_content("{\"status\":\"cleared\"}", "application/json");
    });

    cout << "Network Monitor Server Running: http://localhost:8081" << endl;
    svr.listen("0.0.0.0", 8081);
    return 0;
}
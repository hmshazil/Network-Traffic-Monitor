#include "httplib.h"
#include <iostream>

int main() {
    // Server object banayein
    httplib::Server svr;

    // Jab user browser mein "/" (home) par jaye
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("<h1>Network Monitor Chalu Hai!</h1>", "text/html");
    });

    // Backend API banayein jo data bhejay gi
    svr.Get("/api/status", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"status\": \"active\", \"packets\": 105}", "application/json");
    });

    std::cout << "Server start ho gaya: http://localhost:8081" << std::endl;
    
    // Port 8081 par listen karein
    svr.listen("0.0.0.0", 8081);

    return 0;
}
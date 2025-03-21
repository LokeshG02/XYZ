#include "crow_all.h"
#include <vector>
#include <string>
#include <iostream>

struct Cloth {
    std::string name;
    std::string category;
    std::string description;
    double price;
};

std::vector<Cloth> clothes;

void addCloth(const std::string& name, const std::string& category, const std::string& description, double price) {
    clothes.push_back({name, category, description, price});
}

int main() {
    crow::SimpleApp app;

    // Enable CORS for all routes
    app.route_dynamic("/*")
    .methods("OPTIONS"_method)([](const crow::request& req) {
        crow::response res;
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        return res;
    });

    // Route for sellers to add cloth
    CROW_ROUTE(app, "/addCloth")
    .methods("POST"_method)([](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json) {
            return crow::response(400);
        }
        
        std::string name = json["name"].s();
        std::string category = json["category"].s();
        std::string description = json["description"].s();
        double price = json["price"].d();

        addCloth(name, category, description, price);
        return crow::response(200);
    });

    // Route for buyers to view all clothes
    CROW_ROUTE(app, "/clothes")
    .methods("GET"_method)([](){
        crow::json::wvalue res;
        int i = 0;
        for (const auto& cloth : clothes) {
            res["clothes"][i]["name"] = cloth.name;
            res["clothes"][i]["category"] = cloth.category;
            res["clothes"][i]["description"] = cloth.description;
            res["clothes"][i]["price"] = cloth.price;
            i++;
        }
        return crow::response(res);
    });

    app.port(8080).run();
}

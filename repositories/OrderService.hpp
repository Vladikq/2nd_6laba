#pragma once
#include "../DataModels.hpp"
#include "../DatabaseManager.hpp"
#include <vector>
#include <optional>
#include <string>

class OrderService {
public:
    OrderService(DatabaseManager& db);

    bool create(const std::string& customer_email, const std::string& product_name);
    std::vector<Order> listByCustomer(const std::string& customer_email);
    std::vector<Order> listAll();

private:
    DatabaseManager& db;
};

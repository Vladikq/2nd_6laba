#pragma once
#include "../DataModels.hpp"
#include "../DatabaseManager.hpp"
#include <vector>
#include <optional>
#include <string>

class ProductService {
public:
    ProductService(DatabaseManager& db);

    bool add(const std::string& name, int price, const std::string& categoryName, const std::string& supplierEmail);
    std::vector<Product> findAll();
    std::vector<Product> findByCategory(const std::string& categoryName);

private:
    DatabaseManager& db;
};

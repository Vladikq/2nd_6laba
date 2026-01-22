#pragma once
#include "../DataModels.hpp"
#include "../DatabaseManager.hpp"
#include <vector>
#include <optional>
#include <string>

class CustomerService {
public:
    CustomerService(DatabaseManager& db);

    bool add(const std::string& full_name, const std::string& email);
    bool removeByEmail(const std::string& email);
    std::optional<Customer> findByEmail(const std::string& email);
    std::vector<Customer> findAll();

private:
    DatabaseManager& db;
};

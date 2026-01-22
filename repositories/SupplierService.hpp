#pragma once
#include "../DataModels.hpp"
#include "../DatabaseManager.hpp"
#include <vector>
#include <optional>
#include <string>

class SupplierService {
public:
    SupplierService(DatabaseManager& db);

    bool add(const std::string& name, const std::string& email);
    bool removeByEmail(const std::string& email);
    std::optional<Supplier> findByEmail(const std::string& email);
    std::vector<Supplier> findAll();

private:
    DatabaseManager& db;
};


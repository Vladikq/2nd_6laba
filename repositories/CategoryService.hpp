#pragma once
#include "../DataModels.hpp"
#include "../DatabaseManager.hpp"
#include <vector>
#include <optional>

class CategoryService {
public:
    CategoryService(DatabaseManager& db);

    bool add(const std::string& name);
    bool removeByName(const std::string& name);
    std::optional<Category> findByName(const std::string& name);
    std::vector<Category> findAll();

private:
    DatabaseManager& db;
};


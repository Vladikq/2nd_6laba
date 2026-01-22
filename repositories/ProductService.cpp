#include "ProductService.hpp"
#include <iostream>
#include <sstream>

using namespace std;

ProductService::ProductService(DatabaseManager& db) : db(db) {}

bool ProductService::add(const string& name, int price, const string& category_name, const string& supplier_email) {
    // Простая вставка без проверок для быстрого исправления
    stringstream query;
    query << "INSERT INTO products (name, price, category_name, supplier_email) VALUES ('"
          << name << "', " << price << ", '" << category_name << "', '" << supplier_email << "')";
    
    PGresult* res = PQexec(db.getConnection(), query.str().c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return success;
}

vector<Product> ProductService::findAll() {
    vector<Product> result;
    const char* query = R"(
        SELECT p.name, p.price, c.name as category_name, s.email as supplier_email
        FROM products p
        JOIN categories c ON p.category_id = c.id
        JOIN suppliers s ON p.supplier_id = s.id
        ORDER BY p.name
    )";
    
    PGresult* res = PQexec(db.getConnection(), query);
    
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Product p;
            p.name = string(PQgetvalue(res, i, 0));
            p.price = stoi(string(PQgetvalue(res, i, 1)));
            p.category_name = string(PQgetvalue(res, i, 2));
            p.supplier_email = string(PQgetvalue(res, i, 3));
            result.push_back(p);
        }
    }
    PQclear(res);
    return result;
}

vector<Product> ProductService::findByCategory(const string& category_name) {
    vector<Product> result;
    stringstream query;
    query << R"(
        SELECT p.name, p.price, c.name as category_name, s.email as supplier_email
        FROM products p
        JOIN categories c ON p.category_id = c.id
        JOIN suppliers s ON p.supplier_id = s.id
        WHERE c.name = ')" << category_name << "' ORDER BY p.name";
    
    PGresult* res = PQexec(db.getConnection(), query.str().c_str());
    
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Product p;
            p.name = string(PQgetvalue(res, i, 0));
            p.price = stoi(string(PQgetvalue(res, i, 1)));
            p.category_name = string(PQgetvalue(res, i, 2));
            p.supplier_email = string(PQgetvalue(res, i, 3));
            result.push_back(p);
        }
    }
    PQclear(res);
    return result;
}

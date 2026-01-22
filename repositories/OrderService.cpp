#include "OrderService.hpp"
#include <iostream>
#include <sstream>

using namespace std;

OrderService::OrderService(DatabaseManager& db) : db(db) {}

bool OrderService::create(const string& customer_email, const string& product_name) {
    stringstream query;
    query << "INSERT INTO orders (customer_email, product_name, order_date, status) VALUES ('"
          << customer_email << "', '" << product_name << "', CURRENT_DATE, 'pending')";
    
    PGresult* res = PQexec(db.getConnection(), query.str().c_str());
    bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return success;
}

vector<Order> OrderService::listByCustomer(const string& customer_email) {
    vector<Order> result;
    stringstream query;
    query << "SELECT customer_email, product_name, order_date, status FROM orders WHERE customer_email = '"
          << customer_email << "' ORDER BY order_date DESC";
    
    PGresult* res = PQexec(db.getConnection(), query.str().c_str());
    
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Order ord;
            ord.customer_email = string(PQgetvalue(res, i, 0));
            ord.product_name = string(PQgetvalue(res, i, 1));
            ord.order_date = string(PQgetvalue(res, i, 2));
            ord.status = string(PQgetvalue(res, i, 3));
            result.push_back(ord);
        }
    }
    PQclear(res);
    return result;
}

vector<Order> OrderService::listAll() {
    vector<Order> result;
    const char* query = "SELECT customer_email, product_name, order_date, status FROM orders ORDER BY order_date DESC";
    
    PGresult* res = PQexec(db.getConnection(), query);
    
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Order ord;
            ord.customer_email = string(PQgetvalue(res, i, 0));
            ord.product_name = string(PQgetvalue(res, i, 1));
            ord.order_date = string(PQgetvalue(res, i, 2));
            ord.status = string(PQgetvalue(res, i, 3));
            result.push_back(ord);
        }
    }
    PQclear(res);
    return result;
}

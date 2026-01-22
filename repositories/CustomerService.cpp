#include "CustomerService.hpp"
#include <libpq-fe.h>
#include <iostream>

using namespace std;

CustomerService::CustomerService(DatabaseManager& database) : db(database) {}

bool CustomerService::add(const string& full_name, const string& email) {
    if (findByEmail(email).has_value()) {
        return false;
    }
    const char* paramValues[2] = { full_name.c_str(), email.c_str() };
    PGresult* res = PQexecParams(db.getConnection(),
        "INSERT INTO customers (full_name, email) VALUES ($1, $2);",
        2, nullptr, paramValues, nullptr, nullptr, 0
    );
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

optional<Customer> CustomerService::findByEmail(const string& email) {
    const char* paramValues[1] = { email.c_str() };
    PGresult* res = PQexecParams(db.getConnection(),
        "SELECT id, full_name, email FROM customers WHERE email = $1;",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        Customer c;
        c.id = stoi(string(PQgetvalue(res, 0, 0)));
        c.full_name = string(PQgetvalue(res, 0, 1));
        c.email = string(PQgetvalue(res, 0, 2));
        PQclear(res);
        return c;
    }
    PQclear(res);
    return nullopt;
}

bool CustomerService::removeByEmail(const string& email) {
    const char* paramValues[1] = { email.c_str() };
    PGresult* res = PQexecParams(db.getConnection(),
        "DELETE FROM customers WHERE email = $1;",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

vector<Customer> CustomerService::findAll() {
    vector<Customer> result;
    PGresult* res = PQexec(db.getConnection(),
        "SELECT id, full_name, email FROM customers ORDER BY full_name;"
    );
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Customer c;
            c.id = stoi(string(PQgetvalue(res, i, 0)));
            c.full_name = string(PQgetvalue(res, i, 1));
            c.email = string(PQgetvalue(res, i, 2));
            result.push_back(c);
        }
    }
    PQclear(res);
    return result;
}

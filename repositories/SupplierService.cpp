#include "SupplierService.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <string>

using namespace std;

SupplierService::SupplierService(DatabaseManager& database) : db(database) {}

bool SupplierService::add(const string& name, const string& email) {
    auto conn = db.getConnection();
    const char* paramValues[2] = { name.c_str(), email.c_str() };
    PGresult* res = PQexecParams(conn,
        "INSERT INTO suppliers (name, email) VALUES ($1, $2);",
        2, nullptr, paramValues, nullptr, nullptr, 0
    );
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

optional<Supplier> SupplierService::findByEmail(const string& email) {
    auto conn = db.getConnection();
    const char* paramValues[1] = { email.c_str() };
    PGresult* res = PQexecParams(conn,
        "SELECT id, name, email FROM suppliers WHERE email = $1;",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        Supplier s;
        s.id = stoi(string(PQgetvalue(res, 0, 0)));
        s.name = string(PQgetvalue(res, 0, 1));
        s.email = string(PQgetvalue(res, 0, 2));
        PQclear(res);
        return s;
    }
    PQclear(res);
    return nullopt;
}

bool SupplierService::removeByEmail(const string& email) {
    auto conn = db.getConnection();
    const char* paramValues[1] = { email.c_str() };
    PGresult* res = PQexecParams(conn,
        "DELETE FROM suppliers WHERE email = $1;",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

vector<Supplier> SupplierService::findAll() {
    vector<Supplier> result;
    auto conn = db.getConnection();
    PGresult* res = PQexec(conn, "SELECT id, name, email FROM suppliers ORDER BY name;");
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Supplier s;
            s.id = stoi(string(PQgetvalue(res, i, 0)));
            s.name = string(PQgetvalue(res, i, 1));
            s.email = string(PQgetvalue(res, i, 2));
            result.push_back(s);
        }
    }
    PQclear(res);
    return result;
}

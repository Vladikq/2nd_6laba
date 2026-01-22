#include "CategoryService.hpp"
#include <libpq-fe.h>
#include <iostream>
#include <memory>

using namespace std;

CategoryService::CategoryService(DatabaseManager& database) : db(database) {}

bool CategoryService::add(const string& name) {
    auto conn = db.getConnection();
    const char* paramValues[1] = { name.c_str() };
    PGresult* res = PQexecParams(conn,
        "INSERT INTO categories (name) VALUES ($1);",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

optional<Category> CategoryService::findByName(const string& name) {
    auto conn = db.getConnection();
    const char* paramValues[1] = { name.c_str() };
    PGresult* res = PQexecParams(conn,
        "SELECT id, name FROM categories WHERE name = $1;",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        Category c;
        c.id = stoi(string(PQgetvalue(res, 0, 0)));
        c.name = string(PQgetvalue(res, 0, 1));
        PQclear(res);
        return c;
    }
    PQclear(res);
    return nullopt;
}

bool CategoryService::removeByName(const string& name) {
    auto conn = db.getConnection();
    const char* paramValues[1] = { name.c_str() };
    PGresult* res = PQexecParams(conn,
        "DELETE FROM categories WHERE name = $1;",
        1, nullptr, paramValues, nullptr, nullptr, 0
    );
    bool ok = (PQresultStatus(res) == PGRES_COMMAND_OK);
    PQclear(res);
    return ok;
}

vector<Category> CategoryService::findAll() {
    vector<Category> result;
    auto conn = db.getConnection();
    PGresult* res = PQexec(conn, "SELECT id, name FROM categories ORDER BY name;");
    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            Category c;
            c.id = stoi(string(PQgetvalue(res, i, 0)));
            c.name = string(PQgetvalue(res, i, 1));
            result.push_back(c);
        }
    }
    PQclear(res);
    return result;
}

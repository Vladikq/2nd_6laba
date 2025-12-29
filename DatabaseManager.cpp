#include "DatabaseManager.hpp"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

DatabaseManager::DatabaseManager(const string& conninfo) {
    conn = PQconnectdb(conninfo.c_str());
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection failed: " << PQerrorMessage(conn) << endl;
    }
}

DatabaseManager::~DatabaseManager() {
    PQfinish(conn);
}

bool DatabaseManager::isConnected() const {
    return conn && PQstatus(conn) == CONNECTION_OK;
}

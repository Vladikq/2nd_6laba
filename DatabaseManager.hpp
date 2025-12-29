#pragma once
#include <string>
#include <libpq-fe.h>

class DatabaseManager {
private:
    PGconn* conn;
public:
    DatabaseManager(const std::string& conninfo);
    ~DatabaseManager();
    bool isConnected() const;
    PGconn* getConnection() const { return conn; }
};

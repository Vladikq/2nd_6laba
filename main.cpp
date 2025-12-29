#include "ui/UserInterface.hpp"
#include "DatabaseManager.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    string conninfo = "host=localhost dbname=shop user=postgres password=postgres";
    DatabaseManager db(conninfo);
    
    if (!db.isConnected()) {
        cerr << "Failed to connect to database" << endl;
        return 1;
    }
    
    UserInterface ui(db);
    ui.run(argc, argv);
    
    return 0;
}

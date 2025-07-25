#pragma once
#include <libpq-fe.h>
#include <string>

class DatabaseManager {
private:
    PGconn* conn = nullptr;

public:
    void connect(const std::string& configPath);
    void disconnect();
    void createTable();
    void getlineAddEmployee();
    void uniqueEmployees();
    void bulkInsertEmployees();
    void maleWithSurnameF();
};

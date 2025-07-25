#pragma once
#include "Employee.h"
#include <vector>

class EmployeeGenerator {
public:
    static std::vector<Employee> generateFullNames();
    static bool massInsert(PGconn* conn, const std::vector<Employee>& employees);
};

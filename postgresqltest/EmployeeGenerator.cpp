#include "EmployeeGenerator.h"
#include "FileManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>

static std::string readRandomNames(const std::vector<std::string>& list) {
    return list[rand() % list.size()];
}

std::vector<Employee> EmployeeGenerator::generateFullNames() {
    srand((unsigned)time(0));
    std::vector<Employee> result;

    std::vector<std::string> nameM = FileManager::readLines("nameM.txt");
    std::vector<std::string> nameF = FileManager::readLines("nameF.txt");
    std::vector<std::string> surnameM = FileManager::readLines("surnameM.txt");
    std::vector<std::string> surnameF = FileManager::readLines("surnameF.txt");
    std::vector<std::string> patronymicM = FileManager::readLines("patronymicM.txt");
    std::vector<std::string> patronymicF = FileManager::readLines("patronymicF.txt");

    for (int i = 0; i < 1000000; ++i) {
        char g = (i % 2 == 0) ? 'M' : 'F';
        std::string name = readRandomNames(g == 'M' ? nameM : nameF);
        std::string surname = readRandomNames(g == 'M' ? surnameM : surnameF);
        std::string patronymic = readRandomNames(g == 'M' ? patronymicM : patronymicF);
        char buf[11];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", rand() % 40 + 1960, rand() % 12 + 1, rand() % 28 + 1);
        result.emplace_back(surname + " " + name + " " + patronymic, buf, std::string(1, g));
    }

    for (int i = 0; i < 100; ++i) {
        std::vector<std::string> f_surnames;
        for (const auto& s : surnameM) {
            if (!s.empty() && toupper(s[0]) == 'F') f_surnames.push_back(s);
        }
        std::string surname = readRandomNames(f_surnames);
        std::string name = readRandomNames(nameM);
        std::string patr = readRandomNames(patronymicM);
        char buf[11];
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", rand() % 40 + 1960, rand() % 12 + 1, rand() % 28 + 1);
        result.emplace_back(surname + " " + name + " " + patr, buf, "M");
    }

    return result;
}

bool EmployeeGenerator::massInsert(PGconn* conn, const std::vector<Employee>& employees) {
    if (employees.empty()) return true;

    PQexec(conn, "BEGIN;");
    const int batchSize = 10000;

    for (size_t start = 0; start < employees.size(); start += batchSize) {
        size_t end = std::min(start + batchSize, employees.size());

        std::string query = "INSERT INTO employees (Full_name, Birthday, Gender) VALUES ";

        std::vector<const char*> paramValues;
        std::vector<std::string> paramData;
        int paramIndex = 1;

        for (size_t i = start; i < end; ++i) {
            const Employee& emp = employees[i];
            if (i > start) query += ", ";
            query += "(";
            query += "$" + std::to_string(paramIndex++);
            query += ", $" + std::to_string(paramIndex++);
            query += ", $" + std::to_string(paramIndex++);
            query += ")";
            paramData.push_back(emp.getName());
            paramData.push_back(emp.getBirthday());
            paramData.push_back(emp.getGender());
        }

        for (const auto& str : paramData)
            paramValues.push_back(str.c_str());

        PGresult* res = PQexecParams(
            conn,
            query.c_str(),
            (int)paramValues.size(),
            NULL,
            paramValues.data(),
            NULL,
            NULL,
            0
        );

        PQclear(res);
    }
    PQexec(conn, "COMMIT;");
    return true;
}





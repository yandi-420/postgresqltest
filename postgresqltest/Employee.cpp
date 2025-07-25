#define _CRT_SECURE_NO_WARNINGS
#include "Employee.h"
#include <ctime>
#include <cstdio>

Employee::Employee(const std::string& name, const std::string& birthday, const std::string& gender)
    : name(name), birthday(birthday), gender(gender) {
}

bool Employee::saveTodb(PGconn* conn) const {
    const char* values[3] = { name.c_str(), birthday.c_str(), gender.c_str() };
    PGresult* res = PQexecParams(conn,
        "INSERT INTO employees (Full_name, Birthday, Gender) VALUES ($1, $2, $3);",
        3, nullptr, values, nullptr, nullptr, 0);
    PQclear(res);
    return true;
}

std::string Employee::getName() const { return name; }
std::string Employee::getBirthday() const { return birthday; }
std::string Employee::getGender() const { return gender; }

int Employee::getAge() const {
    int y, m, d;
    if (sscanf(birthday.c_str(), "%d-%d-%d", &y, &m, &d) != 3) return -1;

    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    int age = (1900 + ltm->tm_year) - y;
    if ((1 + ltm->tm_mon < m) || (1 + ltm->tm_mon == m && ltm->tm_mday < d)) age--;
    return age;
}

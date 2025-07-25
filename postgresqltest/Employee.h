#pragma once
#include <string>
#include <libpq-fe.h>

class Employee {
private:
    std::string name, birthday, gender;

public:
    Employee(const std::string& name, const std::string& birthday, const std::string& gender);
    bool saveTodb(PGconn* conn) const;
    std::string getName() const;
    std::string getBirthday() const;
    std::string getGender() const;
    int getAge() const;
};

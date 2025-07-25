#include "DatabaseManager.h"
#include "Employee.h"
#include "EmployeeGenerator.h"
#include "FileManager.h"
#include <iostream>
#include <regex>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>

void DatabaseManager::connect(const std::string& configPath) {
    std::string conninfo = FileManager::readFile(configPath);
    conn = PQconnectdb(conninfo.c_str());
}

void DatabaseManager::disconnect() {
    if (conn) {
        PQfinish(conn);
        conn = nullptr;
    }
}

void DatabaseManager::createTable() {
    PGresult* res = PQexec(conn, "CREATE TABLE employees(id serial PRIMARY KEY, Full_name text NOT NULL, Birthday date NOT NULL, Gender char(1) NOT NULL);");

    std::cout << "Таблица создана\n";
}

void DatabaseManager::getlineAddEmployee() {
    while (true) {
        std::string name, birthday, gender;
        std::cout << "Имя (Alekseev Aleksey Alekseevich): ";
        std::getline(std::cin, name);
        std::cout << "Дата рождения (2025-12-31): ";
        std::getline(std::cin, birthday);
        std::cout << "Пол (M/F): ";
        std::getline(std::cin, gender);

        std::regex dateRegex(R"(\d{4}-\d{2}-\d{2})");
        bool birthdayValid = regex_match(birthday, dateRegex);
        bool genderValid = (gender == "M" || gender == "F" || gender == "m" || gender == "f");

        if (birthdayValid && genderValid) {
            Employee emp(name, birthday, gender);
            emp.saveTodb(conn);

            std::cout << "Сотрудник добавлен. Возраст: " << emp.getAge() << std::endl;
        }
        else if (birthdayValid == false) {
            std::cout << "Некорректный ввод даты рождения. Попробуйте еще раз" << std::endl;
            continue;
        }
        else if (genderValid == false) {
            std::cout << "Некорректный ввод пола. Попробуйте еще раз" << std::endl;
            continue;
        }

        std::cout << "Добавить ещё одного сотрудника? (1 — Да, иначе — выход): ";

        int choice;
        std::cin >> choice;
        std::cin.ignore();
        if (choice != 1) break;
    }
}

void DatabaseManager::uniqueEmployees() {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    PGresult* res = PQexec(conn, "SELECT DISTINCT ON (Full_name, Birthday) * FROM employees ORDER BY Full_name, Birthday;");

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        Employee emp(PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3));

        std::cout << "ФИО: " << emp.getName() << " Дата рождения: " << emp.getBirthday() << " Пол: " << emp.getGender() << " Возраст: " << emp.getAge() << std::endl;
    }

    std::cout << "\nВсего уникальных сотрудников: " << rows << std::endl;

    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;

    std::cout << elapsed.count() << " секунд\n";

    PQclear(res);
}

void DatabaseManager::bulkInsertEmployees() {
    using namespace std::chrono;

    std::vector<Employee> employees = EmployeeGenerator::generateFullNames();

    auto start = high_resolution_clock::now();

    EmployeeGenerator::massInsert(conn, employees);  

    auto end = high_resolution_clock::now();

    duration<double> elapsed = end - start;
    std::cout << elapsed.count() << " секунд\n";
    std::cout << "Добавлено сотрудников: " << employees.size() << std::endl;
}

void DatabaseManager::maleWithSurnameF() {
    using namespace std::chrono;

    PGresult* idxRes = PQexec(conn, "CREATE INDEX IF NOT EXISTS idx_gender_fullname_pattern ON employees (gender, full_name text_pattern_ops);");
    PQclear(idxRes);

    auto start1 = high_resolution_clock::now();

    PGresult* res = PQexec(conn, "SELECT id, full_name, birthday, gender FROM employees WHERE gender = 'M' AND full_name LIKE 'F%';");

    auto end1 = high_resolution_clock::now();
    duration<double> elapsed1 = end1 - start1;

    auto start2 = high_resolution_clock::now();

    int rows = PQntuples(res);
    for (int i = 0; i < rows; ++i) {
        std::cout << "id: " << PQgetvalue(res, i, 0) << " ФИО: " << PQgetvalue(res, i, 1) << " Дата: " << PQgetvalue(res, i, 2) << " Пол: " << PQgetvalue(res, i, 3) << std::endl;
    }

    auto end2 = high_resolution_clock::now();
    duration<double> elapsed2 = end2 - start2;

    std::cout << "Результатов: " << rows << std::endl;
    std::cout << "Время выполнения запроса: " << elapsed1.count() << " секунд" << std::endl;
    std::cout << "Время вывода на экран: " << elapsed2.count() << " секунд" << std::endl;

    PQclear(res);
}




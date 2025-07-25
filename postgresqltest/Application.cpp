#include "Application.h"
#include "DatabaseManager.h"
#include "EmployeeGenerator.h"
#include "FileManager.h"
#include <iostream>
#include <locale>
#include <Windows.h>

void Application::run() {
    setlocale(LC_ALL, "Russian"); 
    int n;
    std::cout << "Выберите режим от 1 до 5" << std::endl;
    std::cout << "1. Создание таблицы employees" << std::endl;
    std::cout << "2. Добавление сотрудников в таблицу" << std::endl;
    std::cout << "3. Сотрудники с уникальным значением ФИО + дата" << std::endl;
    std::cout << "4. Заполнение автоматически 1000000 строк справочника сотрудников" << std::endl;
    std::cout << "5. Сотрудники по критерию: пол мужской, Фамилия начинается с 'F'" << std::endl;
    std::cin >> n;
    std::cin.ignore();


    DatabaseManager db;
    db.connect("config.txt");

    switch (n) {
    case 1:
        db.createTable();
        break;
    case 2:
        db.getlineAddEmployee();
        break;
    case 3:
        db.uniqueEmployees();
        break;
    case 4:
        db.bulkInsertEmployees();
        break;
    case 5:
        db.maleWithSurnameF();
        break;
    default:
        std::cout << "Только от 1 до 5" << std::endl;
        break;
    }

    db.disconnect();
    system("pause");
}

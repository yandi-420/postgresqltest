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
    std::cout << "�������� ����� �� 1 �� 5" << std::endl;
    std::cout << "1. �������� ������� employees" << std::endl;
    std::cout << "2. ���������� ����������� � �������" << std::endl;
    std::cout << "3. ���������� � ���������� ��������� ��� + ����" << std::endl;
    std::cout << "4. ���������� ������������� 1000000 ����� ����������� �����������" << std::endl;
    std::cout << "5. ���������� �� ��������: ��� �������, ������� ���������� � 'F'" << std::endl;
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
        std::cout << "������ �� 1 �� 5" << std::endl;
        break;
    }

    db.disconnect();
    system("pause");
}

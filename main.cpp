#include "product.h"

#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;

int Menu();

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    FILE* File = NULL;

    while (true) {
        switch (Menu()) {
        case 1:
            createFile(File);
            break;
        case 2:
            viewFile(File);
            break;
        case 3:
            addProduct(File);
            break;
        case 4:
            editProduct(File);
            break;
        case 5:
            deleteProduct(File);
            break;
        case 6:
            linearSearchByName(File);
            break;
        case 7:
            binarySearchByCategory(File);
            break;
        case 8:
            quickSortByCategory(File);
            break;
        case 9:
            selectionSortByDate(File);
            break;
        case 10:
            insertionSortByName(File);
            break;
        case 11:
            searchByCategoryandDate(File);
            break;
        case 12:
            productStatistic(File);
            break;
        default:
            cout << "Завершение программы.\n";
            return 0;
        }
    }
    return 0;
}

int Menu() {
    cout << "Выберите действие для работы с товарами:" << endl << endl
        << setw(40) << left << "1 - Создать файл" << ":   " << setw(40) << left << "7 - Двоичный поиск по категории" << endl
        << setw(40) << left << "2 - Просмотреть товары" << ":   " << setw(40) << left << "8 - Быстрая сортировка по категории" << endl
        << setw(40) << left << "3 - Добавить товар" << ":   " << setw(40) << left << "9 - Сортировка выбором по дате" << endl
        << setw(40) << left << "4 - Изменить товар" << ":   " << setw(40) << left << "10 - Сортировка вставками по названию" << endl
        << setw(40) << left << "5 - Удалить товар" << ":   " << setw(40) << left << "11 - Поиск по категории и дате" << endl
        << setw(40) << left << "6 - Линейный поиск по названию" << ":   " << setw(40) << left << "12 - Статистика по товарам" << endl << endl
        << "0 - Выход\nВаш выбор: ";
    int choice = 0;
    if (!(cin >> choice)) return 0;
    return choice;
}

#include "product.h"

#include <iostream>
#include <cstring>
#include <io.h>

using namespace std;

namespace {

bool isValidDate(const Date& date) {
    if (date.month < 1 || date.month > 12) return false;
    if (date.day < 1 || date.day > 31) return false;

    if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11) {
        if (date.day > 30) return false;
    }
    else if (date.month == 2) {
        bool isLeap = (date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0);
        if (date.day > 29 || (date.day == 29 && !isLeap)) return false;
    }

    return true;
}

} // namespace

Date inputDate() {
    Date date;
    while (true) {
        cout << "Введите дату поступления товара (день месяц год): ";
        cin >> date.day >> date.month >> date.year;

        if (isValidDate(date)) {
            break;
        }
        else {
            cout << "Некорректная дата!\n";
        }
    }
    return date;
}

void createFile(FILE*& File) {
    Product product;
    int n, i;
    fopen_s(&File, "Products.dat", "wb");
    if (File == NULL) {
        cout << "Не удалось создать файл" << endl;
        return;
    }
    cout << "Введите количество товаров: ";
    cin >> n;

    for (i = 0; i < n; i++) {
        cout << "\n" << i + 1 << ". ";
        cout << "Введите название товара: ";
        cin >> product.name;
        cout << "Введите категорию товара: ";
        cin >> product.category;
        cout << "Введите количество товара на складе: ";
        cin >> product.quantity;
        product.date = inputDate();
        cout << "Введите цену товара: ";
        cin >> product.price;
        fwrite(&product, sizeof(Product), 1, File);
        cout << "\n";
    }
    fclose(File);
}

void viewFile(FILE*& File) {
    int i = 1;
    Product product;
    fopen_s(&File, "Products.dat", "rb");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }
    while (fread(&product, sizeof(Product), 1, File)) {
        cout << i << ". ";
        cout << "\nНазвание товара: " << product.name;
        cout << "\nКатегория товара: " << product.category;
        cout << "\nКоличество товара на складе: " << product.quantity;
        printf("\nДата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
        cout << "\nЦена товара: " << product.price << "\n\n";
        i++;
    }
    fclose(File);
}

void addProduct(FILE*& File) {
    fopen_s(&File, "Products.dat", "ab");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    Product newProduct;
    cout << "Введите название товара: ";
    cin >> newProduct.name;
    cout << "Введите категорию товара: ";
    cin >> newProduct.category;
    cout << "Введите количество товара на складе: ";
    cin >> newProduct.quantity;
    newProduct.date = inputDate();
    cout << "Введите цену товара: ";
    cin >> newProduct.price;
    fwrite(&newProduct, sizeof(struct Product), 1, File);
    fclose(File);
}

void editProduct(FILE*& File) {
    fopen_s(&File, "Products.dat", "rb+");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    int index, choice;
    cout << "Введите номер товара для редактирования: ";
    cin >> index;

    Product product;
    fseek(File, (index - 1) * sizeof(Product), SEEK_SET);
    fread(&product, sizeof(Product), 1, File);

    cout << "Редактирование товара:\n";
    cout << "Название товара: " << product.name << "\n";
    cout << "Категория товара: " << product.category << "\n";
    cout << "Количество товара на складе: " << product.quantity << "\n";
    printf("Дата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
    cout << "\nЦена товара: " << product.price << "\n";

    do {
        cout << "\n\nВыберите поле для изменения:\n"
            << "1) Название товара\n"
            << "2) Категория товара\n"
            << "3) Количество товара на складе\n"
            << "4) Дата поступления товара\n"
            << "5) Цена товара\n"
            << "0) Выход\n\nВаш выбор: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "\nВведите новое название товара: ";
            cin >> product.name;
            break;

        case 2:
            cout << "Введите новую категорию товара: ";
            cin >> product.category;
            break;

        case 3:
            cout << "Введите новое количество товара на складе: ";
            cin >> product.quantity;
            break;

        case 4:
            product.date = inputDate();
            break;

        case 5:
            cout << "Введите новую цену товара: ";
            cin >> product.price;
            break;

        case 0:
            cout << "Выход\n";
            break;

        default:
            cout << "Неверный ввод! Попробуйте ещё раз!\n";
            break;
        }
    } while (choice != 0);

    fseek(File, (index - 1) * sizeof(Product), SEEK_SET);
    fwrite(&product, sizeof(Product), 1, File);
    cout << "Товар обновлён.\n";
    fclose(File);
}

void deleteProduct(FILE*& File) {
    int index;
    cout << "Введите номер товара для удаления: ";
    cin >> index;

    fopen_s(&File, "Products.dat", "rb+");
    if (File == NULL) {
        cout << "Ошибка открытия файла!" << endl;
        return;
    }

    int total_products = _filelength(_fileno(File)) / sizeof(Product);

    for (int i = index; i < total_products; i++) {
        Product temp;
        fseek(File, i * sizeof(Product), SEEK_SET);
        fread(&temp, sizeof(Product), 1, File);

        fseek(File, (i - 1) * sizeof(Product), SEEK_SET);
        fwrite(&temp, sizeof(Product), 1, File);
    }

    if (_chsize(_fileno(File), (total_products - 1) * sizeof(Product)) != 0) {
        cout << "Ошибка изменения размера файла!" << endl;
    }
    fclose(File);
    cout << "Товар успешно удалён!" << endl;
}

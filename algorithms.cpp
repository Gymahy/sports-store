#include "product.h"

#include <iostream>
#include <cstring>
#include <io.h>

using namespace std;

namespace {

struct Stack {
    int left;
    int right;
    Stack* next;
};

Stack* inStack(Stack* p, int left, int right) {
    Stack* t = new Stack;
    t->left = left;
    t->right = right;
    t->next = p;
    return t;
}

Stack* outStack(Stack* p, int* left, int* right) {
    Stack* t = p;
    *left = p->left;
    *right = p->right;
    p = p->next;
    delete t;
    return p;
}

}

void linearSearchByName(FILE*& File) {
    char searchName[100];
    cout << "Введите название товара для поиска: ";
    cin >> searchName;

    fopen_s(&File, "Products.dat", "rb");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    Product product;
    bool found = false;

    fseek(File, 0, SEEK_SET);
    while (fread(&product, sizeof(Product), 1, File)) {
        if (strcmp(product.name, searchName) == 0) {
            cout << "Найден товар:\n";
            cout << "Название: " << product.name << "\n";
            cout << "Категория: " << product.category << "\n";
            cout << "Количество: " << product.quantity << "\n";
            printf("Дата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
            cout << "\nЦена: " << product.price << "\n\n";
            found = true;
        }
    }

    if (!found) {
        cout << "Товар с названием \"" << searchName << "\" не найден.\n";
    }

    fclose(File);
    cout << "Линейный поиск по названию завершён.\n";
}

void binarySearchByCategory(FILE*& File) {
    quickSortByCategory(File);

    char searchCategory[50];
    cout << "Введите категорию товара для поиска: ";
    cin >> searchCategory;

    fopen_s(&File, "Products.dat", "rb");
    if (File == NULL) {
        cout << "Ошибка: не удалось открыть файл для чтения!" << endl;
        return;
    }

    int totalProducts = _filelength(_fileno(File)) / sizeof(Product);

    Product product;
    int left = 0;
    int right = totalProducts - 1;
    bool productFound = false;

    while (left <= right) {
        int mid = (right + left) / 2;
        fseek(File, mid * sizeof(Product), SEEK_SET);
        fread(&product, sizeof(Product), 1, File);

        if (strcmp(product.category, searchCategory) == 0) {
            cout << "\nНайден товар:" << endl;
            cout << "Название: " << product.name << endl;
            cout << "Категория: " << product.category << endl;
            cout << "Количество: " << product.quantity << endl;
            printf("Дата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
            cout << "\nЦена: " << product.price << endl << endl;
            productFound = true;

            int temp = mid - 1;
            while (temp >= 0) {
                fseek(File, temp * sizeof(Product), SEEK_SET);
                fread(&product, sizeof(Product), 1, File);
                if (strcmp(product.category, searchCategory) == 0) {
                    cout << "\nНайден товар:" << endl;
                    cout << "Название: " << product.name << endl;
                    cout << "Категория: " << product.category << endl;
                    cout << "Количество: " << product.quantity << endl;
                    printf("Дата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
                    cout << "\nЦена: " << product.price << endl << endl;
                    temp--;
                }
                else {
                    break;
                }
            }

            temp = mid + 1;
            while (temp < totalProducts) {
                fseek(File, temp * sizeof(Product), SEEK_SET);
                fread(&product, sizeof(Product), 1, File);
                if (strcmp(product.category, searchCategory) == 0) {
                    cout << "\nНайден товар:" << endl;
                    cout << "Название: " << product.name << endl;
                    cout << "Категория: " << product.category << endl;
                    cout << "Количество: " << product.quantity << endl;
                    printf("Дата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
                    cout << "\nЦена: " << product.price << endl << endl;
                    temp++;
                }
                else {
                    break;
                }
            }

            break;
        }
        else if (strcmp(product.category, searchCategory) < 0) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    if (!productFound) {
        cout << "Товары категории \"" << searchCategory << "\" не найдены." << endl;
    }

    fclose(File);
    cout << "Двоичный поиск по категории завершён.\n";
}

void insertionSortByName(FILE*& File) {
    fopen_s(&File, "Products.dat", "rb+");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    int count = _filelength(_fileno(File)) / sizeof(Product);

    for (int i = 1; i < count; i++) {
        Product temp;
        fseek(File, i * sizeof(Product), SEEK_SET);
        fread(&temp, sizeof(Product), 1, File);

        int j = i - 1;
        while (j >= 0) {
            Product current;
            fseek(File, j * sizeof(Product), SEEK_SET);
            fread(&current, sizeof(Product), 1, File);

            if (strcmp(current.name, temp.name) > 0) {
                fseek(File, (j + 1) * sizeof(Product), SEEK_SET);
                fwrite(&current, sizeof(Product), 1, File);
                j--;
            }
            else {
                break;
            }
        }

        fseek(File, (j + 1) * sizeof(Product), SEEK_SET);
        fwrite(&temp, sizeof(Product), 1, File);
    }

    fclose(File);
    cout << "Сортировка вставками по названию завершена.\n";
}

void selectionSortByDate(FILE*& File) {
    fopen_s(&File, "Products.dat", "rb+");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    int n = _filelength(_fileno(File)) / sizeof(Product);

    for (int i = 0; i < n - 1; i++) {
        Product current;
        fseek(File, i * sizeof(Product), SEEK_SET);
        fread(&current, sizeof(Product), 1, File);

        int minIndex = i;
        Product minProduct = current;

        for (int j = i + 1; j < n; j++) {
            fseek(File, j * sizeof(Product), SEEK_SET);
            fread(&current, sizeof(Product), 1, File);

            if (current.date.year < minProduct.date.year ||
                (current.date.year == minProduct.date.year && current.date.month < minProduct.date.month) ||
                (current.date.year == minProduct.date.year && current.date.month == minProduct.date.month && current.date.day < minProduct.date.day)) {
                minIndex = j;
                minProduct = current;
            }
        }

        if (minIndex != i) {
            fseek(File, i * sizeof(Product), SEEK_SET);
            Product temp;
            fread(&temp, sizeof(Product), 1, File);

            fseek(File, i * sizeof(Product), SEEK_SET);
            fwrite(&minProduct, sizeof(Product), 1, File);

            fseek(File, minIndex * sizeof(Product), SEEK_SET);
            fwrite(&temp, sizeof(Product), 1, File);
        }
    }

    fclose(File);
    cout << "Сортировка выбором по дате завершена.\n";
}

void quickSortByCategory(FILE*& File) {
    fopen_s(&File, "Products.dat", "rb+");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }
    Stack* begin = NULL;
    int productSize = sizeof(Product);
    int left, right;

    begin = inStack(begin, 0, _filelength(_fileno(File)) / productSize - 1);

    while (begin != NULL) {
        begin = outStack(begin, &left, &right);

        int i = left;
        int j = right;

        Product pivotProduct;
        fseek(File, (left + right) / 2 * productSize, SEEK_SET);
        fread(&pivotProduct, productSize, 1, File);

        while (i <= j) {
            Product productI, productJ;
            fseek(File, i * productSize, SEEK_SET);
            fread(&productI, productSize, 1, File);

            while (strcmp(productI.category, pivotProduct.category) < 0) {
                i++;
                fseek(File, i * productSize, SEEK_SET);
                fread(&productI, productSize, 1, File);
            }

            fseek(File, j * productSize, SEEK_SET);
            fread(&productJ, productSize, 1, File);

            while (strcmp(productJ.category, pivotProduct.category) > 0) {
                j--;
                fseek(File, j * productSize, SEEK_SET);
                fread(&productJ, productSize, 1, File);
            }

            if (i <= j) {
                fseek(File, i * productSize, SEEK_SET);
                fwrite(&productJ, productSize, 1, File);

                fseek(File, j * productSize, SEEK_SET);
                fwrite(&productI, productSize, 1, File);

                i++;
                j--;
            }
        }

        if (left < j) {
            begin = inStack(begin, left, j);
        }
        if (i < right) {
            begin = inStack(begin, i, right);
        }
    }

    fclose(File);
    cout << "Быстрая сортировка по категории завершена.\n";
}

void searchByCategoryandDate(FILE*& File) {
    char searchCategory[50];
    Date searchDate = inputDate();

    cout << "Введите категорию товара для поиска: ";
    cin >> searchCategory;

    insertionSortByName(File);

    fopen_s(&File, "Products.dat", "rb");
    if (File == NULL) {
        cout << "Не удалось открыть файл" << endl;
        return;
    }

    bool found = false;
    Product product;
    FILE* endFile = NULL;
    char answer[5];

    cout << "Товары категории \"" << searchCategory << "\", поступившие после ";
    printf("%02d.%02d.%04d", searchDate.day, searchDate.month, searchDate.year);
    cout << ":\n";

    cout << "Сохранить результаты поиска в файл? (да/нет): ";
    cin >> answer;

    if (strcmp(answer, "да") == 0 || strcmp(answer, "Yes") == 0) {
        char reportName[256];
        cout << "Введите название отчёта: ";
        cin >> reportName;
        strcat_s(reportName, sizeof(reportName), ".txt");

        fopen_s(&endFile, reportName, "w");
        if (endFile == NULL) {
            cout << "Не удалось открыть файл.\n";
        }
    }

    while (fread(&product, sizeof(Product), 1, File)) {
        if (strcmp(product.category, searchCategory) == 0 && (product.date.year > searchDate.year || (product.date.year == searchDate.year && product.date.month > searchDate.month) ||
            (product.date.year == searchDate.year && product.date.month == searchDate.month && product.date.day > searchDate.day)))
        {
            cout << "\nНазвание: " << product.name;
            cout << "\nКатегория: " << product.category;
            cout << "\nКоличество: " << product.quantity;
            printf("\nДата поступления товара: %02d.%02d.%04d", product.date.day, product.date.month, product.date.year);
            cout << "\nЦена: " << product.price << "\n";
            found = true;

            if (endFile != NULL) {
                fprintf(endFile, "Название: %s\n", product.name);
                fprintf(endFile, "Категория: %s\n", product.category);
                fprintf(endFile, "Количество: %d\n", product.quantity);
                fprintf(endFile, "Дата поступления: %02d.%02d.%04d\n", product.date.day, product.date.month, product.date.year);
                fprintf(endFile, "Цена: %.2f\n\n", product.price);
            }
        }
    }

    if (!found) {
        cout << "По вашему запросу ничего не найдено.\n";
        if (endFile != NULL) {
            fprintf(endFile, "Товары категории \"%s\", поступившие после %02d.%02d.%04d, не найдены.\n",
                searchCategory, searchDate.day, searchDate.month, searchDate.year);
        }
    }

    if (endFile != NULL) {
        fclose(endFile);
        cout << "Результаты поиска сохранены в файл.\n";
    }

    fclose(File);
}

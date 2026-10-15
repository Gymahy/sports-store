#include "product.h"

#include <iostream>
#include <cstring>
#include <io.h>

using namespace std;

namespace {

void selectionSortForStatistic(FILE* file, int left, int right);

} 

void productStatistic(FILE*& File) {
    quickSortByCategory(File);

    fopen_s(&File, "Products.dat", "rb+");
    if (!File) {
        cout << "Ошибка открытия файла\n";
        return;
    }

    int totalProducts = _filelength(_fileno(File)) / sizeof(Product);
    if (!totalProducts) {
        cout << "Файл пуст!\n";
        fclose(File);
        return;
    }

    char answer[5];
    cout << "Сохранить статистику в файл? (да/нет): ";
    cin >> answer;

    FILE* resFile = NULL;
    if (strcmp(answer, "да") == 0 || strcmp(answer, "Yes") == 0) {
        char reportName[256];
        cout << "Введите название отчёта: ";
        cin >> reportName;
        strcat_s(reportName, sizeof(reportName), ".txt");

        fopen_s(&resFile, reportName, "w");
        if (resFile == NULL) {
            cout << "Не удалось открыть файл.\n";
        }
    }

    char currentCategory[50] = "";
    char maxUniqueCategory[50] = "";
    int maxUniqueCount = 0;
    Product maxPrice;
    maxPrice.price = -1;
    Product product;

    for (int i = 0; i < totalProducts; i++) {
        fseek(File, i * sizeof(Product), SEEK_SET);
        fread(&product, sizeof(Product), 1, File);
        if (product.price > maxPrice.price) {
            maxPrice = product;
        }
    }

    for (int i = 0; i < totalProducts;) {
        fseek(File, i * sizeof(Product), SEEK_SET);
        fread(&product, sizeof(Product), 1, File);

        if (strcmp(product.category, currentCategory) != 0) {
            strcpy_s(currentCategory, sizeof(currentCategory), product.category);
            int end = i;
            while (end < totalProducts) {
                fseek(File, end * sizeof(Product), SEEK_SET);
                fread(&product, sizeof(Product), 1, File);
                if (strcmp(product.category, currentCategory) != 0) break;
                end++;
            }

            selectionSortForStatistic(File, i, end - 1);

            int uniqueCount = 0;
            for (int j = i; j < end; j++) {
                bool isUnique = true;
                fseek(File, j * sizeof(Product), SEEK_SET);
                fread(&product, sizeof(Product), 1, File);

                for (int k = i; k < j; k++) {
                    Product prev;
                    fseek(File, k * sizeof(Product), SEEK_SET);
                    fread(&prev, sizeof(Product), 1, File);
                    if (strcmp(prev.name, product.name) == 0) {
                        isUnique = false;
                        break;
                    }
                }
                if (isUnique) uniqueCount++;
            }

            if (uniqueCount > maxUniqueCount) {
                maxUniqueCount = uniqueCount;
                strcpy_s(maxUniqueCategory, sizeof(maxUniqueCategory), currentCategory);
            }

            cout << "----------------------------------------\nКатегория: " << currentCategory << "\n----------------------------------------\n";
            if (resFile) fprintf(resFile, "----------------------------------------\nКатегория: %s\n----------------------------------------\n", currentCategory);

            for (int j = i; j < end; j++) {
                fseek(File, j * sizeof(Product), SEEK_SET);
                fread(&product, sizeof(Product), 1, File);
                printf("Название: %s\nКоличество: %d\nДата: %02d.%02d.%04d\nЦена: %.2f\n\n",
                    product.name, product.quantity, product.date.day, product.date.month, product.date.year, product.price);

                if (resFile) {
                    fprintf(resFile, "Название: %s\nКоличество: %d\nДата: %02d.%02d.%04d\nЦена: %.2f\n\n",
                        product.name, product.quantity, product.date.day, product.date.month, product.date.year, product.price);
                }
            }

            i = end; 
        }
        else {
            i++;
        }
    }

    cout << "----------------------------------------\n";
    cout << "Категория с наибольшим количеством уникальных товаров: " << maxUniqueCategory << "\n";
    cout << "Количество уникальных товаров: " << maxUniqueCount << "\n";
    cout << "Самый дорогой товар:\n";
    cout << "Название: " << maxPrice.name << "\nЦена: " << maxPrice.price << "\n";
    printf("Дата поступления: %02d.%02d.%04d\n", maxPrice.date.day, maxPrice.date.month, maxPrice.date.year);
    cout << "----------------------------------------\n";

    if (resFile) {
        fprintf(resFile, "----------------------------------------\n");
        fprintf(resFile, "Категория с наибольшим количеством уникальных товаров: %s\n", maxUniqueCategory);
        fprintf(resFile, "Количество уникальных товаров: %d\n", maxUniqueCount);
        fprintf(resFile, "Самый дорогой товар:\nНазвание: %s\nЦена: %.2f\nДата: %02d.%02d.%04d\n",
            maxPrice.name, maxPrice.price, maxPrice.date.day, maxPrice.date.month, maxPrice.date.year);
        fclose(resFile);
    }

    fclose(File);
}

namespace {

void selectionSortForStatistic(FILE* File, int left, int right) {
    for (int i = left; i < right; i++) {
        int minIndex = i;
        Product minProduct;
        fseek(File, minIndex * sizeof(Product), SEEK_SET);
        fread(&minProduct, sizeof(Product), 1, File);

        for (int j = i + 1; j <= right; j++) {
            Product current;
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
            Product temp;
            fseek(File, i * sizeof(Product), SEEK_SET);
            fread(&temp, sizeof(Product), 1, File);

            fseek(File, i * sizeof(Product), SEEK_SET);
            fwrite(&minProduct, sizeof(Product), 1, File);

            fseek(File, minIndex * sizeof(Product), SEEK_SET);
            fwrite(&temp, sizeof(Product), 1, File);
        }
    }
}

} // namespace

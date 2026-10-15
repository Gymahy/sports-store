#pragma once

#include <cstdio>

struct Date {
    int day, month, year;
};

struct Product {
    char name[100];
    char category[50];
    int quantity;
    Date date;
    double price;
};

Date inputDate();

void createFile(FILE*& file);
void viewFile(FILE*& file);
void addProduct(FILE*& file);
void editProduct(FILE*& file);
void deleteProduct(FILE*& file);
void linearSearchByName(FILE*& file);
void insertionSortByName(FILE*& file);
void selectionSortByDate(FILE*& file);
void binarySearchByCategory(FILE*& file);
void quickSortByCategory(FILE*& file);
void searchByCategoryandDate(FILE*& file);
void productStatistic(FILE*& file);

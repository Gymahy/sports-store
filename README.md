# sports-store

# Sports Store Inventory

A C++ console application for managing inventory in a sports store. Developed as a course project for the Fundamentals of Algorithms and Programming course.

The project focuses on binary file handling, data structures, and implementing search and sorting algorithms from scratch.

## Features

- Create a product data file; view, add, edit, and delete records.
- Search by exact product name or category.
- Find products in a selected category received after a specified date.
- Sort products by name, category, or arrival date.
- Display products by category and identify the most expensive product.
- Save search results and statistics as text reports.

Each product record contains its name, category, stock quantity, arrival date, and price. Data is stored in the binary file `Products.dat` and retained between runs.

## Algorithms

| Task | Algorithm |
| --- | --- |
| Search by name | Linear search |
| Search by category | Binary search after sorting |
| Sort by name | Insertion sort |
| Sort by arrival date | Selection sort |
| Sort by category | Iterative quicksort using a custom linked-list stack |

The algorithms operate directly on file records using `fread`, `fwrite`, and `fseek`. Sorting changes the order of records in `Products.dat`.

## Build and Run

The application targets **Windows** and uses the Windows API. Build it with **MinGW-w64**, such as the MSYS2 UCRT64 toolchain, or **Visual Studio 2022** with the C++ development tools installed.

Open the project directory in PowerShell and run the commands below. The `g++` compiler must be available on your `PATH`.

```powershell
g++ -std=c++17 -finput-charset=UTF-8 -fexec-charset=CP1251 -static main.cpp product.cpp algorithms.cpp statistics.cpp -o Soe.exe
.\Soe.exe
```

In **VS Code**, use the included build task by pressing **Ctrl+Shift+B** with the Microsoft C/C++ extension installed. The compiler path is configured in `.vscode/tasks.json`.

In **Visual Studio**, open `Store.vcxproj`, build the project, and run it with **Ctrl+F5**.

## Usage

1. On the first run, select **1 — Create file** and enter the products. This option overwrites any existing data file.
2. Use **2** to view products, **3** to add a product, and the remaining menu options to search, sort, and edit records.
3. Enter **0** to exit.

## Project Structure

| File | Purpose |
| --- | --- |
| `main.cpp` | Application entry point and menu |
| `product.h` | `Date` and `Product` structures and shared function declarations |
| `product.cpp` | Date input and product record operations |
| `algorithms.cpp` | Search, sorting, and saving search results |
| `statistics.cpp` | Statistics and report generation |

# Employee Management System

A simple, console-based application for managing employee records, written in C++. This project demonstrates fundamental C++ concepts including classes, objects, `std::vector`, and basic input/output handling.

The application allows users to perform standard CRUD (Create, Read, Update, Delete) operations on employee data stored in memory.

## Features

* **Add Employee**: Add a new employee with details like ID, name, salary, and department.
* **View Employees**: Display a list of all current employees, including a calculated net salary after a 10% tax deduction.
* **Update Employee**: Modify the salary or department of an existing employee by searching for their ID.
* **Delete Employee**: Remove an employee from the system using their ID.
* **In-Memory Storage**: Uses a `std::vector` to store employee data. Data is not persisted after the application closes.
* **User-Friendly Console UI**: A straightforward menu-driven interface for easy interaction.

## Requirements

* A C++ compiler that supports the C++11 standard or newer (e.g., g++, Clang, MSVC).

## How to Compile and Run

1.  **Save the Code**: Save the provided source code as `main.cpp`.
2.  **Open a Terminal**: Navigate to the directory where you saved the file.
3.  **Compile the Code**: Use the following command to compile the program. This will create an executable file.

    ```sh
    g++ main.cpp -o employee_manager
    ```

4.  **Run the Application**: Execute the compiled program from your terminal.

    * On Windows:
        ```sh
        .\employee_manager.exe
        ```
    * On macOS or Linux:
        ```sh
        ./employee_manager
        ```

## Usage

Once the application is running, you will see the main menu:

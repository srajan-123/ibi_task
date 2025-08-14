#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <optional>
#include <functional> // For std::reference_wrapper
#include <algorithm>  // For std::find_if, std::remove_if
#include <utility>    // For std::move

// ===================================================================
//  1. Data Model: Employee Class
// ===================================================================

class Employee {
private:
    int m_employeeID;
    std::string m_name;
    double m_salary;
    std::string m_department;

public:
    // Constructor
    Employee(int id, std::string name, double salary, std::string department);

    // Getters (const-correct)
    int getEmployeeID() const;
    const std::string& getName() const;
    double getSalary() const;
    const std::string& getDepartment() const;

    // Setters
    void setSalary(double salary);
    void setDepartment(const std::string& department);

    // Display utility
    void display() const;
};

// --- Employee Class Implementation ---

Employee::Employee(int id, std::string name, double salary, std::string department)
    : m_employeeID(id), m_name(std::move(name)), m_salary(salary), m_department(std::move(department)) {}

int Employee::getEmployeeID() const { return m_employeeID; }
const std::string& Employee::getName() const { return m_name; }
double Employee::getSalary() const { return m_salary; }
const std::string& Employee::getDepartment() const { return m_department; }

void Employee::setSalary(double salary) { m_salary = salary; }
void Employee::setDepartment(const std::string& department) { m_department = department; }

void Employee::display() const {
    const double TAX_RATE = 0.10; // 10% tax
    double netSalary = m_salary * (1 - TAX_RATE);

    std::cout << "  ID         : " << m_employeeID << std::endl;
    std::cout << "  Name       : " << m_name << std::endl;
    std::cout << "  Department : " << m_department << std::endl;
    std::cout << "  Gross Salary: $" << m_salary << std::endl;
    std::cout << "  Net Salary (after 10% tax): $" << netSalary << std::endl;
}

// ===================================================================
//  2. Data Logic: EmployeeRepository Class
// ===================================================================

class EmployeeRepository {
private:
    std::vector<Employee> m_employees;

public:
    bool add(const Employee& employee);
    bool remove(int employeeID);
    Employee* findById(int employeeID);
    const std::vector<Employee>& getAll() const;
};

// --- EmployeeRepository Class Implementation ---

bool EmployeeRepository::add(const Employee& employee) {
    auto it = std::find_if(m_employees.begin(), m_employees.end(),
        [id = employee.getEmployeeID()](const Employee& e) {
        return e.getEmployeeID() == id;
    });

    if (it != m_employees.end()) {
        return false; // Employee with this ID already exists
    }

    m_employees.push_back(employee);
    return true;
}

bool EmployeeRepository::remove(int employeeID) {
    auto it = std::remove_if(m_employees.begin(), m_employees.end(),
        [employeeID](const Employee& e) {
        return e.getEmployeeID() == employeeID;
    });

    if (it != m_employees.end()) {
        m_employees.erase(it, m_employees.end());
        return true; // Deletion successful
    }
    return false; // Employee not found
}

Employee* EmployeeRepository::findById(int employeeID) {
    auto it = std::find_if(m_employees.begin(), m_employees.end(),
        [employeeID](const Employee& e) {
        return e.getEmployeeID() == employeeID;
    });

    if (it != m_employees.end()) {
        return &(*it);
    }
    return nullptr;
}

const std::vector<Employee>& EmployeeRepository::getAll() const {
    return m_employees;
}

// ===================================================================
//  3. Application / User Interface
// ===================================================================

// --- UI Helper and Handler Function Declarations ---
void showMenu();
int getIntegerInput();
double getDoubleInput();
void handleAddEmployee(EmployeeRepository& repo);
void handleViewEmployees(const EmployeeRepository& repo);
void handleUpdateEmployee(EmployeeRepository& repo);
void handleDeleteEmployee(EmployeeRepository& repo);

// --- Main Function ---
int main() {
    EmployeeRepository repository;
    int choice;

    do {
        showMenu();
        choice = getIntegerInput();

        switch (choice) {
            case 1: handleAddEmployee(repository); break;
            case 2: handleViewEmployees(repository); break;
            case 3: handleUpdateEmployee(repository); break;
            case 4: handleDeleteEmployee(repository); break;
            case 5: std::cout << "Exiting application. Goodbye!" << std::endl; break;
            default: std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 5);

    return 0;
}

// --- Implementation of UI and Handler Functions ---

void showMenu() {
    std::cout << "\n===== Employee Management System =====" << std::endl;
    std::cout << "1. Add Employee" << std::endl;
    std::cout << "2. View All Employees" << std::endl;
    std::cout << "3. Update Employee" << std::endl;
    std::cout << "4. Delete Employee" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Enter your choice: ";
}

void handleAddEmployee(EmployeeRepository& repo) {
    std::cout << "\n--- Add New Employee ---" << std::endl;
    std::cout << "Enter Employee ID: ";
    int id = getIntegerInput();
    
    std::cout << "Enter Name: ";
    std::string name;
    std::getline(std::cin, name);
    if (name.empty()) std::getline(std::cin, name); // Fix: skip leftover newline

    std::cout << "Enter Salary: ";
    double salary = getDoubleInput();

    std::cout << "Enter Department: ";
    std::string department;
    std::getline(std::cin, department);
    if (department.empty()) std::getline(std::cin, department); // Fix: skip leftover newline
    
    Employee newEmployee(id, name, salary, department);
    if (repo.add(newEmployee)) {
        std::cout << "Employee added successfully!" << std::endl;
    } else {
        std::cout << "Error: Employee with ID " << id << " already exists." << std::endl;
    }
}

void handleViewEmployees(const EmployeeRepository& repo) {
    std::cout << "\n--- All Employees ---" << std::endl;
    const auto& employees = repo.getAll();
    if (employees.empty()) {
        std::cout << "No employees to display." << std::endl;
        return;
    }
    for (const auto& emp : employees) {
        emp.display();
        std::cout << "---------------------------------" << std::endl;
    }
}

void handleUpdateEmployee(EmployeeRepository& repo) {
    std::cout << "\n--- Update Employee ---" << std::endl;
    std::cout << "Enter Employee ID to update: ";
    int id = getIntegerInput();

    auto employeePtr = repo.findById(id);
    if (!employeePtr) {
        std::cout << "Error: Employee with ID " << id << " not found." << std::endl;
        return;
    }
    
    Employee& emp = *employeePtr; // Get the mutable employee reference

    std::cout << "Employee Found: " << emp.getName() << ". What to update?" << std::endl;
    std::cout << "1. Update Salary" << std::endl;
    std::cout << "2. Update Department" << std::endl;
    std::cout << "Enter your choice: ";
    int choice = getIntegerInput();

    if (choice == 1) {
        std::cout << "Enter new Salary: ";
        emp.setSalary(getDoubleInput());
        std::cout << "Salary updated." << std::endl;
    } else if (choice == 2) {
        std::cout << "Enter new Department: ";
        std::string newDept;
        std::getline(std::cin, newDept);
        if (newDept.empty()) std::getline(std::cin, newDept); // Fix: skip leftover newline
        emp.setDepartment(newDept);
        std::cout << "Department updated." << std::endl;
    } else {
        std::cout << "Invalid choice." << std::endl;
    }
}

void handleDeleteEmployee(EmployeeRepository& repo) {
    std::cout << "\n--- Delete Employee ---" << std::endl;
    std::cout << "Enter Employee ID to delete: ";
    int id = getIntegerInput();
    if (repo.remove(id)) {
        std::cout << "Employee with ID " << id << " deleted successfully." << std::endl;
    } else {
        std::cout << "Error: Employee with ID " << id << " not found." << std::endl;
    }
}

int getIntegerInput() {
    int value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a whole number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

double getDoubleInput() {
    double value;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}
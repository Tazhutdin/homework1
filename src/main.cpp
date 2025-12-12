#include "Library.h"
#include <iostream>
#include <limits>
#include <memory>


// Функции для ввода данных
std::string getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int getIntInput(const std::string& prompt) {
    int value;
    std::cout << prompt;
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayMenu() {
    std::cout << "\n=== LIBRARY MANAGEMENT SYSTEM ===\n";
    std::cout << "1. View all books\n";
    std::cout << "2. View all users\n";
    std::cout << "3. Add new book\n";
    std::cout << "4. Register new user\n";
    std::cout << "5. Borrow book to user\n";
    std::cout << "6. Return book from user\n";
    std::cout << "7. Search book by ISBN\n";
    std::cout << "8. View user profile\n";
    std::cout << "9. Save data to file\n";
    std::cout << "10. Exit\n";
    std::cout << "Your choice: ";
}

int main() {
    // Создаем библиотеку
    Library library("data/library_data.txt");

    int choice = 0;

    do {
        try {
            displayMenu();
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            clearScreen();

            switch (choice) {
            case 1: { // Просмотреть все книги
                library.displayAllBooks();
                break;
            }

            case 2: { // Просмотреть всех пользователей
                library.displayAllUsers();
                break;
            }

            case 3: { // Add new book
                std::cout << "=== ADD NEW BOOK ===\n";
                std::string title = getStringInput("Book title: ");
                std::string author = getStringInput("Author: ");
                int year = getIntInput("Publication year: ");
                std::string isbn = getStringInput("ISBN: ");

                try {
                    Book newBook(title, author, year, isbn);
                    library.addBook(newBook);
                    std::cout << "Book successfully added!\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }

            case 4: { // Register new user
                std::cout << "=== REGISTER NEW USER ===\n";
                std::string name = getStringInput("Last name: ");
                std::string userId = getStringInput("User ID (e.g., USR_001): ");
                int maxBooks = getIntInput("Maximum books (default 3): ");

                try {
                    User newUser(name, userId, maxBooks);
                    library.addUser(newUser);
                    std::cout << "User successfully registered!\n";
                }
                catch (const std::exception& e) {
                    std::cout << "error: " << e.what() << std::endl;
                }
                break;
            }

            case 5: { // Borrow book to user
                std::cout << "=== BORROW BOOK ===\n";
                std::string userName = getStringInput("User last name: ");
                std::string isbn = getStringInput("Book ISBN: ");

                try {
                    library.borrowBook(userName, isbn);
                    std::cout << "Book successfully borrowed!\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }

            case 6: { // Return book from user
                std::cout << "=== RETURN BOOK ===\n";
                std::string isbn = getStringInput("ISBN of book to return: ");

                try {
                    library.returnBook(isbn);
                    std::cout << "Book successfully returned!\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                break;
            }

            case 7: { // Search book by ISBN
                std::cout << "=== SEARCH BOOK BY ISBN ===\n";
                std::string isbn = getStringInput("Enter ISBN: ");

                Book* book = library.findBookByISBN(isbn);
                if (book) {
                    book->displayInfo();
                }
                else {
                    std::cout << "Book with ISBN " << isbn << " not found.\n";
                }
                break;
            }

            case 8: { // View user profile
                std::cout << "=== USER PROFILE ===\n";
                std::string name = getStringInput("Enter user last name: ");

                User* user = library.findUserByName(name);
                if (user) {
                    user->displayProfile();
                }
                else {
                    std::cout << "User with last name " << name << " not found.\n";
                }
                break;
            }

            case 9: { // Save data to file
                try {
                    library.saveToFile();
                    std::cout << "Data successfully saved!\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Error saving data: " << e.what() << std::endl;
                }
                break;
            }

            case 10: { // Exit
                std::cout << "Saving data before exit...\n";
                try {
                    library.saveToFile();
                }
                catch (const std::exception& e) {
                    std::cout << "Error saving: " << e.what() << std::endl;
                }
                std::cout << "Goodbye!\n";
                break;
            }

            default: {
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
            }

            if (choice != 10) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                clearScreen();
            }

        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (choice != 10);

    return 0;
}
#include "User.h"
#include <algorithm>

// Конструктор
User::User(const std::string& name, const std::string& userId, int maxBooks)
    : name(name), userId(userId), maxBooksAllowed(maxBooks) {
}

// Геттеры
std::string User::getName() const { return name; }
std::string User::getUserId() const { return userId; }
std::vector<std::string> User::getBorrowedBooks() const { return borrowedBooks; }
int User::getMaxBooksAllowed() const { return maxBooksAllowed; }

// Проверка возможности взять книгу
bool User::canBorrowMore() const {
    return borrowedBooks.size() < static_cast<size_t>(maxBooksAllowed);
}

// Добавить книгу
void User::addBook(const std::string& isbn) {
    if (!canBorrowMore()) {
        throw std::runtime_error("User cannot borrow more books!");
    }
    borrowedBooks.push_back(isbn);
}

// Удалить книгу
void User::removeBook(const std::string& isbn) {
    auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), isbn);
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
    }
}

// Вывести профиль
void User::displayProfile() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "User ID: " << userId << std::endl;
    std::cout << "Books borrowed: " << borrowedBooks.size() << "/" << maxBooksAllowed << std::endl;

    if (!borrowedBooks.empty()) {
        std::cout << "ISBN of borrowed books:" << std::endl;
        for (const auto& isbn : borrowedBooks) {
            std::cout << "  - " << isbn << std::endl;
        }
    }
    std::cout << "-------------------" << std::endl;
}
#include "Book.h"
#include <ctime>
#include <stdexcept>

// Конструктор с валидацией
Book::Book(const std::string& title, const std::string& author, int year, const std::string& isbn)
    : title(title), author(author), year(year), isbn(isbn), isAvailable(true), borrowedBy("") {

    if (!isValidYear(year)) {
        throw std::invalid_argument("Invalid year! Year must be between 1450 and current year.");
    }

    if (isbn.empty()) {
        throw std::invalid_argument("ISBN cannot be empty!");
    }
}

// Геттеры
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
int Book::getYear() const { return year; }
std::string Book::getISBN() const { return isbn; }
bool Book::getIsAvailable() const { return isAvailable; }
std::string Book::getBorrowedBy() const { return borrowedBy; }

// Выдать книгу
void Book::borrowBook(const std::string& userName) {
    if (!isAvailable) {
        throw std::runtime_error("Book is already borrowed!");
    }
    isAvailable = false;
    borrowedBy = userName;
}

// Вернуть книгу
void Book::returnBook() {
    isAvailable = true;
    borrowedBy = "";
}

// Вывести информацию о книге
void Book::displayInfo() const {
    std::cout << "Title: " << title << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Year: " << year << std::endl;
    std::cout << "ISBN: " << isbn << std::endl;
    std::cout << "Status: " << (isAvailable ? "Available" : "Borrowed by " + borrowedBy) << std::endl;
    std::cout << "-------------------" << std::endl;
}

// Проверка года
bool Book::isValidYear(int year) {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int currentYear = now->tm_year + 1900;

    return year >= 1450 && year <= currentYear;
}
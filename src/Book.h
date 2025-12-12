#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>

class Book {
private:
    std::string title;
    std::string author;
    int year;
    std::string isbn;
    bool isAvailable;
    std::string borrowedBy;

public:
    // Конструктор с валидацией года
    Book(const std::string& title, const std::string& author, int year, const std::string& isbn);

    // Геттеры
    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    std::string getISBN() const;
    bool getIsAvailable() const;
    std::string getBorrowedBy() const;

    // Методы для работы с книгой
    void borrowBook(const std::string& userName);
    void returnBook();
    void displayInfo() const;

    // Статический метод для проверки года
    static bool isValidYear(int year);
};

#endif // BOOK_H
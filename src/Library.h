#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "User.h"
#include <vector>
#include <string>

class Library {
private:
    std::vector<Book> books;
    std::vector<User> users;
    std::string dataFile;

public:
    Library(const std::string& filename = "data/library_data.txt");

    // Методы для работы с книгами
    void addBook(const Book& book);
    void displayAllBooks() const;
    Book* findBookByISBN(const std::string& isbn);

    // Методы для работы с пользователями
    void addUser(const User& user);
    void displayAllUsers() const;
    User* findUserByName(const std::string& name);

    // Основные операции
    void borrowBook(const std::string& userName, const std::string& isbn);
    void returnBook(const std::string& isbn);

    // Работа с файлами
    void saveToFile() const;
    void loadFromFile();
};

#endif // LIBRARY_H
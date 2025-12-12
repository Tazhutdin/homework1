#include "Library.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// Конструктор с загрузкой из файла
Library::Library(const std::string& filename) : dataFile(filename) {
    loadFromFile();
}

// Добавить книгу
void Library::addBook(const Book& book) {
    // Проверка на уникальность ISBN
    for (const auto& b : books) {
        if (b.getISBN() == book.getISBN()) {
            throw std::runtime_error("Book with this ISBN already exists!");
        }
    }
    books.push_back(book);
}

// Показать все книги
void Library::displayAllBooks() const {
    std::cout << "\n=== ALL BOOKS ===\n";
    for (const auto& book : books) {
        book.displayInfo();
    }
}

// Найти книгу по ISBN
Book* Library::findBookByISBN(const std::string& isbn) {
    for (auto& book : books) {
        if (book.getISBN() == isbn) {
            return &book;
        }
    }
    return nullptr;
}

// Добавить пользователя
void Library::addUser(const User& user) {
    // Проверка на уникальность имени
    for (const auto& u : users) {
        if (u.getName() == user.getName()) {
            throw std::runtime_error("User with this name already exists!");
        }
    }
    users.push_back(user);
}

// Показать всех пользователей
void Library::displayAllUsers() const {
    std::cout << "\n=== ALL USERS ===\n";
    for (const auto& user : users) {
        user.displayProfile();
    }
}

// Найти пользователя по имени
User* Library::findUserByName(const std::string& name) {
    for (auto& user : users) {
        if (user.getName() == name) {
            return &user;
        }
    }
    return nullptr;
}

// Выдать книгу
void Library::borrowBook(const std::string& userName, const std::string& isbn) {
    User* user = findUserByName(userName);
    Book* book = findBookByISBN(isbn);

    if (!user) {
        throw std::runtime_error("User not found!");
    }

    if (!book) {
        throw std::runtime_error("Book not found!");
    }

    if (!user->canBorrowMore()) {
        throw std::runtime_error("User cannot borrow more books!");
    }

    if (!book->getIsAvailable()) {
        throw std::runtime_error("Book is already borrowed!");
    }

    // Выполняем операцию
    book->borrowBook(userName);
    user->addBook(isbn);
    std::cout << "Book \"" << book->getTitle() << "\" borrowed by " << userName << std::endl;
}

// Вернуть книгу
void Library::returnBook(const std::string& isbn) {
    Book* book = findBookByISBN(isbn);

    if (!book) {
        throw std::runtime_error("Book not found!");
    }

    if (book->getIsAvailable()) {
        throw std::runtime_error("Book is not borrowed!");
    }

    std::string userName = book->getBorrowedBy();
    User* user = findUserByName(userName);

    if (user) {
        user->removeBook(isbn);
    }

    book->returnBook();
    std::cout << "Book \"" << book->getTitle() << "\" returned" << std::endl;
}

// Сохранить в файл
void Library::saveToFile() const {
    std::ofstream file(dataFile);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing!");
    }

    // Сохраняем книги
    for (const auto& book : books) {
        file << "BOOK\n";
        file << "Title: " << book.getTitle() << "\n";
        file << "Author: " << book.getAuthor() << "\n";
        file << "Year: " << book.getYear() << "\n";
        file << "ISBN: " << book.getISBN() << "\n";
        file << "Available: " << (book.getIsAvailable() ? "yes" : "no") << "\n";
        file << "BorrowedBy: " << book.getBorrowedBy() << "\n\n";
    }

    // Разделитель
    file << "---USERS---\n\n";

    // Сохраняем пользователей
    for (const auto& user : users) {
        file << "USER\n";
        file << "Name: " << user.getName() << "\n";
        file << "UserID: " << user.getUserId() << "\n";

        file << "BorrowedBooks: ";
        auto borrowedBooks = user.getBorrowedBooks();
        for (size_t i = 0; i < borrowedBooks.size(); i++) {
            file << borrowedBooks[i];
            if (i != borrowedBooks.size() - 1) {
                file << "|";
            }
        }
        file << "\n";

        file << "MaxBooks: " << user.getMaxBooksAllowed() << "\n\n";
    }

    file.close();
    std::cout << "Data saved to " << dataFile << std::endl;
}

// Загрузить из файла
void Library::loadFromFile() {
    std::ifstream file(dataFile);

    // Если файл не существует, создаем пустую библиотеку
    if (!file.is_open()) {
        std::cout << "Data file not found. Creating empty library." << std::endl;
        return;
    }

    std::string line;
    bool readingBooks = true;

    // Временные переменные для загрузки книги
    std::string title, author, isbn, borrowedBy;
    int year = 0;
    bool available = true;

    // Временные переменные для загрузки пользователя
    std::string userName, userId;
    std::vector<std::string> borrowedBooksList;
    int maxBooks = 3;

    bool bookInProgress = false;
    bool userInProgress = false;

    while (getline(file, line)) {
        // Пропускаем пустые строки
        if (line.empty()) {
            // Если мы читали книгу и встретили пустую строку, добавляем книгу
            if (bookInProgress) {
                try {
                    Book book(title, author, year, isbn);
                    if (!available) {
                        book.borrowBook(borrowedBy);
                    }
                    books.push_back(book);
                    bookInProgress = false;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error loading book: " << e.what() << std::endl;
                    bookInProgress = false;
                }
            }
            // Если мы читали пользователя и встретили пустую строку, добавляем пользователя
            else if (userInProgress) {
                try {
                    User user(userName, userId, maxBooks);
                    for (const auto& bookIsbn : borrowedBooksList) {
                        Book* book = findBookByISBN(bookIsbn);
                        if (book) {
                            try {
                                user.addBook(bookIsbn);
                            }
                            catch (...) {
                                // Игнорируем, если не можем добавить книгу
                            }
                        }
                    }
                    users.push_back(user);
                    userInProgress = false;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error loading user: " << e.what() << std::endl;
                    userInProgress = false;
                }
            }
            continue;
        }

        // Проверяем разделитель между книгами и пользователями
        if (line == "---USERS---") {
            readingBooks = false;
            continue;
        }

        if (readingBooks) {
            if (line == "BOOK") {
                // Начинаем новую книгу
                title = "";
                author = "";
                isbn = "";
                borrowedBy = "";
                year = 0;
                available = true;
                bookInProgress = true;
            }
            else if (bookInProgress) {
                // Читаем данные книги
                size_t colonPos = line.find(": ");
                if (colonPos != std::string::npos) {
                    std::string key = line.substr(0, colonPos);
                    std::string value = line.substr(colonPos + 2);

                    if (key == "Title") title = value;
                    else if (key == "Author") author = value;
                    else if (key == "Year") year = std::stoi(value);
                    else if (key == "ISBN") isbn = value;
                    else if (key == "Available") available = (value == "yes");
                    else if (key == "BorrowedBy") borrowedBy = value;
                }
            }
        }
        else {
            // Читаем пользователей
            if (line == "USER") {
                // Начинаем нового пользователя
                userName = "";
                userId = "";
                borrowedBooksList.clear();
                maxBooks = 3;
                userInProgress = true;
            }
            else if (userInProgress) {
                // Читаем данные пользователя
                size_t colonPos = line.find(": ");
                if (colonPos != std::string::npos) {
                    std::string key = line.substr(0, colonPos);
                    std::string value = line.substr(colonPos + 2);

                    if (key == "Name") userName = value;
                    else if (key == "UserID") userId = value;
                    else if (key == "MaxBooks") maxBooks = std::stoi(value);
                    else if (key == "BorrowedBooks") {
                        if (!value.empty()) {
                            std::stringstream ss(value);
                            std::string bookIsbn;
                            while (getline(ss, bookIsbn, '|')) {
                                borrowedBooksList.push_back(bookIsbn);
                            }
                        }
                    }
                }
            }
        }
    }

    // Добавляем последнюю книгу или пользователя, если файл не заканчивается пустой строкой
    if (bookInProgress) {
        try {
            Book book(title, author, year, isbn);
            if (!available) {
                book.borrowBook(borrowedBy);
            }
            books.push_back(book);
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading book: " << e.what() << std::endl;
        }
    }

    if (userInProgress) {
        try {
            User user(userName, userId, maxBooks);
            for (const auto& bookIsbn : borrowedBooksList) {
                Book* book = findBookByISBN(bookIsbn);
                if (book) {
                    try {
                        user.addBook(bookIsbn);
                    }
                    catch (...) {
                        // Игнорируем ошибки добавления книг
                    }
                }
            }
            users.push_back(user);
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading user: " << e.what() << std::endl;
        }
    }

    file.close();
    std::cout << "Data loaded from " << dataFile << ". Loaded "
        << books.size() << " books and " << users.size() << " users." << std::endl;
}
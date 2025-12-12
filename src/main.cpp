#include "Book.h"
#include <iostream>

int main() {
    try {
        Book book("Test Book", "Test Author", 2020, "123-456-789");
        book.displayInfo();

        book.borrowBook("Ivanov");
        book.displayInfo();

        book.returnBook();
        book.displayInfo();

    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
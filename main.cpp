#include <iostream>
#include <string>
#include "PasswordChecker.hpp"
#include "ConfigManager.hpp"
#include "Logger.hpp"

int main() {
    Logger logger("password_checker.log");
    ConfigManager config;
    PasswordChecker checker(config);

    std::string password;
    while (true) {
        std::cout << "\nEnter password (or 'exit' to quit): ";
        std::getline(std::cin, password);

        if (password == "exit") {
            break;
        }

        try {
            PasswordStrength strength = checker.checkPassword(password);
            std::cout << "Strength Level: " << checker.strengthToString(strength) << "\n";
            std::cout << "Details: " << checker.getLastCheckDetails() << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
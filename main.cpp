#include <iostream>
#include <string>
#include <iomanip>
#include <memory>
#include <Windows.h>
#include <limits>
#include <vector>
#include <functional>
#include <sstream>
#include <conio.h>

#include "PasswordChecker.hpp"
#include "ConfigManager.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

#include <ftxui/component/captured_mouse.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

using namespace ftxui;

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}

void displayMenu() {
    setConsoleColor(11);
    std::cout << "\n======================================\n";
    std::cout << "     PASSWORD STRENGTH CHECKER        \n";
    std::cout << "======================================\n";
    setConsoleColor(7);
    std::cout << " 1. Check password strength\n";
    std::cout << " 2. Generate secure password\n";
    std::cout << " 3. Configure settings\n";
    std::cout << " 4. Exit\n";
    std::cout << "--------------------------------------\n";
    std::cout << "Enter your choice: ";
}

void drawStrengthBar(PasswordStrength strength) {
    int barWidth = 30;
    int progress = 0;
    
    switch (strength) {
        case PasswordStrength::WEAK: progress = 25; break;
        case PasswordStrength::MEDIUM: progress = 50; break;
        case PasswordStrength::STRONG: progress = 75; break;
        case PasswordStrength::VERY_STRONG: progress = 100; break;
    }
    
    int fill = barWidth * progress / 100;
    
    std::cout << "[";
    
    for (int i = 0; i < barWidth; ++i) {
        if (i < fill) {
            if (progress <= 25) setConsoleColor(12);        // Red
            else if (progress <= 50) setConsoleColor(14);   // Yellow
            else if (progress <= 75) setConsoleColor(11);   // Light blue
            else setConsoleColor(10);                       // Green
            std::cout << "█";
        } else {
            setConsoleColor(8);
            std::cout << " ";
        }
    }
    
    setConsoleColor(7);
    std::cout << "] " << progress << "%\n";
}

std::string generateSecurePassword(const ConfigManager& config) {
    size_t length = config.getMinLength() + 4;
    return Utils::generateRandomPassword(length, true, true, true, true);
}

void configSettings(ConfigManager& config) {
    bool running = true;
    
    while (running) {
        system("cls");
        setConsoleColor(11);
        std::cout << "\n=================================\n";
        std::cout << "           SETTINGS              \n";
        std::cout << "=================================\n";
        setConsoleColor(7);
        std::cout << " 1. Set minimum password length (current: " << config.getMinLength() << ")\n";
        std::cout << " 2. Set maximum password length (current: " << config.getMaxLength() << ")\n";
        std::cout << " 3. Set strict mode: " << (config.isStrictMode() ? "ON" : "OFF") << "\n";
        std::cout << " 4. Back to main menu\n";
        std::cout << "---------------------------------\n";
        std::cout << "Enter your choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(10000, '\n');
        
        switch (choice) {
            case 1: {
                std::cout << "Enter new minimum length: ";
                size_t minLength;
                std::cin >> minLength;
                std::cin.ignore(10000, '\n');
                try {
                    config.setMinLength(minLength);
                    std::cout << "Minimum length updated to " << minLength << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                std::cout << "Press any key to continue...";
                _getch();
                break;
            }
            case 2: {
                std::cout << "Enter new maximum length: ";
                size_t maxLength;
                std::cin >> maxLength;
                std::cin.ignore(10000, '\n');
                try {
                    config.setMaxLength(maxLength);
                    std::cout << "Maximum length updated to " << maxLength << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }
                std::cout << "Press any key to continue...";
                _getch();
                break;
            }
            case 3: {
                bool strictMode = !config.isStrictMode();
                config.setStrictMode(strictMode);
                std::cout << "Strict mode set to: " << (strictMode ? "ON" : "OFF") << std::endl;
                std::cout << "Press any key to continue...";
                _getch();
                break;
            }
            case 4:
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Press any key to continue...";
                _getch();
                break;
        }
    }
}

Elements strengthMeter(PasswordStrength strength) {
    int progress = 0;
    Color barColor = Color::Red;
    
    switch (strength) {
        case PasswordStrength::WEAK: 
            progress = 25; 
            barColor = Color::Red;
            break;
        case PasswordStrength::MEDIUM: 
            progress = 50; 
            barColor = Color::Yellow;
            break;
        case PasswordStrength::STRONG: 
            progress = 75; 
            barColor = Color::Blue;
            break;
        case PasswordStrength::VERY_STRONG: 
            progress = 100; 
            barColor = Color::Green;
            break;
    }
    
    return {
        hbox({
            text("Сила пароля: ") | bold,
            text(progress <= 25 ? "Слабый" : 
                progress <= 50 ? "Средний" : 
                progress <= 75 ? "Сильный" : "Очень сильный") | color(barColor) | bold
        }),
        hbox({
            text("["),
            gauge(float(progress) / 100.0f) | flex | color(barColor),
            text("]"),
            text(" " + std::to_string(progress) + "%")
        })
    };
}

Elements formatAnalysisResults(const std::string& details) {
    std::vector<Element> rows;
    
    std::istringstream ss(details);
    std::string line;
    bool analysisStarted = false;
    
    while (std::getline(ss, line)) {
        if (line.find("Password Analysis:") != std::string::npos) {
            analysisStarted = true;
            continue;
        }
        
        if (analysisStarted) {
            if (line.empty()) continue;
            
            if (line.find("Length:") != std::string::npos) {
                rows.push_back(text(line) | color(Color::Yellow));
            } else if (line.find("Strength:") != std::string::npos) {
                continue; // Skip strength line as we have the meter
            } else if (line.find("[+]") != std::string::npos) {
                rows.push_back(text(line) | color(Color::Green));
            } else if (line.find("[-]") != std::string::npos) {
                rows.push_back(text(line) | color(Color::Red));
            } else {
                rows.push_back(text(line));
            }
        }
    }
    
    return vbox(std::move(rows));
}

class PasswordApp {
public:
    PasswordApp() 
        : logger_("password_checker.log"),
          config_(),
          checker_(config_),
          password_input_(""),
          password_visible_(false),
          selected_tab_(0),
          include_upper_(true),
          include_lower_(true),
          include_digits_(true),
          include_special_(true) {
        
        logger_.info("Application started");
    }
    
    ~PasswordApp() {
        logger_.info("Application closed");
    }
    
    Component createUI() {
        auto check_tab = createCheckTab();
        auto generate_tab = createGenerateTab();
        auto settings_tab = createSettingsTab();
        
        auto tab_selection = Menu(&tab_titles_, &selected_tab_);
        auto tab_content = Container::Tab({
            check_tab,
            generate_tab,
            settings_tab
        }, &selected_tab_);
        
        return Container::Vertical({
            tab_selection,
            tab_content
        });
    }

private:
    Logger logger_;
    ConfigManager config_;
    PasswordChecker checker_;
    
    std::string password_input_;
    bool password_visible_;
    bool show_result_ = false;
    std::string result_details_;
    PasswordStrength current_strength_ = PasswordStrength::WEAK;
    
    std::string generated_password_;
    bool show_generated_ = false;
    
    bool include_upper_ = true;
    bool include_lower_ = true;
    bool include_digits_ = true;
    bool include_special_ = true;
    
    std::string min_length_str_ = std::to_string(config_.getMinLength());
    std::string max_length_str_ = std::to_string(config_.getMaxLength());
    bool strict_mode_ = config_.isStrictMode();
    
    int selected_tab_ = 0;
    std::vector<std::string> tab_titles_ = {
        "Проверка пароля", "Генерация пароля", "Настройки"
    };
    
    Component createCheckTab() {
        auto password_input = Input(&password_input_, "Введите пароль");
        
        auto visibility_toggle = Checkbox("Показать пароль", &password_visible_);
        
        auto check_button = Button("Проверить", [&] {
            if (password_input_.empty()) {
                result_details_ = "Ошибка: Пароль не может быть пустым";
                show_result_ = true;
                return;
            }
            
            try {
                current_strength_ = checker_.checkPassword(password_input_);
                result_details_ = checker_.getLastCheckDetails();
                show_result_ = true;
                logger_.info("Password checked. Strength: " + checker_.strengthToString(current_strength_));
            }
            catch (const std::exception& e) {
                result_details_ = std::string("Ошибка: ") + e.what();
                show_result_ = true;
                logger_.error("Error checking password: " + std::string(e.what()));
            }
        });
        
        auto clear_button = Button("Очистить", [&] {
            password_input_.clear();
            show_result_ = false;
        });
        
        auto password_field = Renderer(password_input, [&] {
            return vbox({
                text("Пароль:") | bold,
                password_visible_ ? 
                    hbox({
                        text("> "),
                        password_input->Render() | flex
                    }) :
                    hbox({
                        text("> "),
                        text(std::string(password_input_.size(), '*')) | flex
                    })
            });
        });
        
        auto container = Container::Vertical({
            password_field,
            visibility_toggle,
            Container::Horizontal({
                check_button,
                clear_button
            })
        });
        
        return Renderer(container, [&] {
            Elements result_elements;
            
            if (show_result_) {
                result_elements = {
                    separator(),
                    text("Результат анализа:") | bold | center,
                    vbox(strengthMeter(current_strength_)),
                    separator(),
                    vbox(formatAnalysisResults(result_details_))
                };
            }
            
            return window(
                text("Проверка надежности пароля") | bold | center,
                vbox({
                    container->Render(),
                    show_result_ ? vbox(result_elements) : Element()
                })
            ) | flex;
        });
    }
    
    Component createGenerateTab() {
        auto includeUpper = Checkbox("Включить заглавные буквы", &include_upper_);
        auto includeLower = Checkbox("Включить строчные буквы", &include_lower_);
        auto includeDigits = Checkbox("Включить цифры", &include_digits_);
        auto includeSpecial = Checkbox("Включить специальные символы", &include_special_);
        
        auto generate_button = Button("Сгенерировать пароль", [&] {
            size_t length = config_.getMinLength() + 4;
            generated_password_ = Utils::generateRandomPassword(
                length, include_upper_, include_lower_, include_digits_, include_special_
            );
            
            try {
                current_strength_ = checker_.checkPassword(generated_password_);
                result_details_ = checker_.getLastCheckDetails();
                show_generated_ = true;
                logger_.info("Generated new password. Strength: " + checker_.strengthToString(current_strength_));
            }
            catch (const std::exception& e) {
                result_details_ = std::string("Ошибка: ") + e.what();
                show_generated_ = true;
                logger_.error("Error checking generated password: " + std::string(e.what()));
            }
        });
        
        auto use_button = Button("Использовать для проверки", [&] {
            if (show_generated_ && !generated_password_.empty()) {
                password_input_ = generated_password_;
                selected_tab_ = 0;
            }
        });
        
        auto container = Container::Vertical({
            includeUpper,
            includeLower,
            includeDigits,
            includeSpecial,
            Container::Horizontal({
                generate_button
            })
        });
        
        return Renderer(container, [&] {
            Elements result_elements;
            
            if (show_generated_) {
                result_elements = {
                    separator(),
                    text("Сгенерированный пароль:") | bold,
                    hbox({
                        text(generated_password_) | color(Color::Green) | bold | flex
                    }),
                    separator(),
                    vbox(strengthMeter(current_strength_)),
                    separator(),
                    use_button->Render()
                };
            }
            
            return window(
                text("Генерация надежного пароля") | bold | center,
                vbox({
                    container->Render(),
                    show_generated_ ? vbox(result_elements) : Element()
                })
            ) | flex;
        });
    }
    
    Component createSettingsTab() {
        auto min_length_input = Input(&min_length_str_, "Минимальная длина");
        auto max_length_input = Input(&max_length_str_, "Максимальная длина");
        auto strict_mode_checkbox = Checkbox("Строгий режим", &strict_mode_);
        
        auto save_button = Button("Сохранить настройки", [&] {
            try {
                size_t min_length = std::stoul(min_length_str_);
                size_t max_length = std::stoul(max_length_str_);
                
                config_.setMinLength(min_length);
                config_.setMaxLength(max_length);
                config_.setStrictMode(strict_mode_);
                
                logger_.info("Configuration updated: min_length=" + min_length_str_ + 
                            ", max_length=" + max_length_str_ + 
                            ", strict_mode=" + (strict_mode_ ? "true" : "false"));
            }
            catch (const std::exception& e) {
                logger_.error("Error updating configuration: " + std::string(e.what()));
            }
        });
        
        auto reset_button = Button("Сбросить к значениям по умолчанию", [&] {
            config_.resetToDefaults();
            min_length_str_ = std::to_string(config_.getMinLength());
            max_length_str_ = std::to_string(config_.getMaxLength());
            strict_mode_ = config_.isStrictMode();
            logger_.info("Configuration reset to defaults");
        });
        
        auto container = Container::Vertical({
            Container::Vertical({
                text("Минимальная длина пароля:") | bold,
                min_length_input
            }),
            Container::Vertical({
                text("Максимальная длина пароля:") | bold,
                max_length_input
            }),
            strict_mode_checkbox,
            Container::Horizontal({
                save_button,
                reset_button
            })
        });
        
        return Renderer(container, [&] {
            return window(
                text("Настройки") | bold | center,
                container->Render()
            ) | flex;
        });
    }
};

int main() {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    
    auto screen = ScreenInteractive::Fullscreen();
    
    PasswordApp app;
    Component ui = app.createUI();
    screen.Loop(ui);
    
    return 0;
}
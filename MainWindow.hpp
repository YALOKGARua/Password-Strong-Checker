#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include "PasswordChecker.hpp"
#include "ConfigManager.hpp"
#include "Logger.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_checkButton_clicked();
    void on_exitButton_clicked();
    void on_showPassword_toggled(bool checked);
    void on_generateButton_clicked();
    void on_configButton_clicked();
    void on_passwordInput_textChanged(const QString& text);
    void on_clearButton_clicked();
    void on_saveConfigButton_clicked();
    void on_loadConfigButton_clicked();

private:
    Ui::MainWindow* ui;
    PasswordChecker checker;
    Logger logger;
    ConfigManager config;

    void setupUI();
    void updatePasswordStrength();
    void showConfigDialog();
    void updatePasswordVisibility(bool show);
    QString generatePassword() const;
    void loadConfiguration();
    void saveConfiguration();
    void setupLogging();
    void updateUI();
};

#endif
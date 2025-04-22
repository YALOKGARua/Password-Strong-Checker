#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    checker(ConfigManager()),
    logger("password_checker.log") {
    ui->setupUi(this);
    setWindowTitle("Password Strength Checker");

    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::on_checkButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton_clicked);
    connect(ui->showPassword, &QCheckBox::toggled, this, &MainWindow::on_showPassword_toggled);

    ui->passwordInput->setEchoMode(QLineEdit::Password);
    logger.log("Application started");
}

MainWindow::~MainWindow() {
    logger.log("Application closed");
    delete ui;
}

void MainWindow::on_checkButton_clicked() {
    QString password = ui->passwordInput->text();
    if (password.isEmpty()) {
        ui->resultOutput->setText("Error: Password cannot be empty");
        logger.log("Empty password check attempted");
        return;
    }

    try {
        PasswordStrength strength = checker.checkPassword(password.toStdString());
        QString result = QString("Strength Level: %1\nDetails:\n%2")
            .arg(QString::fromStdString(checker.strengthToString(strength)))
            .arg(QString::fromStdString(checker.getLastCheckDetails()));
        ui->resultOutput->setText(result);
        logger.log("Password checked. Strength: " + checker.strengthToString(strength));
    }
    catch (const std::exception& e) {
        ui->resultOutput->setText(QString("Error: %1").arg(e.what()));
        logger.log("Error: " + std::string(e.what()));
    }
}

void MainWindow::on_exitButton_clicked() {
    logger.log("Exit button clicked");
    QApplication::quit();
}

void MainWindow::on_showPassword_toggled(bool checked) {
    ui->passwordInput->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    logger.log(checked ? "Password shown" : "Password hidden");
}
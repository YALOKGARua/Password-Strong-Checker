#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Utils.hpp"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    checker(config),
    logger("password_checker.log") {
    ui->setupUi(this);
    setupUI();
    setupLogging();
    loadConfiguration();
    logger.info("Application started");
}

MainWindow::~MainWindow() {
    logger.info("Application closed");
    delete ui;
}

void MainWindow::setupUI() {
    setWindowTitle("Password Strength Checker");
    
    connect(ui->checkButton, &QPushButton::clicked, this, &MainWindow::on_checkButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::on_exitButton_clicked);
    connect(ui->showPassword, &QCheckBox::toggled, this, &MainWindow::on_showPassword_toggled);
    connect(ui->generateButton, &QPushButton::clicked, this, &MainWindow::on_generateButton_clicked);
    connect(ui->configButton, &QPushButton::clicked, this, &MainWindow::on_configButton_clicked);
    connect(ui->passwordInput, &QLineEdit::textChanged, this, &MainWindow::on_passwordInput_textChanged);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
    
    ui->passwordInput->setEchoMode(QLineEdit::Password);
    ui->strengthProgress->setRange(0, 100);
    ui->strengthProgress->setValue(0);
    
    ui->passwordInput->setToolTip("Enter your password here");
    ui->showPassword->setToolTip("Show/Hide password");
    ui->generateButton->setToolTip("Generate a strong password");
    ui->configButton->setToolTip("Configure password requirements");
    ui->clearButton->setToolTip("Clear all fields");
}

void MainWindow::setupLogging() {
    logger.setLogLevel(LogLevel::INFO);
    logger.enableTimestamp(true);
    logger.enableConsoleOutput(false);
}

void MainWindow::on_checkButton_clicked() {
    QString password = ui->passwordInput->text();
    if (password.isEmpty()) {
        ui->resultOutput->setText("Error: Password cannot be empty");
        logger.warning("Empty password check attempted");
        return;
    }

    try {
        PasswordStrength strength = checker.checkPassword(password.toStdString());
        QString result = QString("Strength Level: %1\nDetails:\n%2")
            .arg(QString::fromStdString(checker.strengthToString(strength)))
            .arg(QString::fromStdString(checker.getLastCheckDetails()));
        ui->resultOutput->setText(result);
        
        int progressValue = 0;
        switch (strength) {
            case PasswordStrength::WEAK: progressValue = 25; break;
            case PasswordStrength::MEDIUM: progressValue = 50; break;
            case PasswordStrength::STRONG: progressValue = 75; break;
            case PasswordStrength::VERY_STRONG: progressValue = 100; break;
        }
        ui->strengthProgress->setValue(progressValue);
        
        logger.info("Password checked. Strength: " + checker.strengthToString(strength));
    }
    catch (const std::exception& e) {
        ui->resultOutput->setText(QString("Error: %1").arg(e.what()));
        logger.error("Error: " + std::string(e.what()));
    }
}

void MainWindow::on_generateButton_clicked() {
    try {
        QString password = generatePassword();
        ui->passwordInput->setText(password);
        logger.info("Generated new password");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
        logger.error("Password generation failed: " + std::string(e.what()));
    }
}

QString MainWindow::generatePassword() const {
    size_t length = config.getMinLength() + 4;
    return QString::fromStdString(Utils::generateRandomPassword(
        length, true, true, true, true
    ));
}

void MainWindow::on_configButton_clicked() {
    showConfigDialog();
}

void MainWindow::showConfigDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Password Configuration");
    
    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    
    QSpinBox* minLengthSpinner = new QSpinBox(&dialog);
    minLengthSpinner->setRange(1, 128);
    minLengthSpinner->setValue(config.getMinLength());
    layout->addWidget(new QLabel("Minimum Length:"));
    layout->addWidget(minLengthSpinner);
    
    QCheckBox* strictModeBox = new QCheckBox("Strict Mode", &dialog);
    strictModeBox->setChecked(config.isStrictMode());
    layout->addWidget(strictModeBox);
    
    QDialogButtonBox* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    layout->addWidget(buttons);
    
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        config.setMinLength(minLengthSpinner->value());
        config.setStrictMode(strictModeBox->isChecked());
        logger.info("Configuration updated");
        saveConfiguration();
    }
}

void MainWindow::on_passwordInput_textChanged(const QString& text) {
    updatePasswordStrength();
}

void MainWindow::updatePasswordStrength() {
    if (!ui->passwordInput->text().isEmpty()) {
        on_checkButton_clicked();
    } else {
        ui->strengthProgress->setValue(0);
        ui->resultOutput->clear();
    }
}

void MainWindow::on_showPassword_toggled(bool checked) {
    updatePasswordVisibility(checked);
}

void MainWindow::updatePasswordVisibility(bool show) {
    ui->passwordInput->setEchoMode(show ? QLineEdit::Normal : QLineEdit::Password);
    logger.info(show ? "Password shown" : "Password hidden");
}

void MainWindow::on_clearButton_clicked() {
    ui->passwordInput->clear();
    ui->resultOutput->clear();
    ui->strengthProgress->setValue(0);
    logger.info("Fields cleared");
}

void MainWindow::on_exitButton_clicked() {
    logger.info("Exit button clicked");
    QApplication::quit();
}

void MainWindow::loadConfiguration() {
    QSettings settings("PasswordChecker", "Config");
    
    if (settings.contains("min_length")) {
        config.setMinLength(settings.value("min_length").toUInt());
    }
    if (settings.contains("strict_mode")) {
        config.setStrictMode(settings.value("strict_mode").toBool());
    }
    
    logger.info("Configuration loaded");
}

void MainWindow::saveConfiguration() {
    QSettings settings("PasswordChecker", "Config");
    
    settings.setValue("min_length", config.getMinLength());
    settings.setValue("strict_mode", config.isStrictMode());
    settings.sync();
    
    logger.info("Configuration saved");
}

void MainWindow::on_saveConfigButton_clicked() {
    QString filename = QFileDialog::getSaveFileName(
        this, "Save Configuration",
        QString(), "Configuration Files (*.json)");
        
    if (!filename.isEmpty()) {
        if (config.saveToFile(filename.toStdString())) {
            logger.info("Configuration saved to file: " + filename.toStdString());
        } else {
            logger.error("Failed to save configuration to file: " + filename.toStdString());
            QMessageBox::warning(this, "Error", "Failed to save configuration");
        }
    }
}

void MainWindow::on_loadConfigButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(
        this, "Load Configuration",
        QString(), "Configuration Files (*.json)");
        
    if (!filename.isEmpty()) {
        try {
            if (config.loadFromFile(filename.toStdString())) {
                logger.info("Configuration loaded from file: " + filename.toStdString());
                updateUI();
            } else {
                throw std::runtime_error("Failed to load configuration");
            }
        }
        catch (const std::exception& e) {
            logger.error("Failed to load configuration: " + std::string(e.what()));
            QMessageBox::warning(this, "Error", "Failed to load configuration");
        }
    }
}

void MainWindow::updateUI() {
    ui->strengthProgress->setValue(0);
    ui->resultOutput->clear();
    updatePasswordStrength();
}
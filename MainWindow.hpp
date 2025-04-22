#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
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

private:
    Ui::MainWindow* ui;
    PasswordChecker checker;
    Logger logger;
};

#endif
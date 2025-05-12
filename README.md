# Password Strength Checker

A modern C++ application for checking password strength with a Qt-based GUI interface.

## Features

- Real-time password strength analysis
- Password strength visualization with progress bar
- Password generation with customizable options
- Configurable password requirements
- Common password detection
- Password entropy calculation
- Detailed strength analysis report
- Configuration management (save/load)
- Comprehensive logging system

## Requirements

- C++17 or later
- Qt 5.12 or later
- JsonCpp library
- CMake 3.12 or later
- Visual Studio 2019 or later (for Windows)

## Build Instructions

### Windows (Visual Studio)

1. Install required dependencies:
   ```powershell
   vcpkg install qt5:x64-windows
   vcpkg install jsoncpp:x64-windows
   ```

2. Open the solution in Visual Studio:
   ```powershell
   cd Password-Strong-Checker
   devenv PasswordChecker.sln
   ```

3. Build the solution (Release or Debug configuration)

### Linux/macOS

1. Install dependencies:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default libqt5widgets5 libjsoncpp-dev

   # macOS
   brew install qt5 jsoncpp
   ```

2. Build using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

## Usage

1. Launch the application
2. Enter a password in the input field
3. The strength analysis will update in real-time
4. Use the "Generate" button to create strong passwords
5. Configure password requirements using the "Configuration" button
6. Save/Load configurations for consistent password policies

## Password Strength Criteria

- Minimum length (configurable, default: 8)
- Character types:
  - Uppercase letters
  - Lowercase letters
  - Numbers
  - Special characters
- No common words
- No sequential characters
- No repeating characters
- Entropy calculation

## Configuration Options

- Minimum password length
- Maximum password length
- Strict mode
- Minimum entropy bits
- Custom common word list
- Logging options

## Project Structure

```
Password-Strong-Checker/
├── src/
│   ├── ConfigManager.cpp/hpp    # Configuration management
│   ├── Logger.cpp/hpp           # Logging system
│   ├── MainWindow.cpp/hpp       # GUI interface
│   ├── PasswordChecker.cpp/hpp  # Password analysis
│   ├── TestSuite.cpp/hpp        # Unit tests
│   └── Utils.cpp/hpp           # Utility functions
├── ui/
│   └── MainWindow.xml          # Qt UI definition
└── README.md
```

## License

MIT License

## Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request 
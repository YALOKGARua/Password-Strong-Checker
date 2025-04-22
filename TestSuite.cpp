#include "TestSuite.hpp"
#include <iostream>
#include <cassert>
#include <chrono>

TestSuite::TestSuite(const ConfigManager& config) : checker_(config) {}

void TestSuite::runTests() {
    testEmptyPassword();
    testShortPassword();
    testWeakPassword();
    testMediumPassword();
    testStrongPassword();
    testVeryStrongPassword();
    testCommonWord();
    testRepeatingChars();
    testSequences();
    testEntropy();
    testLongPassword();
    testNoUpperCase();
    testNoLowerCase();
    testNoDigits();
    testNoSpecialChars();
    testMaxLength();
    testUnicodeChars();
    testMixedCase();
    testEdgeCases();
    testPerformance();
    std::cout << "All tests passed successfully!\n";
}

void TestSuite::testEmptyPassword() {
    try {
        checker_.checkPassword("");
        assert(false);
    }
    catch (const std::exception&) {
        assert(true);
    }
}

void TestSuite::testShortPassword() {
    PasswordStrength strength = checker_.checkPassword("abc");
    assert(strength == PasswordStrength::WEAK);
}

void TestSuite::testWeakPassword() {
    PasswordStrength strength = checker_.checkPassword("password");
    assert(strength == PasswordStrength::WEAK);
}

void TestSuite::testMediumPassword() {
    PasswordStrength strength = checker_.checkPassword("Pass123");
    assert(strength == PasswordStrength::MEDIUM);
}

void TestSuite::testStrongPassword() {
    PasswordStrength strength = checker_.checkPassword("Pass123!");
    assert(strength == PasswordStrength::STRONG);
}

void TestSuite::testVeryStrongPassword() {
    PasswordStrength strength = checker_.checkPassword("K9#mP$2nQz");
    assert(strength == PasswordStrength::VERY_STRONG);
}

void TestSuite::testCommonWord() {
    PasswordStrength strength = checker_.checkPassword("admin123");
    assert(strength == PasswordStrength::WEAK);
}

void TestSuite::testRepeatingChars() {
    PasswordStrength strength = checker_.checkPassword("aaa123");
    assert(strength == PasswordStrength::WEAK);
}

void TestSuite::testSequences() {
    PasswordStrength strength = checker_.checkPassword("abc123");
    assert(strength == PasswordStrength::WEAK);
}

void TestSuite::testEntropy() {
    PasswordStrength strength = checker_.checkPassword("X7$kP#9mW2");
    assert(strength == PasswordStrength::VERY_STRONG);
}

void TestSuite::testLongPassword() {
    PasswordStrength strength = checker_.checkPassword("ThisIsAVeryLongPassword123!");
    assert(strength == PasswordStrength::VERY_STRONG);
}

void TestSuite::testNoUpperCase() {
    PasswordStrength strength = checker_.checkPassword("pass123!");
    assert(strength == PasswordStrength::MEDIUM);
}

void TestSuite::testNoLowerCase() {
    PasswordStrength strength = checker_.checkPassword("PASS123!");
    assert(strength == PasswordStrength::MEDIUM);
}

void TestSuite::testNoDigits() {
    PasswordStrength strength = checker_.checkPassword("PassWord!");
    assert(strength == PasswordStrength::MEDIUM);
}

void TestSuite::testNoSpecialChars() {
    PasswordStrength strength = checker_.checkPassword("PassWord123");
    assert(strength == PasswordStrength::MEDIUM);
}

void TestSuite::testMaxLength() {
    std::string long_password(100, 'A');
    PasswordStrength strength = checker_.checkPassword(long_password);
    assert(strength == PasswordStrength::WEAK);
}

void TestSuite::testUnicodeChars() {
    PasswordStrength strength = checker_.checkPassword("Pass123!þ");
    assert(strength == PasswordStrength::STRONG);
}

void TestSuite::testMixedCase() {
    PasswordStrength strength = checker_.checkPassword("pAsSwOrD123!");
    assert(strength == PasswordStrength::STRONG);
}

void TestSuite::testEdgeCases() {
    PasswordStrength strength = checker_.checkPassword("!@#$%^&*()");
    assert(strength == PasswordStrength::MEDIUM);
}

void TestSuite::testPerformance() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        checker_.checkPassword("TestPassword123!");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    assert(duration < 1000);
}
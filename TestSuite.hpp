#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP
#include "PasswordChecker.hpp"
#include "ConfigManager.hpp"

class TestSuite {
public:
    explicit TestSuite(const ConfigManager& config);
    void runTests();

private:
    PasswordChecker checker_;

    void testEmptyPassword();
    void testShortPassword();
    void testWeakPassword();
    void testMediumPassword();
    void testStrongPassword();
    void testVeryStrongPassword();
    void testCommonWord();
    void testRepeatingChars();
    void testSequences();
    void testEntropy();
    void testLongPassword();
    void testNoUpperCase();
    void testNoLowerCase();
    void testNoDigits();
    void testNoSpecialChars();
    void testMaxLength();
    void testUnicodeChars();
    void testMixedCase();
    void testEdgeCases();
    void testPerformance();
};

#endif
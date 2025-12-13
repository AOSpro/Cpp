// Start:🕒 2025-10-07 Tuesday 16:32:30
// Owner:🔧 AOSpro
// Call: 📞 t.me/aospro
#ifndef AOSIO_H
#define AOSIO_H
#include <iostream>
#include <limits>
#include <string>
// ANSI color codes (works in most terminals)
constexpr const char* RESET  = "\033[0m";
constexpr const char* RED    = "\033[31m";
constexpr const char* GREEN  = "\033[32m";
constexpr const char* YELLOW = "\033[33m";
constexpr const char* BLUE   = "\033[34m";
constexpr const char* MAGENTA= "\033[35m";
constexpr const char* CYAN   = "\033[36m";
using namespace std;
// -----------------------------
// Output utilities
// -----------------------------
// Base case: just newline
inline void outnl() {
    cout << endl;
}
// Variadic: print multiple arguments, then newline
template <typename T, typename... Args>
inline void outnl(const T& first, const Args&... rest) {
    cout << first;
    outnl(rest...);
}
// -----------------------------
// Input utilities
// -----------------------------
// Input with error handling (single token)
template <typename T>
inline T innl(const string& prompt = "") {
    T value{};
    while (true) {
        if (!prompt.empty()) {
            cout << CYAN << prompt << RESET;
        }
        if (cin >> value) {
            // success: consume rest of line
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            // failure: clear error flags and discard bad input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "[ERROR] " << RESET;
            cout << "Invalid input. Please try again." << endl;
        }
    }
}
// Input a full line (including spaces)
inline string innll(const string& prompt = "") {
    if (!prompt.empty()) {
        cout << CYAN << prompt << RESET;
    }
    string line;
    getline(cin, line);
    return line;
}
// -----------------------------
// Semantic logging helpers
// -----------------------------
namespace aosio {
    template <typename... Args>
        inline void info(const Args&... args) {
            cout << MAGENTA << "[INFO] " << RESET;
            outnl(args...);
        }
    template <typename... Args>
        inline void warn(const Args&... args) {
            cout << YELLOW << "[WARN] " << RESET;
            outnl(args...);
        }
    template <typename... Args>
        inline void error(const Args&... args) {
            cout << RED << "[ERROR] " << RESET;
            outnl(args...);
        }
    template <typename... Args>
        inline void success(const Args&... args) {
            cout << GREEN << "[OK] " << RESET;
            outnl(args...);
        }
}
#endif // AOSIO_H

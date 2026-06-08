#ifndef SENECA_SPELLCHECKER_H
#define SENECA_SPELLCHECKER_H

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

namespace seneca {

    class SpellChecker {
        static const int SIZE = 6;
        std::string m_badWords[SIZE];
        std::string m_goodWords[SIZE];
        int m_replacements[SIZE] = { 0 };

    public:
        SpellChecker(const char* filename);
        void operator()(std::string& text);
        void showStatistics(std::ostream& out) const;
    };

} // namespace seneca

#endif // SENECA_SPELLCHECKER_H

#define _CRT_SECURE_NO_WARNINGS
#include "spellChecker.h"
#include "mediaItem.h"

using namespace std;

namespace seneca {

    // Constructor: Reads bad/good word pairs from a file.
    SpellChecker::SpellChecker(const char* filename) {
        ifstream fin(filename);  // Open the file
        if (!fin)
            throw "Bad file name!";  // Error if file not found

        string line;
        int count = 0;
        // Read each line until we fill all SIZE slots
        while (getline(fin, line) && count < SIZE) {
            istringstream iss(line);
            iss >> m_badWords[count] >> m_goodWords[count];  // Get bad and good word
            ++count;
        }
        if (count < SIZE)
            throw runtime_error("Insufficient data in file.");  // Not enough pairs
    }

    // Operator(): Replaces all occurrences of bad words with good ones in text.
    void SpellChecker::operator()(string& text) {
        for (int i = 0; i < SIZE; ++i) {
            size_t pos = 0;
            // Loop to replace every occurrence of m_badWords[i]
            while ((pos = text.find(m_badWords[i], pos)) != string::npos) {
                text.replace(pos, m_badWords[i].length(), m_goodWords[i]);
                ++m_replacements[i];  // Count this replacement
                pos += m_goodWords[i].length();  // Move past the replaced word
            }
        }
    }

    // showStatistics: Displays replacement counts for each bad word.
    void SpellChecker::showStatistics(ostream& out) const {
        out << "Spellchecker Statistics" << endl;
        for (int i = 0; i < SIZE; ++i)
            out << left << setw(15) << m_badWords[i] << ": "
            << m_replacements[i] << " replacements" << endl;
    }

} // namespace seneca

#define _CRT_SECURE_NO_WARNINGS
#include "book.h"
using namespace std;

namespace seneca {

    // Displays the book details in table or detailed view based on settings.
    void Book::display(ostream& os) const {
        if (g_settings.m_tableView) {
            // Table view: fixed-width columns with formatted title, country, year, and summary.
            os << "B | ";
            os << left << setfill('.') << setw(50) << getTitle() << " | ";
            os << right << setfill(' ') << setw(2) << m_country << " | ";
            os << setw(4) << getYear() << " | " << left;
            // Print summary based on maximum allowed width.
            if (g_settings.m_maxSummaryWidth > -1) {
                if (static_cast<short>(getSummary().size()) <= g_settings.m_maxSummaryWidth)
                    os << getSummary();
                else
                    os << getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            }
            else {
                os << getSummary();
            }
            os << endl;
        }
        else {
            // Detailed view: prints title, year, author, country, price and summary in chunks.
            size_t currentPos = 0;
            os << getTitle() << " [" << getYear() << "] ["
                << m_author << "] [" << m_country << "] [" << m_price << "]\n";
            // Separator line.
            os << setw(getTitle().size() + 7) << setfill('-') << "" << "\n";
            // Print summary in chunks.
            while (currentPos < getSummary().size()) {
                os << "    " << getSummary().substr(currentPos, g_settings.m_maxSummaryWidth) << "\n";
                currentPos += g_settings.m_maxSummaryWidth;
            }
            // End separator.
            os << setw(getTitle().size() + 7) << setfill('-') << "" << setfill(' ') << "\n";
        }
    }

    // Creates a new Book object from a comma-separated string.
    Book* Book::createItem(const string& bookStr) {
        // Reject empty strings or comments.
        if (bookStr.empty() || bookStr[0] == '#')
            throw "Not a valid book.";

        // Lambda function to trim leading and trailing spaces.
        auto trim = [](string& s) {
            while (!s.empty() && s.front() == ' ')
                s.erase(0, 1);
            while (!s.empty() && s.back() == ' ')
                s.pop_back();
            };

        vector<string> tokens;
        size_t pos = 0, nextPos = 0;
        // Split the input string by commas until we have 5 tokens.
        while ((nextPos = bookStr.find(',', pos)) != string::npos && tokens.size() < 5) {
            string token = bookStr.substr(pos, nextPos - pos);
            trim(token);
            tokens.push_back(token);
            pos = nextPos + 1;
        }
        // Add the final token after the last comma.
        string token = bookStr.substr(pos);
        trim(token);
        tokens.push_back(token);

        // Validate that we have exactly 6 tokens.
        if (tokens.size() != 6)
            throw "Incomplete book data.";

        // Extract book details.
        string author = tokens[0];
        string title = tokens[1];
        string country = tokens[2];
        double price = stod(tokens[3]);
        int year = stoi(tokens[4]);
        string summary = tokens[5];

        // Create and return a new Book object.
        return new Book(title, year, summary, author, country, price);
    }

} // namespace seneca

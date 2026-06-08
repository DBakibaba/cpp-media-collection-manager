#define _CRT_SECURE_NO_WARNINGS
#include "movie.h"  

using namespace std;

namespace seneca {

    void Movie::display(ostream& os) const {
        // Check if the settings require a table view display.
        if (g_settings.m_tableView) {
            // Display in table view format:
            // Prefix with 'M' indicating a Movie.
            os << "M | ";

            // Set text to left-justified and fill with dots.
            // Display the movie title with a width of 50 characters.
            os << left << setfill('.') << setw(50) << getTitle() << " | ";

            // Set text to right-justified and fill with spaces.
            // Display the year with a width of 9 characters.
            os << right << setfill(' ') << setw(9) << getYear() << " | " << left;

            // Check the summary width setting:
            if (g_settings.m_maxSummaryWidth > -1) {
                // If the summary fits within the maximum width, output it directly.
                if (static_cast<short>(getSummary().length()) <= g_settings.m_maxSummaryWidth)
                    os << getSummary();
                else
                    // Otherwise, output a truncated version followed by ellipsis.
                    os << getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            }
            else {
                // If no width constraint is specified, output the entire summary.
                os << getSummary();
            }
            // End the table row.
            os << endl;
        }
        // If not in table view mode, display in a more detailed format.
        else {
            size_t idx = 0;  // Index to keep track of the current position in the summary string

            // Display the title and year in a simple format.
            os << getTitle() << " [" << getYear() << "]\n";

            // Output a separator line; width is the title length plus a constant extra width (7).
            os << setw(getTitle().length() + 7) << setfill('-') << "" << "\n";

            // Loop to display the summary in chunks defined by m_maxSummaryWidth.
            while (idx < getSummary().length()) {
                // Output a chunk of the summary with an indent.
                os << "    " << getSummary().substr(idx, g_settings.m_maxSummaryWidth) << "\n";
                // Advance the index by the maximum summary width.
                idx += g_settings.m_maxSummaryWidth;
            }
            // Output another separator line and reset the fill character.
            os << setw(getTitle().length() + 7) << setfill('-') << ""
                << setfill(' ') << "\n";
        }
    }

    Movie* Movie::createItem(const string& movieStr) {
        // Check if the input string is empty or is a comment (starts with '#').
        if (movieStr.empty() || movieStr[0] == '#')
            throw "Not a valid movie.";

        vector<string> parts;  // Vector to hold the separated tokens
        size_t start = 0;      // Starting index for token extraction

        // Loop to extract the first two tokens (title and year) using commas as delimiters.
        for (int count = 0; count < 2; ++count) {
            size_t commaPos = movieStr.find(',', start);
            // If no comma is found, exit the loop.
            if (commaPos == string::npos)
                break;

            // Extract a token from the current position up to the comma.
            string token = movieStr.substr(start, commaPos - start);

            // Remove leading spaces.
            while (!token.empty() && token.front() == ' ')
                token.erase(0, 1);
            // Remove trailing spaces.
            while (!token.empty() && token.back() == ' ')
                token.pop_back();

            // Save the trimmed token.
            parts.push_back(token);
            // Move past the comma for the next token extraction.
            start = commaPos + 1;
        }

        // Process the final token (summary) after the last comma.
        string token = movieStr.substr(start);
        // Trim leading spaces.
        while (!token.empty() && token.front() == ' ')
            token.erase(0, 1);
        // Trim trailing spaces.
        while (!token.empty() && token.back() == ' ')
            token.pop_back();

        // Add the final trimmed token to the vector.
        parts.push_back(token);

        // Ensure that exactly three tokens were found: title, year, summary.
        if (parts.size() != 3)
            throw "Not a valid movie.";

        // Extract the movie properties from the tokens.
        string title = parts[0];
        int year = stoi(parts[1]);   // Convert the year token to an integer.
        string summary = parts[2];

        // Create a new Movie object using the extracted properties.
        return new Movie(title, year, summary);
    }

} // namespace seneca

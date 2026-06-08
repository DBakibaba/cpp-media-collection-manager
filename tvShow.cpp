#define _CRT_SECURE_NO_WARNINGS

#include "tvShow.h"
#include "collection.h"
#include "mediaItem.h"

using namespace std;

namespace seneca {

    // Displays the TV show details.
    void TvShow::display(ostream& out) const {
        if (g_settings.m_tableView) {
            // Table view output.
            out << "S | ";
            out << left << setfill('.') << setw(50) << getTitle() << " | ";
            out << right << setfill(' ') << setw(2) << m_episodes.size() << " | ";
            out << setw(4) << getYear() << " | " << left;
            if (g_settings.m_maxSummaryWidth > -1) {
                if (getSummary().size() <= static_cast<size_t>(g_settings.m_maxSummaryWidth))
                    out << getSummary();
                else
                    out << getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            }
            else {
                out << getSummary();
            }
            out << endl;
        }
        else {
            // Detailed view output.
            size_t pos = 0;
            out << getTitle() << " [" << getYear() << "]\n";
            out << setw(getTitle().size() + 7) << setfill('-') << "" << '\n';

            // Print summary in chunks.
            while (pos < getSummary().size()) {
                out << "    " << getSummary().substr(pos, g_settings.m_maxSummaryWidth) << '\n';
                pos += g_settings.m_maxSummaryWidth;
            }

            // Print each episode's info.
            for (auto& ep : m_episodes) {
                out << setfill('0') << right;
                out << "    " << 'S' << setw(2) << ep.m_season
                    << "E" << setw(2) << ep.m_numberInSeason << ' ';
                out << (!ep.m_title.empty() ? ep.m_title : "Episode " + to_string(ep.m_numberOverall)) << '\n';

                pos = 0;
                // Print episode summary in smaller chunks.
                while (pos < ep.m_summary.size()) {
                    out << "            " << ep.m_summary.substr(pos, g_settings.m_maxSummaryWidth - 8) << '\n';
                    pos += g_settings.m_maxSummaryWidth - 8;
                }
            }
            out << setw(getTitle().size() + 7) << setfill('-') << "" << setfill(' ') << '\n';
        }
    }

    // Creates a TvShow instance from a comma-separated string.
    TvShow* TvShow::createItem(const string& strshow) {
        if (strshow.empty() || strshow[0] == '#')
            throw "Not a valid show.";

        string data = strshow;
        vector<string> parts;
        size_t pos = 0;

        // Split input string by commas.
        do {
            pos = data.find(',');
            parts.push_back(data.substr(0, pos));
            MediaItem::trim(parts.back());
            data.erase(0, pos + 1);
        } while (pos != string::npos && parts.size() < 3);

        parts.push_back(data);
        MediaItem::trim(parts.back());

        if (parts.size() != 4)
            throw "Not a valid show format.";

        const string& id = parts[0];
        const string& title = parts[1];

        // Validate year contains only digits.
        for (char ch : parts[2])
            if (!isdigit(ch))
                throw "Invalid year format.";
        int year = stoi(parts[2]);

        const string& summary = parts[3];
        return new TvShow(id, title, year, summary);
    }

    // Calculates the average length of episodes.
    double TvShow::getEpisodeAverageLength() const {
        auto total = accumulate(m_episodes.begin(), m_episodes.end(), 0,
            [](unsigned long long sum, const TvEpisode& ep) { return sum + ep.m_length; });
        return static_cast<double>(total) / m_episodes.size();
    }

    // Returns titles of episodes with a length >= 3600 seconds.
    list<string> TvShow::getLongEpisodes() const {
        list<string> longEps;
        for_each(m_episodes.begin(), m_episodes.end(), [&](const TvEpisode& ep) {
            if (ep.m_length >= 3600)
                longEps.push_back(ep.m_title);
            });
        return longEps;
    }

} // namespace seneca

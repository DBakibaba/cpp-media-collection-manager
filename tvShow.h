#define _CRT_SECURE_NO_WARNINGS
#ifndef SENECA_TVSHOW_H
#define SENECA_TVSHOW_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include <iterator>
#include "mediaItem.h"
#include "settings.h"

namespace seneca {

    // TvShow class: Represents a TV show and its episodes.
    class TvShow : public MediaItem {
        // Nested struct to hold episode details.
        struct TvEpisode {
            const TvShow* m_show = nullptr;
            unsigned short m_numberOverall = 0;
            unsigned short m_season = 0;          
            unsigned short m_numberInSeason = 0;    
            std::string m_airDate;                 
            unsigned int m_length = 0;              
            std::string m_title;                  
            std::string m_summary;               

            // Default constructor initializing numeric values.
            TvEpisode() : m_numberOverall(0), m_season(0), m_numberInSeason(0), m_length(0) {}
        };

        std::string m_id;               
        std::list<TvEpisode> m_episodes; 

        // Private constructor; used by createItem to instantiate a TvShow.
        TvShow(const std::string& id, const std::string& title, unsigned short year, const std::string& summary)
            : MediaItem(title, summary, year), m_id(id) {
        }

    public:
        // Displays TV show details (table or detailed view based on settings).
        void display(std::ostream& out) const;

        // Creates a TvShow from a comma-separated string.
        static TvShow* createItem(const std::string& strshow);

        // Template function to add an episode to a TV show within a collection.
        template<typename Collection_t>
        static void addEpisode(Collection_t& collection, const std::string& episodeStr) {
            // Ignore empty lines or comments.
            if (episodeStr.empty() || episodeStr[0] == '#') {
                throw "Not a valid episode.";
            }
            std::string line = episodeStr;
            std::vector<std::string> tokenList;
            size_t pos = 0;
            // Split the episode string by commas.
            do {
                pos = line.find(',');
                tokenList.push_back(line.substr(0, pos));
                MediaItem::trim(tokenList.back());
                if (pos != std::string::npos) {
                    line.erase(0, pos + 1);
                }
            } while (pos != std::string::npos && tokenList.size() < 7);
            // Add the remaining part as the final token.
            tokenList.push_back(line);
            MediaItem::trim(tokenList.back());

            // Ensure we have exactly 8 tokens.
            if (tokenList.size() != 8) {
                throw "Not a valid movie.";
            }

            // Find the matching TV show in the collection.
            TvShow* foundShow = nullptr;
            for (size_t i = 0; i < collection.size(); ++i) {
                if (auto* tvPtr = dynamic_cast<TvShow*>(collection[i])) {
                    if (tvPtr->m_id == tokenList[0]) {
                        foundShow = tvPtr;
                        break;
                    }
                }
            }

            // Create and populate a new episode.
            TvEpisode newEpisode;
            newEpisode.m_show = foundShow;
            try {
                newEpisode.m_numberOverall = static_cast<unsigned short>(std::stoi(tokenList[1]));
                newEpisode.m_season = tokenList[2].empty() ? 1 : static_cast<unsigned short>(std::stoi(tokenList[2]));
                newEpisode.m_numberInSeason = static_cast<unsigned short>(std::stoi(tokenList[3]));
                newEpisode.m_airDate = tokenList[4];

                // Convert HH:MM:SS format into seconds.
                int hrs = 0, mins = 0, secs = 0;
                sscanf(tokenList[5].c_str(), "%2d:%2d:%2d", &hrs, &mins, &secs);
                newEpisode.m_length = hrs * 3600 + mins * 60 + secs;
                newEpisode.m_title = tokenList[6];
                newEpisode.m_summary = tokenList[7];
            }
            catch (const std::exception&) {
                throw "Not a valid episode.";
            }

            // Ensure a matching show was found.
            if (!foundShow) {
                throw "TV Show not found.";
            }
            // Add the new episode to the TV show's episode list.
            foundShow->m_episodes.push_back(newEpisode);
        }

        // Returns the average length of episodes.
        double getEpisodeAverageLength() const;

        // Returns a list of episode titles with a duration of at least 3600 seconds.
        std::list<std::string> getLongEpisodes() const;
    };

}

#endif

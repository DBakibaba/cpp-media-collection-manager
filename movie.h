#ifndef SENECA_MOVIE_H
#define SENECA_MOVIE_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "mediaItem.h"
#include "settings.h"

namespace seneca {

    class Movie : public MediaItem {
        // Private constructor to force creation via createItem.
        Movie(const std::string& title, int year, const std::string& summary)
            : MediaItem(title, summary, year)
        {
        }
    public:
        void display(std::ostream& out) const;
        static Movie* createItem(const std::string& strMovie);
    };

} // namespace seneca

#endif // SENECA_MOVIE_H

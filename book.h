#ifndef SENECA_BOOK_H
#define SENECA_BOOK_H

#include <iomanip>
#include <string>
#include <vector>
#include "mediaItem.h"
#include "settings.h"

namespace seneca {

    class Book : public MediaItem {
        std::string m_author;
        std::string m_country;
        double m_price;

        // Private constructor ensuring controlled creation.
        Book(const std::string& title, int year, const std::string& summary,
            const std::string& author, const std::string& country, double price)
            : MediaItem(title, summary, year),
            m_author(author),
            m_country(country),
            m_price(price)
        {
        }

    public:
        void display(std::ostream& out) const;
        static Book* createItem(const std::string& strBook);
    };

} // namespace seneca

#endif // SENECA_BOOK_H

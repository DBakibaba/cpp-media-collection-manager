#define _CRT_SECURE_NO_WARNINGS
#include "collection.h"
#include "mediaItem.h"

namespace seneca {

    // Constructor: Initializes the collection with a name.
    Collection::Collection(const std::string& name)
        : m_collectionName(name)
    {
    }

    // Destructor: Deletes all dynamically allocated media items.
    Collection::~Collection() {
        for (MediaItem* item : m_media) {
            delete item;
        }
    }

    // Returns the collection's name.
    const std::string& Collection::name() const {
        return m_collectionName;
    }

    // Returns the number of media items in the collection.
    size_t Collection::size() const {
        return m_media.size();
    }

    // Sets the observer function that gets notified when a new media item is added.
    void Collection::setObserver(void (*observer)(const Collection&, const MediaItem&)) {
        m_observer = observer;
    }

    // Adds a media item to the collection if not already present (based on title).
    // Notifies the observer if set.
    Collection& Collection::operator+=(MediaItem* item) {
        for (MediaItem* existing : m_media) {
            if (existing->getTitle() == item->getTitle()) {
                delete item;  // Avoid duplicate: delete new item.
                return *this;
            }
        }
        m_media.push_back(item);
        if (m_observer) {
            m_observer(*this, *item);
        }
        return *this;
    }

    // Returns the media item at the specified index.
    // Throws an exception if the index is out of range.
    MediaItem* Collection::operator[](size_t idx) const {
        if (idx >= m_media.size()) {
            std::string err = "Bad index [" + std::to_string(idx) +
                "]. Collection has [" + std::to_string(m_media.size()) + "] items.";
            throw std::out_of_range(err);
        }
        return m_media[idx];
    }

    // Returns the first media item that matches the given title.
    // Returns nullptr if not found.
    MediaItem* Collection::operator[](const std::string& title) const {
        auto iter = std::find_if(m_media.begin(), m_media.end(), [&title](const MediaItem* item) {
            return item->getTitle() == title;
            });
        return (iter != m_media.end()) ? *iter : nullptr;
    }

    // Removes surrounding quotes from titles and summaries of all media items.
    void Collection::removeQuotes() {
        for (MediaItem* item : m_media) {
            std::string title = item->getTitle();
            // Remove quotes from title if present.
            if (!title.empty()) {
                if (title.front() == '"' && title.back() == '"')
                    title = title.substr(1, title.size() - 2);
                else {
                    if (title.front() == '"')
                        title = title.substr(1);
                    if (!title.empty() && title.back() == '"')
                        title = title.substr(0, title.size() - 1);
                }
            }
            item->setTitle(title);

            std::string summary = item->getSummary();
            // Remove quotes from summary if present.
            if (!summary.empty()) {
                if (summary.front() == '"' && summary.back() == '"')
                    summary = summary.substr(1, summary.size() - 2);
                else {
                    if (summary.front() == '"')
                        summary = summary.substr(1);
                    if (!summary.empty() && summary.back() == '"')
                        summary = summary.substr(0, summary.size() - 1);
                }
            }
            item->setSummary(summary);
        }
    }

    // Sorts the collection based on the specified field ("title", "year", or "summary").
    void Collection::sort(const std::string& field) {
        std::sort(m_media.begin(), m_media.end(), [&field](const MediaItem* a, const MediaItem* b) -> bool {
            if (field == "title")
                return a->getTitle() < b->getTitle();
            if (field == "year")
                return a->getYear() < b->getYear();
            if (field == "summary")
                return a->getSummary() < b->getSummary();
            return false;
            });
    }

    // Overloaded output operator to print all media items in the collection.
    std::ostream& operator<<(std::ostream& os, const Collection& col) {
        for (size_t i = 0; i < col.size(); ++i) {
            os << *col[i];
        }
        return os;
    }

} // namespace seneca

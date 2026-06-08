#ifndef SENECA_COLLECTION_H
#define SENECA_COLLECTION_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "mediaItem.h"

namespace seneca {

	class Collection {
		std::string m_collectionName;
		std::vector<MediaItem*> m_media;
		void (*m_observer)(const Collection&, const MediaItem&) = nullptr;

	public:
		// Constructor with collection name.
		explicit Collection(const std::string& name);

		// Delete copy and move constructors/assignments.
		Collection(const Collection&) = delete;
		Collection& operator=(const Collection&) = delete;
		Collection(Collection&&) = delete;
		Collection& operator=(Collection&&) = delete;
		~Collection();

		// Member functions.
		const std::string& name() const;
		size_t size() const;
		void setObserver(void (*observer)(const Collection&, const MediaItem&));
		Collection& operator+=(MediaItem* item);
		MediaItem* operator[](size_t idx) const;
		MediaItem* operator[](const std::string& title) const;
		void removeQuotes();
		void sort(const std::string& field);
	};

	// Overloaded output operator.
	std::ostream& operator<<(std::ostream& os, const Collection& collection);

} // namespace seneca

#endif // SENECA_COLLECTION_H

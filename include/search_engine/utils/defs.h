#pragma once

#include <map>
#include <string>
/**
 * @brief Represents an entry in the inverted index, containing a document ID and a count.
 */
struct Entry
{
    size_t doc_id, count;
};

/**
 * @brief Represents an answer object, containing a request number, results, and a flag.
 */

struct Answer {
    std::string requestNum;
    std::map<int, double> result;
    bool flag;
};
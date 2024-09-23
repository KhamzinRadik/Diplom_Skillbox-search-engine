#include "SearchServer.h"

#include <spdlog/spdlog.h>

#include <algorithm>


std::set<std::string> SearchServer::getUniqueWords(const std::string &request) {
    std::set < std::string > result;
    std::istringstream ist(request);
    for (std::string word; ist >> word;) {
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        result.emplace(word);
    }
    return result;
}

std::vector<std::pair<std::string, size_t>> SearchServer::getWordsEntries(const std::set<std::string> &words) {
    std::vector<std::pair<std::string, size_t>> result;
    for (const auto &word: words) {
        auto wordEntries = _index.getWordCount(word);
        size_t wordEntriesSum = 0;
        for (auto wordEntry: wordEntries) {
            wordEntriesSum += wordEntry.count;
        }
        std::pair<std::string, size_t> wordAndEntry;
        wordAndEntry.first = word;
        wordAndEntry.second = wordEntriesSum;
        result.push_back(wordAndEntry);
    }
    return result;
}

std::vector<size_t> SearchServer::getAllDocumentsWithWords(const std::vector<std::pair<std::string, size_t>> &words) {
    std::vector<size_t> docIds{};
    // Getting entries and docIds:
    for (const auto &wordAndEntry: words) {
        auto entries = _index.getWordCount(wordAndEntry.first);
        for (auto entry: entries) {
            docIds.push_back(entry.doc_id);
        }
    }

    std::set < size_t > uniqueDocIds(docIds.begin(), docIds.end());
    docIds.clear();
    docIds.assign(uniqueDocIds.begin(), uniqueDocIds.end());
    std::sort(docIds.begin(), docIds.end(), std::less<size_t>());
    return docIds;
}

void SearchServer::sortWordsAscendingToEntries(std::vector<std::pair<std::string, size_t>> &wordsEntries) {
    std::sort(wordsEntries.begin(), wordsEntries.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
}

size_t SearchServer::getAbsoluteRelevanceForDocument(size_t docId, std::set<std::string> &uniqueWords) {
    size_t absoluteRelevance{0};
    for (const auto &word: uniqueWords) {
        size_t wordCountInDoc = _index.getWordCountInDoc(word, docId);
        absoluteRelevance += wordCountInDoc;
    }
    return absoluteRelevance;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> result{};
    if (queries_input.empty()) {
        spdlog::info("Requests are empty.");
        return result;
    }

    for (const auto &query: queries_input) {
        // Get unique words from query
        std::set < std::string > uniqueWords = getUniqueWords(query);
        if (uniqueWords.empty()) {
            spdlog::info( "\t-bad request.");
            continue;
        }

        auto wordsEntries = getWordsEntries(uniqueWords);

        sortWordsAscendingToEntries(wordsEntries);

        auto documentIds = getAllDocumentsWithWords(wordsEntries);
        std::string docOrDocs = documentIds.size() == 1 ? " document " : " documents ";
        std::string wordOrWords = uniqueWords.size() == 1 ? " word: " : " words: ";


        auto *relativeIndexes = new std::vector<RelativeIndex>();
        size_t maxAbsoluteRelevance{0};
        for (const auto &docId: documentIds) {
            size_t absoluteRelevance = getAbsoluteRelevanceForDocument(docId, uniqueWords);
            auto *relativeIndex = new RelativeIndex();
            relativeIndex->doc_id = docId;
            relativeIndex->absoluteIndex = absoluteRelevance;
            relativeIndexes->push_back(*relativeIndex);
            delete relativeIndex;
            if (absoluteRelevance > maxAbsoluteRelevance) maxAbsoluteRelevance = absoluteRelevance;
        }

        for (auto &relativeIndex: *relativeIndexes) {
            if (maxAbsoluteRelevance != 0) {
                float rank = (float) relativeIndex.absoluteIndex / (float) maxAbsoluteRelevance;
                int rounded = (int) std::round(rank * 100);
                rank = (float) rounded / 100;
                relativeIndex.rank = rank;
            } else relativeIndex.rank = 0;
        }

        std::sort(relativeIndexes->begin(), relativeIndexes->end(),
                  [&relativeIndexes](RelativeIndex &left, RelativeIndex &right) {
                      return (left.rank > right.rank || (left.rank == right.rank && left.doc_id < right.doc_id));
                  });

        if (relativeIndexes->size() > maxResponses) {
            relativeIndexes->erase(relativeIndexes->begin() + maxResponses, relativeIndexes->end());
        }

        result.push_back(*relativeIndexes);
        delete relativeIndexes;
    }
    return result;
}

void SearchServer::setMaxResponses(const int &newMaxResponses) {
    this->maxResponses = newMaxResponses;
}
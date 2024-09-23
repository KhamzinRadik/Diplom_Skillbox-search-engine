#include "InvertedIndex.h"

#include <spdlog/spdlog.h>

std::mutex InvertedIndex::mutexIndexMap;

void InvertedIndex::updateDocumentBase(const std::vector<std::string> &input_docs) {
    if (input_docs.empty()) {
        spdlog::info("\t- Indexing: no content in docs content base");
        return;
    }

    indexingIsOngoing = true;

    frequencyDictionary.clear();
    size_t docId = 0;
    for (const auto &content: input_docs) {
        // Start indexing thread.
        std::thread index([this, &content, docId]() { indexTheFile(content, docId); });
        ++docId;
        index.join();
    }
    indexingIsOngoing = false;
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string &word) {
    if (indexingIsOngoing) {
        spdlog::info("Index is ongoing, please repeat the request later.");
        return {};
    }
    auto it = frequencyDictionary.find(word);
    if (it != frequencyDictionary.end()) {
        return it->second;
    } else {
        return {};
    }
}

void InvertedIndex::indexTheFile(const std::string &fileContent, size_t docId) {
    // Split doc on words.
    std::map<std::string, Entry> fileFreqDictionary;
    Entry entry{};
    entry.doc_id = docId;
    entry.count = 1;
    std::istringstream ist(fileContent);
    for (std::string word; ist >> word;) {
        //Convert symbols to lower case:
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        std::pair<std::string, Entry> file_word_frequency{word, entry};
        if (!fileFreqDictionary.emplace(file_word_frequency).second) {
            fileFreqDictionary.find(word)->second.count += 1;
        }
    }
    // Place the word in map.
    mutexIndexMap.lock();
    for (const auto &wordIterator: fileFreqDictionary) {
        std::pair<std::string, std::vector<Entry>> wordFrequency;
        wordFrequency.first = wordIterator.first;
        std::vector<Entry> entryVector{wordIterator.second};
        wordFrequency.second = entryVector;
        if (!frequencyDictionary.emplace(wordFrequency).second) {
            frequencyDictionary.find(wordFrequency.first)->second.push_back(wordIterator.second);
        }
    }
    mutexIndexMap.unlock();
}

size_t InvertedIndex::getWordCountInDoc(const std::string &word, const size_t doc_id) const {
    if (indexingIsOngoing) {
        spdlog::info("Index is ongoing, please repeat the request later.");
        return 0;
    }
    size_t wordCountInDoc = 0;
    auto it = frequencyDictionary.find(word);
    if (it != frequencyDictionary.end()) {
        auto entryVector = it->second;
        for (auto &entry: entryVector) {
            if (entry.doc_id == doc_id) wordCountInDoc += entry.count;
        }
    } else {
        spdlog::info("Word \"" + word + "\" not found.");
    }
    return wordCountInDoc;
}




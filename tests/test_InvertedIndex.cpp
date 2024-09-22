#include "InvertedIndex.h"
#include "ConverterJson.h"
#include <gtest/gtest.h>


TEST(TestCaseInvertedIndex, TestBasic) {
    std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    std::vector<std::string> requests = {"london", "the"};
    std::vector<std::vector<std::pair<int, int>>> expected = {
            {
                    {0, 1}
            },
            {
                    {0, 1}, {1, 3}
            }
    };

    std::vector<std::vector<std::pair<int, int>>> result;
    ConverterJson conv(docs, requests);
    InvertedIndex idx(conv);
    idx.UpdateDocumentBase(docs);
    idx.threadsDistribution();
    for (const auto& request: requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        std::vector<std::pair<int, int>> tempPairVec;
        for (auto it2: word_count) {
            std::pair<int, int> tempPair;
            tempPair.first = it2.doc_id;
            tempPair.second = it2.count;
            tempPairVec.push_back(tempPair);
        }
        result.push_back(tempPairVec);
    }

    ASSERT_EQ(result, expected);
}


TEST(TestCaseInvertedIndex, TestBasic2) {
    std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    std::vector<std::string> requests = {"milk", "water", "cappuccino"};
    std::vector<std::vector<std::pair<int, int>>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            },
            {
                    {0, 3}, {1, 2}, {2, 5}
            },
            {
                    {3, 1}
            }
    };
    std::vector<std::vector<std::pair<int, int>>> result;
    ConverterJson conv(docs, requests);
    InvertedIndex idx(conv);
    idx.UpdateDocumentBase(docs);
    idx.threadsDistribution();
    for (auto request: requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        std::vector<std::pair<int, int>> tempPairVec;
        for (auto it2: word_count) {
            std::pair<int, int> tempPair;
            tempPair.first = it2.doc_id;
            tempPair.second = it2.count;
            tempPairVec.push_back(tempPair);
        }
        result.push_back(tempPairVec);
    }

    ASSERT_EQ(result, expected);
}


TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    std::vector<std::string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    std::vector<std::string> requests = {"m", "statement"};
    std::vector<std::vector<std::pair<int, int>>> expected = {
            {},
            {{1, 1}}
    };
    std::vector<std::vector<std::pair<int, int>>> result;
    ConverterJson conv(docs, requests);
    InvertedIndex idx(conv);
    idx.UpdateDocumentBase(docs);
    idx.threadsDistribution();
    for (const auto& request: requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        std::vector<std::pair<int, int>> tempPairVec;
        for (auto it2: word_count) {
            std::pair<int, int> tempPair;
            tempPair.first = it2.doc_id;
            tempPair.second = it2.count;
            tempPairVec.push_back(tempPair);
        }
        result.push_back(tempPairVec);
    }

    ASSERT_EQ(result, expected);
}

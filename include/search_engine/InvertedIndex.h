#pragma once

/**
 * @file InvertedIndex.h
 * @brief Header file for the InvertedIndex class, which provides functionality for creating an inverted index.
 */
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include "ConverterJson.h"



/**
 * @brief Class for creating an inverted index.
 */
class InvertedIndex
{
public:
    /**
     * @brief Constructor for the InvertedIndex class, taking a ConverterJson object as input.
     *
     * @param inJson A ConverterJson object.
     */
    InvertedIndex(ConverterJson &inJson) : m_jsonData(inJson) {};

    /**
     * @brief Updates the document base with a vector of input documents.
     *
     * @param input_docs A vector of input documents.
     */
    void UpdateDocumentBase(std::vector<std::string>& input_docs);

    /**
     * @brief Gets the word count for a given word.
     *
     * @param word The word to search for.
     * @return A vector of entries representing the word count.
     */
    std::vector<Entry> GetWordCount(const std::string &word);

    /**
     * @brief Infills the frequency dictionary in a thread.
     *
     * @param textFromDoc The text document to process.
     */
    void freqDictInfillThread(std::string &textFromDoc);

    /**
     * @brief Merges the data from multiple threads.
     */
    void dataMerge();

    /**
     * @brief Distributes the work among threads.
     */
    void threadsDistribution();

    /**
     * @brief Checks if a character is valid for word formation.
     *
     * @param symbol The character to check.
     * @return True if the character is valid, false otherwise.
     */
    bool characterCondition(char &symbol);

    /**
     * @brief Checks if two characters form a valid word boundary.
     *
     * @param symbol The first character.
     * @param nextSymbol The second character.
     * @return True if the characters form a valid word boundary, false otherwise.
     */
    static bool wordCondition(char &symbol, char &nextSymbol);

    /**
     * @brief Gets the frequency dictionary.
     *
     * @return A pointer to the frequency dictionary.
     */
    std::map<std::string, std::vector<Entry>> *getFreqDictionary();

private:
    std::map<std::string, std::vector<Entry>> m_freq_dictionary;  /**< Frequency dictionary. */
    std::vector<std::string> m_docs;           /**< Vector of documents. */
    ConverterJson m_jsonData;                  /**< ConverterJson object. */
    std::vector<std::string> m_threads;         /**< Vector of threads. */
    std::vector<InvertedIndex> m_classParts;     /**< Vector of InvertedIndex class parts. */
};


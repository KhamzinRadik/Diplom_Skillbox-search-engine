#pragma once

/**
 * @file SearchServer.h
 * @brief Header file for the SearchServer class, which provides functionality for searching through an inverted index.
 */
#include <vector>
#include <iostream>
#include <map>
#include "InvertedIndex.h"
#include "ConverterJson.h"

/**
 * @brief Class for searching through an inverted index.
 */
class SearchServer {
public:
    /**
     * @brief Constructor for the SearchServer class, taking an InvertedIndex and ConverterJson object as input.
     *
     * @param inIdx An InvertedIndex object.
     * @param inJson A ConverterJson object.
     */
    SearchServer(InvertedIndex &inIdx, ConverterJson &inJson) : m_index(inIdx), m_dataJson(inJson) {};

    /**
     * @brief Searches for queries in the inverted index.
     *
     * @param queries_input A vector of queries.
     * @return A vector of vectors of pairs of integers and floats representing the search results.
     */
    std::vector<std::vector<std::pair<int, float>>> searchFoo(const std::vector<std::string> &queries_input);

private:
    InvertedIndex m_index;  /**< Inverted index. */
    ConverterJson m_dataJson;  /**< ConverterJson object. */
    std::map<std::string, int> m_uniqRequests;  /**< Map of unique requests. */
    std::multimap<int, int> m_preRelevance;  /**< Multimap of pre-relevance. */
    std::vector<std::vector<std::pair<int, float>>> m_result;  /**< Vector of results. */
    std::vector<std::pair<int, float>> m_relativeIndex;  /**< Vector of relative index. */

    /**
     * @brief Fills the map of unique requests.
     *
     * @param request The request to add.
     */
    void uniqRequestsFill(const std::string &request);

    /**
     * @brief Fills the multimap of pre-relevance.
     */
    void preRelevanceFill();

    /**
     * @brief Finds the maximum absolute relevance.
     *
     * @return The maximum absolute relevance.
     */
    int findMaxAbsRel();

    int m_maxAbsRelevance;  /**< Maximum absolute relevance. */

    /**
     * @brief Sorts the relative index.
     *
     * @return The sorted relative index.
     */
    std::vector<std::pair<int, float>> sortRelativeIndex();
};

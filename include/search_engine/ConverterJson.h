#pragma once

/**
 * @file ConverterJson.h
 * @brief Header file for the ConverterJson class, which provides functionality for converting JSON data.
 */
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

#include "utils/defs.h"

/**
 * @brief Class for converting JSON data.
 */
class ConverterJson {
public:
    /**
     * @brief Default constructor for the ConverterJson class.
     */
    ConverterJson() = delete;
    /**
     * @brief Constructor for the ConverterJson class with dir for configs.json.
     */
    explicit ConverterJson(std::ifstream &_configRead);

    /**
     * @brief Constructor for the ConverterJson class, taking input documents and requests.
     *
     * @param inDocs A vector of input documents.
     * @param inReqs A vector of input requests.
     */
    ConverterJson(std::vector<std::string> &inDocs, std::vector<std::string> &inReqs);

    /**
     * @brief Gets the vector of text documents.
     *
     * @return A vector of text documents.
     */
    std::vector<std::string> &GetTextDocuments();

    /**
     * @brief Gets the number of files.
     *
     * @return The number of files.
     */
    int GetFilesNum();

    /**
     * @brief Gets the response limit.
     *
     * @return The response limit.
     */
    int GetResponseLimit() const;

    /**
     * @brief Gets the vector of requests data.
     *
     * @return A vector of requests data.
     */
    std::vector<std::string> GetRequestsData();

    /**
     * @brief Puts answers into the ConverterJson object.
     *
     * @param answers A vector of vectors of pairs of integers and floats.
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

private:
    /**
     * @brief Reads the configuration file.
     */
    void readConfig();

    /**
     * @brief Sets the response limit.
     */
    void ResponseLimit();

    /**
     * @brief Gets the requests.
     *
     * @return A vector of requests.
     */
    std::vector<std::string> GetRequests();

    int m_numOfFiles{};  /**< Number of files. */
    int m_respLimit{};   /**< Response limit. */
    std::vector<std::string> m_textFromDocs;  /**< Vector of text documents. */
    std::vector<std::string> m_requests;      /**< Vector of requests. */
    std::vector<std::string> m_filePaths;     /**< Vector of file paths. */

    nlohmann::json m_configFile;  /**< Configuration file. */
    nlohmann::json m_requestFile; /**< Request file. */
    nlohmann::json m_answerFile;  /**< Answer file. */
    Answer m_ans;        /**< Answer object. */
};




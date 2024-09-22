#pragma once

/**
 * @file JsonFill.h
 * @brief Header file for the JsonFill class, which provides functionality for filling JSON files.
 */
#include <nlohmann/json.hpp>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


/**
 * @brief Class for filling JSON files.
 */
class JsonFill {
private:
    /**
 * @brief Represents a configuration file.
 */
    struct ConfigFile {
        std::map<std::string, std::string> config;
        std::vector<std::string> files;
    };

    /**
 * @brief Represents a request file.
 */
    struct RequestFile {
        std::vector<std::string> requests;
    };

/**
 * @brief Represents an answer file.
 */
    struct AnswerFile {
    };
    ConfigFile m_configExample;  /**< Configuration file example. */
    RequestFile m_requestExample;  /**< Request file example. */
    AnswerFile m_answerExample;  /**< Answer file example. */

public:
    /**
     * @brief Fills a JSON file of the specified type.
     *
     * @param type The type of JSON file to fill (e.g., "config", "request", "answer").
     */
    void fillFile(const std::string &type);

    /**
     * @brief Prints a JSON file of the specified type to the specified path.
     *
     * @param type The type of JSON file to print.
     * @param path The path to print the file to.
     */
    void printFile(const std::string &type, const std::string &path);
};


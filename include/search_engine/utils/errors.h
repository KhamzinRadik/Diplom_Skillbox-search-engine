#pragma once

#include <exception>
#include <string>
/**
 * @brief Exception class for when the "config.json" file cannot be opened.
 */
class failFileOpenException : public std::exception {
public:
    const char *what() const noexcept override {
        return "No file 'config.json'!";
    }
};

/**
 * @brief Exception class for when the "config" field is not found in the "config.json" file.
 */
class failConfigException : public std::exception {
public:
    const char *what() const noexcept override {
        return "No field 'config' in 'config.json'!";
    }
};

/**
 * @brief Exception class for when all documents in the search base are empty.
 */
class failEmptyDataBase : public std::exception {
public:
    const char *what() const noexcept override {
        return "All documents in search base are empty!";
    }
};

#include "ConverterJson.h"
#include "utils/defs.h"

#include <spdlog/spdlog.h>

#include <iostream>




ConverterJSON *ConverterJSON::instance = nullptr;

ConverterJSON *ConverterJSON::getInstance() {
    if (instance == nullptr) {
        instance = new ConverterJSON();
    }
    return instance;
}

std::vector<std::string> ConverterJSON::getTextDocuments() {
    textDocuments.clear();
    for (const auto &doc: resourcesPaths) {
        std::ifstream docReadingStream(doc);
        if (docReadingStream.is_open()) {
            std::string buffer;
            while (!docReadingStream.eof()) {
                std::string b;
                docReadingStream >> b;
                buffer += b;
                buffer += " ";
            }
            textDocuments.push_back(buffer);
            docReadingStream.close();
        } else {
            spdlog::error("File content reading:\t- file not found error " + doc);
        }
    }
    spdlog::info("Input docs read success: %d, %s",resourcesPaths.size(), " files");
    return textDocuments;
}

int ConverterJSON::getResponsesLimit() const {

    return maxResponses;
}

std::vector<std::string> ConverterJSON::getRequests() {
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {
    nlohmann::json answersDict;
    if (!answers.empty()) {
        spdlog::info("Answers pushing... ");
        std::ofstream answersFile(ANSWERS_FILE_PATH, std::ios_base::trunc);
        if (answersFile.is_open()) {
            int requestCount{0};
            nlohmann::json answerDictionary;
            for (auto request: answers) {
                answersDict["answers"]["request" + std::to_string(requestCount)]["result"] = !request.empty();
                if (request.size() == 1) {
                    answersDict["answers"]["request" + std::to_string(requestCount)]["docId"] = request[0].first;
                    answersDict["answers"]["request" + std::to_string(requestCount)]["rank"] = request[0].second;
                } else {
                    auto relevance_array = nlohmann::json::array();
                    int relevanceID{0};
                    for (auto relevance: request) {
                        ++relevanceID;
                        if (relevanceID > maxResponses) break;

                        auto relevance_member = nlohmann::json::object();
                        relevance_member["docId"] = relevance.first;
                        relevance_member["rank"] = relevance.second;
                        relevance_array.push_back(relevance_member);
                    }
                    answersDict["answers"]["request" + std::to_string(requestCount)]["relevance"] = relevance_array;
                }
                ++requestCount;
            }
            answersFile << answersDict;
            answersFile.close();
            spdlog::info("done");
        } else {
            spdlog::error("\t error - file not found: " + ANSWERS_FILE_PATH);
        }
    } else {
        spdlog::error("No answers to push.");
    }
}

void ConverterJSON::readConfigFile(std::string path) {
    std::ifstream configFile(path);
    if (configFile.is_open()) {
        nlohmann::json configDictionary;
        configFile >> configDictionary;
        applicationName = configDictionary["config"]["name"];
        applicationVersion = configDictionary["config"]["version"];
        maxResponses = configDictionary["config"]["max_responses"];
        resourcesPaths.clear();
        for (const auto &f: configDictionary["files"]) {
            resourcesPaths.push_back(f);
        }

        for (int i = 0; i < HEADER_SPACER; ++i) { std::cout << "="; }
        spdlog::info("[Initialization]");
        for (int i = 0; i < HEADER_SPACER; ++i) { std::cout << "="; }
        spdlog::info(applicationName);
        spdlog::info("Version: " + applicationVersion);
        spdlog::info(&"Max responses per request: "[maxResponses]);
        spdlog::info(&"Files library: "[resourcesPaths.size()]);
        for (int i = 0; i < LINE_LENGTH; ++i) { std::cout << "-"; }
        std::cout << "\n";
        configFile.close();
    } else {
        spdlog::error("\t - file not found error: " + path);
    }
}

void ConverterJSON::readRequestFile(std::string path) {
    spdlog::info("Requests reading: ");
    std::ifstream configFile(path);
    if (configFile.is_open()) {
        nlohmann::json requestsDictionary;
        configFile >> requestsDictionary;
        requests.clear();
        for (const auto& f: requestsDictionary["requests"]) {
            requests.push_back(f);
        }
        configFile.close();
        std::string requestOrRequests = requests.size() == 1 ? " request is " : " requests are ";
        spdlog::info(&""[requests.size()] + requestOrRequests + "found");
    } else {
        spdlog::info("\t - file not found error: " + path);
    }
}

int ConverterJSON::getMaxResponses() const {
    return maxResponses;
}

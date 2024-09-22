#include "ConverterJson.h"

#include "utils/errors.h"
#include <string>
#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>

ConverterJson::ConverterJson(std::ifstream &configRead) {
    spdlog::info("ConverterJson: run");
    if (!configRead.is_open()) {
        throw failFileOpenException();
    }
    try {
        configRead >> m_configFile;
    }
    catch (const failFileOpenException &x) {
        spdlog::error("Caught exception: can't open file %s", x.what());
    }
    configRead.close();

    readConfig(); //check for config field in the file and max.resp. limit

    std::ifstream requestRead("../configs/request.json");
    requestRead >> m_requestFile;
    requestRead.close();
}

void ConverterJson::readConfig() {
    auto it_find = m_configFile.find("config");
    if (it_find == m_configFile.end()) {
        throw failConfigException();
    }
    try {
        m_filePaths.clear();
        auto it_files = m_configFile.find("files");
        if (it_files != m_configFile.end()) {
            for (auto it2 = it_files.value().begin(); it2 != it_files.value().end(); it2++) {
                m_filePaths.push_back(it2.value());

            }
        }

    }
    catch (const failConfigException &x) {
        spdlog::error("Can't find 'config' field in the file 'config.json'. Check this file.");
    }

    ResponseLimit();
}

std::vector<std::string> &ConverterJson::GetTextDocuments() {
    m_textFromDocs.clear();

    for (const auto &filePath: m_filePaths) {
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::string tmp1{}, tmp2{};
            while (!file.eof()) {
                file >> tmp1;
                if (!tmp1.empty()) {
                    tmp2 += " " + tmp1;
                }
            }
            if (!tmp2.empty()) {
                m_textFromDocs.push_back(tmp2);
            } else {
                spdlog::info("File '{}' is empty. Please check.", filePath);
            }
        } else {
            spdlog::error("Can't open file '{}'. Please check data in 'config.json' and file address.", filePath);
        }
    }

    if (m_textFromDocs.empty()) {
        throw failEmptyDataBase();
    }

    return m_textFromDocs;
}


void ConverterJson::ResponseLimit() {
    auto it_config = m_configFile.find("config");
    if (it_config != m_configFile.end()) {
        auto it_resp = it_config->find("max_responses");
        if (it_resp != it_config->end()) {
            std::string tmp = it_resp.value();
            try {
                m_respLimit = std::stoi(tmp);
            } catch (std::invalid_argument &e) {
                spdlog::error("Invalid value for 'max_responses': {}", e.what());
                spdlog::info("Using default value of {}", m_respLimit);
            } catch (std::out_of_range &e) {
                spdlog::error("Value for 'max_responses' is out of range: {}", e.what());
                spdlog::info("Using default value of {}", m_respLimit);
            }
        } else {
            spdlog::info("Can't find field 'max_responses' in 'config.json'. Using default value of {}", m_respLimit);
        }
    }
}

int ConverterJson::GetResponseLimit() const {
    return m_respLimit;
}

std::vector<std::string> ConverterJson::GetRequests() {
    m_requests.clear();
    for (auto it = m_requestFile.begin().value().begin(); it != m_requestFile.begin().value().end(); it++) {
        m_requests.push_back(it.value());
    }
    return m_requests;
}

void ConverterJson::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers, const std::string &dir_answers) {
    std::ofstream file(dir_answers, std::ios::out);
    std::map<std::string, nlohmann::json> outerLayer;
    std::vector<std::pair<std::pair<std::string, int>, std::pair<std::string, float>>> vecData;

    for (int i = 0; i < answers.size(); ++i) {
        float check = 0;
        for (const auto& j : answers[i]) {
            check += j.second;
        }

        m_ans.flag = (check > 0);

        nlohmann::json tmp;
        tmp["result"] = m_ans.flag;

        if (m_ans.flag) {
            int var = 0;
            for (const auto& j : answers[i]) {
                if (j.second > 0) {
                    std::pair<std::string, int> pair1 = {"docID", j.first};
                    std::pair<std::string, float> pair2 = {"rank", floorf(100 * j.second) / 100};
                    vecData.emplace_back(pair1, pair2);
                    ++var;
                }
            }

            if (var > 1) {
                tmp["relevance"] = vecData;
            } else {
                tmp["docID"] = answers[i].begin()->first;
                tmp["rank"] = answers[i].begin()->second;
            }

            vecData.clear();
        }

        std::stringstream request;
        request << "request00" << i + 1;
        outerLayer.insert({request.str(), tmp});
    }

    m_answerFile["answers"] = outerLayer;
    file.precision(4);
    file << std::setw(4) << m_answerFile << std::endl;
}

std::vector<std::string> ConverterJson::GetRequestsData() {
    GetRequests();
    const std::vector<std::string> res = m_requests;
    return res;
}

int ConverterJson::GetFilesNum() {
    m_numOfFiles = m_textFromDocs.size();
    return m_numOfFiles;
}

ConverterJson::ConverterJson(std::vector<std::string> &inDocs, std::vector<std::string> &inReqs) {
    spdlog::info("ConverterJson: run with input documents and requests");

    m_textFromDocs = inDocs;
    m_requests = inReqs;
}

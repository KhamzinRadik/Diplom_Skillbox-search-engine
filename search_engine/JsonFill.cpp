#include "JsonFill.h"

void JsonFill::fillFile(const std::string &type) {
    if (type == "config") {
        m_configExample.config.insert({"name", "search_engine"});
        m_configExample.config.insert({"version", "0.1"});
        m_configExample.config.insert({"max_responses", "5"});
        m_configExample.files.emplace_back("../examples/resources/file001.txt");
        m_configExample.files.emplace_back("../examples/resources/file002.txt");
        m_configExample.files.emplace_back("../examples/resources/file003.txt");
        m_configExample.files.emplace_back("../examples/resources/file004.txt");
    } else if (type == "request") {
        m_requestExample.requests.emplace_back("milk water");
        m_requestExample.requests.emplace_back("sugar");
        m_requestExample.requests.emplace_back("It pepper salt milk number one saw");
    }
}

void JsonFill::printFile(const std::string &type, const std::string &path) {
    if (type == "config") {
        std::ofstream file(path, std::ios::out);
        nlohmann::json configFile;
        configFile["config"] = m_configExample.config;
        configFile["files"] = m_configExample.files;
        std::string toOut;
        toOut = configFile.dump(4);
        file << std::setw(4) << toOut << std::endl;
    } else if (type == "request") {
        std::ofstream file(path, std::ios::out);
        nlohmann::json requestFile;
        requestFile["requests"] = m_requestExample.requests;
        file << std::setw(4) << requestFile << std::endl;
    }

}
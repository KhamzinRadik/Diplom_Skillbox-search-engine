
#include "ConverterJson.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

#include <spdlog/spdlog.h>

/**
 * Write answers to the JSON file answers.json
 * @param [in] allRequestsResults result of search for relevant pages
 */
void writeAnswers(const std::vector<std::vector<RelativeIndex>> &allRequestsResults) {
    if (allRequestsResults.empty()) {
        spdlog::error("No matches are found.");
        return;
    }
    std::vector<std::vector<std::pair<int, float>>> allRequestsResultsReadyForJSON;
    for (auto &requestResult: allRequestsResults) {
        std::vector<std::pair<int, float>> requestResultReadyForJSON;
        for (auto &pageRelevance: requestResult) {
            std::pair<int, float> relevancePair;
            relevancePair.first = (int) pageRelevance.doc_id;
            relevancePair.second = pageRelevance.rank;
            requestResultReadyForJSON.push_back(relevancePair);
        }
        allRequestsResultsReadyForJSON.push_back(requestResultReadyForJSON);
    }
    ConverterJSON::getInstance()->putAnswers(allRequestsResultsReadyForJSON);
}

int main() {

    ConverterJSON::getInstance()->readConfigFile();
    ConverterJSON::getInstance()->readRequestFile();
    std::vector<std::string> documents = ConverterJSON::getInstance()->getTextDocuments();
    auto *invertedIndex = new InvertedIndex();
    invertedIndex->updateDocumentBase(documents);

    spdlog::info("Searching...");
    SearchServer searchServer(*invertedIndex);
    searchServer.setMaxResponses(ConverterJSON::getInstance()->getMaxResponses());
    auto allRequestsResults = searchServer.search(ConverterJSON::getInstance()->getRequests());
    writeAnswers(allRequestsResults);
    spdlog::info("Successful!");
    spdlog::info("Enter for close.");
    getchar();
    spdlog::info("Bye");
}


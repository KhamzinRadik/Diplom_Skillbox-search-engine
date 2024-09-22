#include "SearchServer.h"

std::vector<std::vector<std::pair<int, float>>> SearchServer::searchFoo(const std::vector<std::string> &queries_input) {
    for (int i = 0; i < queries_input.size(); i++) {
        uniqRequestsFill(queries_input[i]);
        preRelevanceFill();
        m_maxAbsRelevance = findMaxAbsRel();
        m_result.push_back(sortRelativeIndex());
    }
    return m_result;
}

void SearchServer::uniqRequestsFill(const std::string &request) {
    m_uniqRequests.clear();
    int i = 0;
    std::string requestWord;
    while (request[i]) {
        char tmpSym = request[i];
        char tmpNxtSym = request[i + 1];
        if (m_index.characterCondition(tmpSym)) {
            requestWord.push_back(request[i]);
        }
        if (InvertedIndex::wordCondition(tmpSym, tmpNxtSym)) {
            m_uniqRequests.insert({requestWord, 0});
            requestWord.clear();
        }
        i++;
    }
}

void SearchServer::preRelevanceFill() {
    m_preRelevance.clear();
    for (int i = 0; i < m_dataJson.GetFilesNum(); i++) {
        int absRelevance = 0;
        for (const auto &it: m_uniqRequests) {
            auto it2 = m_index.getFreqDictionary()->find(it.first);
            if (it2 != m_index.getFreqDictionary()->end()) {
                for (auto &j: it2->second) {
                    absRelevance += j.doc_id == i ? j.count : 0;
                }
            }
        }
        m_preRelevance.insert({i, absRelevance});
    }
}

int SearchServer::findMaxAbsRel() {
    for (auto &it: m_preRelevance) {
        if (it.second > m_maxAbsRelevance) {
            m_maxAbsRelevance = it.second;
        }
    }
    return m_maxAbsRelevance;
}

std::vector<std::pair<int, float>> SearchServer::sortRelativeIndex() {
    int counter = 0;
    m_relativeIndex.clear();

    std::multimap<int, int> reverseMyMap;
    for (auto it: m_preRelevance) {
        reverseMyMap.insert({it.second, it.first});
    }

    for (auto it = reverseMyMap.rbegin(); it != reverseMyMap.rend(); it++) {
        std::pair<int, float> tempIndex;
        tempIndex.first = it->second;
        if (m_maxAbsRelevance != 0)
            tempIndex.second = floorf(100 * (float) (it->first) / m_maxAbsRelevance) / 100;
        else tempIndex.second = 0;
        if (tempIndex.second > 0) {
            m_relativeIndex.push_back(tempIndex);
        }
    }

    std::vector<std::pair<int, float>> relativeIndexTemp1;
    std::vector<std::pair<int, float>> relativeIndexTemp2;

    for (int i = 0; i < m_relativeIndex.size(); i++) {
        while (m_relativeIndex[i].second == m_relativeIndex[i + 1].second) {
            relativeIndexTemp1.push_back(m_relativeIndex[i]);
            i++;
        }
        relativeIndexTemp1.push_back(m_relativeIndex[i]);
        std::sort(relativeIndexTemp1.begin(), relativeIndexTemp1.end());
        for (auto it: relativeIndexTemp1) {
            if (counter < m_dataJson.GetResponseLimit()) {
                relativeIndexTemp2.push_back(it);
                counter++;
            }
        }
        relativeIndexTemp1.clear();
    }

    return relativeIndexTemp2;
}
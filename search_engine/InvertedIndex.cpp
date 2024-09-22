#include "InvertedIndex.h"


std::mutex myMutex;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> &input_docs) {
    m_docs = input_docs;

}

bool InvertedIndex::characterCondition(char &symbol) {
    if (symbol != ' ' && symbol != '\0' && symbol != ',' && symbol != '.'
        && symbol != '!' && symbol != '?' && symbol != ';' && symbol != ':') {
        return true;
    } else return false;
}

bool InvertedIndex::wordCondition(char &symbol, char &nextSymbol) {
    if (symbol == ' ' || nextSymbol == '\0' || symbol == ',' || symbol == '.'
        || symbol == '!' || symbol == '?' || symbol == ';' || symbol == ':') {
        return true;
    } else return false;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    std::vector<Entry> wordData{};
    for (int i = 0; i < m_docs.size(); i++) {
        Entry wordEntry{};
        wordEntry.doc_id = i;
        wordEntry.count = 0;

        int j = 0;
        std::string tempWord;
        while (m_docs[i][j]) {
            if (characterCondition(m_docs[i][j])) {
                tempWord.push_back(m_docs[i][j]);
            }
            if (wordCondition(m_docs[i][j], m_docs[i][j + 1])) {
                if (std::strcmp(word.c_str(), tempWord.c_str()) == 0)
                    wordEntry.count++;
                tempWord.clear();
            }
            j++;
        }
        if (wordEntry.count != 0)
            wordData.push_back(wordEntry);
    }
    return wordData;
}

void InvertedIndex::freqDictInfillThread(std::string &textFromDoc) {
    int i = 0;
    std::string singleWord;
    while (textFromDoc[i]) {
        if (characterCondition(textFromDoc[i])) {
            singleWord.push_back(textFromDoc[i]);
        }
        if (wordCondition(textFromDoc[i], textFromDoc[i + 1])) {
            m_freq_dictionary.insert({singleWord, GetWordCount(singleWord)});
            singleWord.clear();
        }
        i++;
    }
}

void InvertedIndex::dataMerge() {
    for (auto it: m_classParts) {
        for (auto &it2: it.m_freq_dictionary) {
            m_freq_dictionary.insert({it2.first, it2.second});
        }
    }
}

std::map<std::string, std::vector<Entry>> *InvertedIndex::getFreqDictionary() {
    return &m_freq_dictionary;
}

void InvertedIndex::threadsDistribution() {
    for (auto it: m_docs) {
        InvertedIndex tempPart(m_jsonData);
        tempPart.UpdateDocumentBase(m_docs);
        m_classParts.push_back(tempPart);
    }

#ifdef SingleThread
    for (int i = 0; i < classParts.size(); i++)
    {
        classParts[i].freqDictInfillThread(docs[i]);
    }
#endif

#ifdef MultiThread
    std::vector<std::thread> ThreadVector;
    for (int i = 0; i < classParts.size(); i++) {
        ThreadVector.emplace_back([&, i]() {
            classParts[i].freqDictInfillThread(docs[i]);
        });
    }

    for(auto& t: ThreadVector)
    {
        t.join();
    }
#endif

    dataMerge();
}





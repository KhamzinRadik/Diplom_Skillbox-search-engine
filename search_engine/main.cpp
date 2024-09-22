#include <iostream>
#include "JsonFill.h"
#include "ConverterJson.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "thread"
#include "ctime"
#include <spdlog/spdlog.h>

void showDocs();

const std::string CONFIG = "configs";
const std::string CONFIG_DIR = "../configs/configs.json";
const std::string REQUEST = "request";
const std::string REQUEST_DIR = "../configs/request.json";

void createJsons() {
    JsonFill fileExample;
    fileExample.fillFile(CONFIG);
    fileExample.printFile(CONFIG, CONFIG_DIR);
    fileExample.fillFile(REQUEST);
    fileExample.printFile(REQUEST, REQUEST_DIR);
}

int main() {
    createJsons();
    std::ifstream _dir(CONFIG_DIR);
    ConverterJson conv(_dir);
    conv.GetResponseLimit();
    conv.GetRequestsData();
    InvertedIndex inv(conv);
    inv.UpdateDocumentBase(conv.GetTextDocuments());
    inv.threadsDistribution();
    SearchServer serv(inv, conv);
    conv.putAnswers(serv.searchFoo(conv.GetRequestsData()));

    spdlog::info("Diff time: %h");
    showDocs();
}

void showDocs() {
    std::cout << std::endl;
    for (int i = 0; i < 3; i++) {
        std::stringstream path;
        path << "../examples/resources/file00" << i + 1 << ".txt";
        std::ifstream file(path.str());
        spdlog::info("File name: " +  path.str() + ".");
        std::string s;
        while (!file.eof()) {
            std::string tmp{};
            file >> tmp;
            s += tmp + " ";
        }
        spdlog::info("founded: -> \"" + s +  "\"");
    }
}


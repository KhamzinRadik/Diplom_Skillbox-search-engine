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
const std::string ANSWER_DIR = "../examples/answers/answers.json";

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
    auto _limitResponse = conv.GetResponseLimit();
    InvertedIndex inv(conv);
    inv.UpdateDocumentBase(conv.GetTextDocuments());
    inv.threadsDistribution();
    SearchServer serv(inv, conv);
    conv.putAnswers(serv.finder(conv.GetRequestsData()), ANSWER_DIR);

    spdlog::info("Diff time: %h");
    showDocs();
    std::cout << '\n';

    spdlog::info("Answers is here: " + ANSWER_DIR);
}

void showDocs() {
    std::cout << std::endl;
    for (int i = 0; i < 3; i++) {
        std::stringstream path;
        path << "../examples/resources/file00" << i + 1 << ".txt";
        std::ifstream file(path.str());
        spdlog::info("File name: " +  path.str() + ".");

    }
}


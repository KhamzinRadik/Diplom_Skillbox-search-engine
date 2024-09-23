#include "SearchServer.h"
#include "ConverterJson.h"
#include "gtest/gtest.h"


TEST(TestCaseSearchServer, TestSimple) {
std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
};
std::vector<std::string> request = {"milk water", "sugar"};
std::vector<std::vector<std::pair<int, float>>> expected = {
        {
                {2, 1},
                {0, 0.7},
                {1, 0.3}
        },
        {
        }
};
ConverterJson conv(docs,request);

InvertedIndex idx(conv);
idx.UpdateDocumentBase(docs);
idx.threadsDistribution();
SearchServer srv(idx,conv);
ASSERT_EQ(srv.finder(request), expected);
}
TEST(TestCaseSearchServer, TestTop5) {
std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
};
std::vector<std::string> request = {"moscow is the capital of russia"};
std::vector<std::vector<std::pair<int, float>>> expected = {
        {
                {7, 1},
                {14, 1},
                {0, 0.66},
                {1, 0.66},
                {2, 0.66}
        }
};
ConverterJson conv(docs,request);
InvertedIndex idx(conv);
idx.UpdateDocumentBase(docs);
idx.threadsDistribution();
SearchServer srv(idx,conv);

ASSERT_EQ(srv.finder(request), expected);
}
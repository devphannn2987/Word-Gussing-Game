#include "WordDataSet.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <cctype>

WordDataSet::WordDataSet() : cursor(0) {}

bool WordDataSet::addWord(std::string w) {
    while (!w.empty() && std::isspace((unsigned char)w.back())) w.pop_back();
    if (w.size() < 3) return false;

    for (char& c : w) {
        if (!std::isalpha((unsigned char)c)) return false;
        c = std::tolower((unsigned char)c);
    }

    if (!seen.insert(w).second) return false;
    words.push_back(w);
    return true;
}

void WordDataSet::loadFromFile(const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) throw FileNotFoundException(path);

    std::string line;
    while (std::getline(fin, line)) addWord(line);

    shuffle();
}

void WordDataSet::shuffle() {
    std::mt19937 rng(std::random_device{}());
    std::shuffle(words.begin(), words.end(), rng);
    cursor = 0;
}

std::string WordDataSet::getNextWord() {
    if (words.empty()) throw std::runtime_error("Bo du lieu tu dang rong");
    if (cursor >= words.size()) shuffle();
    return words[cursor++];
}

std::vector<std::string> WordDataSet::getWordsByLength(size_t minLen, size_t maxLen) const {
    std::vector<std::string> result;
    for (const std::string& w : words)
        if (w.size() >= minLen && w.size() <= maxLen) result.push_back(w);
    return result;
}

size_t WordDataSet::size() const { return words.size(); }

std::ostream& operator<<(std::ostream& os, const WordDataSet& ds) {
    return os << "WordDataSet co " << ds.words.size() << " tu";
}

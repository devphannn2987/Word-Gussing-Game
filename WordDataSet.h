#ifndef WORDDATASET_H
#define WORDDATASET_H
#include <string>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <ostream>

class FileNotFoundException : public std::runtime_error {
public:
    FileNotFoundException(const std::string& path)
        : std::runtime_error("Khong mo duoc file: " + path) {}
};

class WordDataSet {
private:
    std::vector<std::string> words;
    std::unordered_set<std::string> seen;
    size_t cursor;

    void shuffle();

public:
    WordDataSet();

    void loadFromFile(const std::string& path);
    bool addWord(std::string w);
    std::string getNextWord();
    std::vector<std::string> getWordsByLength(size_t minLen, size_t maxLen) const;
    size_t size() const;

    friend std::ostream& operator<<(std::ostream& os, const WordDataSet& ds);
};

#endif

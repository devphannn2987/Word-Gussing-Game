#pragma once
#include <string>
#include <vector>
using namespace std;
struct ScoreEntry {
    string name;
    int score;
};
class HighScore {
private:
    vector<ScoreEntry> entries;
    string fileName;
    size_t maxEntries;
public:
    explicit HighScore(const string& fileName = "highscore.txt", size_t maxEntries = 5);
    void load();
    void save() const;
    void add(const string& name, int score);
    void display() const;
};
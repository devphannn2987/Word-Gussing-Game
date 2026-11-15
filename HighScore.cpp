#include "HighScore.h"
#include <algorithm>
#include <fstream>
#include <iostream>
HighScore::HighScore(const string& fileName, size_t maxEntries)
    : fileName(fileName), maxEntries(maxEntries) {}
void HighScore::load() {
    entries.clear();
    ifstream in(fileName);
    if (!in.is_open()) return;
    ScoreEntry e;
    while (in >> e.score) {
        in.ignore(1);
        getline(in, e.name);
        if (e.name.empty()) e.name = "Unknown";
        entries.push_back(e);
    }
}
void HighScore::save() const {
    ofstream out(fileName);
    for (const auto& e : entries) out << e.score << ' ' << e.name << '\n';
}
void HighScore::add(const string& name, int score) {
    string n = name.empty() ? "Unknown" : name;
    entries.push_back({n, score});
    stable_sort(entries.begin(), entries.end(),
         [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    if (entries.size() > maxEntries) entries.resize(maxEntries);
}
void HighScore::display() const {
    cout << "TOP NGUOI CHOI CO SO DIEM CAO NHAT\n";
    if (entries.empty()) { cout << "(chua co)\n"; return; }
    for (size_t i = 0; i < entries.size(); ++i)
        cout << i + 1 << ". " << entries[i].name << " - " << entries[i].score << '\n';
}
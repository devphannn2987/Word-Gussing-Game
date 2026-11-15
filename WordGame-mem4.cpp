#include "WordGame-mem4.h"
#include <algorithm>
#include <cctype>

using namespace std;

WordPuzzle::WordPuzzle(const string& word, int hiddenCount)
    : answer(word), rng(random_device{}()) {
    for (char& c : answer) c = (char)tolower((unsigned char)c);
    display = answer;
    hideRandomChars(hiddenCount);
}

void WordPuzzle::hideRandomChars(int hiddenCount) {
    vector<int> idx(answer.size());
    for (size_t i = 0; i < idx.size(); ++i) idx[i] = (int)i;
    shuffle(idx.begin(), idx.end(), rng);

    size_t count = (size_t)max(1, hiddenCount);
    count = min(count, answer.size());
    for (size_t i = 0; i < count; ++i) display[idx[i]] = '_';
}

bool WordPuzzle::isValidInput(const string& s) {
    return s.size() == 1 && isalpha((unsigned char)s[0]);
}

GuessResult WordPuzzle::guess(const string& input) {
    if (!isValidInput(input)) return GuessResult::Invalid;

    char c = (char)tolower((unsigned char)input[0]);
    if (guessed.count(c)) return GuessResult::AlreadyGuessed;
    guessed.insert(c);

    if (reveal(c)) return GuessResult::Correct;

    if (answer.find(c) != string::npos) return GuessResult::AlreadyGuessed;

    return GuessResult::Wrong;
}

bool WordPuzzle::reveal(char c) {
    c = (char)tolower((unsigned char)c);
    bool found = false;
    for (size_t i = 0; i < answer.size(); ++i) {
        if (answer[i] == c && display[i] == '_') {
            display[i] = c;
            found = true;
        }
    }
    if (found) guessed.insert(c);
    return found;
}

bool WordPuzzle::isSolved() const { return display == answer; }

const string& WordPuzzle::getDisplay() const { return display; }

const string& WordPuzzle::getAnswer() const { return answer; }
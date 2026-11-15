#ifndef WORDGAME_MEM4_H
#define WORDGAME_MEM4_H

#include <random>
#include <set>
#include <string>
#include <vector>

enum class GuessResult {
    Invalid,
    AlreadyGuessed,
    Correct,
    Wrong
};

class WordPuzzle {
private:
    std::string answer;
    std::string display;
    std::set<char> guessed;
    std::mt19937 rng;

    void hideRandomChars(int hiddenCount);

public:
    WordPuzzle(const std::string& word, int hiddenCount);

    static bool isValidInput(const std::string& s);

    GuessResult guess(const std::string& input);

    bool reveal(char c);

    bool isSolved() const;
    const std::string& getDisplay() const;
    const std::string& getAnswer() const;
};

#endif
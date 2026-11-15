#pragma once
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
class Score {
private:
    int total;
    int streak;
public:
    Score();
    void onCorrectGuess();
    void onWrongGuess();
    void onWordSolved(int seconds, int livesLeft);
    int getTotal() const;
    void reset();
    void onHintUsed();
};
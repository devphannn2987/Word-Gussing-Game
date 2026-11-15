#include "Score.h"
Score::Score() : total(0), streak(0) {}
void Score::onCorrectGuess() {
    ++streak;
    total += 10 + (streak - 1) * 2;
}
void Score::onWrongGuess() {
    streak = 0;
    total = max(0, total - 5);  
}
void Score::onWordSolved(int seconds, int livesLeft) {
    int timeBonus = max(0, 30 - seconds);
    total += 50 + livesLeft * 10 + timeBonus;
}
int Score::getTotal() const { return total; }
void Score::reset() { total = 0; streak = 0; }
void Score::onHintUsed() { total = max(0, total - 15); }
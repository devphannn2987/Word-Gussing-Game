#include "Hint.h"
Hint::Hint(int maxHints) : maxHints(maxHints), used(0) {}
void Hint::reset() { used = 0; }
bool Hint::canUse() const { return used < maxHints; }
int Hint::getRemaining() const { return maxHints - used; }
char Hint::useHint(const string& answer, const string& display) {
    if (!canUse()) return '\0';
    for (size_t i = 0; i < answer.size() && i < display.size(); ++i) {
        if (display[i] == '_') {
            ++used;
            return answer[i];
        }
    }
    return '\0';
}
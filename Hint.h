#pragma once
#include <string>
using namespace std;
class Hint {
private:
    int maxHints;
    int used;
public:
    explicit Hint(int maxHints = 2);
    void reset();
    bool canUse() const;
    int getRemaining() const;
    char useHint(const string& answer, const string& display);
};
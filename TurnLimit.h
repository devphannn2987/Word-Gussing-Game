#pragma once
class TurnLimit {
private:
    int maxTurns;
    int remaining;
public:
    explicit TurnLimit(int maxTurns = 5);
    void reset();
    void loseTurn();
    bool hasTurns() const;
    int getRemaining() const;
};
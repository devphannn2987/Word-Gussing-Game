#include "TurnLimit.h"
TurnLimit::TurnLimit(int maxTurns) : maxTurns(maxTurns), remaining(maxTurns) {}
void TurnLimit::reset() { remaining = maxTurns; }
void TurnLimit::loseTurn() { if (remaining > 0) --remaining; }
bool TurnLimit::hasTurns() const { return remaining > 0; }
int TurnLimit::getRemaining() const { return remaining; }
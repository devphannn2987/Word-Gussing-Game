#include "GameTime.h"
GameTime::GameTime() : startTime(time(nullptr)) {}
void GameTime::start() { startTime = time(nullptr); }
int GameTime::elapsedSeconds() const {
    return (int)difftime(time(nullptr), startTime);
}
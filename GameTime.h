#pragma once
#include <ctime>  
class GameTime {
private:
    time_t startTime;
public:
    GameTime();
    void start();
    int elapsedSeconds() const;
};
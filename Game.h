#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "WordDataSet.h"     // Mem 2
#include "Difficulty.h"      // Mem 3
#include "WordGame-mem4.h"   // Mem 4
#include "Score.h"           // Mem 5
#include "TurnLimit.h"       // Mem 5
#include "GameTime.h"        // Mem 5
#include "Hint.h"            // Mem 5
#include "HighScore.h"       // Mem 5

class Game {
private:
    static const int WORDS_PER_GAME = 10;     // moi van gom 10 chu
    static const int MAX_WRONG_PER_WORD = 7;  // moi chu duoc doan sai toi da 7 lan
    static const int HINT_PENALTY = 15;       // diem bi tru moi lan dung goi y

    bool isRunning;

    WordDataSet dataSet;
    Difficulty* difficulty;

    Score score;          // diem cua CHU dang choi
    int totalPoints;      // tong diem cua VAN dang choi
    TurnLimit turnLimit;  // dung de dem so lan doan SAI cua chu dang choi
    GameTime gameTime;
    Hint hint;
    HighScore highScore;

    std::string playerName;
    std::string levelName;
    size_t minLen, maxLen;                      // khoang do dai tu theo do kho
    std::unordered_set<std::string> used;       // tu da ra (cac van truoc)
    std::unordered_set<std::string> usedInGame; // tu da ra trong van nay
    std::mt19937 rng;

    bool chooseDifficulty();
    std::string pickWord();
    bool playWord(int index, const std::string& secretWord);   // true neu doan ra

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    Game();
    ~Game();

    void init();
    void run();
    void playRound();       // mot VAN = 10 chu
    bool askToContinue();
};

#endif

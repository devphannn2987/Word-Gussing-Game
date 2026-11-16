#include "Game.h"
#include <algorithm>

Game::Game()
    : isRunning(true), difficulty(nullptr), totalPoints(0),
      turnLimit(MAX_WRONG_PER_WORD), hint(3),
      highScore("highscore.txt", 5),
      minLen(3), maxLen(100), rng(std::random_device{}()) {}

Game::~Game() {
    delete difficulty;
}

void Game::init() {
    std::cout << "========================================\n";
    std::cout << "      WELCOME TO WORD GUESSING GAME     \n";
    std::cout << "========================================\n";

    highScore.load();
    highScore.display();
    std::cout << "----------------------------------------\n";

    try {
        dataSet.loadFromFile("words.txt");
        std::cout << "[He thong] Da tai thanh cong: " << dataSet << "\n";
    } catch (const FileNotFoundException& e) {
        std::cerr << "[Loi] " << e.what() << "\n";
        isRunning = false;
        return;
    }

    std::cout << "\nLUAT CHOI:\n"
              << " - Moi van gom " << WORDS_PER_GAME << " chu. Doan dung CA " << WORDS_PER_GAME
              << " chu de thang.\n"
              << " - Moi chu duoc doan sai toi da " << MAX_WRONG_PER_WORD
              << " lan, sai het thi qua chu tiep theo va chu do khong duoc diem.\n"
              << " - Nhap '?' de xin goi y (bi tru " << HINT_PENALTY << " diem).\n";

    // Chon do kho 1 lan duy nhat o dau tro choi
    if (!chooseDifficulty()) { isRunning = false; return; }

    std::cout << "\nNhap ten cua ban: ";
    if (!std::getline(std::cin, playerName)) isRunning = false;
}

// Do kho quyet dinh: so chu bi an (Kytuan - Mem 3) va khoang do dai tu (Mem 2).
// Chon muc nao thi van chi choi 10 chu.
bool Game::chooseDifficulty() {
    while (true) {
        std::cout << "\nChon do kho:\n"
                  << "1. Easy   (tu 3-5 chu cai, an it chu)\n"
                  << "2. Medium (tu 6-8 chu cai, an vua)\n"
                  << "3. Hard   (tu 9 chu cai tro len, an nhieu chu)\n"
                  << "Lua chon cua ban: ";
        std::string s;
        if (!std::getline(std::cin, s)) return false;

        if (s != "1" && s != "2" && s != "3") {
            std::cout << "-> Lua chon khong hop le!\n";
            continue;
        }

        delete difficulty;
        difficulty = nullptr;

        if (s == "1")      { difficulty = new Easy();   levelName = "Easy";   minLen = 3; maxLen = 5; }
        else if (s == "2") { difficulty = new Medium(); levelName = "Medium"; minLen = 6; maxLen = 8; }
        else               { difficulty = new Hard();   levelName = "Hard";   minLen = 9; maxLen = 100; }
        return true;
    }
}

// Chon ngau nhien 1 tu trong khoang do dai. Khong lap trong cung 1 van;
// uu tien tu chua ra o cac van truoc.
std::string Game::pickWord() {
    std::vector<std::string> pool = dataSet.getWordsByLength(minLen, maxLen);
    if (pool.empty()) return "";

    std::vector<std::string> fresh, notInGame;
    for (const std::string& w : pool) {
        if (usedInGame.count(w)) continue;
        notInGame.push_back(w);
        if (!used.count(w)) fresh.push_back(w);
    }
    if (notInGame.empty()) notInGame = pool;   // bo tu qua nho: cho phep lap
    if (fresh.empty()) {
        for (const std::string& w : pool) used.erase(w);
        fresh = notInGame;
    }
    std::string w = fresh[rng() % fresh.size()];
    used.insert(w);
    usedInGame.insert(w);
    return w;
}

void Game::run() {
    init();
    if (!isRunning) return;

    while (isRunning) {
        playRound();
        if (!isRunning) break;
        if (!askToContinue()) isRunning = false;
    }

    std::cout << "\nCam on ban da choi game! Hen gap lai.\n";
}

// Mot van = 10 chu. Thang neu doan ra du 10/10 chu.
void Game::playRound() {
    if (dataSet.getWordsByLength(minLen, maxLen).empty()) {
        std::cerr << "[Loi] Khong co tu nao phu hop o muc do nay!\n";
        isRunning = false;
        return;
    }

    usedInGame.clear();
    totalPoints = 0;
    int solvedCount = 0;

    std::cout << "\n######## VAN MOI - Muc do: " << levelName << " ########\n";

    for (int i = 1; i <= WORDS_PER_GAME; ++i) {
        std::string secretWord = pickWord();
        if (playWord(i, secretWord)) ++solvedCount;
        if (!isRunning) return;   // het du lieu nhap
    }

    std::cout << "\n========================================\n";
    std::cout << "KET QUA VAN CHOI: doan dung " << solvedCount << "/" << WORDS_PER_GAME << " chu\n";
    std::cout << "Tong diem: " << totalPoints << "\n";
    if (solvedCount == WORDS_PER_GAME)
        std::cout << "CHUC MUNG! BAN DA THANG!\n";
    else
        std::cout << "Ban da thua cuoc, hay co gang vao van sau!\n";
    std::cout << "========================================\n\n";

    highScore.load();
    highScore.add(playerName, totalPoints);
    highScore.save();
    highScore.display();
}

// Choi 1 chu. Tra ve true neu doan ra, false neu sai qua 7 lan.
bool Game::playWord(int index, const std::string& secretWord) {
    int hiddenCount = difficulty->Kytuan(secretWord);
    WordPuzzle puzzle(secretWord, hiddenCount);

    score.reset();
    turnLimit.reset();
    hint.reset();
    gameTime.start();
    int hintsUsed = 0;

    std::cout << "\n=============== CHU " << index << "/" << WORDS_PER_GAME << " ===============\n";
    std::cout << "Tu co " << secretWord.size() << " chu cai, an " << hiddenCount << " chu\n";

    while (turnLimit.hasTurns() && !puzzle.isSolved()) {
        int wrongCount = MAX_WRONG_PER_WORD - turnLimit.getRemaining();
        std::cout << "\n----------------------------------------\n";
        std::cout << "Diem: " << totalPoints + score.getTotal()
                  << " | Sai: " << wrongCount << "/" << MAX_WRONG_PER_WORD
                  << " | Goi y con lai: " << hint.getRemaining()
                  << " | Thoi gian: " << gameTime.elapsedSeconds() << "s\n";

        std::cout << "Tu can doan: " << puzzle.getDisplay() << "\n";
        std::cout << "Nhap 1 ky tu de doan (hoac nhap '?' de dung goi y): ";

        std::string inputStr;
        if (!std::getline(std::cin, inputStr)) { isRunning = false; return false; }

        if (inputStr == "?") {
            if (hint.canUse()) {
                char hintChar = hint.useHint(secretWord, puzzle.getDisplay());
                if (hintChar != '\0') {
                    ++hintsUsed;
                    score.onHintUsed();
                    std::cout << "[Goi y] Ky tu can tim la: '" << hintChar
                              << "' (-" << HINT_PENALTY << " diem)\n";
                    puzzle.reveal(hintChar);
                } else {
                    std::cout << "[Thong bao] Khong con chu nao de goi y!\n";
                }
            } else {
                std::cout << "[Thong bao] Ban da het luot goi y!\n";
            }
            continue;
        }

        GuessResult res = puzzle.guess(inputStr);

        if (res == GuessResult::Correct) {
            std::cout << "Chinh xac!\n";
            score.onCorrectGuess();
        }
        else if (res == GuessResult::AlreadyGuessed) {
            std::cout << "[Thong bao] Ky tu nay ban da doan roi hoac da hien thi san!\n";
        }
        else if (res == GuessResult::Wrong) {
            score.onWrongGuess();
            turnLimit.loseTurn();
            std::cout << "Sai roi! (" << MAX_WRONG_PER_WORD - turnLimit.getRemaining()
                      << "/" << MAX_WRONG_PER_WORD << " lan sai)\n";
        }
        else {
            std::cout << "[Loi] Ky tu khong hop le! Vui long nhap dung 1 chu cai.\n";
        }
    }

    if (puzzle.isSolved()) {
        int seconds = gameTime.elapsedSeconds();
        score.onWordSolved(seconds, turnLimit.getRemaining());
        totalPoints += score.getTotal();
        std::cout << "\nDOAN DUNG: " << puzzle.getAnswer() << " (+" << score.getTotal()
                  << " diem, " << seconds << "s)\n";
        return true;
    }

    // Sai du 7 lan: chu nay khong duoc diem, tu dong qua chu tiep theo.
    // Neu da dung goi y thi van bi tru diem nhu binh thuong.
    int penalty = hintsUsed * HINT_PENALTY;
    totalPoints = std::max(0, totalPoints - penalty);
    std::cout << "\nBan da doan sai " << MAX_WRONG_PER_WORD << " lan! Chu nay khong duoc diem. "
              << "Dap an la: " << puzzle.getAnswer() << "\n";
    if (penalty > 0)
        std::cout << "(Van bi tru " << penalty << " diem do da dung goi y)\n";
    return false;
}

bool Game::askToContinue() {
    std::cout << "\nBan co muon choi van moi khong? (y/n): ";
    std::string s;
    if (!std::getline(std::cin, s) || s.empty()) return false;
    return s[0] == 'y' || s[0] == 'Y';
}

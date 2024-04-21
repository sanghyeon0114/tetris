#include "game.h"
#include "console/console.h"

#include <random>

std::chrono::system_clock::time_point startTime;

Tetromino Game::getRandomTetromino() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 6);
    return randomTetro[dis(gen)];
}

void Game::makeNextTetromino() {
    tetro = next;
    next = Game::getRandomTetromino();
    tetroPosition[0] = (BOARD_WIDTH/2-tetro.size()/2)+1;
    tetroPosition[1] = 1;
}

Game::Game() {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            board_[i][j] = false;
        }
    }
    tick = 0;
    lineCount = LINES;

    tetro = Game::getRandomTetromino();
    next = Game::getRandomTetromino();
    hold = nullptr;
    tetroPosition[0] = (BOARD_WIDTH/2-tetro.size()/2)+1;
    tetroPosition[1] = 1;
    startTime = std::chrono::system_clock::now();
}

bool Game::checkTetrominoPosition(Tetromino tetro, int x, int y) {
    for(int i = 0; i < tetro.size(); i++) {
        for(int j = 0; j < tetro.size(); j++) {
            if(tetro.check(i, j)) {
                if(!(x+i >= 1 && x+i <= BOARD_WIDTH && y+j >= 1 && y+j <= BOARD_HEIGHT && !board_[x+i-1][y+j-1])) {
                    return false;
                }
            }
        }
    }
    return true;
}

void Game::inputKey() {
    if(console::key(console::K_LEFT)) {
        if(Game::checkTetrominoPosition(tetro, tetroPosition[0]-1, tetroPosition[1])) {
            tetroPosition[0]--;
        }
    } else if(console::key(console::K_RIGHT)) {
        if(Game::checkTetrominoPosition(tetro, tetroPosition[0]+1, tetroPosition[1])) {
            tetroPosition[0]++;
        }
    } else if(console::key(console::K_UP)) {
        tetroPosition[0] = shadowPosition[0];
        tetroPosition[1] = shadowPosition[1];
        tick = DROP_DELAY;
    } else if(console::key(console::K_DOWN)) {
        if(Game::checkTetrominoPosition(tetro, tetroPosition[0], tetroPosition[1]+1)) {
            tetroPosition[1]++;
        }
    } else if(console::key(console::K_ESC)) {
        isFinish = true;
    } else if(console::key(console::K_SPACE)) {
        if(canHold) {
            if(hold == nullptr) {
                hold = tetro.original();
                Game::makeNextTetromino();
            } else {
                Tetromino tmp = tetro;
                tetro = *hold;
                hold = tmp.original();
                tetroPosition[0] = (BOARD_WIDTH/2-tetro.size()/2)+1;
                tetroPosition[1] = 1;
            }
            canHold = false;
        }
    } else if(console::key(console::K_Z)) {
        Tetromino tmp = tetro.rotatedCCW();
        if(Game::checkTetrominoPosition(tmp, tetroPosition[0], tetroPosition[1])) {
            tetro = tmp;
        }
    } else if(console::key(console::K_X)) {
        Tetromino tmp = tetro.rotatedCW();
        if(Game::checkTetrominoPosition(tmp, tetroPosition[0], tetroPosition[1])) {
            tetro = tmp;
        }
    }
}

void Game::printLineCount() {
    std::string result = std::to_string(lineCount) + " lines left";
    console::draw(0, BOARD_HEIGHT+2, result);
}

std::string Game::getTime() {
    std::chrono::system_clock::time_point endTime = std::chrono::system_clock::now();
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    
    std::string result;
    int minutes = ((ms.count()/1000)/60);
    if(minutes < 10) {
        result.append("0");
    }
    result.append(std::to_string(minutes));
    result.append(":");

    int second = (ms.count()/1000)%60;
    if(second < 10) {
        result.append("0");
    }
    result.append(std::to_string(second));
    result.append(".");

    int millisecond = (ms.count()/10)%100;
    if(millisecond < 10) {
        result.append("0");
    }
    result.append(std::to_string(millisecond));
    
    return result;
}

void Game::printTime() {
    std::string result = Game::getTime();
    console::draw(BOARD_WIDTH/2-result.length()/2+1, BOARD_HEIGHT+3, result);
}   

void Game::printTetromino() {
    tetro.drawAt(BLOCK_STRING, tetroPosition[0], tetroPosition[1]);
}

void Game::printNextTetromino() {
    int nextBoxPos = (BOARD_WIDTH+1)+3;
    int boxSize = 5;
    next.drawAt(BLOCK_STRING, (nextBoxPos + (boxSize-next.size())/2), next.name().compare("I") == 0 ? 1 : 2);
}

void Game::printHoldTetromino() {
    if(hold == nullptr) {
        return;
    }
    int holdBoxPos = ((BOARD_WIDTH+1)+2+5)+2;
    int boxSize = 5;
    hold->drawAt(BLOCK_STRING, (holdBoxPos + (boxSize-hold->size())/2), hold->name().compare("I") == 0 ? 1 : 2);
}

void Game::printGroundTetromino() {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        for(int j = 0; j < BOARD_HEIGHT; j++) {
            if(board_[i][j]) {
                console::draw(i+1, j+1, BLOCK_STRING);
            }
        }
    }
}

void Game::printFinishedTetromino() {
    for(int i = 0; i < tetro.size(); i++) {
        for(int j = 0; j < tetro.size(); j++) {
            if(tetro.check(i, j)) {
                board_[tetroPosition[0]+i-1][tetroPosition[1]+j-1] = true;
            }
        }
    }
}

void Game::checkAndRemoveLine(int y) {
    for(int i = 0; i < BOARD_WIDTH; i++) {
        if(!board_[i][y]) {
            return;
        }
    }

    for(int i = y; i > 0; i--) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            board_[j][i] = board_[j][i-1];
        }
    }
    for(int i = 0; i < BOARD_WIDTH; i++) {
        board_[i][0] = false;
    }
    lineCount--;
}

void Game::removeLines() {
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        Game::checkAndRemoveLine(i);
    }
}

void Game::printShadow() {
    shadowPosition[0] = tetroPosition[0];
    shadowPosition[1] = tetroPosition[1];

    while(checkTetrominoPosition(tetro, shadowPosition[0], shadowPosition[1]+1)) {
        shadowPosition[1]++;
    }

    tetro.drawAt(SHADOW_STRING, shadowPosition[0], shadowPosition[1]);
}

bool Game::isTetrominoGround() {
    return !checkTetrominoPosition(tetro, tetroPosition[0], tetroPosition[1]+1);
}

void Game::printWinMessage() {
    std::string message = "You Win";
    std::string time = Game::getTime();
    console::draw(BOARD_WIDTH/2-message.length()/2+1, BOARD_HEIGHT/2, message);
    console::draw(BOARD_WIDTH/2-message.length()/2, BOARD_HEIGHT/2+1, time);
}

void Game::printLostMessage() {
    std::string message = "You Lost";
    console::draw(BOARD_WIDTH/2-message.length()/2+1, BOARD_HEIGHT/2, message);
}

bool Game::isWin() {
    if(lineCount <= 0) {
        lineCount = 0;
        return true;
    }
    return false;
}

void Game::update() {
    Game::printLineCount();
    Game::printTime();
    Game::printShadow();
    Game::printTetromino();
    Game::printNextTetromino();
    Game::printHoldTetromino();
    Game::printGroundTetromino();

    Game::inputKey();
    
    if(++tick >= DROP_DELAY) {
        tick = 0;
        if(Game::isTetrominoGround()) {
            Game::printFinishedTetromino();
            Game::removeLines();
            Game::makeNextTetromino();
            if(!Game::checkTetrominoPosition(tetro, tetroPosition[0], tetroPosition[1])) {
                Game::printLostMessage();
                isFinish = true;
            }
            canHold = true;
        } else {
            tetroPosition[1]++;
        }
    }
    if(Game::isWin()) {
        Game::printWinMessage();
        Game::printLineCount();
        isFinish = true;
    }
}

void Game::drawGameBox() {
    console::drawBox(0, 0, BOARD_WIDTH+1, BOARD_HEIGHT+1);
}

void Game::drawNextBox() {
    int startPosX = (BOARD_WIDTH+1)+2;
    int startPosY = 0;
    int boxSize = 5;
    console::drawBox(startPosX, startPosY, startPosX+boxSize, startPosY+boxSize);
    console::draw(startPosX+1, startPosY, "Next");
}

void Game::drawHoldBox() {
    int startPosX = ((BOARD_WIDTH+1)+2+5)+1;
    int startPosY = 0;
    int boxSize = 5;
    console::drawBox(startPosX, startPosY, startPosX+boxSize, startPosY+boxSize);
    console::draw(startPosX+1, startPosY, "Hold");
}

void Game::draw() {
    Game::drawGameBox();
    Game::drawNextBox();
    Game::drawHoldBox();
}

bool Game::shouldExit() {
    return isFinish;
}
#include "game.h"
#include "console/console.h"

#include <random>

// Todo 2 : Up 키 누르면 바로 설치

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
    lineCount = 40;

    tetro = Game::getRandomTetromino();
    next = Game::getRandomTetromino();
    hold = nullptr;
    tetroPosition[0] = (BOARD_WIDTH/2-tetro.size()/2)+1;
    tetroPosition[1] = 1;
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
    if(console::key(console::K_NONE)) {

    } else if(console::key(console::K_OTHER)) {

    } else if(console::key(console::K_LEFT)) {
        if(Game::checkTetrominoPosition(tetro, tetroPosition[0]-1, tetroPosition[1])) {
            tetroPosition[0]--;
        }
    } else if(console::key(console::K_RIGHT)) {
        if(Game::checkTetrominoPosition(tetro, tetroPosition[0]+1, tetroPosition[1])) {
            tetroPosition[0]++;
        }
    } else if(console::key(console::K_UP)) {

    } else if(console::key(console::K_DOWN)) {
        if(Game::checkTetrominoPosition(tetro, tetroPosition[0], tetroPosition[1]+1)) {
            tetroPosition[1]++;
        }
    } else if(console::key(console::K_ESC)) {
        exit(0);
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
    } else if(console::key(console::K_ENTER)) {
        
    }
}

void Game::printLineCount() {
    std::string result = std::to_string(lineCount) + " lines left";
    console::draw(0, BOARD_HEIGHT+2, result);
}

void Game::printTime() {
    // Todo
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

bool Game::isTetrominoGround() {
    return !checkTetrominoPosition(tetro, tetroPosition[0], tetroPosition[1]+1);
}

int Game::checkGroundLines() {
    int count = 0;
    for(int i = BOARD_HEIGHT-1; i >= 0; i--) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            if(!board_[j][i]) {
                return count;
            }
        }
        count++;
    }
    return count;
}

void Game::removeGroundLines() {
    int removeLineCount = Game::checkGroundLines();
    if(removeLineCount == 0) {
        return;
    }
    for(int i = BOARD_HEIGHT-1; i >= removeLineCount ; i--) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            board_[j][i] = board_[j][i-1];
        }
    }
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < removeLineCount; j++) {
            board_[j][i] = false;
        }
    }
    lineCount -= removeLineCount;
    if(lineCount < 0) {
        lineCount = 0;
    }
}

// 게임의 한 프레임을 처리한다.
void Game::update() {
    // input Code.
    Game::printLineCount();
    Game::printTime();

    Game::printTetromino();
    Game::printNextTetromino();
    Game::printHoldTetromino();
    Game::printGroundTetromino();

    Game::inputKey();
    
    if(++tick == DROP_DELAY) {
        // input Code 2.
        tick = 0;
        if(Game::isTetrominoGround()) {
            Game::printFinishedTetromino();
            Game::removeGroundLines();
            Game::makeNextTetromino();
            if(!Game::checkTetrominoPosition(tetro, tetroPosition[0], tetroPosition[1])) {
                // Todo : Lose!
                exit(0);
            }
            canHold = true;
        } else {
            tetroPosition[1]++;
        }
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

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit() {
    return false;
}
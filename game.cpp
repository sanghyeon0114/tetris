#include "game.h"
#include "console/console.h"

Game::Game() {
    // Todo: board 처리
    tick = 0;
}

void Game::inputKey() {
    if(console::key(console::K_NONE)) {

    } else if(console::key(console::K_OTHER)) {

    } else if(console::key(console::K_LEFT)) {

    } else if(console::key(console::K_RIGHT)) {

    } else if(console::key(console::K_UP)) {

    } else if(console::key(console::K_DOWN)) {

    } else if(console::key(console::K_ESC)) {
        exit(0);
    } else if(console::key(console::K_SPACE)) {

    } else if(console::key(console::K_Z)) {

    } else if(console::key(console::K_X)) {

    } else if(console::key(console::K_ENTER)) {
        
    }
}

// 게임의 한 프레임을 처리한다.
void Game::update() {
    // input Code.
    Game::inputKey();
    if(++tick == DROP_DELAY) {
        // input Code 2.
        tick = 0;
    }
}

void Game::drawGameBox() {
    // can use [1 ~ BOARD_WIDTH][1 ~ BOARD_HEIGHT]
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

// 게임 화면을 그린다.
void Game::draw() {
    Game::drawGameBox();
    Game::drawNextBox();
    Game::drawHoldBox();
}

// 게임 루프가 종료되어야 하는지 여부를 반환한다.
bool Game::shouldExit() {
    return false;
}
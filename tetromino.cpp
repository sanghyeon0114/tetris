#include "tetromino.h"
#include "console/console.h"

#include <string>

Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("O", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");

Tetromino::Tetromino(std::string name, int size, std::string shape) {
    name_ = name;
    size_ = size;
    original_ = this;

    for(int i = 0; i < size_; i++) {
        for(int j = 0; j < size_; j++) {
            shape_[j][i] = shape[i*size_+j] == 'O' ? true : false;
        }
    }
}

// 시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCW() {
    std::string shapeStr;
    for(int i = 0; i < size(); i++) {
        for(int j = size()-1; j >= 0; j--) {
            shapeStr.append(shape_[i][j] ? "O" : "X");
        }
    }
    Tetromino result = Tetromino(name(), size(), shapeStr);
    result.original_ = original();
    return result;
}

// 반시계 방향으로 회전한 모습의 테트로미노 객체를 반환한다.
Tetromino Tetromino::rotatedCCW() {
    std::string shapeStr;
    for(int i = size()-1; i >= 0; i--) {
        for(int j = 0; j < size(); j++) {
            shapeStr.append(shape_[i][j] ? "O" : "X");
        }
    }
    Tetromino result = Tetromino(name(), size(), shapeStr);
    result.original_ = original();
    return result;
}

// 화면의 x, y 위치에 s 문자열로  테트로미노를 그린다
void Tetromino::drawAt(std::string s, int x, int y) {
    for(int i = 0; i < size(); i++) {
        for(int j = 0; j < size(); j++) {
            if(check(i, j)) {
                console::draw(x+i, y+j, s);
            }
        }
    }
}
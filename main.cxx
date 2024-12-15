#include "fltk.h"


enum Type {NOT = -1, O = 0, X = 1};

void checkWinner();

class Pool : public Fl_Box {
public:
    Pool(short xx, short yy, short ww, short hh) : Fl_Box(xx, yy, ww, hh), x(xx+ww/2), y(yy+hh/2), w(ww), h(hh) {};

    int handle(int event){
        if(event == FL_PUSH && type == Type::NOT){
            type = static_cast<Type>(turn);
            redraw();
            turn = !turn;
            checkWinner();
            return 1;
        }

        return 0;
    }

    void draw() override {
        if(type == Type::NOT) return;

        fl_line_style(FL_SOLID, 7);
        if(turn){
            fl_color(FL_GREEN);

            fl_line(x - delta, y + delta, x + delta, y - delta);
            fl_line(x + delta, y + delta, x - delta, y - delta);            
        }else{
            fl_color(FL_BLUE);
            fl_arc(x - delta, y - delta, delta*2, delta*2, 0, 360);
        }
        fl_line_style(0);
    }

    bool getType() const { return type; }
    bool isPushed() const { return type == Type::NOT? false : true; }

private:
    short x, y, w, h;
    short delta = 50;
    Type type{Type::NOT};
    static bool turn;
};
bool Pool::turn = true; // true - x / false - o 

Pool* board[3][3];

class TicToe : public Fl_Window {
public:
    TicToe() : Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT) {
        int cellWidth = WINDOW_WIDTH / 3;
        int cellHeight = WINDOW_HEIGHT / 3;
        for (int x = 0; x < 3; ++x) {
            for (int j = 0; j < 3; ++j) {
                board[x][j] = new Pool(x * cellWidth, j * cellHeight, cellWidth, cellHeight);
            }
        }
    }

    void draw() override {
        Fl_Window::draw();
        fl_line_style(FL_SOLID, 10);
        fl_color(FL_RED);

        fl_rect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        for (int i = 0; i < 3; ++i) {
            fl_line((WINDOW_WIDTH / 3) * i + WINDOW_WIDTH / 3, 0, (WINDOW_WIDTH / 3) * i + WINDOW_WIDTH / 3, WINDOW_HEIGHT);
            fl_line(0, (WINDOW_HEIGHT / 3) * i + WINDOW_HEIGHT / 3, WINDOW_WIDTH, (WINDOW_HEIGHT / 3) * i + WINDOW_HEIGHT / 3);
        }
        fl_line_style(0);
    }

private:
    static constexpr short WINDOW_WIDTH = 500;
    static constexpr short WINDOW_HEIGHT = 500;

};

void announceWinner(int winner) {
    const char* message = (winner == 0) ? "X wins!" : "O wins!";
    fl_message(message);
}

void checkWinner() {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0]->isPushed() && board[i][1]->isPushed() && board[i][2]->isPushed() && 
            board[i][0]->getType() == board[i][1]->getType() && board[i][1]->getType() == board[i][2]->getType()) {
            announceWinner(board[i][0]->getType());
            return;
        }
        if (board[0][i]->isPushed() && board[1][i]->isPushed() && board[2][i]->isPushed() && 
            board[0][i]->getType() == board[1][i]->getType() && board[1][i]->getType() == board[2][i]->getType()) {
            announceWinner(board[0][i]->getType());
            return;
        }
    }
    if (board[0][0]->isPushed() && board[1][1]->isPushed() && board[2][2]->isPushed() && 
        board[0][0]->getType() == board[1][1]->getType() && board[1][1]->getType() == board[2][2]->getType()) {
        announceWinner(board[0][0]->getType());
        return;
    }
    if (board[0][2]->isPushed() && board[1][1]->isPushed() && board[2][0]->isPushed() &&
        board[0][2]->getType() == board[1][1]->getType() && board[1][1]->getType() == board[2][0]->getType()) {
        announceWinner(board[0][2]->getType());
        return;
    }
}

int main(int argc, char const *argv[]) {
    TicToe* game = new TicToe();

    game->show();
    return Fl::run();
}

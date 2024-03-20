#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <random>
#include <ncurses.h>

using namespace std;

class Game {
    vector<vector<int>> tiles;
private:
    int randNum() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 100);
        return dis(gen);
    }

public:
    Game() {
        tiles.resize(4, vector<int>(4, 0));
        generate();
        generate();
    }

    string line(vector<int>& v) {
        stringstream ss;
        string sp = sep();
        ss << sp << endl;
        ss << '*';
        for (auto i : v) ss << tile(i);
        ss << "*" << endl;
        ss << sp << endl;
        return ss.str();
    }

    string sep() {
        string sep(32, ' ');
        return "*" + sep + "*";
    }

    string tile(int n) {
        if (n == 0) return string(8, ' ');
        string sn = to_string(n);
        if (sn.length()%2 != 0) sn.push_back(' ');
        int spaces = 8 - sn.length();
        string prefix(spaces/2, ' ');
        return prefix + sn + prefix;
    }

    string board() {
        string bar(34,'*');
        stringstream ss;
        ss << bar << endl;
        for (auto v: tiles) ss << line(v);
        ss << bar << endl;
        return ss.str();
    }

    bool full() {
        int count = 0;
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                if (tiles[i][j] == 0) count++;
            }
        }
        return count == 0;
    }

    void generate() {
        vector<pair<int,int>> emptyCells;
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                if (tiles[i][j] == 0) emptyCells.push_back({i, j});
            }
        }

        int rId = randNum() % emptyCells.size();
        int rn = (randNum() % 2 == 0) ? 2 : 4;
        tiles[emptyCells[rId].first][emptyCells[rId].second] = rn;
    }

    void left() {
        for (int r=0; r<4; r++) {
            int last = -1;
            bool merged = false;
            for (int c=0; c<4; c++) {
                if (tiles[r][c] == 0) continue;
                if (last != -1 && tiles[r][c] == tiles[r][last] && !merged) {
                    tiles[r][last] += tiles[r][c];
                    merged = true;
                } else {
                    last++;
                    tiles[r][last] = tiles[r][c];
                    merged = false;
                }
            }
            while (++last < 4) tiles[r][last] = 0;
        }
    }

    void right() {
        for (int r=0; r<4; r++) {
            int last = 4;
            bool merged = false;
            for (int c=3; c>=0; c--) {
                if (tiles[r][c] == 0) continue;
                if (last != 4 && tiles[r][c] == tiles[r][last] && !merged) {
                    tiles[r][last] += tiles[r][c];
                    merged = true;
                } else {
                    last--;
                    tiles[r][last] = tiles[r][c];
                    merged = false;
                }
            }
            while (--last >= 0) tiles[r][last] = 0;
        }
    }

    void up() {
        for (int c=0; c<4; c++) {
            int last = -1;
            bool merged = false;
            for (int r=0; r<4; r++) {
                if (tiles[r][c] == 0) continue;
                if (last != -1 && tiles[r][c] == tiles[last][c] && !merged) {
                    tiles[last][c] += tiles[r][c];
                    merged = true;
                } else {
                    last++;
                    tiles[last][c] = tiles[r][c];
                    merged = false;
                }
            }
            while (++last < 4) tiles[last][c] = 0;
        }
    }

    void down() {
        for (int c=0; c<4; c++) {
            int last = 4;
            bool merged = false;
            for (int r=3; r>=0; r--) {
                if (tiles[r][c] == 0) continue;
                if (last != 4 && tiles[r][c] == tiles[last][c] && !merged) {
                    tiles[last][c] += tiles[r][c];
                    merged = true;
                } else {
                    last--;
                    tiles[last][c] = tiles[r][c];
                    merged = false;
                }
            }
            while (--last >= 0) tiles[last][c] = 0;
        }
    }

};


int main() {
    Game game;
    game.board();

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    while (true) {
        clear();
        printw(game.board().c_str());
        refresh();

        int key = getch();
        switch(key) {
        case KEY_LEFT:
            game.left();
            break;
        case KEY_RIGHT:
            game.right();
            break;
        case KEY_UP:
            game.up();
            break;
        case KEY_DOWN:
            game.down();
            break;
        case 'q':
            clear();
            mvprintw(0, 0, "quit...");
            getch();
            return 0;
        }

        if (game.full()) {
            clear();
            mvprintw(0, 0, "game over.");
            getch();
            return 0;
        }
        game.generate();
    }

    endwin();
    return 0;
}

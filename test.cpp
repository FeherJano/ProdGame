#include <iostream>
#include <vector>
#include <algorithm> // std::shuffle
#include <random>    // std::default_random_engine
#include <ctime>     // std::time

using namespace std;

const int WIDTH = 21;  // A labirintus szélessége
const int HEIGHT = 21; // A labirintus magassága

// A mozgási irányok meghatározása: balra, jobbra, fel, le
int dx[] = {-2, 2, 0, 0};
int dy[] = {0, 0, -2, 2};

// Játékos osztály
class Player {
public:
    int x, y; // Játékos pozíciója

    Player(int startX, int startY) : x(startX), y(startY) {}

    // Játékos mozgatása, ha az új pozíció érvényes
    void move(char direction, const vector<vector<int>>& maze) {
        int newX = x;
        int newY = y;

        if (direction == 'w') newX--;
        if (direction == 's') newX++;
        if (direction == 'a') newY--;
        if (direction == 'd') newY++;

        // Csak akkor mozgatjuk, ha az új pozíció nem fal (azaz 1-es)
        if (newX >= 0 && newX < HEIGHT && newY >= 0 && newY < WIDTH && maze[newX][newY] == 1) {
            x = newX;
            y = newY;
        }
    }

    // Játékos pozíciójának megjelenítése
    void displayPosition() const {
        cout << "Játékos pozíció: (" << x << ", " << y << ")" << endl;
    }
};

// A labirintus megjelenítése a konzolon
void printMaze(const vector<vector<int>>& maze, const vector<Player>& players) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            bool isPlayerHere = false;
            for (const Player& player : players) {
                if (player.x == i && player.y == j) {
                    cout << "P"; // Játékos pozíciója
                    isPlayerHere = true;
                    break;
                }
            }
            if (!isPlayerHere) {
                cout << (maze[i][j] == 0 ? "#" : " ");
            }
        }
        cout << endl;
    }
}

// A pályageneráló algoritmus (DFS alapú)
void generateMaze(vector<vector<int>>& maze, int x, int y) {
    maze[x][y] = 1;

    // Véletlenszerű sorrendben lépkedünk
    vector<int> directions = {0, 1, 2, 3};
    
    // Shuffle használata
    random_device rd;
    mt19937 g(rd());
    shuffle(directions.begin(), directions.end(), g);

    // Végigmegyünk a 4 irányon
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[directions[i]];
        int ny = y + dy[directions[i]];

        // Ellenőrizzük, hogy a következő cella a labirintuson belül van-e és még nem látogatott-e
        if (nx > 0 && nx < HEIGHT-1 && ny > 0 && ny < WIDTH-1 && maze[nx][ny] == 0) {
            // A fal lebontása
            maze[x + dx[directions[i]] / 2][y + dy[directions[i]] / 2] = 1;
            generateMaze(maze, nx, ny);
        }
    }
}

int main() {
    srand(time(0)); // Véletlenszerűsítés inicializálása

    // 2D mátrix, kezdetben minden fal
    vector<vector<int>> maze(HEIGHT, vector<int>(WIDTH, 0));

    // A labirintus generálása
    generateMaze(maze, 1, 1);

    // Játékos hozzáadása
    vector<Player> players;
    players.push_back(Player(1, 1)); // Első játékos kezdőpozíciója

    char input;
    while (true) {
        // Labirintus megjelenítése
        printMaze(maze, players);

        // Játékos mozgatása
        cout << "Mozgás (w/a/s/d, q a kilépéshez): ";
        cin >> input;

        if (input == 'q') break; // Kilépés

        // Játékos 0 mozgatása
        players[0].move(input, maze);

        // Játékos aktuális pozíciója
        players[0].displayPosition();
    }

    return 0;
}

// Snake Game with Walls, Tail, Obstacles, Bonus Items
// Name: Shubham Agarwal | UID: 23bcs11878

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h> // for usleep()
using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

struct Node {
    int x, y;
    Node* next;
    Node(int _x, int _y) : x(_x), y(_y), next(nullptr) {}
};

Node* head = nullptr;
int score = 0;
bool gameOver = false;
int dirX = 0, dirY = 1; // initial direction → right
char board[HEIGHT][WIDTH];
int foodX, foodY;

void initBoard() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            board[i][j] = ' ';
}

void addWall(int x, int y) {
    board[x][y] = '#';
}

void addObstacle(int x, int y) {
    board[x][y] = '@';
}

void addBonus(int x, int y) {
    board[x][y] = '$';
}

void spawnFood() {
    do {
        foodX = rand() % HEIGHT;
        foodY = rand() % WIDTH;
    } while (board[foodX][foodY] != ' ');
    board[foodX][foodY] = '*';
}

void printBoard(Node* head) {
    // Reset board before drawing snake
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            if (board[i][j] == 'O' || board[i][j] == 'o') board[i][j] = ' ';

    Node* temp = head;
    bool isHead = true;
    while (temp != nullptr) {
        if (isHead) board[temp->x][temp->y] = 'O';
        else board[temp->x][temp->y] = 'o';
        isHead = false;
        temp = temp->next;
    }

    system("clear"); // use "cls" on Windows
    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << "\n";

    for (int i = 0; i < HEIGHT; i++) {
        cout << "#";
        for (int j = 0; j < WIDTH; j++) {
            cout << board[i][j];
        }
        cout << "#\n";
    }

    for (int i = 0; i < WIDTH + 2; i++) cout << "#";
    cout << "\nScore: " << score << "\n";
}

void moveSnake(Node*& head) {
    int newX = head->x + dirX;
    int newY = head->y + dirY;

    // Check bounds
    if (newX < 0 || newX >= HEIGHT || newY < 0 || newY >= WIDTH) {
        gameOver = true;
        return;
    }

    char nextCell = board[newX][newY];

    // Check collision
    if (nextCell == '#' || nextCell == '@' || nextCell == 'o') {
        gameOver = true;
        return;
    }

    // Create new head
    Node* newHead = new Node(newX, newY);
    newHead->next = head;
    head = newHead;

    if (nextCell == '*') {
        score += 10;
        spawnFood();
    }
    else if (nextCell == '$') {
        score += 30;
        board[newX][newY] = ' '; // remove bonus item
    }
    else {
        // Remove tail
        Node* temp = head;
        while (temp->next->next != nullptr)
            temp = temp->next;
        delete temp->next;
        temp->next = nullptr;
    }
}

void setupGame() {
    srand(time(0));
    initBoard();

    // Initial snake head at center
    int startX = HEIGHT / 2;
    int startY = WIDTH / 2;
    head = new Node(startX, startY);
    board[startX][startY] = 'O';

    // Add horizontal wall
    for (int i = 5; i < 15; i++) addWall(10, i);

    // Add vertical obstacle
    for (int i = 5; i < 10; i++) addObstacle(i, 5);

    // Add bonus item
    addBonus(3, 3);

    spawnFood();
}

int main() {
    setupGame();
    char ch;

    while (!gameOver) {
        printBoard(head);

        if (cin.peek() != EOF) {
            cin >> ch;
            switch (ch) {
                case 'w': dirX = -1; dirY = 0; break;
                case 's': dirX = 1; dirY = 0; break;
                case 'a': dirX = 0; dirY = -1; break;
                case 'd': dirX = 0; dirY = 1; break;
            }
        }

        moveSnake(head);
        usleep(200000); // 200ms delay
    }

    cout << "\nGame Over! Final Score: " << score << endl;
    return 0;
}
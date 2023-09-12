#include <iostream>
#include <cmath>

using namespace std;

bool isSafe(int **board, int row, int col, int n) { // Проверяем, можно ли разместить ферзя
    int i, j;
    for (i = 0; i < row; i++) // Проверяем вертикальную линию
        if (board[i][col])
            return false;
    for (i = row, j = col; i >= 0 && j >= 0; i--, j--) // Проверяем левую диагональ
        if (board[i][j])
            return false;
    for (i = row, j = col; i >= 0 && j < n; i--, j++)  // Проверяем правую диагональ
        if (board[i][j])
            return false;
    return true;
}

void printBoard(int **board, int n) {
    static int count = 0;
    cout << "Solution " << ++count << ":\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j])
                cout << "Q ";
            else
                cout << "- ";
        }
        cout << endl;
    }
    cout << endl;
}

void solve(int **board, int row, int n) { 
    if (row == n) {
        printBoard(board, n);  // вывод решенеия
        return;
    }
    for (int i = 0; i < n; i++) {
        if (isSafe(board, row, i, n)) {
            board[row][i] = 1; // Размещаем ферзя
            solve(board, row + 1, n);  // Проверяем, приводит ли это к решению
            board[row][i] = 0; // Убираем ферзя и пробуем следующую колонку(элемент строки + 1..n)
        }
    }
}

int main() {
    int n;
    cout << "Enter the size of the board: ";
    cin >> n;
    int **board = new int *[n];
    for (int i = 0; i < n; i++) {
        board[i] = new int[n];
        for (int j = 0; j < n; j++) {
            board[i][j] = 0;
        }
    }
    solve(board, 0, n);
    for (int i = 0; i < n; i++) {
        delete[] board[i];
    }
    delete[] board;
    return 0;
}
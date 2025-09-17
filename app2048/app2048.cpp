#include <iostream>
#include <vector>
#include <utility>    // Para std::pair
#include <ctime>      // Para time()
#include <cstdlib>    // Para srand() e rand()
#include <algorithm>  // Para std::reverse e std::swap
#include <conio.h>    // Para _getch() no Windows

using namespace std;

// O enum é mais seguro e moderno como 'enum class'
enum class Direction { UP, DOWN, LEFT, RIGHT };

class Game {
public: // ✅ CORREÇÃO: Membros agora são públicos para que main possa acessá-los
    int table[4][4];
    int score;

    // Construtor: Prepara o jogo quando um objeto é criado
    Game() {
        score = 0;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                table[i][j] = 0;
            }
        }
    }

    // -- Funções de Lógica e Exibição --

    void printTable() {
        system("cls"); // Limpa o console (funciona no Windows)
        cout << "2048 | Pontuacao: " << score << endl;
        cout << "-------------------------" << endl;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (table[i][j] == 0) {
                    cout << ".\t";
                }
                else {
                    cout << table[i][j] << "\t";
                }
            }
            cout << endl << endl;
        }
        cout << "-------------------------" << endl;
        cout << "Use as setas para mover. ESC para sair." << endl;
    }

    void addNewTile() {
        vector<pair<int, int>> empty_spaces;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (table[i][j] == 0) {
                    empty_spaces.push_back({ i, j });
                }
            }
        }
        if (empty_spaces.empty()) { return; }

        int random_index = rand() % empty_spaces.size();
        pair<int, int> random_spot = empty_spaces[random_index];
        int new_value = (rand() % 10 < 9) ? 2 : 4;
        table[random_spot.first][random_spot.second] = new_value;
    }

    // A função PÚBLICA que o main irá chamar para TODOS os movimentos
    bool move(Direction dir) {
        bool changed = false;
        switch (dir) {
        case Direction::LEFT:
            changed = moveLeftLogic();
            break;
        case Direction::RIGHT:
            reverseRows();
            changed = moveLeftLogic();
            reverseRows();
            break;
        case Direction::UP:
            transpose();
            changed = moveLeftLogic();
            transpose();
            break;
        case Direction::DOWN:
            transpose();
            reverseRows();
            changed = moveLeftLogic();
            reverseRows();
            transpose();
            break;
        }
        return changed;
    }

private: // Funções auxiliares que só a classe precisa conhecer

    // ✅ CORREÇÃO: Loop interno corrigido para evitar troca dupla
    void transpose() {
        for (int i = 0; i < 4; ++i) {
            for (int j = i + 1; j < 4; ++j) {
                swap(table[i][j], table[j][i]);
            }
        }
    }

    // ✅ CORREÇÃO: 'const' removido pois a função modifica a classe
    void reverseRows() {
        for (int i = 0; i < 4; ++i) {
            reverse(table[i], table[i] + 4);
        }
    }

    // ✅ CORREÇÃO: Lógica de movimento robusta e funcional
    bool moveLeftLogic() {
        bool moved = false;
        for (int i = 0; i < 4; ++i) {
            vector<int> temp;
            for (int j = 0; j < 4; ++j) {
                if (table[i][j] != 0) {
                    temp.push_back(table[i][j]);
                }
            }

            for (size_t j = 0; j < temp.size() && j + 1 < temp.size(); ++j) {
                if (temp[j] == temp[j + 1]) {
                    temp[j] *= 2;
                    score += temp[j]; // Atualiza a pontuação
                    temp.erase(temp.begin() + j + 1); // Remove o segundo elemento do par
                }
            }

            for (int j = 0; j < 4; ++j) {
                int newValue = (j < temp.size()) ? temp[j] : 0;
                if (table[i][j] != newValue) {
                    moved = true; // Marca que o tabuleiro mudou
                }
                table[i][j] = newValue;
            }
        }
        return moved;
    }
};

int main() {
    srand(time(0)); // Inicializa o gerador de números aleatórios

    Game game; // Cria o objeto do jogo
    game.addNewTile();
    game.addNewTile();
    game.printTable();

    while (true) {
        int key = _getch();
        bool changed = false;

        if (key == 27) { break; } // Sai do jogo se a tecla ESC for pressionada

        if (key == 0xE0) { // Prefixo para teclas especiais no Windows
            int arrowKey = _getch();
            switch (arrowKey) {
                // ✅ CORREÇÃO: Chamando a função 'move' correta para cada direção
            case 75: changed = game.move(Direction::LEFT); break;  // Esquerda
            case 77: changed = game.move(Direction::RIGHT); break; // Direita
            case 72: changed = game.move(Direction::UP); break;    // Cima
            case 80: changed = game.move(Direction::DOWN); break;  // Baixo
            }
        }

        if (changed) {
            game.addNewTile();
            game.printTable();
        }

        // TODO: Adicionar verificação de fim de jogo (se não houver mais movimentos)
    }

    cout << "Fim de jogo! Pontuacao final: " << game.score << endl;
    return 0;
}

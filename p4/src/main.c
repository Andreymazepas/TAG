#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

int getposX(int X);
int getposY(int Y);
void desenhaFundo();             // desenha o fundo e as intrucoes
void inicializaCampo();          // inicializa a matriz do jogo com os valores da matriz fixa do jogo;
void desenhaCampo(int x, int y); // desenha os numeros do jogo, selecionando onde esta o cursor
int possuiIgual(int x, int y);   // checa se possui numero igual na col/linha/quadrado;
void geraSolucao();              // Gera o passo-a-passo da solução do sudoku
void criaNovoJogo();             // Cria nova matriz fixa aleatoria

int sudokuInitial[9][9] = {{1, 0, 0, 0, 0, 0, 0, 0, 0},
                           {0, 2, 0, 0, 0, 0, 0, 0, 0},
                           {0, 0, 3, 0, 0, 0, 0, 0, 0},

                           {0, 0, 0, 4, 0, 0, 0, 0, 0},
                           {0, 0, 0, 0, 5, 0, 0, 0, 0},
                           {0, 0, 0, 0, 0, 6, 0, 0, 0},

                           {0, 0, 0, 0, 0, 0, 7, 0, 0},
                           {0, 0, 0, 0, 0, 0, 0, 8, 0},
                           {0, 0, 0, 0, 0, 0, 0, 0, 9}};

/*int sudokuInitial[9][9] =  {{8, 0, 0,    1, 5, 0,   6, 0, 0},
                            {0, 0, 0,    3, 0, 0,   0, 4, 1},
                            {5, 0, 0,    0, 0, 0,   7, 0, 0},
                            
                            {0, 0, 0,    0, 0, 9,   0, 6, 2},
                            {0, 0, 0,    0, 3, 0,   0, 0, 0},
                            {1, 4, 0,    8, 0, 0,   0, 0, 0},
                            
                            {0, 0, 8,    0, 0, 0,   0, 0, 9},
                            {2, 9, 0,    0, 0, 1,   0, 0, 0},
                            {0, 0, 5,    0, 9, 7,   0, 0, 6}};*/

int sudoku[9][9];

int main(void)
{
    srand(time(NULL));

    int y, x; // variaveis de uso situacional para x e y, como cursor ou posicao de numero
    int ch;   // caractere lido

    /* inicializa ncurses */

    initscr();
    clear();
    keypad(stdscr, TRUE); //habilita uso de setas e outras teclas
    cbreak();
    noecho();

    if (has_colors())
    {
        // cria os pares para facilitar uso das cores no jogo
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);   //normal
        init_pair(2, COLOR_CYAN, COLOR_BLACK);    //numero fixo
        init_pair(3, COLOR_BLACK, COLOR_CYAN);    //numero fixo selecionado
        init_pair(4, COLOR_BLACK, COLOR_GREEN);   //normal selecionado
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK); //alerta
        init_pair(6, COLOR_BLACK, COLOR_MAGENTA); //alerta selecionado
    }
    else
    {
        printw("Terminal nao suporta cores!\n");
        getch();
        exit(EXIT_FAILURE);
    }

    desenhaFundo();
    inicializaCampo();

    //inicializa o cursor
    y = 0;
    x = 0;
    int isRunning = 1;
    /* loop principal */
    while (isRunning)
    {
        desenhaCampo(x, y);
        refresh();
        ch = getch();
        switch (ch)
        {
        case KEY_UP:
        case 'w':
        case 'W':
            if (y > 0)
            {
                y = y - 1;
            }
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if (y < 8)
            {
                y = y + 1;
            }
            break;
        case KEY_LEFT:
        case 'a':
        case 'A':
            if (x > 0)
            {
                x = x - 1;
            }
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if (x < 8)
            {
                x = x + 1;
            }
            break;
        case 'q':
        case 'Q':
            isRunning = 0;
            break;
        case KEY_BACKSPACE:
        case 127:
        case 'x':
        case 'X':
            if (!sudokuInitial[x][y])
                sudoku[x][y] = '.' - 48;
            break;
        case 'r':
        case 'R':
            inicializaCampo();
            break;
        case 'c':
        case 'C':
            geraSolucao();
            break;
        case 'n':
        case 'N':
            inicializaCampo();
            criaNovoJogo();
            inicializaCampo();
            refresh();
            break;
        }

        if ((ch > '0' && ch <= '9'))
        {
            if (!sudokuInitial[x][y])   //se o usuario nao esta tentando escrever em cima da inicial
                sudoku[x][y] = ch - 48; //atualiza a matriz de jogo
        }
    }

    endwin();

    exit(0);
}

int getposX(int x)
{
    int posx = x * 2 + 3;
    if (x > 2)
        posx += 2;
    if (x > 5)
        posx += 2;
    return posx;
}

int getposY(int y)
{
    int posy = y + 1;
    if (y > 2)
        posy++;
    if (y > 5)
        posy++;
    return posy;
}

void desenhaFundo()
{
    //desenha o background
    for (int y = 0; y < LINES; y++)
    {
        mvhline(y, 0, ' ', COLS);
    }
    //linhas verticais
    mvvline(1, 1, ACS_VLINE, 11);
    mvvline(1, 9, ACS_VLINE, 11);
    mvvline(1, 17, ACS_VLINE, 11);
    mvvline(1, 25, ACS_VLINE, 11);
    //linhas horizontais
    mvhline(0, 1, ACS_HLINE, 24);
    mvhline(4, 1, ACS_HLINE, 24);
    mvhline(8, 1, ACS_HLINE, 24);
    mvhline(12, 1, ACS_HLINE, 24);
    //cantos
    mvaddch(0, 1, ACS_ULCORNER);
    mvaddch(0, 25, ACS_URCORNER);
    mvaddch(12, 1, ACS_LLCORNER);
    mvaddch(12, 25, ACS_LRCORNER);
    mvaddch(4, 1, ACS_LTEE);
    mvaddch(8, 1, ACS_LTEE);
    mvaddch(4, 25, ACS_RTEE);
    mvaddch(8, 25, ACS_RTEE);
    mvaddstr(3, 30, "Setas ou WASD para mover");
    mvaddstr(4, 30, "Inserir numeros normalmente");
    mvaddstr(5, 30, "Q para sair");
    mvaddstr(6, 30, "X ou Backspace para apagar");
    mvaddstr(7, 30, "R para reiniciar");
    mvaddstr(8, 30, "C para apresentar a solução");
    mvaddstr(9, 30, "N para gerar um novo jogo");
    return;
}

void criaNovoJogo()
{
    // zera a matriz fixa
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            sudokuInitial[i][j] = 0;
        }
    }

    // gera novos numeros para a matriz fixa
    // funciona com numeros aleatorios, checando se podem fazer parte de uma solucaos
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (rand() % 10 > 7)
            {
                int new = rand() % 10;
                sudokuInitial[i][j] = new;
                if (possuiIgual(i, j))
                {
                    new = rand() % 10;
                    sudokuInitial[i][j] = new;
                    if (possuiIgual(i, j))
                        sudokuInitial[i][j] = 0;
                }
            }
        }
    }

    return;
}

void desenhaCampo(int x, int y)
{
    //printa os numeros
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (i == y && j == x)
            { //mudar a cor do selecionado atual
                attron(COLOR_PAIR(4));
                if (sudokuInitial[j][i])
                    attron(COLOR_PAIR(3));
                else if (possuiIgual(j, i))
                {
                    attron(COLOR_PAIR(6));
                }
                mvaddch(getposY(i), getposX(j), sudoku[j][i] + 48);
            }
            else
            {
                attron(COLOR_PAIR(1));
                if (sudokuInitial[j][i])
                    attron(COLOR_PAIR(2));
                else if (possuiIgual(j, i))
                {
                    attron(COLOR_PAIR(5));
                }
                mvaddch(getposY(i), getposX(j), sudoku[j][i] + 48);
            }
        }
    }
    mvaddch(1, 30, x + 48);
    mvaddch(1, 32, y + 48);
}

void inicializaCampo()
{
    memcpy(sudoku, sudokuInitial, 9 * 9 * sizeof(int)); //copia a matriz inicial para a matriz de jogo;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (sudoku[i][j] == 0)
                sudoku[i][j] = '.' - 48;
        }
    }
}

int possuiIgual(int x, int y)
{
    for (int i = 0; i < 9; i++)
    {
        if (i != x &&
            sudoku[x][y] == sudoku[i][y] &&
            sudoku[x][y] != '.' - 48)
            return 1;
    }
    for (int i = 0; i < 9; i++)
    {
        if (i != y &&
            sudoku[x][y] == sudoku[x][i] &&
            sudoku[x][y] != '.' - 48)
            return 1;
    }
    for (int i = (y / 3) * 3; i < (y / 3) * 3 + 3; i++)
    {
        for (int j = (x / 3) * 3; j < (x / 3) * 3 + 3; j++)
        {
            if (j != x &&
                i != y &&
                sudoku[x][y] == sudoku[j][i] &&
                sudoku[x][y] != '.' - 48)
                return 1;
        }
    }

    return 0;
}

void geraSolucao()
{
    int x, y, n;

    for (x = 0; x < 9; x++)
    {
        for (y = 0; y < 9; y++)
        { //Percorre o Grafo
            if (!sudokuInitial[x][y])
            { //Caso não seja um vértice do grafo inicial
                for (n = 1; n <= 9; n++)
                {
                    sudoku[x][y] = n; //Atualiza a matriz do jogo
                    if (!possuiIgual(x, y))
                    {
                        break;
                    }
                }
                //getchar();
                desenhaCampo(x, y);
                refresh();
                usleep(100000);
            }
        }
    }
    //printf("Teste\n");
    return;
}
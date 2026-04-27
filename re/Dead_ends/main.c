#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>

#define WIDTH 45
#define HEIGHT 23

int opposite[] = {1, 0, 3, 2};
int dy[] = {1, -1, 0, 0};
int dx[] = {0, 0, 1, -1};

typedef struct {
    bool active;
    bool visited;
    bool walls[4];
} Cell;

typedef struct {
    int width;
    int height;
    Cell matrix[HEIGHT][WIDTH];
    int pos_x, pos_y;
    int tella_x, tella_y;
    bool won;
} Maze;

void set_active(Maze* m, int x, int y, bool b) {
    m->matrix[y][x].active = b;
}

void set_visited(Maze* m, int x, int y) {
    m->matrix[y][x].visited = true;
}

bool in_bounds(Maze* m, int x, int y) {
    return x >= 0 && x < m->width && y >= 0 && y < m->height;
}

bool is_visited(Maze* m, int x, int y) {
    return m->matrix[y][x].visited;
}

bool is_wall(Maze* m, int x, int y, int dir) {
    return m->matrix[y][x].walls[dir];
}

void break_wall(Maze* m, int x, int y, int dir) {
    m->matrix[y][x].walls[dir] = false;
    m->matrix[y + dy[dir]][x + dx[dir]].walls[opposite[dir]] = false;
}

void generate(Maze* m) {
    int stack[WIDTH * HEIGHT][2];
    int top = 0;
    int cx = m->width / 2, cy = m->height / 2;
    stack[top][0] = cx; stack[top++][1] = cy;
    set_visited(m, cx, cy);
    srand(time(NULL));

    while (top > 0) {
        int x = stack[top - 1][0];
        int y = stack[top - 1][1];
        int dirs[] = {0, 1, 2, 3};
        for (int i = 3; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = dirs[i];
            dirs[i] = dirs[j];
            dirs[j] = temp;
        }

        bool backtrack = true;
        for (int i = 0; i < 4; i++) {
            int dir = dirs[i];
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (in_bounds(m, nx, ny) && !is_visited(m, nx, ny)) {
                break_wall(m, x, y, dir);
                set_visited(m, nx, ny);
                stack[top][0] = nx; stack[top++][1] = ny;
                backtrack = false;
                break;
            }
        }

        if (backtrack) top--;
    }

    m->matrix[m->height - 1][m->width - 1].walls[2] = false;
}

bool end_square(Maze* m) {
    return (m->pos_x == m->width - 1 && m->pos_y == m->height - 1);
}

int move_player(Maze* m, int dir) {
    int nx = m->pos_x + dx[dir];
    int ny = m->pos_y + dy[dir];
    if (in_bounds(m, nx, ny) && !is_wall(m, m->pos_x, m->pos_y, dir)) {
        set_active(m, m->pos_x, m->pos_y, false);
        m->pos_x = nx;
        m->pos_y = ny;
        set_active(m, m->pos_x, m->pos_y, true);
        return 1;
    } else if (end_square(m) && dir == 2) {
        m->won = true;
        set_active(m, m->pos_x, m->pos_y, false);
        m->pos_x = m->pos_y = -1;
        return 1;
    }
    return 0;
}

void visited_reset(Maze* m) {
    for (int y = 0; y < m->height; y++)
        for (int x = 0; x < m->width; x++)
            m->matrix[y][x].visited = false;
}

int solve(Maze* m, int out[][3]) {
    visited_reset(m);
    int stack[WIDTH * HEIGHT][3];
    int top = 0;
    stack[top][0] = stack[top][1] = 0;
    stack[top++][2] = 0;
    set_visited(m, 0, 0);

    while (top > 0) {
        int x = stack[top - 1][0];
        int y = stack[top - 1][1];
        if (x == m->width - 1 && y == m->height - 1) break;
        bool backtrack = true;
        int dirs[] = {0, 2, 1, 3};
        for (int i = 0; i < 4; i++) {
            int dir = dirs[i];
            int nx = x + dx[dir];
            int ny = y + dy[dir];
            if (in_bounds(m, nx, ny) && !is_visited(m, nx, ny) && !is_wall(m, x, y, dir)) {
                set_visited(m, nx, ny);
                stack[top][0] = nx;
                stack[top][1] = ny;
                stack[top++][2] = opposite[dir];
                backtrack = false;
                break;
            }
        }
        if (backtrack) top--;
    }
    memcpy(out, stack, sizeof(int) * 3 * top);
    return top;
}

void fun_twist(Maze* m) {
    int stack[WIDTH * HEIGHT][3];
    int len = solve(m, stack);
    int i = len * 4 / 5;
    int x = stack[i][0], y = stack[i][1], dir = stack[i][2];
    m->tella_x = x; m->tella_y = y;
    m->matrix[y][x].walls[dir] = true;
    m->matrix[y + dy[dir]][x + dx[dir]].walls[opposite[dir]] = true;
}

void tella(Maze* m) {
    set_active(m, m->pos_x, m->pos_y, false);
    m->pos_x = m->tella_x;
    m->pos_y = m->tella_y;
    set_active(m, m->pos_x, m->pos_y, true);
}

void print_maze_ascii(Maze* m) {
    for (int y = m->height - 1; y >= 0; y--) {
        for (int x = 0; x < m->width; x++) {
            printf("+");
            printf(m->matrix[y][x].walls[0] ? "---" : "   ");
        }
        printf("+\n");
        for (int x = 0; x < m->width; x++) {
            printf(m->matrix[y][x].walls[3] ? "|" : " ");
            printf(m->matrix[y][x].active ? " X " : "   ");
        }
        printf(m->matrix[y][m->width - 1].walls[2] ? "|\n" : " \n");
    }
    for (int x = 0; x < m->width; x++) {
        printf("+");
        printf(m->matrix[0][x].walls[1] ? "---" : "   ");
    }
    printf("+\n");
    printf("\nUse arrow keys to move, r to refresh view, and q to quit. Exit the Maze for your flag.\n");
}

void move_cursor_to_cell(int x, int y, int width, int height) {
    // Converts maze (x,y) to terminal coordinates
    int row = (height - y - 1) * 2 + 2;
    int col = x * 4 + 2;
    printf("\033[%d;%dH", row, col);
}

void draw_player(Maze *maze, int dir) {
    // Compute previous position using direction
    int prev_x = maze->pos_x - dx[dir];
    int prev_y = maze->pos_y - dy[dir];

    // Erase previous player marker
    move_cursor_to_cell(prev_x, prev_y, maze->width, maze->height);
    printf("   ");

    // Draw new player marker
    move_cursor_to_cell(maze->pos_x, maze->pos_y, maze->width, maze->height);
    printf(" X ");

    int below_row = maze->height * 2 + 4;
    printf("\033[%d;1H", below_row);

    fflush(stdout);
}

char get_key() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void clear_screen() {
    printf("\033[2J\033[1;1H");
}

void check_dim(Maze* maze){
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        perror("ioctl");
        return;
    }
    if(w.ws_row < 50 || w.ws_col < 181){
        clear_screen();
        printf("Terminal not big enough, expand terminal to full screen.\n");
        while(w.ws_row < 50 || w.ws_col < 181) ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        clear_screen();
        print_maze_ascii(maze);
    }
    return;
}

int main() {
    Maze maze = {WIDTH, HEIGHT};
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++) {
            maze.matrix[y][x].active = false;
            maze.matrix[y][x].visited = false;
            for (int d = 0; d < 4; d++) maze.matrix[y][x].walls[d] = true;
        }
    maze.pos_x = maze.pos_y = 0;
    maze.won = false;
    generate(&maze);
    set_active(&maze, 0, 0, true);
    fun_twist(&maze);

    const char* SEQ[] = {"A","A","B","B","D","C","D","C","b","a","\n"};
    int seq_i = 0;

    clear_screen();
    print_maze_ascii(&maze);

    while (!maze.won) {
        char* key = NULL;
        while(!key){
            check_dim(&maze);
            char c = get_key();
            if (c == '\x1b') {
                char seq[2];
                if (read(STDIN_FILENO, seq, 2) == 2) {
                    if (seq[0] == '[') {
                        if (seq[1] == 'A') key = "A";
                        else if (seq[1] == 'B') key = "B";
                        else if (seq[1] == 'C') key = "C";
                        else if (seq[1] == 'D') key = "D";
                    }
                }
            } else if (c == 'q'){
                printf("Quiting...\n");
                return 0;
            } else if (c == 'r'){
                clear_screen();
                print_maze_ascii(&maze);
            } else if (c == 'a'){
                key = "a";
            } else if (c == 'b'){
                key = "b";
            } else if (c == '\n'){
                key = "\n";
            }
        }
        if (key) {
            if (strcmp(key, SEQ[seq_i]) == 0) seq_i++;
            else seq_i = 0;
            if (seq_i == 11) {
                tella(&maze);
                clear_screen();
                print_maze_ascii(&maze);
                seq_i = 0;
            } else {
                if (!strcmp(key,"A")) {if (move_player(&maze,0)) draw_player(&maze,0);}
                if (!strcmp(key,"B")) {if (move_player(&maze,1)) draw_player(&maze,1);}
                if (!strcmp(key,"C")) {if (move_player(&maze,2)) draw_player(&maze,2);}
                if (!strcmp(key,"D")) {if (move_player(&maze,3)) draw_player(&maze,3);}
            }
        }
    }
    FILE* f = fopen("flag.txt","r");
    if (f != NULL) {
        char flag[256];
        fgets(flag, sizeof(flag), f);
        fclose(f);
        clear_screen();
        printf("🎉 You beat the maze! 🎉\n");
        printf("Here is your flag: %s\n", flag);
    }
    return 0;
}
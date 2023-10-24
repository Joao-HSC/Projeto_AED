#ifndef tiles_h_
#define tiles_h_
#include <stdbool.h>

typedef struct Coordinates {
    int row;
    int col;
} Coordinates;

typedef struct Coordinates_plus {
    int row;
    int col;
    int score;
    struct Coordinates_plus* next;
} Coordinates_plus;

int coords_analyze(int** tileset, bool** visited, int row, int col, int num_rows, int num_col);
Coordinates_plus* coords_list(int** tileset, bool ** visited, int num_rows, int num_col);
int **coords_replace(int** tileset, int row, int col, int n_col, int n_row);
int score(int n_tiles);
int **gravity(int **tileset, int num_rows, int num_columns);

#endif

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

/* element for the b&b algorithm */
typedef struct Best_case {
    int tile_n;
    int freq;
    struct Best_case* next;
} Best_case;

int coords_analyze(int** tileset, bool** visited, int row, int col, int n_rows, int n_col);
Coordinates_plus* coords_list(int** tileset, bool ** visited, int n_rows, int num_col);
int** coords_replace(int** tileset, int row, int col, int n_col, int n_row);
int score(int n_tiles);
int** gravity(int **tileset, int n_rows, int n_cols);
int best_score_possible(int** tileset, int n_rows, int n_cols);

#endif

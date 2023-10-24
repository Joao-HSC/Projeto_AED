#ifndef vars_h_
#define vars_h_
#include "tiles.h"

typedef struct Node {
    struct Node* parent;
    struct Coordinates* coordinates;
    struct Coordinates_plus* children;
    struct Node* child;
    int** tileset;
    int score;
} Node;

Coordinates_plus* dfs_2(int** tileset, int v, int n_rows, int n_cols, bool** visited);
Coordinates_plus* dfs_3(int** tileset, int n_rows, int n_cols, bool** visited);
Coordinates_plus* extract_path_2(Node* current);
Coordinates_plus* extract_path_3(Node* current);
Node* create_child(Node* current, Coordinates_plus* node_aux, int n_rows, int n_cols, bool** visited);
Node* next_branch(Node* current);

#endif

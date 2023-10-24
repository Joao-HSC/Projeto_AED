#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tiles.h"
#include "stack.h"

/******************************************************************************
 * score()
 *
 * Arguments: n_tiles (the number of tiles)
 * 
 * Returns: n_tiles*(n_tiles - 1) (= value of the score)
 * 
 *
 * Description: Calculates the score based on the input number
 *****************************************************************************/
int score(int n_tiles){
    return n_tiles*(n_tiles - 1);
}

/******************************************************************************
 * coords_analyze()
 *
 * Arguments: tileset, visited, row, col, n_row and n_col
 * row, col, n_col and n_row
 * 
 * Returns: n_adj
 * 
 *
 * Description: Analyzes the matrix and returns the number of adjacent 
 * tiles that share the same colour
 *****************************************************************************/
int coords_analyze(int** tileset, bool ** visited, int row, int col, int num_rows, int num_col) {
    static int drow[] = { 0, 0, -1, 1 };
    static int dcol[] = { -1, 1, 0, 0 };
    int tile_n = tileset[row][col];
    int n_adj = 1;

    if (tile_n == -1 || visited[row][col] == 1) return 1;

    visited[row][col] = 1;

    Stack* front = newStack(2 * num_col * num_rows / 3 + 1);

    Coordinates aux = {row, col};
    push(front, aux);

    while (!is_empty(front)) {
        Coordinates current = pop(front);

        for (int i = 0; i < 4; i++) {
            int adj_row = current.row + drow[i];
            int adj_col = current.col + dcol[i];

            if (0 <= adj_col && adj_col < num_col && 0 <= adj_row && adj_row < num_rows && visited[adj_row][adj_col] != 1) {
                if (tileset[adj_row][adj_col] == tile_n) {
                    n_adj++;
                    visited[adj_row][adj_col] = 1;

                    Coordinates new_aux = {adj_row, adj_col};
                    push(front, new_aux);
                }
            }
        }
    }

    free(front->array);
    free(front);
    return n_adj;
}

/******************************************************************************
 * coords_replace()
 *
 * Arguments: tileset, row, col, num_col and num_row
 * 
 * Returns: tileset (after all the changes)
 * 
 *
 * Description: Changes all the values of the same colour adjacent tiles to "-1"
 *****************************************************************************/

int **coords_replace(int** tileset, int row, int col, int num_row, int num_col) {
    static int drow[] = { 0, 0, -1, 1 };
    static int dcol[] = { -1, 1, 0, 0 };
    int tile_n = tileset[row][col];

    if (tile_n == -1) return tileset;

    Stack* front = newStack(2 * num_col * num_row / 3 + 1);

    Coordinates aux = {row, col};
    push(front, aux);

    while (!is_empty(front)) {
        Coordinates current = pop(front);

        for (int i = 0; i < 4; i++) {
            int adj_row = current.row + drow[i];
            int adj_col = current.col + dcol[i];

            if (0 <= adj_col && adj_col < num_col && 0 <= adj_row && adj_row < num_row) {
                if (tileset[adj_row][adj_col] == tile_n) {
                    
                    tileset[adj_row][adj_col] = -1; // Mark as visited.

                    Coordinates new_aux = { adj_row, adj_col };
                    push(front, new_aux);
                    tileset[row][col] = -1;
                }
            }
        }
    }
    
    free(front->array);
    free(front);
    return tileset;
}

/******************************************************************************
 * gravity()
 *
 * Arguments: tileset, num_rows and num_columns.
 * 
 * Returns: tileset (after all the changes)
 * 
 *
 * Description: Aplies gravity to the colourful tiles, then aplies the 
 * horizontal push to each individual column.
 *****************************************************************************/

int **gravity(int **tileset, int num_rows, int num_columns) {
   for (int i = 0; i < num_columns; i++) {
    int fall = 0;
    
    /* Start from the bottom of the column and move upwards. */
    for (int j = 0; j < num_rows; j++) {
        if (tileset[j][i] != -1) {
            tileset[j - fall][i] = tileset[j][i];
            if (fall > 0) {
                tileset[j][i] = -1;
            }
        } else {
            fall++;
        }
    }

    /* Checks if the whole column is empty or not, if it is empty, move all 
    * the columns to the left one posision to the rigth.
    */
    if (fall == num_rows && i != 0) {
        for (int k = 0; k < num_rows; k++) {
            for (int h = i; h > 0; h--) { 
                tileset[k][h] = tileset[k][h - 1]; 
            }
            tileset[k][0] = -1; 
        }
    }
}

return tileset;

}

/******************************************************************************
 * coords_list()
 *
 * Arguments: tileset, visited, n_row and n_col
 * row, col, n_col and n_row
 * 
 * Returns: coordinates_list
 * 
 *
 * Description: Analyzes the matrix and returns a a list with the stains' coordinates
 *****************************************************************************/
Coordinates_plus* coords_list(int** tileset, bool** visited, int num_rows, int num_col) {
    static int drow[] = {0, 0, -1, 1};
    static int dcol[] = {-1, 1, 0, 0};
    int tile_n, n_adj;

    Stack* front = newStack(num_col * num_rows);
    Coordinates_plus* head = NULL;
    Coordinates_plus* tail = NULL;

    /* search the tileset every tile or so */
    for (int j = 0; j < num_rows; j++) {
        for (int h = (j % 2 == 0) ? 1 : 0; h < num_col; h += 2) {
            tile_n = tileset[j][h];

            if (tile_n == -1 || visited[j][h] == 1) continue;
            visited[j][h] = 1;
            n_adj = 1;
            Coordinates aux = {j, h};
            push(front, aux);

            /* while the stack isn't empty, keep popping adjacent coords so we make the list */
            while (!is_empty(front)) {
                Coordinates current = pop(front);

                for (int i = 0; i < 4; i++) {
                    int adj_row = current.row + drow[i];
                    int adj_col = current.col + dcol[i];

                    if (0 <= adj_col && adj_col < num_col && 0 <= adj_row && adj_row < num_rows &&
                        visited[adj_row][adj_col] != 1) {
                        if (tileset[adj_row][adj_col] == tile_n) {
                            visited[adj_row][adj_col] = 1; 
                            Coordinates new_aux = {adj_row, adj_col};
                            push(front, new_aux);
                            n_adj++;
                        }
                    }
                }
            }
            if (n_adj > 1) {
                Coordinates_plus* new_coord = (Coordinates_plus*)malloc(sizeof(Coordinates_plus));
                new_coord->row = j;
                new_coord->col = h;
                new_coord->score = score(n_adj);
                new_coord->next = NULL;

                if (head == NULL) {
                    head = new_coord;
                    tail = new_coord;

                } else {
                    tail->next = new_coord;
                    tail = new_coord;
                }

            }
        }
    }

    free(front->array);
    free(front);
    return head; 
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mem.h"

/******************************************************************************
 * read_store_tileset()
 *
 * Arguments: file - pointer to the file we're reading the matrix from,
 * num_rows and num_columns
 * 
 * Returns:  Stored tileset
 *
 * Description: Stored the matrix we're going to analyze
 *****************************************************************************/
int **read_store_tileset(FILE *file, int num_rows, int num_columns) {
    int **tileset;

    // Allocate memory for the tileset
    tileset = (int **)malloc(num_rows * sizeof(int *));
    if (tileset == NULL) {
        exit(1);
    }

    // Allocate memory for each row
    for (int i = 0; i < num_rows; i++) {
        tileset[i] = (int *)malloc(num_columns * sizeof(int));
        if (tileset[i] == NULL) {
            free_tileset(tileset, i);
            exit(1);
        }
    }

    // Read and store tileset data
    for (int i = num_rows - 1; i >= 0; i--) {
        for (int j = 0; j < num_columns; j++) {
            if (fscanf(file, "%d", &tileset[i][j]) != 1) {
                free_tileset(tileset, num_rows);
                exit(1);
            }
            
        }
    }

    return tileset;
}

/******************************************************************************
 * alloc_tileset()
 *
 * Arguments: tileset - tileset we're storing,
 * n_rows and n_columns
 * 
 * Returns:  Stored tileset
 *
 * Description: Stored the matrix we're going to analyze
 *****************************************************************************/
int **alloc_tileset(int n_rows, int n_columns){

    // Allocate memory for the tileset
    int** tileset = (int **)malloc(n_rows * sizeof(int *));
    if (tileset == NULL) {
        exit(1);
    }

    // Allocate memory for each row
    for (int i = 0; i < n_rows; i++) {
        tileset[i] = (int *)malloc(n_columns * sizeof(int));
        if (tileset[i] == NULL) {
            free_tileset(tileset, i);
            exit(1);
        }
    }
    return tileset;
}


/******************************************************************************
 * free_tileset()
 *
 * Arguments: tileset - so we have access to the memory allocated to the tileset,
 * num_rows - so we can free the pointers being pointed by a pointer in the tileset
 * 
 * Returns:  pointer to the opened file
 * 
 * Description: Frees a 2D array
 *****************************************************************************/
void free_tileset(int **tileset, int num_rows) {
    if(num_rows < 0){
        num_rows = -num_rows;
    }

    for (int i = 0; i < num_rows; i++) {
        free(tileset[i]);
    }
    free(tileset);
}

/******************************************************************************
 * alloc_visit()
 *
 * Arguments: num_columns and num_rows (dimensions of the matrix)
 * 
 * Returns: visited (new matrix full of "0")
 * 
 *
 * Description: Allocates memory to create a parallel matrix full of "0"s to later be used
 * to check which elements have been visited
 *****************************************************************************/
bool** alloc_visit(int num_rows, int num_columns){
    bool **visited;

    visited = (bool **)malloc(num_rows * sizeof(bool *));
    if (visited == NULL) {
        exit(1);
    }

    for (int i = 0; i < num_rows; i++) {
        visited[i] = (bool *)malloc(num_columns * sizeof(bool));
        if (visited[i] == NULL) {
            free_visited(visited, i);
            exit(1);
        }
    }

    for(int i = 0; i < num_rows; i++){
        for(int j = 0; j < num_columns; j++){
            visited[i][j] = 0;
        }
    }

    return visited;
}

/******************************************************************************
 * reset_visit()
 *
 * Arguments: visited matrix, num_columns and num_rows (dimensions of the matrix)
 * 
 * Returns: visited (matrix full of "0")
 * 
 *
 * Description: Resets the parallel matrix so we can re-execute
 *****************************************************************************/
bool** reset_visit(bool** visited, int num_rows, int num_columns){
    for (int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_columns; j++){
            visited[i][j] = 0;
        }
    }

    return visited;
}

/******************************************************************************
 * free_visited()
 *
 * Arguments: visited - so we have access to the memory allocated to the visited matrix,
 * num_rows - so we can free the pointers being pointed by a pointer in the tileset
 * 
 * Returns:  pointer to the opened file
 * 
 * Description: Frees a 2D array
 *****************************************************************************/
void free_visited(bool **visited, int num_rows) {
    if(num_rows < 0){
        num_rows = -num_rows;
    }

    for (int i = 0; i < num_rows; i++) {
        free(visited[i]);
    }
    free(visited);
}

#ifndef files_h_
#define files_h_
#include "tiles.h"
FILE* open_file(int argc, char *argv[]);
int write_file(char *argv[], int **tileset, int num_rows, int num_columns, int v, int points, int n_broken, Coordinates_plus* head, char *filename);
char *create_ss(char *argv[]);
void printListBackward(Coordinates_plus* head, FILE* file);

#endif
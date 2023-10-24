#ifndef mem_h_
#define mem_h_

int **read_store_tileset(FILE *file, int num_rows, int num_columns);
void free_tileset(int **tileset, int numRows);
bool** alloc_visit(int num_rows, int num_columns);
bool** reset_visit(bool** visited, int num_rows, int num_columns);
void free_visited(bool **visited, int num_rows);

#endif
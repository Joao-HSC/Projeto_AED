#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "files.h"
#include "tiles.h"
#include "vars.h"

int main(int argc, char *argv[]) {

    int num_columns = 0;
    int num_rows = 0;
    int v = 0, h = 0;
    int points, n_broken, aux = 0;
    int restart = 0;
    int **tileset;
    bool **visited;
    FILE *file = NULL;
    Coordinates_plus* current;
    Coordinates_plus* head_coords;
    Coordinates_plus* tmp = NULL;

    file = open_file(argc, argv);
    char *ss_name = create_ss(argv);

    while (fscanf(file, "%d %d %d", &num_rows, &num_columns, &v) == 3) {
  
        head_coords = NULL;
        h = 0;
        points = 0;
        n_broken = 0;

        tileset = read_store_tileset(file, num_rows, num_columns);

            /* Variant 1 */
            /*
            * We start by analyzing the tileset like a chess board starting at (1 , 1). Everytime we find
            * adjacent tiles we'll break them no matter what, apply gravity/push and start from (1, 1) again
            * until there's no stains left.
            */
            if (v == -1) {
                visited = alloc_visit(num_rows, num_columns);
                restart = 1;
                current = NULL; 

                while (1) {
                    restart = 0;

                    for (int i = 0; i < num_rows; i++) {
                        if (restart == 1) break;

                        for (int j = (i % 2 == 0) ? 1 : 0; j < num_columns; j += 2) {
                            aux = coords_analyze(tileset, visited, i, j, num_rows, num_columns);

                            if (aux > 1) {
                                restart = 1;

                                if (current == NULL) {
                                    current = (Coordinates_plus*)malloc(sizeof(Coordinates_plus));
                                    if (current == NULL) {
                                        exit(1);
                                    }
                                    
                                    head_coords = current;

                                } else {
                                    current->next = (Coordinates_plus*)malloc(sizeof(Coordinates_plus));
                                    if (current->next == NULL) {
                                        exit(1);
                                    }
                                    current = current->next;
                                    
                                }

                                current->row = i;
                                current->col = j;
                                points += score(aux); 
                                current->score = points;
                                current->next = NULL;
                                h++;

                                tileset = coords_replace(tileset, i, j, num_rows, num_columns);
                                n_broken++;
                                tileset = gravity(tileset, num_rows, num_columns);
                                visited = reset_visit(visited, num_rows, num_columns);
                            }
                        }
                    }

                    if (restart == 0) {
                        break;
                    }
                }


                free_visited(visited, num_rows);
            }
                            

            /* Variant 2 */
            /*
            * We'll start by starting the making of an implicit tree, and everytime we reach a leaf,
            * we'll check whether or not the score associated with that node is greater or
            * equal than v. If so, we'll return a list of Coordinates which will give
            * us the specific path for us to achieve that score.
            */
            else if(v >= 0){

                visited = alloc_visit(num_rows, num_columns);
                
                head_coords = dfs_2(tileset, v, num_rows, num_columns, visited);

                if (head_coords != NULL) {
                    Coordinates_plus* current = head_coords;
                    
                    for (n_broken = -1; current != NULL; n_broken++) {
                        points += current->score;
                        if(current->next == NULL) points = current->score;
                        current = current->next; 
                        
                    }
                }
                
                free_visited(visited, num_rows);
              
            }

            /* Variant 3 */
            /*
            * We'll start by starting the making of a branch, and everytime we reach a leaf,
            * we'll check whether or not the score associated with that node is greater
            * than the previous. If so, we'll store the node with the highest score and
            * extract its path in the end.
            */
            else if(v == -3){
                visited = alloc_visit(num_rows, num_columns);
                
                head_coords = dfs_3(tileset, num_rows, num_columns, visited);

                if (head_coords != NULL) {
                    Coordinates_plus* current = head_coords;
                    
                    for (n_broken = -1; current != NULL; n_broken++) {
                        points += current->score;
                        if(current->next == NULL) points = current->score;
                        current = current->next; 
                        
                    }
                }
                
                free_visited(visited, num_rows);

            }
          
        
        write_file(argv, tileset, num_rows, num_columns, v, points, n_broken, head_coords, ss_name);        
        free_tileset(tileset, num_rows);
        current = head_coords;
        while (current != NULL) {
            tmp = current;  
            current = current->next;          
            free(tmp);                       
        }
    }

    fclose(file);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mem.h"
#include "files.h"
#include "stack.h"

/******************************************************************************
 * open_file()
 *
 * Arguments: argc - number of arguments, argv - file's name
 * 
 * Returns:  pointer to the opened file
 *
 * Description: Opens the .tilewalls1 file when the extension exists
 *****************************************************************************/
FILE* open_file(int argc, char *argv[]) {

    /* must have at least one argument besides the executable name */
    if (argc < 2) {
        exit(1);
    }

    /* must have .tilewalls1 extension */
    char *valid_ext = (char *)malloc(strlen(".tilewalls") + 1);

    strcpy(valid_ext, ".tilewalls");

    const char *file_extension = strrchr(argv[1], '.');

    if (file_extension == NULL || strcmp(file_extension, valid_ext) != 0) {
        exit(1);
    }

    free(valid_ext);

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        exit(1);
    }

    return file;
}

/******************************************************************************
 * create_ss()
 *
 * Arguments: argv - file's name
 * 
 * Returns: pointer to the .singlestep file 
 *
 * Description: Create the .singlestep file
 *****************************************************************************/
char *create_ss(char *argv[]){

    char *filename = argv[1];

    /* alloc memory for the extension */
    char *new_ext = (char *)malloc(strlen(".tileblasts") + 1);

    strcpy(new_ext, ".tileblasts");

    char *lastDot = strrchr(filename, '.');

    strcpy(lastDot, new_ext);

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        exit(1);
    }

    fclose(file);
    free(new_ext);
    
    return filename;
}

/******************************************************************************
 * write_file()
 *
 * Arguments: argv - name of the opened file so we know the string before the extension
 *
 * Returns: 0
 *
 * Description: Writes in the new .singlestep file
 *****************************************************************************/
int write_file(char *argv[], int n_rows, int n_cols, int v, int points, int n_broken, Coordinates_plus* head, char *filename) {

    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        exit(1);
    }

        fprintf(file, "%d %d %d\n", n_rows, n_cols, v);
 
        /* variant 1 */
        if (v == -1) {
            fprintf(file, "%d %d\n", n_broken, points);

            while (head != NULL) {
                fprintf(file, "%d %d\n", head->row + 1, head->col + 1);
                head = head->next; 
            }
        }

        /* variant 2 */
        else if(v >= 0){ 
            
            fprintf(file, "%d %d\n", n_broken, points);
            if(head != NULL){
                head = head->next;
            }
            while (head != NULL) {
                fprintf(file, "%d %d\n", head->row + 1, head->col + 1);
                head = head->next; 
            }
            
        }

        /* variant 3 */
        else if(v == -3){ 

            fprintf(file, "%d %d\n", n_broken, points);
            if(head != NULL){
                head = head->next;
            }
            while (head != NULL) {
                fprintf(file, "%d %d\n", head->row + 1, head->col + 1);
                head = head->next; 
            }
        }
    
         fprintf(file, "\n");

    fclose(file);
    return 0;
}


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vars.h"
#include "stack.h"
#include "tiles.h"
#include "mem.h"

/******************************************************************************
 * extract_path_2()
 *
 * Arguments: node - leaf node
 * 
 * Returns: the head of the path list
 * 
 * Description: Find the path back to the root knowing the final node for the 2nd variant
 *****************************************************************************/

Coordinates_plus* extract_path_2(Node* current){

    Coordinates_plus* path_head = NULL;
    Coordinates_plus* aux = NULL;
    Node* prev = NULL;

     while (current != NULL) {
        Coordinates_plus* path_node = (Coordinates_plus*)malloc(sizeof(Coordinates_plus));
        path_node->row = current->coordinates->row;
        path_node->col = current->coordinates->col;
        path_node->score = current->score;
        path_node->next = path_head;
        path_head = path_node;
        prev = current;
        current = current->parent; 
        while(prev->children != NULL){
            aux = prev->children;
            prev->children = aux->next;
            free(aux);
        }
        free(prev->coordinates);
        free(prev);
    }

    return path_head;
}

/******************************************************************************
 * extract_path_3()
 *
 * Arguments: node - leaf node
 * 
 * Returns: the head of the path list
 * 
 * Description: Find the path back to the root knowing the final node for the 3rd variant
 *****************************************************************************/

Coordinates_plus* extract_path_3(Node* current){

    Coordinates_plus* path_head = NULL;

     while (current != NULL) {
        Coordinates_plus* path_node = (Coordinates_plus*)malloc(sizeof(Coordinates_plus));
        path_node->row = current->coordinates->row;
        path_node->col = current->coordinates->col;
        path_node->score = current->score;
        path_node->next = path_head;
        path_head = path_node;
        current = current->parent;
    }

    return path_head;
}

/******************************************************************************
 * create_child()
 *
 * Arguments: current - node we're exploring, node_aux - element of the children list, n_rows - number of rows,
 * n_cols - number of columns, visited - boolean matrix
 * 
 * Returns: 
 * 
 * Description: creates a new node
 *****************************************************************************/

Node* create_child(Node* current, Coordinates_plus* node_aux, int n_rows, int n_cols, bool** visited) {

    Node* child = (Node*)malloc(sizeof(Node));
    child->parent = current;
    child->coordinates = (Coordinates*)malloc(sizeof(Coordinates));
    child->coordinates->row = node_aux->row;
    child->coordinates->col = node_aux->col;
    child->tileset = gravity(coords_replace(current->tileset, node_aux->row, node_aux->col, n_rows, n_cols), n_rows, n_cols);
    child->children = coords_list(child->tileset, visited, n_rows, n_cols);
    reset_visit(visited, n_rows, n_cols);
    child->score = current->score + node_aux->score;

    return child;
}

/******************************************************************************
 * next_branch()
 *
 * Arguments: current - leaf node
 * 
 * Returns: the next node we're able to investigate which has not been analyzed yet
 *
 * Description: will start a new branch so we can find an alternative path which 
 * will give us a different score while freeing the nodes we're not using anymore
 *****************************************************************************/

Node* next_branch(Node* current) {

    Coordinates_plus* aux = NULL;
    Node* parent = NULL;

    /* while we don't reach the parent of the head_node (NULL), backtrack */
    while (1) {
       
        /* if there's children asscoiated with a node, free one */
        if (current->children != NULL) {
            if(current->children->next != NULL){  printf("I FOUND CHILDREN ");
                aux = current->children; 
                current->children = current->children->next;  
                free(aux); //printf("%d %d %d |",current->children->row, current->children->col, current->children->score);
                return current;
            }
            printf("IM GOIN BACK ");
            parent = current->parent;
            free(current->children);
            free(current->coordinates);
            free(current);
            current = parent;
        }
        /* if there's no children asscoiated with a node, backtrack */
        else{
            parent = current->parent;
            free(current->coordinates);
            free(current);
            current = parent;
        }
        /* if we reach the head_node and there's no more kids, return NULL */
        if (current->parent == NULL) {  printf("IM STOPPIN ");
            if (current->children) {
                free(current->children->next);
            }
            free(current->coordinates);
            free(current);
            return NULL;
        }
            
    }
}
/******************************************************************************
 * var 1()
 *
 * Arguments: tileset - , visited - , n_rows, n_columns
 * 
 * Returns: A list with the coordinates that have been broken until the tileset is unsolvable
 *
 * Description: Just break everything
 *****************************************************************************/
Coordinates_plus* var_1(int**tileset, bool**visited, int n_rows, int n_cols){

    int restart = 1;
    int aux = 0;
    Coordinates_plus* current = NULL; 
    Coordinates_plus* head_coords = NULL;

    while (1) {
        restart = 0;

        for (int i = 0; i < n_rows; i++) {
            if (restart == 1) break;

            for (int j = (i % 2 == 0) ? 1 : 0; j < n_cols; j += 2) {
                aux = coords_analyze(tileset, visited, i, j, n_rows, n_cols);

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
                            current->score = score(aux);
                            current->next = NULL;

                            tileset = coords_replace(tileset, i, j, n_rows, n_cols);
                            tileset = gravity(tileset, n_rows, n_cols);
                            visited = reset_visit(visited, n_rows, n_cols);
                            }
                        }
                    }

                    if (restart == 0) {
                        break;
                    }
                }
                return head_coords;
}



/******************************************************************************
 * dfs_2()
 *
 * Arguments: tileset - initial tileset, v - min score, n_rows, n_cols, visited
 * 
 * Returns: A list with the coordinates of the path to the minimum v score
 * 
 * Description: Find the leaf which corresponds to the minimum score and extract its path
 *****************************************************************************/

Coordinates_plus* dfs_2(int** tileset, int v, int n_rows, int n_cols, bool** visited) {
    
    Node* head_node = NULL;
    Node* current = NULL;
    int min_score = v; 

            /* initialize head_node */
            current = (Node*)malloc(sizeof(Node));
            head_node = current;
            head_node->parent = NULL;
            head_node->coordinates = (Coordinates*)malloc(sizeof(Coordinates));
            head_node->coordinates->row = 0;
            head_node->coordinates->col = 0;
            head_node->tileset = tileset;
            head_node->score = 0;
            head_node->children = coords_list(head_node->tileset, visited, n_rows, n_cols);
            reset_visit(visited, n_rows, n_cols);
            //printf("%d %d %d |",current->children->row, current->children->col, current->children->score);
            /* if the head_node has no children it means there's nothing we can do */
            if(head_node->children == NULL){
                free(head_node->coordinates);
                free(head_node);
                return NULL;
            }
            /* creates a child to the head node and we analyze it after */
            head_node->child = create_child(head_node, head_node->children, n_rows, n_cols, visited);
            current = current->child;

        /* while none of the conditions inside is met we'll keep backtracking and branching the tree */
        while(1) {     
            /* 
            * if we find a leaf node we'll check its score,
            * if it's greater or equal than the the minimum score, we'll retrieve its path
            * else we'll try to find a new branch
            */
            if(current->children == NULL){
                if(current->score >= min_score){
                    return extract_path_2(current);
                }
                /* backtrack */
                current = next_branch(current);
                if (current == NULL){
                    return NULL;
                }
            }
            current->child = create_child(current, current->children, n_rows, n_cols, visited);
            current = current->child;
        }
    }    

/******************************************************************************
 * dfs_3()
 *
 * Arguments: tileset - initial tileset, v - min score, n_rows, n_cols, visited
 * 
 * Returns: A list with the coordinates of the path to the max score
 * 
 * Description: Find the leaf which corresponds to the max score and extract its path
 *****************************************************************************/

Coordinates_plus* dfs_3(int** tileset, int n_rows, int n_cols, bool** visited) {
    
    Node* head_node = NULL;
    Node* current = NULL;
    Coordinates_plus* best_path = NULL;
    int max_score = -1; 

    /* initialize head_node */
    head_node = (Node*)malloc(sizeof(Node));
    current = head_node;
    head_node->parent = NULL;
    head_node->child = NULL;
    head_node->coordinates = (Coordinates*)malloc(sizeof(Coordinates));
    head_node->coordinates->row = 0;
    head_node->coordinates->col = 0;
    head_node->tileset = tileset; 
    head_node->score = 0;
    head_node->children = coords_list(head_node->tileset, visited, n_rows, n_cols);
    reset_visit(visited, n_rows, n_cols);

    /* if the head_node has no children it means there's nothing we can do */
    if(head_node->children == NULL) {
        free(head_node->coordinates);
        free(head_node);
        return NULL;
    }
    
    current->child = create_child(current, current->children, n_rows, n_cols, visited);
    current = current->child;
    
    /* while none of the conditions inside is met we'll keep backtracking and branching the tree */
    while (1) {

        /* 
        * if we find a leaf node we'll check its score to see if it's greater than max_score
        * ff so, we'll retrieve its path
        * else we'll try to find a new branch
        */
        if(current->children == NULL) {
            if(current->score > max_score) {  
                max_score = current->score;  
                
                /* free the previous best path */
                while(best_path != NULL) {
                    Coordinates_plus* aux = best_path;
                    best_path = best_path->next;
                    free(aux);
                }
                 
                /* extract the new best path */
                best_path = extract_path_3(current);
                
            }
            /* backtrack */ printf("branch ");
           
            current = next_branch(current);
            
            if (current == NULL) {
                return best_path;
            }
        } 
        
        current->child = create_child(current, current->children, n_rows, n_cols, visited);
        current = current->child;   
    }    
}

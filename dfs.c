#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "dfs.h"
#include "stack.h"
#include "tiles.h"
#include "mem.h"

/******************************************************************************
 * dfs_2()
 *
 * Arguments: tileset - initial tileset, v - min score, n_rows, n_cols, visited
 * 
 * Returns: An array with the coordinates of the path to the minimum v score
 * 
 * Description: Find the leaf which corresponds to the minimum score and extract its path
 *****************************************************************************/

Coordinates_plus* dfs_2(int** tileset, int v, int n_rows, int n_cols, bool** visited) {
    
    Node* head_node = NULL;
    Node* current = NULL;
    int min_score = v; 

    while (1) {
        
        /* create head node */
        if (head_node == NULL) {
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

            if(head_node->children == NULL){
                free(head_node->coordinates);
                free(head_node);
                return NULL;
            }
           head_node->child = create_child(head_node, head_node->children, n_rows, n_cols, visited);

        } else {     
            /* if we find a leaf node we'll investigate its score */
            if(current->children == NULL){
                if(current->score >= min_score){
                    return extract_path(current);
                }
                current = next_branch(current);
                if (current == NULL){
                    return NULL;
                }
            }
            current->child = create_child(current, current->children, n_rows, n_cols, visited);
        }
        current = current->child;
    }    

}

/******************************************************************************
 * dfs_3()
 *
 * Arguments: tileset - initial tileset, v - min score, n_rows, n_cols, visited
 * 
 * Returns: An array with the coordinates of the path to the max score
 * 
 * Description: Find the leaf which corresponds to the max score and extract its path
 *****************************************************************************/

Coordinates_plus* dfs_3(int** tileset, int v, int n_rows, int n_cols, bool** visited) {
    
    Node* head_node = NULL;
    Node* current = NULL;
    Coordinates_plus* head_list = NULL;
    int max_score = -1;  // Set to INT_MIN, which is the smallest possible int value

    while (1) {
        
        /* create head node */
        if (head_node == NULL) {
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

            if(head_node->children == NULL){
                free(head_node->coordinates);
                free(head_node);
                return NULL;
            }
            head_node->child = create_child(head_node, head_node->children, n_rows, n_cols, visited);

        } else {     
            /* if we find a leaf node we'll investigate its score */
            if(current->children == NULL){
                /* Check if current score is greater than max_score */
                if(current->score > max_score){  
                    max_score = current->score;  
                    if(head_list != NULL){
                        Coordinates_plus* aux = head_list;
                        while(aux != NULL){
                            head_list = head_list->next;
                            free(aux);
                            aux = head_list;
                        }
                    }
                    head_list = extract_path(current);
                }
                current = next_branch(current);
                if (current == NULL){
                    return head_list;
                }
            }
            current->child = create_child(current, current->children, n_rows, n_cols, visited);
        }
        current = current->child;
    }    
}

/******************************************************************************
 * extract_path()
 *
 * Arguments: node - leaf node
 * 
 * Returns: the head of the path list
 * 
 * Description: Find the path back to the root knowing the final node
 *****************************************************************************/

Coordinates_plus* extract_path(Node* current){

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

    Node* node = (Node*)malloc(sizeof(Node));
    node->parent = current;
    node->coordinates = (Coordinates*)malloc(sizeof(Coordinates));
    node->coordinates->row = node_aux->row;
    node->coordinates->col = node_aux->col;
    node->tileset = gravity(coords_replace(current->tileset, node_aux->row, node_aux->col, n_rows, n_cols), n_rows, n_cols);
    node->children = coords_list(node->tileset, visited, n_rows, n_cols);
    reset_visit(visited, n_rows, n_cols);
    node->score = current->score + node_aux->score;
    
    return node;
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
    Node* prev = current;
    Coordinates_plus* aux = NULL;

    while (current) {
        current = current->parent;
        if(prev->children != NULL) free(prev->children);
        free(prev->coordinates);
        free(prev);

        /* if we get to the head_node, it means there's no more paths possible */
        if (current->parent == NULL && current->children->next == NULL) {
            free(current->children);
            free(current->coordinates);
            free(current);
            return NULL;
        }

        if (current->children->next != NULL) {
            aux = current->children;
            current->children = current->children->next;
            free(aux);
            return current;
        }
        prev = current;
    }
    return NULL;
}




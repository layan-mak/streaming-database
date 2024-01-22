//
// Created by layan on 5/21/2023.
//

#ifndef GROUP_H_HELPER_FUNCS_H
#define GROUP_H_HELPER_FUNCS_H

#include "utilities.h"
#include "Movie.h"

//merges two arrays of movie node pointers, to one array of movie node pointers sorted by movieKeys
void mergeTwoArrays(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr1, int size1,
        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr2, int size2,
        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr3);

//moves a tree to array of std::shared_ptrs of movies nodes, sorted by moviekey
//assumes array is in the right size
void moveToArray(std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> tree,
std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr, int arr_size);

//moves movie_node_ptrs array to movie_ids array in decreasing order by MovieKey
void moveNodeArrToIntArr(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* node_arr,
int* int_arr, int size);

std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> getGenreTree(Genre genre);



#endif //GROUP_H_HELPER_FUNCS_H

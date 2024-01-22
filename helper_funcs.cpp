//
// Created by layan on 5/21/2023.
//
#include "helper_funcs.h"

void moveToArrAux(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>> node, int* arr_index,
                  std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr, int arr_size)
{
    if(!node || !arr_index || (*arr_index) >= arr_size) return;
    arr[*arr_index] = node;
    (*arr_index)++;
}

//moves a tree to array of std::shared_ptrs of movies nodes, sorted by moviekey
//assumes array is in the right size
void moveToArray(std::shared_ptr<AVLTree<MovieKey, std::shared_ptr<Movie>>> tree,
                 std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr, int arr_size)
{
    int* index = new int(0);
    tree->inOrder(tree->getRoot(), moveToArrAux, index, arr, arr_size);
    delete index;
}

//merges two arrays of movie node pointers, to one array of movie node pointers sorted by movieKeys
void mergeTwoArrays(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr1, int size1,
                                                                        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr2, int size2,
                                                                        std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* arr3)
{
    if(size1 == 0 && size2 > 0)
    {
        //arr3 = arr2;
        int i, j;
        for(i = 0, j = 0; j < size2; i++, j++)
        {
            arr3[i] = arr2[j];
        }
        return;
    }
    if(size1 > 0 && size2 == 0)
    {
        //arr3 = arr1;
        int i, j;
        for(i = 0, j = 0; j < size1; i++, j++)
        {
            arr3[i] = arr1[j];
        }
        return;
    }
    if(size1 == 0 && size2 == 0)
    {
        return;
    }
    //int new_size = size1 + size2;
    int i1, i2, i3;
    for(i1 = 0, i2 = 0, i3 = 0; (i1 < size1) && (i2 < size2); i3++)
    {
        if(arr1[i1]->getKey() < arr2[i2]->getKey())
        {
            arr3[i3] = arr1[i1];
            i1++;
        }
        else
        {
            arr3[i3] = arr2[i2];
            i2++;
        }
    }

    for(; i1 < size1; i1++, i3++)
    {
        arr3[i3] = arr1[i1];
    }
    for(; i2 < size2; i2++, i3++)
    {
        arr3[i3] = arr2[i2];
    }
}

void moveNodeArrToIntArr(std::shared_ptr<Node<MovieKey, std::shared_ptr<Movie>>>* node_arr, int* int_arr, int size)
{
    int i, j; //i = iterator for movie_nodes_arr, j = iterator for move_ids_arr
    for(i = size - 1, j = 0; (i >= 0) && (j < size); i--, j++)
    {
        int_arr[j] = node_arr[i]->getValue()->getId();
    }
}

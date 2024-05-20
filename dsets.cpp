/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num){
    for(int i = 0; i < num; i++)
        elements.push_back(-1);
    
    return;
}

int DisjointSets::find(int elem){
    if(elements[elem] < 0)
        return elem;

    int result = find(elements[elem]);
    elements[elem] = result;
    return elements[elem];
}

void DisjointSets::setunion(int a, int b) {
    int root_a = find(a);
    int root_b = find(b);

    if (root_a == root_b) 
        return;

    int size_a = size(root_a);
    int size_b = size(root_b);

    if(size_a >= size_b) {
        elements[root_a] = elements[root_a] + elements[root_b];
        elements[root_b] = root_a;
    }
    else {
        elements[root_b] = elements[root_a] + elements[root_b]; 
        elements[root_a] = root_b;
    }
}

int DisjointSets::size(int elem){
    int root = find(elem);
    
    return (-1) * elements[root];
}
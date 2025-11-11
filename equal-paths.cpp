#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int length(Node * root){
    if(root == NULL){
        return 0;
    } else if (!(root->left) && !(root->right)){
        return 1;
    }
    int left = length(root->left);
    int right = length(root->right);

    if(left > right){
        return left + 1;
    } else {
        return right + 1;
    }

}

bool equalPaths(Node * root)
{

    if(root == NULL){
        return true;
    } else if (!(root->left) && !(root->right)){
        return true;
    }

    bool leftP = equalPaths(root->left);
    bool rightP = equalPaths(root->right);

    int leftL = length(root->left);
    int rightL = length(root->right);

    if(leftP && rightP){
        if (leftL == rightL || root->left == NULL || root->right == NULL){
            return true;
        }
    }

    return false;

}


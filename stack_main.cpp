#include <stdio.h>
#include "stack_functions.h"


int main(){

    Stack* stk = stk_ctor(1);
    push_in_stk(stk, 228);
    push_in_stk(stk, 1);
    //printf("%d\n", peek(stk));
    //printf("%d\n", pop_from_stk(stk));
    //stk->right_canary = 0;
    //stk->capacity--;
    push_in_stk(stk, 100);
    for(int i = 0; i < 50; i++){
        push_in_stk(stk, i);
    }

    for(int i = 0; i < 37; i++){
        pop_from_stk(stk);
    }
    stk->left_canary = 0;
    //stk = NULL;
    stk_dtor(stk, 1);

}

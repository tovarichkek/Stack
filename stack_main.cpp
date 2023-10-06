#include <stdio.h>
#include <stdlib.h>
#include "stack_functions.h"

int main(){

    Stack* stk = (Stack*)calloc(1, sizeof(Stack));

	stk_ctor(stk);
    push_in_stk(stk, 228);
    push_in_stk(stk, 666);

    push_in_stk(stk, 100);
    for(int i = 0; i < 49; i++){
        push_in_stk(stk, i);
    }

    for(int i = 0; i < 37; i++){
        pop_from_stk(stk);
    }

    #ifdef STACK_PROTECTION
        //stk->left_canary = 0;
    #endif // STACK_PROTECTION
    //stk = NULL;
    stk_dtor(stk);

}

#include <stdio.h>
#include "stack_functions.h"


int main(){

    unsigned int a = 228;
    printf("%ld %p %p\n",calculate_control_sum(&a, &a +1), &a, &a + 1);

    Stack* stk = stk_ctor(1);
    push_in_stk(stk, 228);
    push_in_stk(stk, 1);
    printf("%d\n", peek(stk));
    printf("%d\n", pop_from_stk(stk));
        stk->right_canary = 0;

    stk_dtor(stk, 1);

}

#include <stdio.h>
#include "stack_functions.h"


int main(){

    unsigned int a = 1;
    printf("%ld %p %p\n",calculate_control_sum(&a, &a +1), &a, &a + 1);

    Stack* stk = stk_ctor(1);
    push_in_stk(stk, 228);
    printf("%d\n", peek(stk));
    stk_dtor(stk, 1);

}

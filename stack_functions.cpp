#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <assert.h>
#include "stack_functions.h"

void __stk_ctor(Stack* stk, const int line_creating, const char* funcname_creating, const char* filename_creating)
{
	assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk->left_canary = left_canary_value;
    #endif // STACK_PROTECTION

    stk->capacity = stk_start_size;
    stk->occupied_cells = 0;
    stk->data = (stk_elem_t*)calloc(stk_start_size, sizeof(stk_elem_t));
    if(stk->data == NULL){
        printf("Can't construct, no free memory\n");
        stk->capacity = 0;
    }
    for(int elem_stk = 0; elem_stk < stk_start_size; elem_stk++)
    {
        stk->data[elem_stk] = stk_poison;
    }

    stk->line_creating = line_creating;
    stk->funcname_creating = funcname_creating;
    stk->filename_creating = filename_creating;

    #ifdef STACK_PROTECTION
        stk->FCS = calculate_control_sum(stk->data, stk->data + stk->capacity);
        stk->right_canary = right_canary_value;
    #endif // STACK_PROTECTION
    return;
}

void stk_dtor(Stack* stk)
{
    assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk_dump(stk, calculate_state_code(stk));
        stk->left_canary = bad_canary_value;
    #endif // STACK_PROTECTION

    stk->capacity = 0;
    stk->occupied_cells = stk_poison;
    free(stk->data);

    #ifdef STACK_PROTECTION
        stk->FCS = 0;
        stk->right_canary = bad_canary_value;
    #endif // STACK_PROTECTION
	free(stk);
}

stk_elem_t peek(const Stack* stk)
{
    assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION
    return stk->data[stk->occupied_cells - 1];
}

bool stk_increase_data_size(Stack* stk)
{
    assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION
    stk_elem_t* buff = NULL;
    buff = (stk_elem_t*)realloc(stk->data, stk->capacity * sizeof(stk_elem_t) * size_multiplier);
    if(buff == NULL){
        printf("Can't increase size, no free memory\n");
        return false;
    }
    stk->data = buff;
    stk->capacity *= size_multiplier;
    for(size_t i = stk->occupied_cells; i < stk->capacity; i++)
    {
        stk->data[i] = stk_poison;
    }
    #ifdef STACK_PROTECTION
        stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION
    return true;
}

bool stk_decrease_data_size(Stack* stk)
{
    assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION

    stk_elem_t* buff = NULL;
    buff = (stk_elem_t*)realloc(stk->data, stk->capacity * sizeof(stk_elem_t) /size_multiplier);
    if(buff == NULL){
        printf("Can't decrease capacity of stk, problems with memory\n");
        return false;
    }
    stk->data = buff;
    stk->capacity /= size_multiplier;

    #ifdef STACK_PROTECTION
        stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION
    return true;
}

stk_elem_t push_in_stk(Stack* stk, stk_elem_t elem)
{
    assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION

    if(stk->capacity == stk->occupied_cells)
    {
        if(!stk_increase_data_size(stk)){
            printf("Can't push in stack, problems with memory\n");
            return stk_poison;
        }
    }
    stk->data[stk->occupied_cells] = elem;
    stk->occupied_cells += 1;
    #ifdef STACK_PROTECTION
        stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION
    return stk->data[stk->occupied_cells - 1];
}

stk_elem_t pop_from_stk(Stack* stk)
{
    assert(stk != NULL);
    #ifdef STACK_PROTECTION
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION

    stk_elem_t buff = stk->data[stk->occupied_cells - 1];

    if((stk->occupied_cells <= stk->capacity / (size_multiplier * size_multiplier)) && (stk->capacity > stk_start_size))
    {
        if(!stk_decrease_data_size(stk))
        {
            printf("Can't decrease stack, problems with memory\n");
            stk->data[stk->occupied_cells - 1] = stk_poison;
            return stk_poison;
        }
    }
    stk->data[stk->occupied_cells - 1] = stk_poison;
    stk->occupied_cells -= 1;

    #ifdef STACK_PROTECTION
        stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));
        stk_dump(stk, calculate_state_code(stk));
    #endif // STACK_PROTECTION
    return buff;
}

#ifdef STACK_PROTECTION
/**
 * take each byte from start to end
 * mult each bit(1) on 2^(number in byte)
 * and sum with (number of byte * numver of bit)
 * for dependence from count of bytes
 */
unsigned long long calculate_control_sum(const void* start, const void* finish)
{
    assert(start != NULL);
    assert(finish != NULL);

    unsigned long long hash_data = 0;
    const char* elem = (const char*)start;
    int index = 0;
    while(elem + index != finish)
    {
        for(int bit = 0; bit < bits_in_byte; bit++)
        {
            hash_data += (unsigned long long)((index + bit) * (*(elem + index) & (1 << bit))) + (elem + index) * bit;
        }
        index++;
    }
    return hash_data;
}

/**
0 - 1 - left_canary
1 - 2 - left_canary
2 - 4 - FCS
3 - 8 - occupied_cells
4 - 16 - capacity
5 - 32 - data
*/
unsigned long long calculate_state_code(const Stack* stk)
{
    assert(stk != NULL);

    unsigned long long state_code = 0;
    if(stk->left_canary != left_canary_value)
    {
        state_code |= (1 << 0);
    }
    if(stk->right_canary != right_canary_value)
    {
        state_code |= (1 << 1);
    }
    if(stk->FCS != calculate_control_sum(stk->data, stk->data + stk->capacity))
    {
        state_code |= (1 << 2);
    }
    if(stk->occupied_cells > stk->capacity)
    {
        state_code |= (1 << 3);
    }
    if(stk->capacity < stk->occupied_cells || stk->capacity < stk_start_size)
    {
        state_code |= (1 << 4);
    }
    if(stk->data == NULL)
    {
        state_code |= (1 << 5);
    }
    return state_code;
}

bool __stk_dump(const Stack* stk, unsigned long long state_code, const int line, const char* funcname, const char* filename)
{
    assert(stk != NULL);

    if(state_code == 0){
        return true;
    }
    fprintf(stderr, "Bad stack [%p], created in file: %s, function: %s, line: %d\n\n", stk, stk->filename_creating, stk->funcname_creating, stk->line_creating);

    fprintf(stderr, "dump called from file: %s, function: %s, line: %d\n", filename, funcname, line);
    fprintf(stderr, "state_code: %llu\n", state_code);
    fprintf(stderr, "occupied_cells: %lu\n", stk->occupied_cells);
    fprintf(stderr, "capacity: %lu\n", stk->capacity);
    fprintf(stderr, "data: [%p]\n", stk->data);
    fprintf(stderr, "FCS: %llu\n", stk->FCS);
    for(size_t i = 0; i < stk->capacity; i++){
        if(stk->data[i] != stk_poison)
        {
            fprintf(stderr, "* ");
        }
        fprintf(stderr, "[%lu] = %d\n", i, stk->data[i]);
    }
    return false;
}
#endif // STACK_PROTECTION

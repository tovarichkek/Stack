#include <stdio.h>
#include <cstdlib>
#include <math.h>
#include <assert.h>
#include "stack_functions.h"

Stack* __stk_ctor(const size_t count_of_stks, const int line_creating, const char* funcname_creating, const char* filename_creating)
{

    Stack* stks = (Stack*)calloc(count_of_stks, sizeof(Stack));
    if(stks == NULL)
    {
        printf("No free memory\n");
        return NULL;
    }
    for(size_t i = 0; i < count_of_stks; i++)
    {
        stks[i].left_canary = left_canary_value;
        stks[i].capacity = stk_start_size;
        stks[i].occupied_cells = 0;
        stks[i].data = (stk_elem_t*)calloc(stk_start_size, sizeof(stk_elem_t));
        if(stks[i].data == NULL){
            printf("Can't construct, no free memory\n");
            stks[i].capacity = 0;
        }
        for(int elem_stk = 0; elem_stk < stk_start_size; elem_stk++)
        {
            stks[i].data[elem_stk] = stk_poison;
        }

        stks[i].line_creating = line_creating;
        stks[i].funcname_creating = funcname_creating;
        stks[i].filename_creating = filename_creating;

        stks[i].FCS = calculate_control_sum(stks[i].data, stks[i].data + stks[i].capacity);

        stks[i].right_canary = right_canary_value;
    }
    return stks;
}

void stk_dtor(Stack* stks, const size_t count_of_stks)
{
    assert(stks != NULL);
    for(size_t i = 0; i < count_of_stks; i++)
    {
        stk_dump(&(stks[i]), calculate_state_code(&(stks[i])));

        stks[i].left_canary = bad_canary_value;
        stks[i].capacity = 0;
        stks[i].occupied_cells = stk_poison;
        free(stks[i].data);
        stks[i].FCS = 0;
        stks[i].right_canary = bad_canary_value;
    }
    free(stks);
}

stk_elem_t peek(const Stack* stk)
{
    assert(stk != NULL);
    stk_dump(stk, calculate_state_code(stk));
    return stk->data[stk->occupied_cells - 1];
}

bool stk_increase_data_size(Stack* stk)
{
    assert(stk != NULL);
    stk_dump(stk, calculate_state_code(stk));

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
    stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));
    stk_dump(stk, calculate_state_code(stk));
    return true;
}

bool stk_decrease_data_size(Stack* stk)
{
    assert(stk != NULL);
    stk_dump(stk, calculate_state_code(stk));

    stk_elem_t* buff = NULL;
    buff = (stk_elem_t*)realloc(stk->data, stk->capacity * sizeof(stk_elem_t) /size_multiplier);
    if(buff == NULL){
        printf("Can't decrease capacity of stk, problems with memory\n");
        return false;
    }
    stk->data = buff;
    stk->capacity /= size_multiplier;
    stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));

    stk_dump(stk, calculate_state_code(stk));
    return true;
}

stk_elem_t push_in_stk(Stack* stk, stk_elem_t elem)
{
    assert(stk != NULL);
    stk_dump(stk, calculate_state_code(stk));

    if(stk->capacity == stk->occupied_cells)
    {
        if(!stk_increase_data_size(stk)){
            printf("Can't push in stack, problems with memory\n");
            return stk_poison;
        }
    }
    stk->data[stk->occupied_cells] = elem;
    stk->occupied_cells += 1;
    stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));

    stk_dump(stk, calculate_state_code(stk));
    return stk->data[stk->occupied_cells - 1];
}

stk_elem_t pop_from_stk(Stack* stk)
{
    assert(stk != NULL);
    stk_dump(stk, calculate_state_code(stk));

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
    stk->FCS = calculate_control_sum(stk->data, (stk->data + stk->capacity));

    stk_dump(stk, calculate_state_code(stk));
    return buff;
}


/**
 * take each byte from start to end
 * and mult each bit(1) on 2^(number in byte)
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
            hash_data += (unsigned long long)((index + bit) * (*(elem + index) & (1 << bit)));

            if(((int)*(elem + index) & (int)pow(2, bit)) == 0){
                hash_data += 1;
            }
        }
        index++;
    }
    return hash_data;
}

/**
1 - left_canary
2 - left_canary
4 - FCS
8 - occupied_cells
16 - capacity
32 - data
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
    printf("Bad stack [%p], created in file: %s, function: %s, line: %d\n\n", stk, stk->filename_creating, stk->funcname_creating, stk->line_creating);

    printf("dump called from file: %s, function: %s, line: %d\n", filename, funcname, line);
    printf("state_code: %llu\n", state_code);
    printf("occupied_cells: %lu\n", stk->occupied_cells);
    printf("capacity: %lu\n", stk->capacity);
    printf("data: [%p]\n", stk->data);
    printf("FCS: %llu\n", stk->FCS);
    for(size_t i = 0; i < stk->capacity; i++){
        if(stk->data[i] != stk_poison)
        {
            printf("* ");
        }
        printf("[%lu] = %d\n", i, stk->data[i]);
    }
    return false;
}

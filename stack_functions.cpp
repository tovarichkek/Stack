#include <stdio.h>

Stack* stk_ctor(const size_t count_of_stks)
{

    Stack* stks = (Stack*)calloc(count_of_stks, sizeof(Stack));
    if(stks = NULL)
    {
        printf("No free memory\n");
        return NULL;
    }
    for(int i = 0; i < count_of_stks; i++)
    {
        stks[i].capacity = stk_start_size;
        stks[i].occupied_cells = 0;
        stks[i].data = calloc(stk_start_size, sizeof(stk_elem_t));
        if(stks[i].data == NULL){
            printf("Can't construct, no free memory\n");
            sks[i].capacity = 0;
        }
        stks[i].FCS = get_FCS(data, data + stks[i].capacity);
    }
    return stks;
}

void stk_dtor(Stack* stk, const size_t count_of_stks)
{
    assert(stk != NULL);
    for(int i = 0; i < count_of_stks; i++)
    {
        stks[i].capacity = -1;
        stks[i].occupied_cells = -1;
        free(stks[i].data);
        stks[i].FCS = 0;
    }
}

stk_elem_t* peek(const Stack* stk)
{
    assert(stk != NULL);
    return stk->data[stk->occupied_cells - 1];
}

void stk_increase_data_size(Stack* stk)
{
    stk_elem_t* buff = NULL;
    assert(stk != NULL);
    buff = realloc(stk.data, stk.capacity * sizeof(stk_elem_t) * in_how_many_times_increase_and_decrease_buffer);
    if(buff == NULL){
        printf("Can't increase size, no free memory\n");
        return;
    }
    stk->data = buff;
    stk->capacity *= in_how_many_times_increase_and_decrease_buffer;
    stk->FCS = calculate_control_sum(stk->data, stk->(data + (capacity + 1) * sizeof(stk_elem_t)));  //capacity + 1 for finish(last element) in caltulationg FCS1}


void stk_decrease_data_size(Stack* stk)
{
    stk_elem_t* buff = NULL;
    assert(stk != NULL);
    buff = realloc(stk.data, stk.capacity * sizeof(stk_elem_t) / in_how_many_times_increase_and_decrease_buffer);
    if(buff == NULL){
        printf("Can't decrease size, problems with memory\n");
        return;
    }
    stk->data = buff;
    stk->capacity *= in_how_many_times_increase_and_decrease_buffer;
    stk->FCS = calculate_control_sum((void*)stk->data, (void*)stk->(data + (capacity + 1) * sizeof(stk_elem_t)));  //capacity + 1 for finish(last element) in caltulationg FCS1
}

stk_elem_t push_in_stk(Stack* stk, stk_elem_t elem)
{
    if(stk->capacity == stk->occupied_cells)
    {
        stk_increase_data_size(stk);
        if(stk->capacity == stk->occupied_cells){
            printf("Can't push in stack, problems with memory\n");
            return NULL;
        }
    }
    stk->data[stk->occupied_cells] = stk_elem_t;
    stk->occupied_cells += 1;
    return stk->data[stk->occupied_cells - 1]
}

stk_elem_t pop_from_stk(Stack* stk)
{
    stk_elem_t buff = stk->data[stk->occupied_cells - 1];
    stk->data[stk->occupied_cells - 1] = stk_poison;
    stk->occupied_cells -= 1;

    if((stk->capacity / 2 <= stk->occupied_cells) && stk->capacity > stk_start_size)
    {
        stk_decrease_data_size(stk);
        if(stk->capacity == stk->occupied_cells)
        {
            printf("Can't decrease stack, problems with memory\n");
        }
    }
    return buff;
}



unsigned long long calculate_control_sum(const void* start, const void* finish)
{
    assert(start != NULL);
    assert(finish != NULL);

    unsigned long long hash_data = 0;
    char* elem = start;
    int index = 0;
    while(elem + index != finish)
    {
        for(int bit = 0; bit < bits_in_byte; bit++)
        {
            if((elem & pow(2, bit)) == 0){
                hash_data += 1;
            }
        }
    }
    return hash_data;
}



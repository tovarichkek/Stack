#ifndef STACK_FUNCTIONS
#define STACK_FUNCTIONS

typedef int stk_elem_t;

const int stk_start_size = 8;
const stk_elem_t stk_poison = -9999999;
const int in_how_many_times_increase_and_decrease_buffer = 2;
const int bits_in_byte = 8;
const size_canary = sizeof(long long);

//struct Stack
{
    unsigned long long left_canary;
    int capacity;
    int occupied_cells;
    stk_elem_t* data;
    unsigned long long FCS;
    unsigned long long right_canary;
};

bool error_handler(long long code_errors);
long long count_code_errors(Stack* stk);

//stk_elem_t push_in_stk(Stack* stk, stk_elem_t elem);

//stk_elem_t pop_from_stk(Stack* stk);

//stk_elem_t* peek(Stack* stk); // only return last elem

//Stack* stk_ctor(int count_of_stks);

//void stk_dtor(Stack* stk);

//bool stk_increase_data_size(Stack* stk);

//bool stk_decrease_data_size(Stack* stk);




//unsigned long long calculate_control_sum(void* start, void* finish);
















#endif // STACK_FUNCTIONS

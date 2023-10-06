#ifndef STACK_FUNCTIONS
#define STACK_FUNCTIONS


#define stk_dump(stk, state_code) __stk_dump((stk), (state_code), __LINE__, __FUNCTION__, __FILE__)
#define stk_ctor(count) __stk_ctor((count), __LINE__, __FUNCTION__, __FILE__)

typedef int stk_elem_t;

//Constants
const int stk_start_size = 8;

const stk_elem_t stk_poison = 0x1ADBADBA;

const int size_multiplier = 2;

const int bits_in_byte = 8;

#ifdef STACK_PROTECTION
    const int size_canary = sizeof(long long);
    const long long left_canary_value = 0xEDEEDEEDE;
    const long long right_canary_value = 0xDEDDEDDED;
    const long long bad_canary_value = 0xAAAAAAAAA;
#endif // STACK_PROTECTION

//Struct
struct Stack
{
    #ifdef STACK_PROTECTION
        unsigned long long left_canary;
    #endif // STACK_PROTECTION

    size_t capacity;
    size_t occupied_cells;
    stk_elem_t* data;

    int line_creating;
    const char* funcname_creating;
    const char* filename_creating;

    #ifdef STACK_PROTECTION
        unsigned long long FCS;
        unsigned long long right_canary;
    #endif // STACK_PROTECTION
};

//Prototypes
void __stk_ctor(Stack* stk, const int line_creating, const char* funcname_creating, const char* filename_creating);
void stk_dtor(Stack* stk);
bool stk_increase_data_size(Stack* stk);
bool stk_decrease_data_size(Stack* stk);
stk_elem_t push_in_stk(Stack* stk, stk_elem_t elem);
stk_elem_t pop_from_stk(Stack* stk);
stk_elem_t peek(const Stack* stk); // only return last elem

#ifdef STACK_PROTECTION
    unsigned long long calculate_state_code(const Stack* stk);
    bool __stk_dump(const Stack* stk, unsigned long long state_code, const int line, const char* funcname, const char* filename);  //return true if stack valid, false if no
    unsigned long long calculate_control_sum(const void* start, const void* finish);
#endif // STACK_PROTECTION

#endif // STACK_FUNCTIONS

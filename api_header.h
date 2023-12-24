#ifndef API_HEADER_H_INCLUDED
#define API_HEADER_H_INCLUDED

typedef struct Operator_ Operator_;
struct Operator_ {
    char value;
    int position_;
    int index;
};

int is_operator(char char_) ;
int is_exist(char operator_, Operator_ *operatos, int nbr_operator) ;
int get_nbr_operators(char main_exp[]) ;
Operator_* get_operator_list(char main_exp[], int nbr_operator);
double* get_numbers_list(char main_exp[], Operator_ *list_operator, int nbr_operator) ;
double *create_new_numbers(int index, double *numbers, int nbr_operator) ;
Operator_ *create_new_operators(int index, Operator_ *operators, int nbr_operators) ;
double _eval(double *numbers, Operator_ *operators, int nbr_operators);
double get_result(char main_exp[]) ;

#endif // API_HEADER_H_INCLUDED

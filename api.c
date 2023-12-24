#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "api_header.h"


double get_result(char main_exp[100]) {
    Operator_ *operator_;
    double *number ;
    double result ;
    int nbr_operator ;

    printf("Calcul de : %s\n", main_exp) ;
    nbr_operator = get_nbr_operators(main_exp) ;
    operator_ = get_operator_list(main_exp, nbr_operator) ;

    number = get_numbers_list(main_exp, operator_, nbr_operator) ;
    // printf("\n") ;
    // printf("Evaluation...\n") ;

    result = _eval(number, operator_, nbr_operator) ;
    // free(number) ;
    // free(operator_) ;

    return result ;
    printf("Fin\n") ;
}

int get_nbr_operators(char main_exp[]) {
    int index ;
    int nbr_operator = 0;

    for(index = 0; index < strlen(main_exp); index++) {
        if(is_operator(main_exp[index])) {
            nbr_operator++ ;
        }
    }
    printf("get_nbr_operators\n") ;
    return nbr_operator ;
}

int is_operator(char char_) {
    char operator_[] = "+-*/" ;
    int i ;
    int boolean_ = 0;

    for(i = 0; i < strlen(operator_); i++){
        if (char_ == operator_[i]){
            boolean_ = 1;
        }
    }

    printf("is_operator\n") ;
    return boolean_;
}

int is_exist(char operator_, Operator_ *operatos, int nbr_operator){
    int i;

    for(i = 0; i < nbr_operator; i++) {
        if (operatos[i].value == operator_){
            return i;
        }
    }

    printf("is_exist\n") ;
    return -1;
}

double *create_new_numbers(int index, double *numbers, int nbr_operator){
    double *new_numbers ;
    int decal = 0;
    int j ;

    new_numbers = malloc(nbr_operator*sizeof(double)) ;
    for (j = 0; j < nbr_operator + 1; j++){
        if (j == index + 1) {
            decal = 1;
            continue ;
        }

        if (decal){
            new_numbers[j - 1] = numbers[j] ;
        }
        else {
            new_numbers[j] = numbers[j] ;
        }
    }
    free(numbers) ;

    printf("create_new_numbers\n") ;
    return new_numbers ;
}

Operator_ *create_new_operators(int index, Operator_ *operators, int nbr_operators){
    Operator_ *new_operators;
    int decal = 0 ;
    int j ;

    new_operators = malloc((nbr_operators)*sizeof(*new_operators));
    for (j = 0; j < nbr_operators ; j++){
        if (j == index ) {
            decal = 1;
            continue ;
        }

        if (decal == 1){
            new_operators[j - 1].value = operators[j].value ;
            new_operators[j - 1].position_ = operators[j].position_ ;
            new_operators[j - 1].index = operators[j].index ;
        }
        else {
            new_operators[j].value = operators[j].value ;
            new_operators[j].position_ = operators[j].position_ ;
            new_operators[j].index = operators[j].index ; ;
        }

    }

    printf("create_new_operators\n") ;

    return new_operators ;
}

double _eval(double *numbers, Operator_ *operators, int nbr_operators) {
    int index;
    double *new_numbers ;
    Operator_ *new_operators ;
    double result = 0 ;

    if (nbr_operators == 1) {
        if (operators[0].value == '*'){
            result = numbers[0]*numbers[1] ;
        }

        else if (operators[0].value == '/'){
            result = numbers[0]/numbers[1] ;
        }
        else if (operators[0].value == '+'){
            result = numbers[0]+numbers[1] ;
        }
        else if (operators[0].value == '-'){
            result = numbers[0]-numbers[1] ;
        }
        else{
            return 0;
        }

        free(numbers) ;
        free(operators) ;

        return result ;
    }

    // Priorit� numero 0 : *
    else
    {
        if (is_exist('*', operators, nbr_operators) != -1) {
            index = is_exist('*', operators, nbr_operators) ;
            numbers[index] *= numbers[index + 1] ;
        }
        else if (is_exist('/', operators, nbr_operators) != -1) {
            index = is_exist('/', operators, nbr_operators) ;
            numbers[index] /= numbers[index + 1] ;
        }
        else if (is_exist('+', operators, nbr_operators) != -1) {
            index = is_exist('+', operators, nbr_operators) ;
            numbers[index] += numbers[index + 1] ;
        }
        else if (is_exist('-', operators, nbr_operators) != -1) {
            index = is_exist('-', operators, nbr_operators) ;
            numbers[index] -= numbers[index + 1] ;
        }

        // New numbers
        new_numbers = create_new_numbers(index, numbers, nbr_operators) ;

        // New operators
        new_operators = create_new_operators(index, operators, nbr_operators) ;

        /*
        printf("\n") ;
        for(int i = 0; i < nbr_operators; i++){
            if (i == nbr_operators - 1){
                printf("%.2lf", new_numbers[i]) ;
            }
            else {
                printf("%.2lf  %c   ", new_numbers[i], new_operators[i].value) ;
            }
        }
        printf("\n") ;
        */

        return _eval(new_numbers, new_operators, nbr_operators - 1);
    }

    printf("eval\n") ;
}

Operator_* get_operator_list(char main_exp[100], int nbr_operator) {
    Operator_ *operators = NULL ;
    int index ;
    int current_operator_index = 0;

    // liste d'operateur dans l'expression
    operators = malloc(nbr_operator*sizeof(*operators)) ;

    for(index = 0; index < strlen(main_exp); index++) {
        if (is_operator(main_exp[index])) {
            operators[current_operator_index].value = main_exp[index] ;
            operators[current_operator_index].index = current_operator_index ;
            operators[current_operator_index].position_ = index ;
            current_operator_index++ ;
        }
    }
    return operators ;
}

double* get_numbers_list(char main_exp[100], Operator_ *list_operator, int nbr_operator) {
    int index;
    double *list_num ;

    /*
    for(index = 0; index < nbr_operator; index++){
        printf("Operateur : %c, index : %d, position : %d\n", list_operator[index].value, list_operator[index].index, list_operator[index].position_) ;
    }
    */

    // liste de nombre dans l'expression
    list_num = malloc((nbr_operator + 1)*sizeof(double)) ;
    int current_index = 0;
    for(index = 0; index < nbr_operator; index ++){
        char *current_number;
        int number_length;
        int i;

        // dernier operateur
        if (index == nbr_operator - 1) {
            // avant dernier chiffre
            number_length = list_operator[index].position_ - 1 - list_operator[index - 1].position_;
            current_number = malloc((number_length + 1)*sizeof(char)) ;
            current_number[number_length] = '\0' ;

            for (i = list_operator[index - 1].position_ + 1; i < list_operator[index].position_; i++){
                current_number[i - list_operator[index - 1].position_ - 1] =  main_exp[i] ;
            }

            list_num[current_index] = atof(current_number) ;
            // printf("Ajout du %d-ieme nombre dans la liste (%.2lf)... \n", current_index + 1, atof(current_number));
            current_index++;

            // Dernier chiffre
            number_length = strlen(main_exp) - list_operator[index].position_ - 1;
            current_number = malloc((number_length + 1)*sizeof(char)) ;
            current_number[number_length] = '\0' ;

            for(i = list_operator[index].position_ + 1; i < strlen(main_exp); i++){
                current_number[i - list_operator[index].position_ - 1] = main_exp[i] ;
            }

            list_num[current_index] = atof(current_number) ;
            // printf("Ajout du %d-ieme nombre dans la liste (%.2lf)... \n", current_index + 1, atof(current_number));
            current_index++;
        }

        // premier operateur
        else if (index == 0) {
             number_length = list_operator[index].position_;
             current_number = malloc((number_length + 1)*sizeof(char)) ;
             current_number[number_length] = '\0' ;

             for(i = 0; i < list_operator[index].position_; i++) {
                current_number[i] =  main_exp[i] ;
             }

             list_num[current_index] = atof(current_number) ;
             // printf("Ajout du %d-ieme nombre dans la liste (%.2lf)... \n", current_index + 1, atof(current_number));
             current_index++;
        }

        // Nombre au milieu
        else {

            number_length = list_operator[index].position_ - 1 - list_operator[index - 1].position_;
            current_number = malloc((number_length + 1)*sizeof(char)) ;
            current_number[number_length] = '\0' ;

            for (i = list_operator[index - 1].position_ + 1; i < list_operator[index].position_; i++){
                current_number[i - list_operator[index - 1].position_ - 1] =  main_exp[i] ;
            }

            list_num[current_index] = atof(current_number) ;
            // printf("Ajout du %d-ieme nombre dans la liste (%.2lf)... \n", current_index + 1, atof(current_number));
            current_index++;
        }
    }

    /*
    for(int j = 0; j <= nbr_operator; j++){
        if (j != nbr_operator) {
            printf("%.2lf  %c  ", list_num[j], list_operator[j].value);
            continue ;
        }
        printf("%.2lf  ", list_num[j]);
    }
    */

    return list_num ;
}

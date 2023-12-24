#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gtk/gtk.h"
#include "api_header.h"


// Global var
GtkWidget *input_widget ;
GtkEntryBuffer *buffer ;

int get_point_position(char expr[]) {
    char point;
    int i ;

    for(i = 0; i < strlen(expr); i++) {
        if (expr[i] == '.') return i ;
    }

    return -1 ;
}

void compute(char main_exp[]){
    double result ;
    char str_result[50] ;
    int i, boolean_= 0;
    char new_main_exp[100] ;
    char *ZeroDivisionError ;
    char str_Math_ERROR[] = "Impossible" ;
    int result_null = 0;
    int notZeroDivisionError ;
    char *char_;
    char zero = '0';
    char main_exp_last_char ;


    char_ = &zero ;
    ZeroDivisionError = strstr(main_exp, "/0") ;
    // Caractere apres le Zero de ZeroDivisionError
    main_exp_last_char = main_exp[strlen(main_exp) - 1] ;

    if((main_exp_last_char != '0') && (ZeroDivisionError != NULL)) {
        char_ = strchr(ZeroDivisionError, '0') ;
        if (char_ != NULL) {
            // Incrementer l'adresse du zero pour avoir le caractere apres zero ;
            char_++ ;
        }
    }

    notZeroDivisionError = ((ZeroDivisionError != NULL) && ((!is_operator(*char_)) && (*char_ != '0'))) ;

    printf("notZeroDivisionError : %d\nCaractere apres ZeroDivision : %c\n", notZeroDivisionError, *char_) ;
    if (is_operator(main_exp[0]) && (main_exp[0] != '-')){
        gtk_entry_buffer_set_text(buffer, str_Math_ERROR, strlen(str_Math_ERROR)) ;
    }
    else if ((ZeroDivisionError == NULL) || (notZeroDivisionError)){
        if(main_exp[0] == '-') {
            boolean_ = 1;

            for(i = 0; i < strlen(main_exp); i++) {
                if (is_operator(main_exp[i])) {
                        if (main_exp[i] == '-') {
                            main_exp[i] = '+';
                        }
                        else if (main_exp[i] == '+'){
                            main_exp[i] = '-' ;
                        }
                    }
            }
        }

        sprintf(new_main_exp, "%s+0+0", main_exp) ;

        result = get_result(new_main_exp) ;

        if (result == 0) {
            result_null = 1;
        }

        if (boolean_) {
            if (result <= 0) {
                result *= -1 ;
                sprintf(str_result, "%.4lf", result);
            }
            else {
                sprintf(str_result, "-%.4lf", result);
            }

        }
        else {
            sprintf(str_result, "%.4lf", get_result(new_main_exp));
        }

        // supression des zeros si result == integer ;
        boolean_ = 1;
        for (i = get_point_position(str_result) + 1; i < strlen(str_result); i++) {
            if (str_result[i] != '0') boolean_ = 0;
        }


        if (boolean_) str_result[get_point_position(str_result)] = '\0' ;

        // supression des deux derniers chiffres s'ils sont == 0 (ou le dernier s'il est == 0);
        if (str_result[strlen(str_result) - 1] == '0'){
            if (str_result[strlen(str_result) - 2] == '0' ){
                str_result[strlen(str_result) - 2] = '\0' ;
            }
            else {
                str_result[strlen(str_result) - 1] = '\0' ;
            }
        }

        if (result_null){
            gtk_entry_buffer_set_text(buffer, "0", 2) ;
        }
        else {
            gtk_entry_buffer_set_text(buffer, str_result, strlen(str_result));
        }

    }

    // Division par zero ;
    else {
        gtk_entry_buffer_set_text(buffer, str_Math_ERROR, strlen(str_Math_ERROR)) ;
    }
                                        // Math error ;

}

int user_interaction(GtkWidget *button_clicked, gpointer data) {
    char *label ;
    char *current_text, *new_text ;
    long long unsigned int len;
    char last_char ;
    char *error_in_text;


    label = gtk_button_get_label(GTK_BUTTON(button_clicked)) ;

    current_text = gtk_entry_buffer_get_text(buffer) ;

    error_in_text = strstr(current_text, "Impossible") ;

    if (error_in_text) {
        gtk_entry_buffer_set_text(buffer, "", 0) ;
    }

    if ( strlen(label) == 3 ) {
        char *return_ ;
        len = strlen(current_text) ;
        current_text[(len - 1)] = '\0' ;
        len = strlen(current_text) ;

        new_text = malloc((len + 1)*sizeof(char)) ;

        return_ = strcpy(new_text, current_text) ;

        gtk_entry_buffer_set_text(buffer, new_text, strlen(new_text)) ;

        free(new_text) ;
        return 0;
    }

    else if (*label == 'C') {
        gtk_entry_buffer_delete_text(buffer, 0, strlen(current_text)) ;
        return 0;
    }

    else if (*label == '=') {
        char *point ;

        // Changement des virgules en point ;
        point = strchr(current_text, ',') ;

        while (point != NULL) {
            *point = '.' ;
            point = strchr(current_text, ',') ;
        }

        compute(current_text);
    }
    else{
        last_char = current_text[strlen(current_text) - 1];
        if (is_operator(last_char)){
            if (is_operator(*label)) {
                return -1;
            }
            else {
                current_text = malloc(strlen(current_text)*sizeof(char) + 10*sizeof(char)) ;

                sprintf(current_text, gtk_entry_buffer_get_text(buffer)) ;

                strcat(current_text, label) ;

                gtk_entry_buffer_set_text(buffer, current_text, strlen(current_text)) ;
            }
        }
        else {
        current_text = malloc(strlen(current_text)*sizeof(char) + 10*sizeof(char)) ;

        sprintf(current_text, gtk_entry_buffer_get_text(buffer)) ;

        strcat(current_text, label) ;

        gtk_entry_buffer_set_text(buffer, current_text, strlen(current_text)) ;
        }

        free(current_text) ;
        return 0;
    }
}


void createWidgetAndlayout(GtkApplication *app, gpointer data) {
    GtkWidget *window ;
    GtkWidget *grdMainLayout;
    GtkWidget *btn_0, *btn_1, *btn_2, *btn_3, *btn_4, *btn_5, *btn_6, *btn_7, *btn_8, *btn_9 ;
    GtkWidget *btn_C, *btn_point, *btn_del, *btn_sum, *btn_soustr, *btn_mult, *btn_equal, *btn_div;

                /*                         */
                /* Creation de l'interface */
                /*                         */

    // Creation des buttons
    buffer = GTK_ENTRY_BUFFER(gtk_entry_buffer_new("", 1)) ;
    input_widget = gtk_entry_new_with_buffer(GTK_ENTRY_BUFFER(buffer)) ;
    btn_0 = gtk_button_new_with_label("0") ;
    btn_1 = gtk_button_new_with_label("1") ;
    btn_2 = gtk_button_new_with_label("2") ;
    btn_3 = gtk_button_new_with_label("3") ;
    btn_4 = gtk_button_new_with_label("4") ;
    btn_5 = gtk_button_new_with_label("5") ;
    btn_6 = gtk_button_new_with_label("6") ;
    btn_7 = gtk_button_new_with_label("7") ;
    btn_8 = gtk_button_new_with_label("8") ;
    btn_9 = gtk_button_new_with_label("9") ;
    btn_C = gtk_button_new_with_label("C") ;
    btn_point = gtk_button_new_with_label(",") ;
    btn_del = gtk_button_new_with_label("DEL") ;
    btn_sum = gtk_button_new_with_label("+") ;
    btn_soustr = gtk_button_new_with_label("-") ;
    btn_mult = gtk_button_new_with_label("*") ;
    btn_equal = gtk_button_new_with_label("=") ;
    btn_div = gtk_button_new_with_label("/") ;

    // Creation du layout  ( grid )
    grdMainLayout = gtk_grid_new() ;

    gtk_grid_set_column_homogeneous(GTK_GRID(grdMainLayout), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grdMainLayout), TRUE);
    gtk_grid_attach(GTK_GRID(grdMainLayout), input_widget, 0, 0, 4, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_C, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_del, 2, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_div, 3, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_7, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_8, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_9, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_mult, 3, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_4, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_5, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_6, 2, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_sum, 3, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_1, 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_2, 1, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_3, 2, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_soustr, 3, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_0, 0, 5, 2, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_point, 2, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grdMainLayout), btn_equal, 3, 5, 1, 1);

    // Creation du fenetre principale
    window = gtk_window_new();
    gtk_window_set_application(GTK_WINDOW(window), app) ;
    gtk_window_set_title(GTK_WINDOW(window), "Mini-Project") ;
    gtk_window_set_default_size(GTK_WINDOW(window), 350, 550);
    gtk_window_present(GTK_WINDOW(window)) ;

    gtk_window_set_child(GTK_WINDOW(window), grdMainLayout) ;

                /*                         */
                /* Gestion des signales    */
                /*                         */

    g_signal_connect(btn_0, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_1, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_2, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_3, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_4, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_5, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_6, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_7, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_8, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_9, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_C, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_point, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_del, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_sum, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_div, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_mult, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_soustr, "clicked", G_CALLBACK(user_interaction), NULL) ;
    g_signal_connect(btn_equal, "clicked", G_CALLBACK(user_interaction), NULL) ;


    // Modification des widgets
    gtk_entry_set_alignment(GTK_ENTRY(input_widget), 0.90) ;
    gtk_entry_buffer_set_max_length(buffer, 50) ;
    // gtk_widget_set_size_request(input_widget, 50, 50) ;
    // gtk_widget_set_vexpand(input_widget, FALSE) ;
    // gtk_widget_set_vexpand_set(input_widget, TRUE) ;
    // gtk_grid_set_row_spacing(GTK_GRID(grdMainLayout), 0) ;

                /*                                       */
                /* Personnalisation des widgets avec CSS */
                /*                                       */


    gtk_widget_add_css_class(btn_0, "btn-number") ;
    gtk_widget_add_css_class(btn_1, "btn-number") ;
    gtk_widget_add_css_class(btn_2, "btn-number") ;
    gtk_widget_add_css_class(btn_3, "btn-number") ;
    gtk_widget_add_css_class(btn_4, "btn-number") ;
    gtk_widget_add_css_class(btn_5, "btn-number") ;
    gtk_widget_add_css_class(btn_6, "btn-number") ;
    gtk_widget_add_css_class(btn_7, "btn-number") ;
    gtk_widget_add_css_class(btn_8, "btn-number") ;
    gtk_widget_add_css_class(btn_9, "btn-number") ;
    gtk_widget_add_css_class(btn_point, "btn-number") ;

    gtk_widget_add_css_class(btn_sum, "btn-operator") ;
    gtk_widget_add_css_class(btn_soustr, "btn-operator") ;
    gtk_widget_add_css_class(btn_div, "btn-operator") ;
    gtk_widget_add_css_class(btn_mult, "btn-operator") ;

    gtk_widget_add_css_class(btn_C, "other-btns") ;
    gtk_widget_add_css_class(btn_del, "other-btns") ;

    gtk_widget_add_css_class(btn_C, "btn-c") ;

    gtk_widget_add_css_class(btn_equal, "btn-equal") ;

    gtk_widget_add_css_class(window, "window") ;
    GtkCssProvider *provider = gtk_css_provider_new() ;
    gtk_css_provider_load_from_path(provider, "style.css") ;

    //gtk_window_set_resizable(window, FALSE) ;
    gtk_style_context_add_provider_for_display(gtk_widget_get_display(btn_0), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION) ;

}

int main(int argc, char* argv[]) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("Calculator.app", G_APPLICATION_DEFAULT_FLAGS);
    status = g_signal_connect(app, "activate", G_CALLBACK(createWidgetAndlayout), NULL);
    g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status ;

}

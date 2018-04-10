#include <stdio.h>
#include <stdlib.h>

typedef struct data_frame {
    char *name;
    double **data;
    int length;
    int width;
    int initialized;
} DataFrame;


typedef struct simple_perceptron {
    double **data;
    int input_neurons_nb;
    double learning_rate;
    double *weights;
    double error;
    long epochs;
} Perceptron;

Perceptron train(DataFrame data_frame, double learning_rate, void *epochs);
DataFrame create_dataframe(char* file_path, const char* delimiter);
void visualize_weights(Perceptron perceptron);
void visualize_dataframe(DataFrame data_frame);

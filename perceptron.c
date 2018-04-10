#define _GNU_SOURCE
#include "perceptron.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>



void initialize_weight(double* v, int n) {
    int i;
    srand(time(NULL));
    for (i=0; i<n; i++) {
        double random_weight = (double)rand() / (double)RAND_MAX;
        v[i] = random_weight * random_weight;
    }
}


void visualize_dataframe(DataFrame data_frame) {
    int i, j;
    printf("\n");
    if (!data_frame.initialized) {
        printf("Data frame not initialized!");
        return ;
    }
    
    for (i=0; i<data_frame.length; i++) {
        for (j=0; j<data_frame.width+1; j++) {
            printf("%.3lf\t", data_frame.data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void visualize_weights(Perceptron perceptron) {
    int i;
    printf("\n");
    if (perceptron.weights == NULL) {
        printf("Perceptrons was not trained!");
        return ;
    }
    
    for (i=0; i<perceptron.input_neurons_nb-1; i++) {
        printf("%lf\n", perceptron.weights[i]);
    }
    printf("Trained in %ld epochs.\n", perceptron.epochs);
}

double** add_bias(double **data_frame, int m, int n) {
    int i, j;
    double **aux = (double**)malloc(m*sizeof(double*));
    for (i=0; i<m; i++) {
        aux[i] = (double*)malloc((n+1)*sizeof(double));
        for (j=0; j<n; j++) {
            aux[i][j+1] = data_frame[i][j];
        }
    }
    
    for (i=0; i<m; i++) {
        free(data_frame[i]);
        aux[i][0] = 1.0;
    }
    
    return aux;
}


DataFrame create_dataframe(char* file_path, const char* delimiter) {
    DataFrame frame;
    double** data_frame;
    int i, j, file_length = 0, file_width = 0;
    char* line = NULL;
    char *tok = NULL;
    size_t parameter = 0;
    FILE* fp = fopen(file_path, "r");

    frame.initialized = 0;
    if (fp == NULL) {
      return frame;
    }

    while ((getline(&line, &parameter, fp)) != -1) {
        if (!file_width) {
            tok = strtok(line, delimiter);
            file_width++;
            while (tok != NULL) {
                tok = strtok(NULL, delimiter);
                file_width++;
            }
            file_width--;
        }
        file_length++;
    }
    fclose(fp);

    fp = fopen(file_path, "r");

    if (fp == NULL) {
      return frame;
    }

    data_frame = (double**)malloc(file_length*sizeof(double*));
    for (i=0; i<file_length; i++) {
        data_frame[i] = (double*)malloc(file_width*sizeof(double));
    }

    line = NULL;
    i = j = 0;
    while((getline(&line, &parameter, fp)) != -1) {
        tok = strtok(line, delimiter);
        data_frame[i][j] = atof(tok);
        j++;
        while (tok != NULL) {
            tok = strtok(NULL, delimiter);
            if (tok != NULL) {
                data_frame[i][j] = atof(tok);
            }
            j++;
        }
        i++;
        j = 0;
    }
    
    data_frame = add_bias(data_frame, file_length, file_width);
    
    fclose(fp);
    free(line);

    frame.length = file_length;
    frame.width = file_width + 1;
    frame.name = file_path;
    frame.initialized = 1;
    frame.data = data_frame;
    
    printf("Dataframe created!");
    return frame;
}

int* get_expected_output(const DataFrame data_frame) {
    if (data_frame.initialized == 0)
        return NULL;

    int i;
    int *y = (int*)malloc((data_frame.length)*sizeof(int));

    for (i=0; i<data_frame.length; i++) {
        y[i] = (int)data_frame.data[i][data_frame.width - 1];
    }

    return y;
}


int step_function(double x) {
    return (x > 0) ? 1 : 0;
}

Perceptron train(const DataFrame data_frame, double learning_rate, void *epochss) {
    printf("\nTraining started!\n");
    long epochs = (epochss == NULL) ? 10 : (long)epochss;
    int i, j, flag = 1, actual_epoch = 0, actual_output;
    double u;
    Perceptron neural = {
        .data = data_frame.data,
        .input_neurons_nb = data_frame.width,
        .learning_rate = learning_rate,
        .epochs = epochs};
    const int *y = get_expected_output(data_frame);
    double *weights = (double*)calloc((neural.input_neurons_nb-1), sizeof(double));
    initialize_weight(weights, neural.input_neurons_nb-1);
    
    j = 0;
    while (actual_epoch <= epochs && flag) {
        flag = 0;
        for (i=0; i<data_frame.length; i++) {
            u = 0.0;

            for (j=0; j<neural.input_neurons_nb-1; j++) {
                u += weights[j] * data_frame.data[i][j];
            }
            actual_output = step_function(u);

            if (y[i] != actual_output) {
                for (j=0; j<neural.input_neurons_nb-1; j++) {
                    weights[j] = weights[j] + learning_rate * (y[i] - actual_output) * data_frame.data[i][j];
                }
                flag = 1;
            }
        }
        
        j++;
        actual_epoch++;
    }

    neural.epochs = actual_epoch;
    neural.weights = weights;
    return neural;
}


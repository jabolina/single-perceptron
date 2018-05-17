#include "perceptron.h"


int main() {
    char *file_path = "jogadores.csv";
    DataFrame data_frame = create_dataframe(file_path, ",");
    Perceptron trained = train(data_frame, 1.0, NULL);
    
    visualize_weights(trained);
    
    return 0;
}

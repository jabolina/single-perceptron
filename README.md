# Single perceptron ANN

A single perceptron artificial neural network implementation in C. This network can solve problems linearly separable, because 
it's a single perceptron for the output.
<br><br>
This implementation contains only the sigmoid function with the special case where the <b>β</b> degree is equals to 90º, 
also called, the step function.
<br><br>
Also contains a function where a dataset can be parsed to a matrix, the parameters needed are the file path, the file separator
(commas, tab,...) and if need to ignore the first line in the file.
<br><br>
The artificial neural network will train the number of epochs defined, but, if the links between the neurons are already
satisfied, and no reajust was made, the train will stop.
<br><br>
To learn about the single perceptron, you can read it 
<a href="http://computing.dcu.ie/~humphrys/Notes/Neural/single.neural.html">here</a>.

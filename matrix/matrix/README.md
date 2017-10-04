
# CONSOLE APPLICATION : Matrix-Computation Project Overview

## Development Environment
 IDE: Microsoft Visual Studio 2017.

## Development Schedule
1. Implement Matrix Class
 1.1 matrix transpose, spliting, etc
 1.2 reverse, determine
 1.3 optimize matrix operations with iterator & parallelism
 1.4 exceptions

2. Optimize Matrix I/O
  - file read/write

3. The second class "matlab" provides methods for 
  - generating special matrices: diagonal, unit
  - more element-wise arithmetic operations
  - operations associated with more than two matrices (SVD, eigendecomposition)
  - statistics methods: mean, variance, covariance, etc.

4. The third class "ml" encapsulates machine learning architecture
  - prepocessing 
  - linear/logistic regression
  - shadow neural network

### Attention
1. The Matrix class index from ZERO, not one.
2. The core Matrix elements are stored in vector of vector.
3. Matrix constructors should not support prmitave array (like double[][]) for safety reason.


### Files
- matrix.cpp
  This is the entry of the application.

- mymatrix.h
  This is the definition of matrix class.

- matlab.h
  This is the definition of matlab class.

- test.cpp
	This is the test file.




Supplimentary Materials:
1. About user-defined iterator. See https://stackoverflow.com/questions/8054273/how-to-implement-an-stl-style-iterator-and-avoid-common-pitfalls.
2. About disable "D_SCL_SECURE_NO_WARNING". See https://stackoverflow.com/a/25046975/7110228.
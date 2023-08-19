written by Zenab Mahmoud 212384390

Ex3q1:

==description==
Make Calculaters for Polynomials.

==functions==

void addPolynomials(int *pol1, int *pol2, int power1, int power2)
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
Add the two Polynomials togather and print the result. 

void subtractPolynomials(int* pol1, int *pol2, int power1, int power2)
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
subtract the two Polynomials togather and print the result. 

void multiplyPolynomials(int *pol1, int *pol2, int power1, int power2)
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
multiply the two Polynomials togather and print the result. 
 
void calculator (int* pol1 ,int* pol2, int power1, int power2 ,void (callBack)(int*,int*,int,int))
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
callBack: the function that the program should go to it.
return the funcion that should go to it.

==Program files==
Ex3q1.c

==How yo compile==
compile: gcc Ex3q1.c -o Ex3q1 

==input==
sentence that has the two polynomials and the operation between them.

==output==
polynomial , its the result of the operation between the two polynomials.


Ex3q2a:

==description==
take the equation from the user and write it to the shared memory.

==Program files==
Ex3q2a.c

==How yo compile==
compile: gcc Ex3q2a.c -o Ex3q2a 

==input==
the equation.

==output==

Ex3q2b:

==description==
Take the equation from the shared memory and calculate the polynomials.

==functions==

void addPolynomials(int *pol1, int *pol2, int power1, int power2)
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
Add the two Polynomials togather and print the result. 

void subtractPolynomials(int* pol1, int *pol2, int power1, int power2)
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
subtract the two Polynomials togather and print the result. 

void multiplyPolynomials(int *pol1, int *pol2, int power1, int power2)
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
multiply the two Polynomials togather and print the result. 
 
void calculator (int* pol1 ,int* pol2, int power1, int power2 ,void (callBack)(int*,int*,int,int))
parameters:
*pol1:the first Polynomial.
*pol2:the second Polynomial.
power1: the power of the first Polynomial.
power2: the power of the second Polynomial.
callBack: the function that the program should go to it.
return the funcion that should go to it.

==Program files==
Ex3q2b.c

==How yo compile==
compile: gcc Ex3q2b.c -o Ex3q2b   

==input==

==output==
polynomial , its the result of the operation between the two polynomials.


Ex3q3a:

==description==
take the equation from the user and write it to the shared memory.

==Program files==
Ex3q3a.c

==How yo compile==
compile: gcc -o Ex3q3a Ex3q3a.c -pthread

==input==
the equation.

==output==

Ex3q3b:

==description==
Take the equation from the shared memory and calculate the polynomials by threads.

==functions==

void addPolynomials()
Add the two Polynomials togather.

void subtractPolynomials()
subtract the two Polynomials togather. 

void multiplyPolynomials()
multiply the two Polynomials togather and print the result. 
 
==Program files==
Ex3q3b.c

==How yo compile==
compile: gcc -o Ex3q3b Ex3q3b.c -pthread  

==input==

==output==
polynomial , its the result of the operation between the two polynomials.

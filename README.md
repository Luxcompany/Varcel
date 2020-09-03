# ![logo](misc/vlogo.png)

*note: Varcel is still in alpha; most features don't exist yet.*

## What's Varcel?
Varcel is an open source, platform-independent, functional flavored, graphics-focused programmng language geared towards UI design, data visualization, and general computation. As of now Varcel can be contained in **.vl** or **.varcel** files, and "executed" via the command line.

"Execution" is an umbrella term that covers different methods of building/running Varcel code (e.g. compiling, interpreting, and transpiling). For instance, a Varcel program can be transpiled to JS for use on the web.

## How it works.

**Details are being worked out in [planning folder](planning).**

## Using it with other languages.
Varcel can be used alone, or in tandem with other languages. *Integration of Varcel and other languages hasn't been worked out yet, but the plan is something like the code c below:*
```c
double numbers[100]; //declare array "numbers" with 100 elements
memset(numbers, 3, 100*sizeof(double)); //fill "numbers" with 3

/*
format:
VARCEL( (varcel code), captures...)

maybe not the most practical example; multiply each element by 2.
"numbers" is captured, variables can't be acessed otherwise.
The following code effectively multiplies each element by 2
*/
VARCEL((numbers = for numbers@i: numbers@i * 2), numbers)
```

Varcel can also be used in a less direct way via API:
```c
#include <varcel.h> //varcel api header

//...

/*
Runs on thread.
If program finishes execution, then variables are held in suspension until v_close is called on the thread.
*/
v_thread program = v_execute("./file/src.vl"); //start program

//...

//get handle of "varName" in program, and store it in myVar
v_var myVar = v_var_get("varName"); 

//print myVar if it's a string
if(v_check_type(myVar, v_string))
    printf(*v_var_pointer(myVar)); //v_var_pointer returns pointer to var

//...

v_close(program); //ends program
```
#
Feel free to help out and make a suggestion (via issues)!

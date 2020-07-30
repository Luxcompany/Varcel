# ![logo](misc/vlogo.png)

*note: Varcel is still in alpha; some features don't exist yet.*

## What's Varcel?
Varcel is an open source, functional flavored, graphics-focused programmng language geared towards UI design, data visualization, and elegant data processing. As of now Varcel can be contained in **.vl** or **.varcel** files, and compiled via the command line (or an IDE-like application in the future that'll include debuging features). Varcel is first transpiled to C, then compiled into machine code by a regular C compiler (TCC used by default). Transpiling to c makes Varcel more platform independent (as any platform with a c compiler will also be able to compile Varcel), and allows compiled applications to run faster than they would if Varcel was an interpreted language.

## How it works.

**Details are being worked out in [planning file](planning/Varcel_DRAFT.txt).**

## Using it with other languages.
Varcel can be used alone, or in tandem with other languages. *Integration of Varcel and other languages hasn't been worked out yet, but the plan is something like the code c below:*
```c
double numbers[100]; //declare array "numbers" with 100 elements
memset(numbers, 3, 100); //fill "numbers" with 3

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

//get value of "varName" in program, and store it in myVar
v_var myVar = v_var_get("varName"); 

//print myVar if it's a string
if(v_check_type(myVar, v_string))
    printf(*v_var_pointer(myVar)); //v_var_pointer returns pointer to var

//...

v_close(program); //ends program
```
#
Feel free to help out and make a suggestion, or (even better) a pull request!

#include <iostream>
#include <stdio>
#include <stdlib>

// This describes an abstract machine similar to the x86 two-address machine.
// the SP points at the value on top of the stack
// FP is the activation record (Frame) pointer.  It will always point at the
// old FP (that of the caller).  The first parameter is 1 word below
// the FP and the first local variable of size s words will be -s words
// before it. So initial parameter offset is 1 and initial local offset
// is 0.  Remember all sizes must now be in words not bytes.

typedef int  Word; // most instructions deal with words

typedef Word * WPointer;
typedef Word Register; 

// The registers
Register  R0; // the return register
Register  R1, R2, R3; // General purpose registers

// Control Stack and operations

#define STK_SIZE 50000
typedef Word     ControlStack[STK_SIZE];
ControlStack STK;

// stack pointer and frame pointer
WPointer STKEnd = STK+STK_SIZE-1;
WPointer SP = STKEnd;
Word FP = (Word)STKEnd;

#define push(W) *--SP = (Word)(W) 
#define pop(W) (W) = *SP++

typedef int (*Vfn)();      // A virtual function pointer.

void put_char()
{
    push(FP);
    FP = (Word)SP; 
    printf("%c", SP[1]);
    fflush(stdout); 
    pop(FP); 
    SP += 1;
}

void new_line()
{
    printf("\n");
    fflush(stdout); 
}

void put_str()
{
    WPointer p;
    push(FP);
    FP = (Word)SP; 
    for(p = (WPointer)(SP[1]); *p != 0; p++)
    {
        printf("%c", *p);
        fflush(stdout); 
    }
    pop(FP); 
    SP += 1;
}

void put_int()
{
    push(FP);
    FP = (Word)SP; 
    printf("%d",(Word)SP[1]);
    fflush(stdout); 
    pop(FP); 
    SP += 1;
}

void get_int()
{
    push(FP);
    FP = (Word)SP; 
    scanf("%d",&R0); 
    pop(FP); 
}

void get_char()
{
    char c; 
    push(FP);
    FP = (Word)SP; 
    scanf("%c",&c);
    R0 = (Word)c; 
    pop(FP); 
}

void get_str()
{
    WPointer p, q;
    push(FP);
    FP = (Word)SP; 
    char s[512]; 
    char* ps = s;
    gets(s);
    p = q = (WPointer)malloc(sizeof(Word)*(strlen(s)+1));
    for(; *s != '\0'; ps++, p++)
        *p = (Word)*ps; 
    R0 = (Word)q; 
    pop(FP); 
}

void mallocate()
{
    push(FP);
    FP = (Word)SP; 
    R0 = (Word)malloc(SP[1]); 
    pop(FP); 
    SP += 1;
}

void mfree()
{
    push(FP);
    FP = (Word)SP; 
    free((char *)SP[1]); 
    pop(FP); 
    SP += 1;
}

// for debugging - place calls in target code

void dumpStack()
{
    for (Word* p = SP; p < STK+STK_SIZE; p++)
        cout << "value at " << (Word)p << " : " << *p << endl;
}

void dumpRegs()
{
    cout << "R0 = " << R0 << endl;
    cout << "R1 = " << R1 << endl;
    cout << "R2 = " << R2 << endl;
    cout << "R3 = " << R3 << endl;
    cout << "SP = " << (Word)SP << endl;
    cout << "FP = " << FP << endl;
}


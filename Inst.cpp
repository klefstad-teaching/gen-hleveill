// #include "all.h"
#include <iostream>
using namespace std;
#include "error.h"
#include "Inst.h"
#include "tokens.h"
#include "registers.h"

static Oper regOpers[] = {
    new RegisterOper(0),
    new RegisterOper(1),
    new RegisterOper(2),
    new RegisterOper(3),
    new RegisterOper(4),
    new RegisterOper(5),
    new RegisterOper(6),
    new RegisterOper(7)
};

Oper regOper(int i)
{
    return regOpers[i];
}

int TempLabelOper  ::  TEMP_NUMBER = 10;

void LabelInst :: gen() 
{
    cout << labelName << ":\n"; 
}

void UnaryInst :: gen()
{ 
    compiler_error("undefined method gen in UnaryInst");
}

void NegateInst :: gen()
{
    cout << "\t" << operand << " = - (Word)" << operand << ";\n"; 
}

void DecrInst :: gen()
{
    cout << "\t(" << operand << ") --;\n";
}

void IncrInst :: gen()
{
    cout << "\t(" << operand << ") ++;\n";
}

void ReturnInst :: gen()
{
    cout << "\treturn 0;\n";
}


void BinaryInst :: gen()
{
    compiler_error("unknown binary operator");
}

void PlusInst :: gen()
{
   cout << "\t" << dest << " += " << src << ";\n";
}

void MinusInst :: gen()
{
   cout << "\t" << dest << " -= " << src << ";\n";
}

void MultInst :: gen()
{
   cout << "\t" << dest << " *= " << src << ";\n";
}

void DivInst :: gen()
{
   cout << "\t" << dest << " /= " << src << ";\n";
}

    void do_move(Oper src, Oper dest)
    {
        cout << "\t" << dest << " = (Word)" << src << ";\n";
    }

    void do_push(Oper src)
    {
        cout << "\tpush(" << src << ");\n";
    }

    void do_pop(Oper dest)
    {
        cout << "\tpop(" << dest << ");\n";
    }


void MoveInst :: gen()
{
   if (dest == TOS)
      do_push(src);
   else if (src == TOS)
      do_pop(dest);
   else
      do_move(src, dest);
}

void AddressInst :: gen()
{
    cout << "\t" << dest << " = (Word) & " << src << ";\n";
}

void JumpInst :: gen()
{
    cout << "\tgoto " << to << ";\n";
}

void JumpZeroInst :: gen()
{
    cout << "\tif ( !" << oper << " ) goto " << label << ";\n";
}

void JumpNotZeroInst :: gen()
{
    cout << "\tif ( " << oper << " ) goto " << label << ";\n";
}

void CompareAndJumpInst :: gen()
{
    int op = cond;
    cout << "\t";
    cout << "if ( " << left << " ";
    switch (op) {
            case '<':
                    cout << "<";
                    break;
            case LE:
                    cout << "<=";
                    break;
            case '>':
                    cout << ">";
                    break;
            case GE:
                    cout << ">=";
                    break;
            case EQ:
                    cout << "==";
                    break;
            case NE:
                    cout << "!=";
                    break;
            default:
                    compiler_error("CompareAndJumpInst.default");
                    break;
    }
    cout << " " << right << " ) goto " << label <<";\n";
}

void CompareAndSetInst :: gen()
{
    int op = cond;
    cout << "\t";
    cout << "(" << dest << ") = " << "( " << left << " ";
    switch (op) {
            case '<':
                    cout << "<";
                    break;
            case LE:
                    cout << "<=";
                    break;
            case '>':
                    cout << ">";
                    break;
            case GE:
                    cout << ">=";
                    break;
            case EQ:
                    cout << "==";
                    break;
            case NE:
                    cout << "!=";
                    break;
            default:
                    compiler_error("CompareAndSetInst.default");
                    break;
    }
    cout << " " << right << " ) ? true : false;\n";
}

void CallInst :: gen() 
{
   cout << "\t" << subprogramName << "();\n";
}

void BeginFileInst :: gen()
{
    cout << "#include \"machine.h\"\n\n";
}

void BeginSubpInst :: gen()
{
    cout << "int " << name << "() { " << endl;
}

void EndSubpInst :: gen()
{
    cout << "} " << endl << endl;
}

// Write out the beginning of a vtable

void BeginVTableInst :: gen()
{
    cout << "Vfn " << name << "[]={ " << endl;
}

// Write out an entry in the Vtable.

void VTableEntryInst :: gen()
{
    cout << "\t" << name << ",\n";
}

// End a vtable declaration
 
void EndVTableInst :: gen()
{
    cout <<"\t0};\n\n"; 
}


// Write out the header for a String literal

void BeginDataInst :: gen()
{
    cout << "Word " << name << "[]={" << endl;
}

void EndDataInst :: gen()
{
    cout << "\t0};" << endl;
}

void EnterInst :: gen()
{
    int i;
    do_push(regOper(FP_REG));
    do_move(regOper(SP_REG), regOper(FP_REG));
    if (localSize > 0)
    {
        Oper d = regOper(SP_REG);
        cout << "\t";
        d->gen(); 
        cout << " -= " << localSize << ";\n";
    }
    for (i = 0; i < registersUsed ; i++)
        do_push(regOper(MIN_GP_REG+i));
}

void LeaveInst :: gen()
{
    int i;

    for (i = registersUsed-1; i>=0; i--)
        do_pop(regOper(MIN_GP_REG+i));
    if(localSize>0)
    {
        Oper d=regOper(SP_REG);
        cout << "\t";
        d->gen(); 
        cout << " += " << localSize << ";\n";
    }
    do_pop(regOper(FP_REG));
    if(paramSize>0)
    {
        Oper d=regOper(SP_REG);
        cout << "\t";
        d->gen(); 
        cout << " += " << paramSize << ";\n";
    }
}

void ConstantWordInst :: gen()
{
   cout << "\t" << value << ",\n";
}



//  OPERS 

void ImmediateOper :: gen()
{
    immediate->gen();
}

void ConstantOper :: gen()
{
    cout << constValue;
}

// Output a reference to a hardware register

void RegisterOper :: gen()
{
    switch( regNum ) {
            case RETURN_REG:
                    cout << "R0";
                    return;
            case 1:
                    cout << "R1";
                    return;
            case 2:
                    cout << "R2";
                    return;
            case 3:
                    cout << "R3";
                    return;
            case SP_REG:
                    cout << "SP";
                    return;
            case FP_REG:
                    cout << "FP";
                    return;
            case 8:
                    cout << "TOS_ERROR"; //TOS
                    return;
            default:
                    compiler_error("unknown register number in gen");
                    return;
    }
}

void IndexedOper :: gen()
{
    cout << "((WPointer)";
    array->gen();
    cout << ")";
    cout << "[";
    index->gen();
    cout << "]";
}


void IndirectedOper :: gen()
{
    cout << "*(WPointer)";
    indirected->gen();
    cout << ""; 
}

void SelectedOper :: gen()
{
    cout << "((WPointer)"; 
    selected->gen();
    cout << " + " << selectedOffset << ")";
}

void NamedLabelOper :: gen()
{
    cout << labelName;
}


void TempLabelOper :: gen()
{
    cout << "_T" << labelNumber;
}

Oper TOS = new RegisterOper(TOS_REG);

// Inst (instruction) and Oper (operand) structures for assembly language generation

typedef struct OperBlock * Oper;

// The base class of all operand types

struct OperBlock 
{
    OperBlock( ) 
    {
    } 

    // returns any GP registers found anywhere in an Oper

	virtual int findGPRegister()
    {
        compiler_error("undefined method findGPRegister in OperBlock");
        return 0;
    }

	virtual int isRegister()
	{
		return 0;
	}

    virtual void gen()
    {
        compiler_error("undefined method gen in OperBlock");
    }

};

inline ostream & operator << (ostream & out, Oper o)
{
    if (!o)
        out << "NULL";
    else
        o->gen();
    return out;
}

// A simple constant, e.g., 10


struct ConstantOper 
    : OperBlock 
{
    int constValue;

    ConstantOper(int val) 
        : OperBlock(), constValue(val)
    {
    }

    static Oper make(int val)
    {
        return new ConstantOper(val);
    }


    virtual void gen();


};

// A register is an abstraction of a hardware register

struct RegisterOper 
    : OperBlock 
{
    int regNum;

    RegisterOper(int r)
        : OperBlock(), regNum(r)
    {
    }

    static Oper make(int r)
    {
        return new RegisterOper(r);
    }


	virtual int isRegister()
	{
		return 1;
	}
    virtual void gen();

};

struct IndirectedOper 
    : OperBlock 
{
    Oper indirected;

    IndirectedOper(Oper o) :
        OperBlock(), indirected(o)
    {
    }

    static Oper make(Oper o)
    {
        return new IndirectedOper(o);
    }


    virtual void gen();

};

struct SelectedOper 
    : OperBlock 
{
    Oper selected;
    int selectedOffset;

    SelectedOper(Oper o, int offset) 
        : OperBlock(), selected(o), selectedOffset(offset)
    {
    }

    static Oper make(Oper o, int offset)
    {
        return new SelectedOper(o, offset);
    }


    virtual void gen();

};

// An array reference

struct IndexedOper 
    : OperBlock 
{
    Oper array;
    Oper index;

    IndexedOper(Oper o, Oper ind) 
        : OperBlock(), array(o), index(ind)
    {
    }

    static Oper make(Oper o, Oper ind)
    {
        return new IndexedOper(o, ind);
    }


    virtual void gen();

};

// A named label is an object with a string name, such as
// a vtable, character string or a function

struct NamedLabelOper 
    : OperBlock 
{

    string labelName;

    NamedLabelOper(string name)
        : OperBlock(), labelName(name)
    {
    }

    static Oper make(string name)
    {
        return new NamedLabelOper(name);
    }


    virtual void gen();

};

// A temp label operand is a simple label like L10:

struct TempLabelOper 
    : OperBlock 
{
    int labelNumber;
    static int TEMP_NUMBER;

    TempLabelOper() 
        : OperBlock(), labelNumber(TEMP_NUMBER++)
    {
    }

    static Oper make()
    {
        return new TempLabelOper();
    }


    virtual void gen();

};

// Immediate means take the next thing literally. This is similar to 
// an "escape" in a regular expression. In machine language, this may
// mean to put a dollar sign in front of the operand.

struct ImmediateOper 
    : OperBlock 
{
    Oper immediate;

    ImmediateOper(Oper o)
        : OperBlock(), immediate(o)
    {
    }

    static Oper make(Oper o)
    {
        return new ImmediateOper(o);
    }


    virtual void gen();

};


// *** CODE ***

typedef struct InstBlock * Inst;

struct InstBlock 
{
    virtual void gen()
    {
        compiler_error("undefined method gen in InstBlock");  
    }
};

struct LabelInst 
    : InstBlock 
{
    Oper labelName;

    LabelInst(Oper o) :
        labelName(o)
    {
    }

    static void make( Oper o )
    {
        LabelInst local( o );
        local.gen();
    }

    virtual void gen();
};


// The abstract base class for unary instructions
 
struct UnaryInst 
    : InstBlock 
{
    Oper operand;

    UnaryInst(Oper o) 
        : operand(o)
    {
    }

    virtual void gen();
};

// The negation unary operator negates its operand

struct NegateInst
    : UnaryInst
{
    NegateInst(Oper o)
        : UnaryInst(o)
    {
    }

    virtual void gen();

    static void make( Oper o )
    {
        NegateInst local( o );
        local.gen();
    }

};

// The decrement unary operator decrements its operand by one

struct DecrInst
    : UnaryInst
{
    DecrInst(Oper o)
        : UnaryInst(o)
    {
    }

    virtual void gen();

    static void make( Oper o )
    {
        DecrInst local( o );
        local.gen();
    }

};

// The increment unary operator increments its operand by one

struct IncrInst
    : UnaryInst
{
    IncrInst(Oper o)
        : UnaryInst(o)
    {
    }

    virtual void gen();

    static void make( Oper o )
    {
        IncrInst local( o );
        local.gen();
    }

};

// The return unary operator returns its operand

struct ReturnInst
    : UnaryInst
{
    ReturnInst(Oper o)
        : UnaryInst(o)
    {
    }

    virtual void gen();

    static void make( Oper o )
    {
        ReturnInst local( o );
        local.gen();
    }

};


// The binary operator base class. All subclasses will have
// src and dest members.

struct BinaryInst
    : InstBlock 
{
    Oper src, dest;

    BinaryInst( Oper s, Oper d) 
        : src(s), dest(d)
    {
    }

    virtual void gen();
};

// The addition operator

struct PlusInst 
    : BinaryInst 
{

    PlusInst(Oper s, Oper d) 
        : BinaryInst(s, d)
    {
    }

    static void make( Oper s, Oper d )
    {
        PlusInst local( s, d );
        local.gen();
    }

    virtual void gen();

};

// The subtraction binary operator

struct MinusInst 
    : BinaryInst 
{

    MinusInst(Oper s, Oper d)
        : BinaryInst(s, d)
    {
    }

    static void make( Oper s, Oper d )
    {
        MinusInst local( s, d );
        local.gen();
    }

    virtual void gen();
};

// The multiplication binary operator

struct MultInst 
    : BinaryInst 
{

    MultInst(Oper s, Oper d) 
        : BinaryInst(s, d)
    {
    }

    static void make( Oper s, Oper d )
    {
        MultInst local( s, d );
        local.gen();
    }

    virtual void gen();
};

// Division bindary operator

struct DivInst 
    : BinaryInst 
{

    DivInst(Oper s, Oper d) 
        : BinaryInst(s, d)
    {
    }

    static void make( Oper s, Oper d )
    {
        DivInst local( s, d );
        local.gen();
    }

    virtual void gen();
};

// The assignment operator

struct MoveInst 
    : BinaryInst 
{

    MoveInst(Oper s, Oper d) 
        : BinaryInst(s,d)
    {
    }

    static void make( Oper s, Oper d )
    {
        MoveInst local( s, d );
        local.gen();
    }

    virtual void gen();
};

// Take the address of the right hand side

struct AddressInst
    : BinaryInst 
{

    AddressInst(Oper s, Oper d) 
        : BinaryInst(s,d)
    {
    }

    static void make( Oper s, Oper d )
    {
        AddressInst local( s, d );
        local.gen();
    }

    virtual void gen();
};


// END BINARY_CODES

struct JumpInst 
    : InstBlock 
{
    Oper to;

    JumpInst(Oper o) 
        : to(o)
    {
    }

    static void make( Oper o )
    {
        JumpInst local( o );
        local.gen();
    }

    virtual void gen();
};

struct JumpZeroInst
    : InstBlock 
{
    Oper oper, label;

    JumpZeroInst(Oper o, Oper la) 
        : oper(o), label(la)
    {
    }

    static void make( Oper o, Oper la )
    {
        JumpZeroInst local( o, la );
        local.gen();
    }

    virtual void gen();

};

struct JumpNotZeroInst
    : InstBlock 
{
    Oper oper, label;

    JumpNotZeroInst(Oper o, Oper la) 
        : oper(o), label(la)
    {
    }

    static void make( Oper o, Oper la )
    {
        JumpNotZeroInst local( o, la );
        local.gen();
    }

    virtual void gen();

};

struct CompareAndJumpInst 
    : InstBlock 
{
    int cond;
    Oper left, right, label;

    CompareAndJumpInst(int t, Oper l, Oper r, Oper la) 
        : cond(t), left(l), right(r), label(la)
    {
    }

    static void make( int t, Oper l, Oper r, Oper la )
    {
        CompareAndJumpInst local( t, l, r, la );
        local.gen();
    }    
        
    virtual void gen();
};

struct CompareAndSetInst
    : InstBlock 
{
    int cond;
    Oper left, right, dest;

    CompareAndSetInst(int t, Oper l, Oper r, Oper d) 
        : cond(t), left(l), right(r), dest(d)
    {
    }

    static void make( int t, Oper l, Oper r, Oper dest )
    {
        CompareAndSetInst local( t, l, r, dest );
        local.gen();
    }    
        
    virtual void gen();
};

struct CallInst
    : InstBlock
{
    Oper subprogramName;

    CallInst(Oper o) 
        : subprogramName(o)
    {
    }

    static void make( Oper o )
    {
        CallInst local( o );
        local.gen();
    }    

    virtual void gen();
};

struct EnterInst 
    : InstBlock 
{
    int localSize;
    int registersUsed;

    EnterInst(int ls, int ru) :
        localSize(ls), registersUsed(ru)
    {
    }

    static void make( int ls, int ru )
    {
        EnterInst local( ls, ru );
        local.gen();
    }    

    virtual void gen();
};

struct LeaveInst 
    : InstBlock 
{
   int localSize;
   int registersUsed;
   int paramSize; 

    LeaveInst(int ls,int ru, int ps) 
        : localSize(ls),registersUsed(ru),paramSize(ps)
    {
    }

    static void make( int ls, int ru, int ps )
    {
        LeaveInst local( ls, ru, ps );
        local.gen();
    }   

    virtual void gen();
};

struct ConstantWordInst 
    : InstBlock 
{
    Oper value;

    ConstantWordInst(Oper o) 
        : value(o)
    {
    }

    static void make( Oper o )
    {
        ConstantWordInst local( o );
        local.gen();
    }   

    virtual void gen();

};

struct BeginFileInst 
    : InstBlock 
{

    BeginFileInst()
    {
    }

    static void make()
    {
        BeginFileInst local;
        local.gen();
    }   

    virtual void gen();
};

struct BeginSubpInst 
    : InstBlock 
{

    Oper name; // The name of the subprogram being defined

    BeginSubpInst(Oper n)
        : name(n)
    {
    }

    static void make(Oper n)
    {
        BeginSubpInst local(n);
        local.gen();
    }   

    virtual void gen();
};

struct EndSubpInst 
    : InstBlock 
{

    EndSubpInst()
    {
    }

    virtual void gen();

    static void make()
    {
        EndSubpInst local;
        local.gen();
    }   
};


// This is a string literal header, i.e., Word Name[]={

struct BeginDataInst 
    : InstBlock 
{

    Oper name; // The name of the string literal

    BeginDataInst(Oper n)
        : name(n)
    {
    }

    static void make(Oper n)
    {
        BeginDataInst local(n);
        local.gen();
    }   

    virtual void gen();
};

struct EndDataInst 
    : InstBlock 
{

    EndDataInst()
    {
    }

    static void make()
    {
        EndDataInst local;
        local.gen();
    }   

    virtual void gen();
};

// An entry in the virtual function table

struct VTableEntryInst 
    : InstBlock 
{

    Oper name; // The name of the function

    VTableEntryInst(Oper n)
        : name(n)
    {
    }

    static void make(Oper n)
    {
        VTableEntryInst local(n);
        local.gen();
    }  
        
    virtual void gen();
};

// Generate a call to a function corresponding to a label

struct CaseLabelDeclInst 
    : InstBlock 
{
    Oper caseLabel; //has to be a TempLabelOper
    CaseLabelDeclInst(Oper cl)
        : caseLabel(cl)
    {
    } 

    virtual void gen();
    
    static void make( Oper cl )
    {
        CaseLabelDeclInst local(cl);
        local.gen();
    }  
};

// Begin a virtual function table 

struct BeginVTableInst 
    : InstBlock 
{
    Oper name; // The name of the virtual member function

    BeginVTableInst(Oper o)
       : name(o) 
    {
    } 

    static void make( Oper o )
    {
        BeginVTableInst local(o);
        local.gen();
    } 

    virtual void gen(); 
};

// End a virtual function table 

struct EndVTableInst 
    : InstBlock 
{
    EndVTableInst() 
    {
    } 

    static void make()
    {
        EndVTableInst local;
        local.gen();
    } 

    virtual void gen(); 
};

extern Oper TOS; // The top of stack (treated as a register)

Oper regOper(int i); // register oper for reg number i

inline void gen_move_immediate(Oper dest, int value)
{
    MoveInst :: make(dest, ImmediateOper :: make(ConstantOper :: make(value)));
}
inline void gen_jump(Oper label)
{
    LabelInst :: make(label);
}
inline void gen_compare(Oper label)
{
    LabelInst :: make(label);
}
inline void gen_set_cc(int t, Oper l, Oper r, Oper d)
{
    CompareAndSetInst :: make(t, l, r, d);
}

#include <iostream>
using namespace std;
#include "error.h"
#include "Inst.h"
#include "tokens.h"
#include "registers.h"

int lineNumber = 1;

int main()
{
	Oper fp = new RegisterOper(FP_REG);
	Oper r = new RegisterOper(1);
	Oper t = new SelectedOper(fp, -8);
	Oper d = new IndirectedOper(t);
	Oper s = new RegisterOper(2);
	Oper o = d;
	Oper l = new TempLabelOper();
	Oper nl = new NamedLabelOper("foo");
	Oper i = new IndexedOper(l,r);
	Oper c = new ConstantOper(50);
	Oper v = new ImmediateOper(c);

	BeginFileInst :: make();
	BeginSubpInst :: make(nl);
	EnterInst :: make( 20, 2 );
	NegateInst :: make(o);
	DecrInst :: make(o);
	IncrInst :: make(o);
	LabelInst :: make( l );
	PlusInst :: make( s, d );
	MinusInst :: make( v, d );
	MultInst :: make( s, d );
	DivInst :: make( s, d );
	MoveInst :: make( i, d );
	AddressInst:: make( s, d );
	JumpInst :: make( l );
	CompareAndJumpInst :: make( LE, s, d, l ); // if s <= d goto l
	CallInst:: make( nl );
	BeginDataInst :: make(l);
	ConstantWordInst :: make( c );
	EndDataInst :: make();
	LeaveInst :: make( 20, 2, 4);
	ReturnInst :: make(o);
	EndSubpInst :: make();
	BeginVTableInst :: make( l );
	VTableEntryInst :: make(nl);
	EndVTableInst :: make();
}

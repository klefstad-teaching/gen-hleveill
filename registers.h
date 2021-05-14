#define RETURN_REG 0
#define MIN_GP_REG 1
#define MAX_GP_REG 3
#define SP_REG 4
#define FP_REG 5
#define TOS_REG 8
#define N_GP_REG MAX_GP_REG-MIN_GP_REG+1

class RegisterManager
{
    int isFree[N_GP_REG];

public:

    RegisterManager()
    {
        for (int i = 0; i < N_GP_REG; i++)
            isFree[i] = 1;
    }

    Oper getRegister()
    {
        for (int i = 0; i < N_GP_REG; i++)
            if (isFree[i])
            {
                isFree[i] = 0;
                return regOper( i + MIN_GP_REG );
            }
        compiler_error("OUT OF REGISTERS");
        return regOper(0);
    }

    void freeRegister(Oper reg)
    {
        int i = reg->findGPRegister();
        if (i>0)
            isFree[i - MIN_GP_REG] = 1;
    }
};


extern RegisterManager RM;

#define FR(e) RM.freeRegister(e)

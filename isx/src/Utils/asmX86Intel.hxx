/*

Why this file ?

In order to call a delphi method from a standard win32 call, we need to make
some adjustments on the call itself !

Bascially, if Innostep function has been defined with 6 parameters (can be less or more)
like so:

    type
        TMyCallback = function(a,b,c,d,e,f:Integer): Integer;

We may be tempted to define the same type on C-DLL side:

    typedef int(__stdcall MyCallbackType*)(int, int, int, int, int, int);
    
And call it with

        callback(11, 22, 33, 44, 55);

The problem is, here what the STACK looks after this call:

    11
    22
    33
    44
    55
    66

Innosetup, on his side, was excepted this STACK layout

    self (from tmethod.data)
    66
    55
    44
    33

    and where, 
        EAX = self
        EDX = 11
        ECX = 22

So here comes the idea of generating dynamically inline assmebly code
to make the adjustment for the call. 

    Win32DLL         | --------> [ Wrapper CODE] ----------> | Innosetup
    callback call    |                                       | function
                     |                                       | 
                     | <------------------------------------ | ret

*/

class Bytes : public std::vector<BYTE>
{
    public:
        Bytes() {};
        Bytes(BYTE byte) { push_back(byte); }
        Bytes& operator+=(const Bytes& rhs) {
            insert(end(), rhs.begin(), rhs.end());
            return *this;
        }
        Bytes& operator+=(const BYTE rhs) {
            push_back(rhs);
            return *this;
        }
        Bytes& push_DWORD(DWORD v) {
            push_back((BYTE)(v >> 0));
            push_back((BYTE)(v >> 8));
            push_back((BYTE)(v >> 16));
            push_back((BYTE)(v >> 24));
            return *this;
        }
        void ApplyToBuff(void* buff) {
            unsigned int addr = (unsigned int)buff;
            for (unsigned int ofst = 0; ofst < this->size(); ofst++)
                (*(BYTE*)(addr + ofst)) = this->at(ofst);
        }
};

// ------------------------------------

// http://ref.x86asm.net/coder32.html
// http://www.mathemainzel.info/files/x86asmref.html#call

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

inline Bytes ASM_PUSH(BYTE reg) { return Bytes(0x50 + reg); }
inline Bytes ASM_PUSHL(DWORD literal) { Bytes inst(0x68); return inst.push_DWORD(literal); }
inline Bytes ASM_POP(BYTE reg) { return Bytes(0x58 + reg); }
inline Bytes ASM_MOVL(BYTE reg, DWORD literal) { Bytes inst(0xB8 + reg); return inst.push_DWORD(literal); }
inline Bytes ASM_RET() { return Bytes(0xC3); }


// Note sure I get it
// better explaination?? 
BYTE notsure(BYTE reg) {
    /* inspiration:
    03A30A09 8B 4C 24 08          mov         ecx,dword ptr [esp+8]  
    03A30A0D 8B 54 24 0C          mov         edx,dword ptr [esp+0Ch]  
    03A30A11 89 54 24 08          mov         dword ptr [esp+8],edx  
    03A30A15 89 4C 24 0C          mov         dword ptr [esp+0Ch],ecx  
    */
    switch(reg) {
        case ECX: return 0x4C;
        case EDX: return 0x54;
        case ESP: return 0x24;
        default:
            assert(FALSE);
            return 0xCC;
    }
}

inline Bytes ASM_MOV_fromPtr(BYTE dst, BYTE src, BYTE scr_ofst) {
    Bytes inst(0x8B);
    inst += notsure(dst);
    inst += notsure(src);
    inst += scr_ofst;
    return inst;
}

inline Bytes ASM_MOV_toPtr(BYTE dst, BYTE dst_ofst, BYTE src) {
    Bytes inst(0x89);
    inst += notsure(src);
    inst += notsure(dst);
    inst += dst_ofst;
    return inst;
}

// ------------------------------------


Bytes Win32ToDelphiCallCode(TMethodPointer method, int paramCount)
{
    Bytes code;
    code += ASM_POP(EAX);

    int start = 2;
    int last = paramCount -1;
    while (last > start) {
        code += ASM_MOV_fromPtr(ECX, ESP, start*4);
        code += ASM_MOV_fromPtr(EDX, ESP, last*4);
        code += ASM_MOV_toPtr(ESP, last*4, ECX);
        code += ASM_MOV_toPtr(ESP, start*4, EDX);
        start++;
        last--;
    }

    if (paramCount > 0) code += ASM_POP(EDX);
    if (paramCount > 1) code += ASM_POP(ECX);

    /* Place DATA to EAX */
    code += ASM_PUSH(EAX);
    code += ASM_MOVL(EAX, (DWORD)(method.data)); 
    /* Jump to CODE pointer */
    code += ASM_PUSHL((DWORD)(method.code));
    code += ASM_RET();
    return code;
}
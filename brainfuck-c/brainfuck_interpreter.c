#include <stdio.h>

#define STACK_PUSH(A)   (STACK[STACKPTR++] = A)
#define STACK_POP()     (STACK[--STACKPTR])
#define STACK_E()       (STACKPTR == 0)         // Stack empty
#define STACK_F()       (STACKPTR == 512)      // Stack full

/*
    -------------------- BRAINFUCK COMMANDS --------------------
    [ > ]       Moves the pointer to the right
    [ < ]       Moves the pointer to the left
    [ + ]       Increments the memory cell at the pointer
    [ - ]       Decremenets the memory cell at the pointer
    [ . ]       Output the char in the cell pointed to by the pointer
    [ , ]       Input a char and store it in the cell at the pointer
    [ [ ]       Jump past the matching ] if the cell at the pointer is 0
    [ ] ]       Jump back to the matching [ if the cell at the pointer != 0
    -------------------- BRAINFUCK COMMANDS --------------------
*/

static unsigned short STACK[512];
static unsigned int STACKPTR = 0;

struct brainfuck_instruction_t {
    unsigned short oprtor;  // Operator
    unsigned short oprnd;   // Operand
};

static struct brainfuck_instruction_t BRAINFUCK_PROGRAM[4096];

int compile_bf_code(FILE* _file) {
    unsigned short jmp_pc;
    unsigned short pc = 0;
    int i;
    while ((i = getc(_file)) != EOF && pc < 4096) {
        switch (i) {
            case '>': BRAINFUCK_PROGRAM[pc].oprtor = 1; break;
            case '<': BRAINFUCK_PROGRAM[pc].oprtor = 2; break;
            case '+': BRAINFUCK_PROGRAM[pc].oprtor = 3; break;
            case '-': BRAINFUCK_PROGRAM[pc].oprtor = 4; break;
            case '.': BRAINFUCK_PROGRAM[pc].oprtor = 5; break;
            case ',': BRAINFUCK_PROGRAM[pc].oprtor = 6; break;
            case '[':
                BRAINFUCK_PROGRAM[pc].oprtor = 7;
                if (STACK_F()) { return 1; }
                STACK_PUSH(pc);
                break;
            case ']':
                if (STACK_E()) { return 1; }
                jmp_pc = STACK_POP();
                BRAINFUCK_PROGRAM[pc].oprtor =  8;
                BRAINFUCK_PROGRAM[pc].oprnd = jmp_pc;
                BRAINFUCK_PROGRAM[jmp_pc].oprnd = pc;
                break;
            default: pc--; break;
        }
        pc++;
    }
    if (!STACK_E() || pc == 4096) { return 1; }
    BRAINFUCK_PROGRAM[pc].oprtor = 0;
    return 0;
}

int exec_bf() {
    unsigned short data[65535], pc = 0;
    unsigned int ptr = 65535;
    while (--ptr) { data[ptr] = 0; }
    while (BRAINFUCK_PROGRAM[pc].oprtor != 0 && ptr < 65535) {
        switch (BRAINFUCK_PROGRAM[pc].oprtor) {
            case 1: ptr++; break;
            case 2: ptr--; break;
            case 3: data[ptr]++; break;
            case 4: data[ptr]--; break;
            case 5: putchar(data[ptr]); break;
            case 6: data[ptr] = (unsigned int)getchar(); break;
            case 7: if(!data[ptr]) { pc = BRAINFUCK_PROGRAM[pc].oprnd; } break;
            case 8: if(data[ptr]) { pc = BRAINFUCK_PROGRAM[pc].oprnd; } break;
            default: return 1;
        }

        pc++;
    }
    return ptr != 65535 ? 0 : 1;
}

int main(int i, const char* args[])
{
    FILE *_file;
    if (i != 2 || (_file = fopen(args[1], "r")) == NULL) {
        printf("---------------------------------------------------------------------------------\n");
        printf("|       In order to use this interpreter, please run the following command:     |\n");
        fprintf(stderr, "|       %s filename.bf   |\n", args[0]);
        printf("---------------------------------------------------------------------------------");
        return 1;
    }
    int status = compile_bf_code(_file);
    fclose(_file);
    if (status == 0) {
        status = exec_bf();
    }
    if (status == 1) {
        fprintf(stderr, "Error!\n");
    }
    return status;
}
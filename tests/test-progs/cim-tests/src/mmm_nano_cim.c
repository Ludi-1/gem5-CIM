#include <errno.h> // Error integer and strerror() function
#include <fcntl.h> // Contains file controls like O_RDWR
#include <inttypes.h>
#include <stdint.h> // uint16_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h> // mmap()
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

// Reserved addresses
#define CIM_ADDR_EXECUTE 0x0 // Execute simulator
#define CIM_ADDR_INSTR_MEM 0x1 // Load instruction into instruction memory
#define CIM_ADDR_OPCODE 0x2 // Address for instruction opcode
#define CIM_ADDR_OP1 0x3 // Operand 1
#define CIM_ADDR_OP2 0x4 // Operand 2
#define CIM_ADDR_OP3 0x5 // etc..
#define CIM_ADDR_OP4 0x6
#define CIM_ADDR_OP5 0x7
#define CIM_ADDR_OP6 0x8
#define CIM_ADDR_OP7 0x9
#define CIM_ADDR_OP8 0xA
#define CIM_ADDR_OP9 0xB
#define CIM_ADDR_WD_MEM 0xC
#define CIM_ADDR_RD_MEM CIM_ADDR_WD_MEM + 0x200
#define CIM_ADDR_OUTPUT_MEM CIM_ADDR_RD_MEM + 0x200

// Opcodes for macro-instructions
#define CIM_OP_STORE 0
#define CIM_OP_MMM 1

int main(void)
{
    //open file
    int fd = open("/dev/cim", O_RDWR);
    printf("/dev/cim open code: %i\n", fd);

    if (fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }

    char* cimPtr = mmap(NULL, sizeof(uint16_t),
        PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);

    // select opcode
    uint16_t* opcodePtr = (uint16_t*) (cimPtr + CIM_ADDR_OPCODE);
    uint16_t* op1Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP1); // op1
    uint16_t* op2Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP2); // op2
    uint16_t* op3Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP3); // op3
    uint16_t* op4Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP4); // op4
    uint16_t* op5Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP5); // op5
    uint16_t* op6Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP6); // op6
    uint16_t* op7Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP7); // op7
    uint16_t* op8Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP8); // op8
    uint16_t* op9Ptr = (uint16_t*) (cimPtr + CIM_ADDR_OP9); // op9

    // load to instr mem
    uint16_t* instrPtr = (uint16_t*) (cimPtr + CIM_ADDR_INSTR_MEM);

    // pointer to start wd mem
    uint16_t* wdPtr = (uint16_t*) (cimPtr + CIM_ADDR_WD_MEM);

    // pointer to start rd mem
    uint16_t* rdPtr = (uint16_t*) (cimPtr + CIM_ADDR_RD_MEM);

    // pointer to start output mem
    uint16_t* outputPtr = (uint16_t*) (cimPtr + CIM_ADDR_OUTPUT_MEM);

    // execute program
    uint16_t* exePtr = (uint16_t*) (cimPtr + CIM_ADDR_EXECUTE);

    FILE *fp_rd;
    FILE *fp_wd;
    fp_rd = fopen("compiler-output/RDfile2.txt", "r");
    fp_wd = fopen("compiler-output/WDfile.txt", "r");
    if (fp_rd == NULL){
        printf("error opening rd file\n");
        exit(1);
    }
    if (fp_wd == NULL){
        printf("error opening wd file\n");
        exit(1);
    }

    uint16_t test_num;
    while (1) {
        if (fscanf(fp_rd, "%hu", &test_num) == EOF){
            break;
        } else {
            printf("test num: %hu\n", test_num);
            *rdPtr = test_num;
        }
    }
    fclose(fp_rd);
    fclose(fp_wd);
    return 0;
}
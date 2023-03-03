#include "stdio.h"
#include "stdio.h"

#define INST_MASK 0xFC   // 6 bits
#define MOV_ENC 0x88     // 6 bits
#define MOV_DMASK 0x2    // 1 bit
#define MOV_WMASK 0x1    // 1 bit
#define MOV_MODMASK 0xC0 // 2 bits
#define MOV_REGMASK 0x38 // 3 bits
#define MOV_RMMASK 0x7   // 3 bits

char namesByte[8][3] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
char namesWord[8][3] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

int main()
{
    FILE *asmfile = fopen("simple", "rb");
    char buffer[2];
    if (asmfile)
    {
        FILE *decodedFile = fopen("simple_dissassembly.asm", "w");
        fputs("bits 16", decodedFile);
        while (!feof(asmfile))
        {
            buffer[0] = fgetc(asmfile);
            buffer[1] = fgetc(asmfile);

            if (buffer[0] == -1 || buffer[1] == -1)
            {
                break;
            }

            fputs("\n", decodedFile);

            // decode mov
            char ismov = (buffer[0] & INST_MASK) == MOV_ENC;
            if (ismov)
            {
                fputs("mov ", decodedFile);
                char movd = buffer[0] & MOV_DMASK;
                char movw = buffer[0] & MOV_WMASK;
                char mod = buffer[1] & MOV_MODMASK;
                char reg = (buffer[1] & MOV_REGMASK) >> 3;
                char rm = buffer[1] & MOV_RMMASK;
                if (!movd)
                {
                    for (int i = 0; i < 8; ++i)
                    {
                        if (rm == i)
                        {
                            fputs(movw ? &namesByte[i][0] : &namesWord[i][0], decodedFile);
                            fputs(", ", decodedFile);
                            for (int j = 0; j < 8; ++j)
                            {
                                if (reg == j)
                                {
                                    fputs(movw ? &namesByte[j][0] : &namesWord[j][0], decodedFile);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        fclose(decodedFile);
    }
    fclose(asmfile);
    return 0;
}
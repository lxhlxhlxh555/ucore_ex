#include <iostream>
#define MEM_SIZE 32*1024

using namespace std;
unsigned char mem[MEM_SIZE];

void printPage(unsigned int pageHead) {
    for (int i = 0; i < 32; ++i)
    {
        printf("%02x ", mem[pageHead+i]);
    }
    printf("\n");
}
int main(int argc, char** argv)
{
    // init Memory
    FILE *fin = fopen("mem.txt","r");
    for (int i = 0; i < MEM_SIZE; ++i)
    {
        unsigned int ch;
        fscanf(fin,"%x",&ch);
        mem[i] = (unsigned char)ch;
    }
    fclose(fin);

    unsigned int ptdr = 0x220;
    unsigned int vaddr;
    unsigned int pde_index,pte_index,offset;
    unsigned int pde_valid,pte_valid;
    unsigned int pde_addr,pte_addr,paddr;
    unsigned int pde,pte;
    unsigned char data;

    printf("input address:");
    scanf("%x",&vaddr);

    printf("Virtual Address 0x%04x\n", vaddr);

    pde_index = (vaddr & (0x1f << 10)) >> 10;
    pte_index = (vaddr & (0x1f << 5)) >> 5;
    offset = (vaddr & 0x1f);

    pde = mem[ptdr+pde_index];
    pde_valid = (pde&0x80) >> 7;
    pde_addr = (pde&0x7f);
    printf("  pde index:0x%02x ",pde_index);
    printf("pde contents:(valid %d, pfn 0x%02x)\n",pde_valid,pde_addr);
    if (!pde_valid) {
        printf("    Fault (page directory entry not valid)\n");
        return -1;
    }

    pte = mem[pde_addr*32+pte_index];
    pte_valid = (pte&0x80) >> 7;
    pte_addr = (pte&0x7f);
    printf("    pte index:0x%02x ",pte_index);
    printf("pte contents:(valid %d, pfn 0x%02x)\n",pte_valid,pte_addr);
    if (!pte_valid) {
        printf("      Fault (page table entry not valid)\n");
        return -1;
    }

    paddr = pte_addr*32+offset;
    data = mem[paddr];
    printf("      Translates to Physical Address 0x%04x --> Value: 0x%02x\n",paddr,data);
    return 0;
}
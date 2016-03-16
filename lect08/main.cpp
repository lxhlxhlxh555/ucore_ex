#include <cstdio>

#define PDX(x) ((x >> 10) & 0x1f)
#define PTX(x) ((x >> 5) & 0x1f)
#define OFF(x) (x & 0x1f)
#define PPN2PA(x) (x<<5)

#define ENTRY_VALID(x) ((x >> 7) & 0x1)
#define ENTRY_CONTENT(x) (x & 0x7f)

unsigned int pdbr = 0xd80;
unsigned char mem[4096];
unsigned char disk[4096];

int main(int argc, char** argv) {
    unsigned int vaddr;
    unsigned int pde,pde_index,pde_valid,pde_content;
    unsigned int pte,pte_index,pte_valid,pte_content;
    unsigned char data;

    // init memory
    FILE* file;
    file = fopen("mem","rb");
    fread(mem,4096,1,file);
    fclose(file);
    file = fopen("disk","rb");
    fread(disk,4096,1,file);
    fclose(file);

    // solve
    while(1) {
        printf("please input virtuall address\n");

        // virtual address
        if (scanf("%x",&vaddr) == 0) {
            break;
        }
        printf("Virtual Address 0x%x\n", vaddr);

        // pde
        pde_index = PDX(vaddr);
        pde = mem[pdbr+pde_index];
        pde_valid = ENTRY_VALID(pde);
        pde_content = ENTRY_CONTENT(pde);
        printf("pde index:0x%x  pde contents:(valid %d, pfn 0x%x)\n"
                ,pde_index , pde_valid, pde_content);
        if (!pde_valid) {
            printf("Fault (page directory entry not valid)\n");
            continue;
        }

        // pte
        pte_index = PTX(vaddr);
        pte = mem[PPN2PA(pde_content)+pte_index];
        pte_valid = ENTRY_VALID(pte);
        pte_content = ENTRY_CONTENT(pte);
        printf("pte index:0x%x  pde contents:(valid %d, pfn 0x%x)\n"
            ,pte_index , pte_valid, pte_content);

        // data
        if (pte_valid) { // on memory
            data = mem[PPN2PA(pte_index)+OFF(vaddr)];
            printf("On Memory, Value: 0x%x\n",data);
        } else { // on disk
            data = disk[PPN2PA(pte_index)+OFF(vaddr)];
            printf("On Disk, Value: 0x%x\n",data);
        }
    }

    printf("bye:)\n");
    return 0;
}

//Osama Kergaye
//UID: U0767339

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <elf.h>  
#include "decode.h"

/* Given the in-memory ELF header pointer as `ehdr` and a section
   header pointer as `shdr`, returns a pointer to the memory that
   contains the in-memory content of the section */
#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)

/*************************************************************/
//Holy cow I have spent a full 2 hours just reading the assignment and I still 
// have no idea what I am supposed to do...

//do a thing
Elf64_Shdr* section_by_name(Elf64_Ehdr *ehdr, char* sname){
  int index;
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  char* strs = (AT_SEC(ehdr, shdrs + ehdr->e_shstrndx));
  int i;
  for(i = 0; i < ehdr->e_shnum; i++)
  {
    if(strcmp((strs + shdrs[i].sh_name),sname)==0){
       index = i;
    }
  }

    return &shdrs[index];
}
 


//state of 0 means closed. state of 1 means open
void check(int call, int* state, code_t* code_ptr, instruction_t* ins){
    
    switch (call)
    {
      case 1://open call
        
        //a call to open it after a call to open_it (meaning state will be set to 1)
        if (*state == 1) {
          replace_with_crash(code_ptr, ins);
        }else{
          *state = 1;
        }
        
        break;
     case 2://close call
     //a call to close_it after a call to close_it
     // with no call to open_it in between; cant start a call with close it
        if (*state == 0) {
          replace_with_crash(code_ptr, ins);
        }else{
          *state = 0;
        }
        break;
      case 3://ret call
     //a return after a call to open_it with no close_it in between;
        if (*state == 1) {
          replace_with_crash(code_ptr, ins);
        }
        
        break;
      case 4://proteced var acces attempt
      //0 state means we are closed
      if (*state == 0) {
        replace_with_crash(code_ptr, ins);
      }
        
        break;
      default:
        break;
    }


}

// void reccor(int* state, code_t* code_ptr, instruction_t ins){



//       decode(&blank, code, address);
//       decode(&blank, code, address);
//       check(int call, int* state, code_t* code_ptr, instruction_t ins){



// }
void oldWhileLoop(instruction_t* blank, code_t* code, Elf64_Addr address, Elf64_Sym *syms, Elf64_Ehdr *ehdr, int i, int state){
  printf("I IZ HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

  Elf64_Rela* relas;
  char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));
  Elf64_Shdr* rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  Elf64_Shdr* rela_plt_shdr = section_by_name(ehdr, ".rela.plt");
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;

  int insLength = 0; 
  int x = 0;  
  //state = 0;
  int j = syms[i].st_shndx;
  int flag = 1;



while(blank->op != RET_OP){
      decode(blank, code, address);
      insLength = blank->length;
      //when the opp from ins is a ret opp
      printf("%d: 0x%x %x\n",x , (unsigned int)address, (unsigned int)code[0]);
      printf("op: %d len: %d adr: 0x%x\n",blank->op , blank->length, (unsigned int)blank->addr);

      
      switch (blank->op)
      {
        case   MOV_ADDR_TO_REG_OP:
            relas = AT_SEC(ehdr, rela_dyn_shdr);

          int k2 = 0;
           
            while(blank->addr != relas[k2].r_offset){ //segfault
              k2++;
            }

            int m2 = ELF64_R_SYM(relas[k2].r_info);
            char* name2 = syms[m2].st_name+strs;
            char* protec = "protected";
            //printf("MEEEEEEEEEEEEEEEEEEEEEEEE%s\n", name2);
            
            if (strncmp(name2,protec,8)==0) {
              //printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrr%s\n", name2);
              check(4, &state, code, blank);
            }
            


        break;      
        case   JMP_TO_ADDR_OP://does nothing...can pass without!
        break;        
        case   MAYBE_JMP_TO_ADDR_OP:
        //Some kind of rec
        
        //call rec func. Decode twice and call fun again.
        //reccur(state, blank, address);
        j = syms[i].st_shndx;
        Elf64_Shdr* text_header = section_by_name(ehdr, ".text");
        // code_t*  new_code= AT_SEC(ehdr, text_header) 
        //     + (blank->addr - text_header->sh_addr);

       instruction_t* blankThree = malloc(sizeof(instruction_t));
        code_t*  new_code= AT_SEC(ehdr, shdrs + j) 
            + (blank->addr - shdrs[j].sh_addr);


        oldWhileLoop(blankThree, new_code, blank->addr, syms, ehdr,i, state);
        ////  oldWhileLoop(blankThree, code+blank->length, address+blank->length,syms,ehdr,i);

        oldWhileLoop(blank, code+insLength, address+insLength, syms, ehdr,i, state);

            
        break;        
        case   CALL_OP:
            relas = AT_SEC(ehdr, rela_plt_shdr);
            code_t* inRelasCode = AT_SEC(ehdr,rela_plt_shdr)
            +(blank->addr-rela_plt_shdr->sh_addr);
            instruction_t* blankTwo = malloc(sizeof(instruction_t));;

            decode(blankTwo,inRelasCode, blank->addr);
            int k = 0;
           
            while(blankTwo->addr != relas[k].r_offset){ //segfault
              k++;
            }

            int m = ELF64_R_SYM(relas[k].r_info);
            char* name = syms[m].st_name+strs;
            char* openName = "open_it";
            char* closeName = "close_it";

            // printf("MYNAMEIS %s\n",name);
            if(strcmp(openName,name)==0){
             /*
            it turns out that doing this results in the code and ins of where
            the jump is gets turned into a crash. IE not the line that called
            the code that was suppoed to be used just to check if its a open or close.sizeof
            the spot that holds an open or closed got swaped into a beak.
             */
             check(1, &state, code, blank);
            }else if(strcmp(closeName,name)==0) {
             check(2, &state, code, blank);
            }
    
            
            
        //track opens and closes to ensure order is correct.

        break;        
        case  RET_OP:
        //clear open close stuff. but also check that we are closed/whatver 
        check(3, &state, code, blank); 
        //state = 0;
        break;
        default://this will be for all other OP's
          
        break;
    }



      //printf("InsLength: %d\n", insLength);
      code += insLength;
      address += insLength;
      x++;

    }//after while loop
    if(flag){

    }



}
void enforce(Elf64_Ehdr *ehdr) {
  /* Your work starts here --- but add helper functions that you call
     from here, instead of trying to put all the work in one
     function. */

    Elf64_Shdr* dynsym_shdr = section_by_name(ehdr, ".dynsym");
    Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
    Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
    Elf64_Rela* relas;

    char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));
    int i, count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
  //fucntion code 
 

  for (i = 0; i < count; i++) {

    printf("%s\n", strs + syms[i].st_name);
    int j = syms[i].st_shndx;
    code_t* code = AT_SEC(ehdr,shdrs + j) 
    + (syms[i].st_value - shdrs[j].sh_addr);

    //start point to print out each line of code
    instruction_t* blank = malloc(sizeof(instruction_t));
    Elf64_Addr address = syms[i].st_value;
    

    if(syms[i].st_size == 0 || ELF64_ST_TYPE(syms[i].st_info) != STT_FUNC){
      continue;
    }

    //while opp code does not equal ret

    // blank->op = 1;// this is because I cant clear blank.op
    // blank->addr = 0;// this is because I cant clear blank.op
    // blank->length = 0; 
    // decode(blank, code, aqddress);
    //0 means closed. So start fun calls with closed
    int state = 0;
    oldWhileLoop(blank, code, address, syms, ehdr,i, state);
    printf("-------------------------------\n");




  }
}


















/*************************************************************/

static void fail(char *reason, int err_code) {
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}

static void check_for_shared_object(Elf64_Ehdr *ehdr) {
  if ((ehdr->e_ident[EI_MAG0] != ELFMAG0)
      || (ehdr->e_ident[EI_MAG1] != ELFMAG1)
      || (ehdr->e_ident[EI_MAG2] != ELFMAG2)
      || (ehdr->e_ident[EI_MAG3] != ELFMAG3))
    fail("not an ELF file", 0);

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    fail("not a 64-bit ELF file", 0);
  
  if (ehdr->e_type != ET_DYN)
    fail("not a shared-object file", 0);
}

int main(int argc, char **argv) {
  int fd_in, fd_out;
  size_t len;
  void *p_in, *p_out;
  Elf64_Ehdr *ehdr;

  if (argc != 3)
    fail("expected two file names on the command line", 0);

  /* Open the shared-library input file */
  fd_in = open(argv[1], O_RDONLY);
  if (fd_in == -1)
    fail("could not open input file", errno);

  /* Find out how big the input file is: */
  len = lseek(fd_in, 0, SEEK_END);

  /* Map the input file into memory: */
  p_in = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd_in, 0);
  if (p_in == (void*)-1)
    fail("mmap input failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p_in;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Open the shared-library output file and set its size: */
  fd_out = open(argv[2], O_RDWR | O_CREAT, 0777);
  if (fd_out == -1)
    fail("could not open output file", errno);
  if (ftruncate(fd_out, len))
    fail("could not set output file file", errno);

  /* Map the output file into memory: */
  p_out = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);
  if (p_out == (void*)-1)
    fail("mmap output failed", errno);

  /* Copy input file to output file: */
  memcpy(p_out, p_in, len);

  /* Close input */
  if (munmap(p_in, len))
    fail("munmap input failed", errno);    
  if (close(fd_in))
    fail("close input failed", errno);

  ehdr = (Elf64_Ehdr *)p_out;

  enforce(ehdr);

  if (munmap(p_out, len))
    fail("munmap input failed", errno);    
  if (close(fd_out))
    fail("close input failed", errno);

  return 0;
}

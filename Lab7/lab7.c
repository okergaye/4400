#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <elf.h>

/* Given the in-memory ELF header pointer as `ehdr` and a section
   header pointer as `shdr`, returns a pointer to the memory that
   contains the in-memory content of the section */
#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)

static void check_for_shared_object(Elf64_Ehdr *ehdr);
static void fail(char *reason, int err_code);
void print_symbols(Elf64_Ehdr* ehdr);

int main(int argc, char **argv) 
{
  int fd;
  size_t len;
  void *p;
  Elf64_Ehdr *ehdr;

  if (argc != 2)
    fail("expected one file on the command line", 0);

  /* Open the shared-library file */
  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    fail("could not open file", errno);
  
  /* Find out how big the file is: */
  len = lseek(fd, 0, SEEK_END);

  /* Map the whole file into memory: */
  p = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
  if (p == (void*)-1)
    fail("mmap failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Your work for parts 1-6 goes here */
  printf("%d\n%d\n", ehdr->e_shoff, ehdr->e_shstrndx);

  //prt 2
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  char* strs = (AT_SEC(ehdr, shdrs + ehdr->e_shstrndx)) ;
  
  printf("%s\n", strs + 1);
  //prt 3
  int i;
  for(i = 0; i < 25; i++)
  {

   // printf("%d %s\n", i, strs + shdrs[i].sh_name);

    if(strcmp((strs + shdrs[i].sh_name),".data")==0){
      printf("%d\n",i);

    }

  }
  //prt 4

  Elf64_Sym *syms;
 // printf("%s\n", (strs + shdrs[3].sh_name));

  for(i = 0; i < 25; i++)
  {
    if(strcmp((strs + shdrs[i].sh_name),".dynsym")==0){
     // printf("%i\n", i);
      syms = AT_SEC(ehdr, shdrs + i);
      //syms = ehdr + shdrs->sh_addr + i;
    }

  }
      //int j = syms[7].st_shndx;
      //unsigned char code = AT_SEC(ehdr,shdrs + j) 
    //  + (syms[7].st_value - shdrs[j].sh_addr);
      printf("0x%x\n",syms[7].st_value);
      //printf("%x\n", code);

     // printf("%s\n", code);


//PART 5 MOTHA FUCKA

  int j = syms[7].st_shndx;
  unsigned char* code = AT_SEC(ehdr,shdrs + j) 
     + (syms[7].st_value - shdrs[j].sh_addr);
  printf("%x\n", code[0]);




  return 0;
}

/* Just do a little bit of error-checking:
   Make sure we're dealing with an ELF file. */
static void check_for_shared_object(Elf64_Ehdr *ehdr) 
{
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

static void fail(char *reason, int err_code) 
{
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}

#include "woody.h"

static int sanitize_hdr(const Elf64_Ehdr *file, const int filesize) {
  return (OOPS_BAD_ELF *
          (filesize < sizeof(Elf64_Ehdr) ||
           file->e_ehsize != sizeof(Elf64_Ehdr) || file->e_type != ET_EXEC ||
           sizeof(Elf64_Phdr) != file->e_phentsize ||
           (file->e_phoff + (file->e_phnum * file->e_phentsize)) > filesize ||
           file->e_shnum <= file->e_shstrndx));
}

static int sanitize_phdr(const Elf64_Ehdr *file, const Elf64_Phdr *phdr,
                         const int filesize, int i) {
  if (i == file->e_phnum) return OOPS_NO_LOAD;
  return (OOPS_BAD_PHDR * (filesize < phdr[i].p_offset + phdr[i].p_filesz ||
                           file->e_entry < phdr[i].p_vaddr ||
                           file->e_entry > phdr[i].p_vaddr + phdr[i].p_memsz));
}

static int sanitize_shdr(const Elf64_Ehdr *file, const Elf64_Shdr *shdr,
                         const int filesize, int j) {
  if (j == file->e_shnum) return OOPS_NO_TEXT;
  return (OOPS_BAD_SHDR * 0);
}

static unsigned int find_codecave(const Elf64_Ehdr *file, Elf64_Off off,
                                  Elf64_Xword size) {
  unsigned int current_cave = 0, biggest_cave = 0;
  Elf64_Off cave_offset;

  printf("offset: %lu, size: %lu, ", off, size);
  while (size) {
    if (((char *)file)[off + size] == '\0') {
      current_cave++;
      biggest_cave = ((current_cave > biggest_cave) * current_cave) +
                     ((current_cave <= biggest_cave) * biggest_cave);
    }
    else {
      if (current_cave == biggest_cave) cave_offset = off + size;
      current_cave = 0;
    }
    size--;
  }
  printf("cave_offset: %lu\n", cave_offset);
  return biggest_cave;
}

unsigned int injection_x64(const Elf64_Ehdr *file, const int filesize) {
  Elf64_Phdr *phdr;
  Elf64_Shdr *shdr;
  char *name;
  int i, j, ret;

  if ((ret = sanitize_hdr(file, filesize))) return ret;

  phdr = (Elf64_Phdr *)((char *)file + file->e_phoff);
  for (i = 0; i < file->e_phnum; i++)
    if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) break;
  if ((ret = sanitize_phdr(file, phdr, filesize, i))) return ret;

  shdr = (Elf64_Shdr *)((char *)file + file->e_shoff);
  name = (char *)((char *)file + shdr[file->e_shstrndx].sh_offset);
  for (j = 0; j < file->e_shnum; j++)
    if (shdr[j].sh_type == SHT_PROGBITS && shdr[j].sh_flags & SHF_EXECINSTR &&
        shdr[j].sh_flags & SHF_ALLOC &&
        !ft_strncmp(".text\0", name + shdr[j].sh_name, 6))
      break;
  if ((ret = sanitize_shdr(file, shdr, filesize, j))) return ret;

  ret = find_codecave(file, phdr[i].p_offset, phdr[i].p_filesz);
  ret = OOPS_NOCAVE * (ret < INJECTSIZE) + ret * (ret >= INJECTSIZE);

  printf("codecave found. Size: %d\n", ret);
  return ret;
}

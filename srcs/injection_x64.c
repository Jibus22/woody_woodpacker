#include "woody.h"

static int sanitize_hdr(const Elf64_Ehdr *file, const int filesize) {
  return (OOPS_BAD_ELF *
          (filesize < sizeof(Elf64_Ehdr) ||
           file->e_ehsize != sizeof(Elf64_Ehdr) ||
           (file->e_type != ET_EXEC && file->e_type != ET_DYN) ||
           sizeof(Elf64_Phdr) != file->e_phentsize ||
           (file->e_phoff + (file->e_phnum * file->e_phentsize)) > filesize ||
           file->e_shnum <= file->e_shstrndx));
}

static int sanitize_phdr(const Elf64_Ehdr *file, const Elf64_Phdr *phdr,
                         const int filesize, int i) {
  if (i == file->e_phnum) return OOPS_NO_LOAD;
  if (phdr[i].p_type != PT_LOAD) return OOPS_NOCAVE;
  return (OOPS_BAD_PHDR * (filesize < phdr[i].p_offset + phdr[i].p_filesz /*||
                           file->e_entry < phdr[i].p_vaddr ||
                           file->e_entry > phdr[i].p_vaddr + phdr[i].p_memsz*/));
}

static int sanitize_shdr(const Elf64_Ehdr *file, const Elf64_Shdr *shdr,
                         const int filesize, int j) {
  if (j == file->e_shnum) return OOPS_NO_TEXT;
  return (OOPS_BAD_SHDR * 0);
}

static unsigned int get_text_data(const Elf64_Ehdr *file, const int filesize) {
  Elf64_Shdr *shdr;
  char *name;
  int j, ret;

  shdr = (Elf64_Shdr *)((char *)file + file->e_shoff);
  name = (char *)((char *)file + shdr[file->e_shstrndx].sh_offset);
  for (j = 0; j < file->e_shnum; j++)
    if (shdr[j].sh_type == SHT_PROGBITS && shdr[j].sh_flags & SHF_EXECINSTR &&
        shdr[j].sh_flags & SHF_ALLOC &&
        !ft_strncmp(".text\0", name + shdr[j].sh_name, 6))
      break;
  if ((ret = sanitize_shdr(file, shdr, filesize, j))) return ret;
  return ret;
}

unsigned int injection_x64(Elf64_Ehdr *file, const int filesize) {
  char code[] = PAYLOAD;
  Elf64_Phdr *phdr;
  Elf64_Off old_entry, new_entry;
  int i, ret;

  if ((ret = sanitize_hdr(file, filesize))) return ret;

  phdr = (Elf64_Phdr *)((char *)file + file->e_phoff);
  for (i = 0; i < file->e_phnum; i++)
    if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) break;
  if ((ret = sanitize_phdr(file, phdr, filesize, i))) return ret;
  if ((ret = sanitize_phdr(file, phdr, filesize, i + 1))) return ret;
  ret = phdr[i + 1].p_vaddr - (phdr[i].p_vaddr + phdr[i].p_memsz);

  printf("cave: %d - size of payload: %lu\n", ret, PAYLOAD_SIZE);

  old_entry = file->e_entry;
  file->e_entry = phdr[i].p_vaddr + phdr[i].p_filesz;
  new_entry = phdr[i].p_offset + phdr[i].p_filesz;

  ft_memcpy((void *)file + new_entry, code, PAYLOAD_SIZE);
  /* patch code */
  ft_memcpy((void *)file + new_entry + (PAYLOAD_SIZE - sizeof(Elf64_Off)),
            &old_entry, sizeof(Elf64_Off));

  phdr[i].p_filesz += PAYLOAD_SIZE;
  phdr[i].p_memsz += PAYLOAD_SIZE;

  ret = OOPS_NOCAVE * (ret < PAYLOAD_SIZE) + ret * (ret >= PAYLOAD_SIZE);

  return ret;
}

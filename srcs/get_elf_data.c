#include "woody.h"

/* LOAD SEGMENT */

static int sanitize_exec_load_segment(const Elf64_Ehdr *file, Elf64_Phdr *phdr,
                                      const int filesize, int i) {
  if (i == file->e_phnum) return OOPS_NO_LOAD;
  if (phdr[i].p_type != PT_LOAD) return OOPS_NOCAVE;
  return (OOPS_BAD_PHDR * (filesize < phdr[i].p_offset + phdr[i].p_filesz ||
                           file->e_entry < phdr[i].p_vaddr ||
                           file->e_entry > phdr[i].p_vaddr + phdr[i].p_memsz));
}

static int sanitize_scnd_load_segment(const Elf64_Ehdr *file,
                                      const Elf64_Phdr *phdr,
                                      const int filesize, int i) {
  if (i == file->e_phnum) return OOPS_NO_LOAD;
  if (phdr[i].p_type != PT_LOAD) return OOPS_NOCAVE;
  return (OOPS_BAD_PHDR * (filesize < phdr[i].p_offset + phdr[i].p_filesz));
}

unsigned int get_load_segment(const Elf64_Ehdr *file, const int filesize,
                              t_woody *woody) {
  Elf64_Phdr *phdr;
  int i, j, ret;

  phdr = (Elf64_Phdr *)((char *)file + file->e_phoff);
  for (i = 0; i < file->e_phnum; i++)
    if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) break;
  if ((ret = sanitize_exec_load_segment(file, phdr, filesize, i))) return ret;

  for (j = i + 1; j < file->e_phnum; j++)
    if (phdr[j].p_type == PT_LOAD) break;
  if ((ret = sanitize_scnd_load_segment(file, phdr, filesize, j))) return ret;

  ret = phdr[j].p_offset - (phdr[i].p_offset + phdr[i].p_filesz);
  if (ret < PAYLOAD_SIZE) return OOPS_NOCAVE;

  woody->load_seg = &phdr[i];

  printf("cave: %d - size of payload: %lu - size of patch: %lu\n", ret,
         PAYLOAD_SIZE, sizeof(t_patch));

  return EXIT_SUCCESS;
}

/* TEXT SECTION */

static int sanitize_shdr(const Elf64_Ehdr *file, const Elf64_Shdr *shdr,
                         const int filesize, int j) {
  if (j == file->e_shnum) return OOPS_NO_TEXT;
  return (OOPS_BAD_SHDR * 0);
}

unsigned int get_text_section(const Elf64_Ehdr *file, const int filesize,
                              t_woody *woody) {
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
  woody->text_sec = &shdr[j];
  return ret;
}

#include "woody.h"

/* LOAD SEGMENT */

static int sanitize_exec_load_segment(const Elf64_Ehdr *file, Elf64_Phdr *phdr,
                                      const unsigned long filesize, int i) {
  if (i == file->e_phnum) return OOPS_NO_LOAD;
  if (phdr[i].p_type != PT_LOAD) return OOPS_NOCAVE;
  return (OOPS_BAD_PHDR *
          (filesize < phdr[i].p_offset || filesize < phdr[i].p_filesz ||
           filesize < phdr[i].p_offset + phdr[i].p_filesz ||
           !phdr[i].p_filesz || file->e_entry < phdr[i].p_vaddr ||
           file->e_entry > phdr[i].p_vaddr + phdr[i].p_memsz));
}

static int sanitize_scnd_load_segment(const Elf64_Ehdr *file,
                                      const Elf64_Phdr *phdr,
                                      const unsigned long filesize, int j,
                                      int i) {
  if (j == file->e_phnum) return OOPS_NO_LOAD;
  if (phdr[j].p_type != PT_LOAD) return OOPS_NOCAVE;
  return (OOPS_BAD_PHDR * (filesize < phdr[j].p_offset + phdr[j].p_filesz ||
          phdr[i].p_memsz + phdr[i].p_vaddr > phdr[j].p_vaddr));
}

int get_load_segment(const Elf64_Ehdr *file, const int filesize,
                     t_woody64 *woody) {
  Elf64_Phdr *phdr;
  int i, j, ret;

  phdr = (Elf64_Phdr *)((char *)file + file->e_phoff);
  for (i = 0; i < file->e_phnum; i++)
    if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) break;
  if ((ret = sanitize_exec_load_segment(file, phdr, filesize, i))) return ret;

  for (j = i + 1; j < file->e_phnum; j++)
    if (phdr[j].p_type == PT_LOAD) break;
  if ((ret = sanitize_scnd_load_segment(file, phdr, filesize, j, i)))
    return ret;

  ret = phdr[j].p_offset - (phdr[i].p_offset + phdr[i].p_filesz);
  /* print_info(ret); */
  woody->load_seg = &phdr[i];
  if ((unsigned long)ret < PAYLOAD_SIZE) return CREATE_CODECAVE;

  return EXIT_SUCCESS;
}

/* TEXT SECTION */

static int sanitize_shdr(const Elf64_Ehdr *file, const Elf64_Shdr *shdr,
                         unsigned long filesize) {
  return (OOPS_BAD_SHDR * ((shdr[0].sh_size != 0 && shdr[0].sh_offset != 0) ||
                           file->e_shstrndx >= file->e_shnum ||
                           shdr[file->e_shstrndx].sh_type != SHT_STRTAB ||
                           shdr[file->e_shstrndx].sh_offset > filesize ||
                           ((char *)file + shdr[file->e_shstrndx].sh_offset) >
                               (char *)file + filesize));
}

static int sanitize_text_hdr(const Elf64_Ehdr *file, const Elf64_Shdr *shdr,
                             const unsigned long filesize, int i) {
  if (i == file->e_shnum) return OOPS_NO_TEXT;
  return (OOPS_BAD_SHDR *
          (filesize < shdr[i].sh_offset || filesize < shdr[i].sh_size ||
           filesize < shdr[i].sh_size + shdr[i].sh_offset ||
           i == file->e_shnum - 1 || i == 0 ||
           (shdr[i].sh_addr - shdr[i].sh_offset) !=
               (shdr[i + 1].sh_addr - shdr[i + 1].sh_offset) ||
           (shdr[i].sh_addr - shdr[i].sh_offset) !=
               (shdr[i - 1].sh_addr - shdr[i - 1].sh_offset) ||
           shdr[i].sh_type != SHT_PROGBITS || !(shdr[i].sh_flags & SHF_ALLOC) ||
           !(shdr[i].sh_flags & SHF_EXECINSTR) ||
           shdr[i].sh_addr != file->e_entry));
}

unsigned int get_text_section(const Elf64_Ehdr *file,
                              const unsigned long filesize, t_woody64 *woody) {
  Elf64_Shdr *shdr;
  char *name;
  int j, ret;

  shdr = (Elf64_Shdr *)((char *)file + file->e_shoff);
  if ((ret = sanitize_shdr(file, shdr, filesize))) return ret;
  name = (char *)((char *)file + shdr[file->e_shstrndx].sh_offset);
  for (j = 0; j < file->e_shnum; j++) {
    if (shdr[j].sh_name > shdr[file->e_shstrndx].sh_size) return OOPS_BAD_SHDR;
    if (shdr[j].sh_type == SHT_PROGBITS && shdr[j].sh_flags & SHF_EXECINSTR &&
        shdr[j].sh_flags & SHF_ALLOC &&
        name + shdr[j].sh_name < (char *)file + filesize &&
        !ft_strncmp(".text\0", name + shdr[j].sh_name, 6))
      break;
  }
  if ((ret = sanitize_text_hdr(file, shdr, filesize, j))) return ret;
  woody->text_sec = &shdr[j];
  return EXIT_SUCCESS;
}

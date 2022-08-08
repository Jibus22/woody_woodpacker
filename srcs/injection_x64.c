#include "woody.h"

/* static int sanitize_shdr(const Elf64_Ehdr *file, const Elf64_Shdr *shdr, */
/*                          const int filesize, int j) { */
/*   if (j == file->e_shnum) return OOPS_NO_TEXT; */
/*   return (OOPS_BAD_SHDR * 0); */
/* } */

/* static unsigned int get_text_data(const Elf64_Ehdr *file, const int filesize)
 * { */
/*   Elf64_Shdr *shdr; */
/*   char *name; */
/*   int j, ret; */

/*   shdr = (Elf64_Shdr *)((char *)file + file->e_shoff); */
/*   name = (char *)((char *)file + shdr[file->e_shstrndx].sh_offset); */
/*   for (j = 0; j < file->e_shnum; j++) */
/*     if (shdr[j].sh_type == SHT_PROGBITS && shdr[j].sh_flags & SHF_EXECINSTR
 * && */
/*         shdr[j].sh_flags & SHF_ALLOC && */
/*         !ft_strncmp(".text\0", name + shdr[j].sh_name, 6)) */
/*       break; */
/*   if ((ret = sanitize_shdr(file, shdr, filesize, j))) return ret; */
/*   return ret; */
/* } */

static int sanitize_hdr(const Elf64_Ehdr *file, const int filesize) {
  return (OOPS_BAD_ELF *
          (filesize < sizeof(Elf64_Ehdr) ||
           file->e_ehsize != sizeof(Elf64_Ehdr) ||
           (file->e_type != ET_EXEC && file->e_type != ET_DYN) ||
           sizeof(Elf64_Phdr) != file->e_phentsize ||
           (file->e_phoff + (file->e_phnum * file->e_phentsize)) > filesize ||
           file->e_shnum <= file->e_shstrndx));
}

static int sanitize_exec_load_segment(const Elf64_Ehdr *file, Elf64_Phdr *phdr,
                                      const int filesize, int i) {
  if (i == file->e_phnum) return OOPS_NO_LOAD;
  if (phdr[i].p_type != PT_LOAD) return OOPS_NOCAVE;
  phdr[i].p_flags |= PF_W;  // TODO delete this and replace with mprotect in payload
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

static int get_random_key(char *key) {
  int fd, ret;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) return OOPS_OPEN;
  ret = read(fd, key, KEYLEN);
  if (ret == -1) return OOPS_READ;
  close(fd);
  return 0;
}

static int encrypt(Elf64_Ehdr *file, t_patch *patch, Elf64_Addr vaddr) {
  int j;
  unsigned long len = patch->payload_entry - patch->main_entry;
  char *ptr = (char *)file + (patch->main_entry - vaddr);

  if ((j = get_random_key(patch->key))) return j;
  ft_strncpy(patch->key,
             "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789()*",
             64);
  for (int i = 0; i < len; i++) {
    j = (0 * (j == patch->key_size)) + ((j + 1) * (j < patch->key_size));
    /* ptr[i] ^= (patch->key)[j]; */
  }
  return 0;
}

unsigned int injection_x64(Elf64_Ehdr *file, const int filesize) {
  char payload[] = PAYLOAD;
  Elf64_Phdr *phdr;
  Elf64_Off off_newentry;
  t_patch patch;
  int i, j, ret;

  if ((ret = sanitize_hdr(file, filesize))) return ret;

  phdr = (Elf64_Phdr *)((char *)file + file->e_phoff);
  for (i = 0; i < file->e_phnum; i++)
    if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) break;
  if ((ret = sanitize_exec_load_segment(file, phdr, filesize, i))) return ret;

  for (j = i + 1; j < file->e_phnum; j++)
    if (phdr[j].p_type == PT_LOAD) break;
  if ((ret = sanitize_scnd_load_segment(file, phdr, filesize, j))) return ret;
  ret = phdr[j].p_offset - (phdr[i].p_offset + phdr[i].p_filesz);
  if (ret < PAYLOAD_SIZE) return OOPS_NOCAVE;

  printf("cave: %d - size of payload: %lu\n", ret, PAYLOAD_SIZE);

  patch.main_entry = file->e_entry;
  file->e_entry = phdr[i].p_vaddr + phdr[i].p_filesz;
  patch.payload_entry = file->e_entry;
  off_newentry = phdr[i].p_offset + phdr[i].p_filesz;

  patch.key_size = KEYLEN;
  if ((ret = encrypt(file, &patch, phdr[i].p_vaddr))) return ret;

  ft_memcpy((void *)file + off_newentry, payload, PAYLOAD_SIZE);
  ft_memcpy((void *)file + off_newentry + (PAYLOAD_SIZE - sizeof(t_patch)),
            &patch, sizeof(t_patch));

  phdr[i].p_filesz += PAYLOAD_SIZE;
  phdr[i].p_memsz += PAYLOAD_SIZE;

  return EXIT_SUCCESS;
}

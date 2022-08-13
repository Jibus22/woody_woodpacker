#include "woody.h"

static void print_key(const unsigned char *key, unsigned int size) {
  printf("KEY=\n");
  for (unsigned int i = 0; i < size; i++) {
    if (i && i % 8 == 0) printf("\n");
    printf("\\x%x", (int)key[i]);
  }
  printf("\n");
}

static int sanitize_hdr(const Elf64_Ehdr *file, const int filesize) {
  return (OOPS_BAD_ELF *
          (filesize < sizeof(Elf64_Ehdr) ||
           file->e_ehsize != sizeof(Elf64_Ehdr) ||
           (file->e_type != ET_EXEC && file->e_type != ET_DYN) ||
           sizeof(Elf64_Phdr) != file->e_phentsize ||
           (file->e_phoff + (file->e_phnum * file->e_phentsize)) > filesize ||
           file->e_shnum <= file->e_shstrndx));
}

static int get_random_key(unsigned char *key) {
  int fd, ret;

  fd = open("/dev/urandom", O_RDONLY);
  if (fd == -1) return OOPS_OPEN;
  ret = read(fd, key, KEYLEN);
  if (ret == -1) return OOPS_READ;
  close(fd);
  return 0;
}

static int encrypt(char *ptr, unsigned long len, const t_patch *patch) {
  int j = 0;

  for (int i = 0; i < len; i++, j++) {
    j = (0 * (j == patch->key_size)) + (j * (j < patch->key_size));
    ptr[i] ^= (patch->key)[j];
  }
  return 0;
}

static int write_woody(t_woody *woody, const int filesize) {
  int fd = open("woody", O_RDWR | O_TRUNC | O_CREAT, 0777);

  if (fd == -1) return OOPS_OPEN;
  write(fd, woody->file, woody->filesize);
  if (woody->filesize == filesize) munmap(woody->file, woody->filesize);
  else free(woody->file);
  close(fd);
  return EXIT_SUCCESS;
}

t_ret injection_x64(Elf64_Ehdr *file, const int filesize) {
  char payload[] = PAYLOAD;
  Elf64_Off payload_off;
  t_woody woody = {file, filesize};
  t_patch patch;
  int ret;

  if ((ret = sanitize_hdr(file, filesize)))
    return ret_wrap(ret, filesize, file);
  if ((ret = get_load_segment(file, filesize, &woody)))
    return ret_wrap(ret, filesize, file);
  if ((ret = get_text_section(file, filesize, &woody)))
    return ret_wrap(ret, filesize, file);

  // fonction qui set shift + offset, mais du coup woody->file change et
  // unmmap file

  patch.entry_offset =
      woody.load_seg->p_vaddr + woody.load_seg->p_filesz - file->e_entry;
  patch.text_offset = woody.load_seg->p_vaddr + woody.load_seg->p_filesz -
                      woody.text_sec->sh_addr;
  patch.segment_offset = woody.load_seg->p_memsz;
  patch.text_len = woody.text_sec->sh_size;
  patch.key_size = KEYLEN;
  if ((ret = get_random_key(patch.key)))
    return ret_wrap(ret, woody.filesize, woody.file);

  payload_off = woody.load_seg->p_offset + woody.load_seg->p_filesz;

  encrypt((char *)file + woody.text_sec->sh_offset, woody.text_sec->sh_size,
          &patch);

  ft_memcpy((void *)file + payload_off, payload, PAYLOAD_SIZE);
  ft_memcpy((void *)file + payload_off + (PAYLOAD_SIZE - sizeof(t_patch)),
            &patch, sizeof(t_patch));

  file->e_entry = woody.load_seg->p_vaddr + woody.load_seg->p_filesz;
  woody.load_seg->p_filesz += PAYLOAD_SIZE;
  woody.load_seg->p_memsz += PAYLOAD_SIZE;

  print_key(patch.key, patch.key_size);
  if ((ret = write_woody(&woody, filesize)))
    return ret_wrap(ret, woody.filesize, woody.file);
  return ret_wrap(EXIT_SUCCESS, 0, NULL);
}

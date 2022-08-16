#include "woody.h"

static int sanitize_hdr(const Elf64_Ehdr *file, const unsigned long filesize) {
  return (OOPS_BAD_ELF *
          (filesize < sizeof(Elf64_Ehdr) ||
           file->e_ehsize != sizeof(Elf64_Ehdr) ||
           (file->e_type != ET_EXEC && file->e_type != ET_DYN) ||
           sizeof(Elf64_Phdr) != file->e_phentsize ||
           sizeof(Elf64_Shdr) != file->e_shentsize ||
           (file->e_phoff + (file->e_phnum * file->e_phentsize)) > filesize ||
           (file->e_shoff + (file->e_shnum * file->e_shentsize)) > filesize ||
           file->e_shnum <= file->e_shstrndx));
}

static int write_woody(t_woody64 *woody, const unsigned long filesize) {
  int fd = open("woody", O_RDWR | O_TRUNC | O_CREAT, 0777);

  if (fd == -1) return OOPS_OPEN;
  write(fd, woody->file, woody->filesize);
  if (woody->filesize == filesize)
    munmap(woody->file, woody->filesize);
  else
    free(woody->file);
  close(fd);
  return EXIT_SUCCESS;
}

static int init_patch(t_patch *patch, const t_woody64 *woody) {
  int ret = get_random_key(patch->key);

  if (ret) return ret;
  patch->entry_offset = woody->load_seg->p_vaddr + woody->load_seg->p_filesz -
                        woody->file->e_entry;
  patch->text_offset = woody->load_seg->p_vaddr + woody->load_seg->p_filesz -
                       woody->text_sec->sh_addr;
  patch->segment_offset = woody->load_seg->p_memsz;
  patch->text_len = woody->text_sec->sh_size;
  patch->key_size = KEYLEN;
  return EXIT_SUCCESS;
}

static int create_codecave(t_woody64 *woody) {
  unsigned int size = ((PAYLOAD_SIZE / PAGESIZE) + 1) * PAGESIZE,
               offset = woody->load_seg->p_offset + woody->load_seg->p_filesz;
  void *ptr = malloc(woody->filesize + size);
  Elf64_Phdr *phdr = (Elf64_Phdr *)((char *)woody->file + woody->file->e_phoff);
  Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)woody->file + woody->file->e_shoff);
  int j;

  if (!ptr) return OOPS_MALLOC;
  write(STDOUT_FILENO, "Creating codecave...\n", 21);
  for (j = 0; j < woody->file->e_phnum; j++)
    if (phdr[j].p_offset > offset) phdr[j].p_offset += size;
  for (j = 0; j < woody->file->e_shnum; j++)
    if (shdr[j].sh_offset > offset) shdr[j].sh_offset += size;
  if (woody->file->e_shoff > offset) woody->file->e_shoff += size;
  ft_memcpy(ptr, woody->file, offset);
  ft_memset((char *)ptr + offset, 0, size);
  ft_memcpy((char *)ptr + offset + size, (char *)woody->file + offset,
            woody->filesize - offset);
  munmap(woody->file, woody->filesize);
  woody->file = ptr;
  woody->filesize += size;

  return EXIT_SUCCESS;
}

static void inject(t_woody64 *woody, const t_patch *patch) {
  char payload[] = PAYLOAD;
  Elf64_Off payload_off;

  payload_off = woody->load_seg->p_offset + woody->load_seg->p_filesz;
  ft_memcpy((void *)woody->file + payload_off, payload, PAYLOAD_SIZE);
  ft_memcpy(
      (void *)woody->file + payload_off + (PAYLOAD_SIZE - sizeof(t_patch)),
      patch, sizeof(t_patch));

  woody->file->e_entry = woody->load_seg->p_vaddr + woody->load_seg->p_filesz;
  woody->load_seg->p_filesz += PAYLOAD_SIZE;
  woody->load_seg->p_memsz += PAYLOAD_SIZE;
}

t_ret injection_x64(Elf64_Ehdr *file, const int filesize) {
  t_woody64 woody = {file, filesize, NULL, NULL};
  t_patch patch;
  int ret;

  if ((ret = sanitize_hdr(file, filesize)))
    return ret_wrap(ret, filesize, file);

  if ((ret = get_load_segment(file, filesize, &woody)) > 0)
    return ret_wrap(ret, filesize, file);
  if (ret == CREATE_CODECAVE) {
    if ((ret = create_codecave(&woody)))
      return ret_wrap(ret, woody.filesize, woody.file);
    if ((ret = get_load_segment(woody.file, woody.filesize, &woody)))
      return ret_wrap(ret, filesize, file);
  }
  if ((ret = get_text_section(woody.file, woody.filesize, &woody)))
    return ret_wrap(ret, woody.filesize, woody.file);

  if ((ret = init_patch(&patch, &woody)))
    return ret_wrap(ret, woody.filesize, woody.file);

  encrypt((char *)woody.file + woody.text_sec->sh_offset,
          woody.text_sec->sh_size, &patch);
  inject(&woody, &patch);

  print_key(patch.key, patch.key_size);
  if ((ret = write_woody(&woody, filesize)))
    return ret_wrap(ret, woody.filesize, woody.file);
  return ret_wrap(EXIT_SUCCESS, 0, NULL);
}

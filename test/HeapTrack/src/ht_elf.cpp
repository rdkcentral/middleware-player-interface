#include "ht_internal.h"

#include <link.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#if __SIZEOF_POINTER__ == 8
#define HT_ST_TYPE ELF64_ST_TYPE
#else
#define HT_ST_TYPE ELF32_ST_TYPE
#endif

bool ht_elf_open(HtElf *e, const char *path)
{
    memset(e, 0, sizeof(*e));

    int fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0)
        return false;

    struct stat st;
    if (fstat(fd, &st) != 0 || (size_t)st.st_size < sizeof(ElfW(Ehdr)))
    {
        close(fd);
        return false;
    }

    void *map = mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED)
        return false;

    e->map = map;
    e->size = (size_t)st.st_size;

    const ElfW(Ehdr) *eh = (const ElfW(Ehdr) *)map;
    if (memcmp(eh->e_ident, ELFMAG, SELFMAG) != 0 ||
        eh->e_shoff == 0 || eh->e_shoff + (size_t)eh->e_shnum * eh->e_shentsize > e->size)
    {
        ht_elf_close(e);
        return false;
    }

    const char *base = (const char *)map;
    const ElfW(Shdr) *sh = (const ElfW(Shdr) *)(base + eh->e_shoff);

    const ElfW(Shdr) *sym = NULL;
    const ElfW(Shdr) *dynsym = NULL;
    for (int i = 0; i < eh->e_shnum; i++)
    {
        if (sh[i].sh_type == SHT_SYMTAB)
            sym = &sh[i];
        else if (sh[i].sh_type == SHT_DYNSYM)
            dynsym = &sh[i];
    }
    if (!sym)
        sym = dynsym;
    if (!sym || sym->sh_link >= eh->e_shnum || sym->sh_entsize == 0)
    {
        ht_elf_close(e);
        return false;
    }

    const ElfW(Shdr) *str = &sh[sym->sh_link];
    if (sym->sh_offset + sym->sh_size > e->size || str->sh_offset + str->sh_size > e->size)
    {
        ht_elf_close(e);
        return false;
    }

    e->syms = base + sym->sh_offset;
    e->nsyms = (uint32_t)(sym->sh_size / sym->sh_entsize);
    e->symentsz = (uint32_t)sym->sh_entsize;
    e->strs = base + str->sh_offset;
    e->strsz = str->sh_size;
    return true;
}

void ht_elf_close(HtElf *e)
{
    if (e->map)
        munmap(e->map, e->size);
    memset(e, 0, sizeof(*e));
}

bool ht_elf_lookup(const HtElf *e, uintptr_t vaddr, const char **name, uintptr_t *off)
{
    if (!e->syms)
        return false;

    const ElfW(Sym) *best = NULL;
    for (uint32_t i = 0; i < e->nsyms; i++)
    {
        const ElfW(Sym) *s = (const ElfW(Sym) *)((const char *)e->syms + (size_t)i * e->symentsz);
        if (HT_ST_TYPE(s->st_info) != STT_FUNC || s->st_shndx == SHN_UNDEF || s->st_value == 0)
            continue;
        if ((uintptr_t)s->st_value > vaddr)
            continue;
        if (s->st_size && vaddr >= (uintptr_t)s->st_value + s->st_size)
            continue;
        if (!best || s->st_value > best->st_value)
            best = s;
    }
    if (!best || best->st_name >= e->strsz)
        return false;

    *name = e->strs + best->st_name;
    *off = vaddr - (uintptr_t)best->st_value;
    return true;
}

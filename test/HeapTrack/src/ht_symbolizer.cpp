#include "ht_internal.h"

#include <link.h>
#include <cxxabi.h>
#include <stdlib.h>

#define HT_MAX_MODULES 128

struct Module
{
    uintptr_t base;
    uintptr_t lo;
    uintptr_t hi;
    char path[192];
    HtElf elf;
    int elf_tried;
};

static Module g_mods[HT_MAX_MODULES];
static int g_nmods;

static void str_put(char *buf, size_t len, size_t *pos, const char *s)
{
    while (*s && *pos + 1 < len)
        buf[(*pos)++] = *s++;
    buf[*pos] = 0;
}

static void str_hex(char *buf, size_t len, size_t *pos, uintptr_t v)
{
    char tmp[2 + sizeof(uintptr_t) * 2 + 1];
    int i = (int)sizeof(tmp) - 1;
    tmp[i--] = 0;
    if (!v)
        tmp[i--] = '0';
    while (v)
    {
        tmp[i--] = "0123456789abcdef"[v & 0xf];
        v >>= 4;
    }
    tmp[i--] = 'x';
    tmp[i] = '0';
    str_put(buf, len, pos, &tmp[i]);
}

static int phdr_cb(struct dl_phdr_info *info, size_t, void *)
{
    if (g_nmods >= HT_MAX_MODULES)
        return 1;

    uintptr_t lo = (uintptr_t)-1, hi = 0;
    for (int i = 0; i < info->dlpi_phnum; i++)
    {
        const ElfW(Phdr) *ph = &info->dlpi_phdr[i];
        if (ph->p_type != PT_LOAD)
            continue;
        uintptr_t a = (uintptr_t)info->dlpi_addr + ph->p_vaddr;
        if (a < lo)
            lo = a;
        if (a + ph->p_memsz > hi)
            hi = a + ph->p_memsz;
    }
    if (hi == 0)
        return 0;

    Module *m = &g_mods[g_nmods++];
    memset(m, 0, sizeof(*m));
    m->base = (uintptr_t)info->dlpi_addr;
    m->lo = lo;
    m->hi = hi;
    const char *name = (info->dlpi_name && info->dlpi_name[0]) ? info->dlpi_name : "/proc/self/exe";
    size_t n = strlen(name);
    if (n >= sizeof(m->path))
        n = sizeof(m->path) - 1;
    memcpy(m->path, name, n);
    return 0;
}

void ht_sym_begin(void)
{
    g_nmods = 0;
    dl_iterate_phdr(phdr_cb, NULL);
}

void ht_sym_end(void)
{
    for (int i = 0; i < g_nmods; i++)
        ht_elf_close(&g_mods[i].elf);
    g_nmods = 0;
}

static const char *short_name(const char *path)
{
    const char *s = strrchr(path, '/');
    return s ? s + 1 : path;
}

void ht_sym_resolve(uintptr_t pc, char *buf, size_t buflen)
{
    size_t pos = 0;
    buf[0] = 0;

    Module *m = NULL;
    for (int i = 0; i < g_nmods; i++)
    {
        if (pc >= g_mods[i].lo && pc < g_mods[i].hi)
        {
            m = &g_mods[i];
            break;
        }
    }
    if (!m)
    {
        str_put(buf, buflen, &pos, "??");
        str_hex(buf, buflen, &pos, pc);
        return;
    }

    if (!m->elf_tried)
    {
        m->elf_tried = 1;
        ht_elf_open(&m->elf, m->path);
    }

    const char *name = NULL;
    uintptr_t off = 0;
    if (m->elf.syms && ht_elf_lookup(&m->elf, pc - m->base, &name, &off))
    {
        char *dem = NULL;
        if (name[0] == '_' && name[1] == 'Z')
        {
            int status = 0;
            dem = abi::__cxa_demangle(name, NULL, NULL, &status);
        }
        str_put(buf, buflen, &pos, dem ? dem : name);
        str_put(buf, buflen, &pos, "+");
        str_hex(buf, buflen, &pos, off);
        if (dem)
            free(dem);
        return;
    }

    str_put(buf, buflen, &pos, short_name(m->path));
    str_put(buf, buflen, &pos, "+");
    str_hex(buf, buflen, &pos, pc - m->base);
}

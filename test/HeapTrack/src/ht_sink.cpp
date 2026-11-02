#include "ht_internal.h"

#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

static HeaptrackSink g_sink;
static int g_fd = -1;

void ht_sink_set(const HeaptrackSink *sink)
{
    if (sink && sink->write)
        g_sink = *sink;
    else
        memset(&g_sink, 0, sizeof(g_sink));
}

static void path_default(char *buf, size_t len)
{
    const char *prefix = "/tmp/heaptrack_";
    size_t i = 0;
    while (*prefix && i + 1 < len)
        buf[i++] = *prefix++;
    char num[16];
    int n = 0;
    unsigned pid = (unsigned)getpid();
    do
    {
        num[n++] = (char)('0' + pid % 10);
        pid /= 10;
    } while (pid && n < (int)sizeof(num));
    while (n-- && i + 1 < len)
        buf[i++] = num[n];
    const char *suffix = ".log";
    while (*suffix && i + 1 < len)
        buf[i++] = *suffix++;
    buf[i] = 0;
}

void ht_sink_open(void)
{
    if (g_sink.write)
        return;
    char path[256];
    if (g_ht_cfg.sink_path)
    {
        size_t n = strlen(g_ht_cfg.sink_path);
        if (n >= sizeof(path))
            n = sizeof(path) - 1;
        memcpy(path, g_ht_cfg.sink_path, n);
        path[n] = 0;
    }
    else
    {
        path_default(path, sizeof(path));
    }
    g_fd = open(path, O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0644);
}

void ht_sink_close(void)
{
    if (g_fd >= 0)
        close(g_fd);
    g_fd = -1;
}

static void sink_write(const char *data, size_t len)
{
    if (g_sink.write)
    {
        g_sink.write(g_sink.ctx, data, len);
        return;
    }
    if (g_fd >= 0)
    {
        size_t off = 0;
        while (off < len)
        {
            ssize_t n = write(g_fd, data + off, len - off);
            if (n <= 0)
                break;
            off += (size_t)n;
        }
    }
}

void ht_w_flush(HtWriter *w)
{
    if (w->len)
        sink_write(w->buf, w->len);
    w->len = 0;
}

void ht_w_raw(HtWriter *w, const char *s, size_t n)
{
    while (n)
    {
        size_t space = sizeof(w->buf) - w->len;
        if (!space)
        {
            ht_w_flush(w);
            continue;
        }
        size_t chunk = n < space ? n : space;
        memcpy(w->buf + w->len, s, chunk);
        w->len += chunk;
        s += chunk;
        n -= chunk;
    }
}

static void w_u64(HtWriter *w, uint64_t v)
{
    char tmp[24];
    int i = (int)sizeof(tmp);
    if (!v)
        tmp[--i] = '0';
    while (v)
    {
        tmp[--i] = (char)('0' + (v % 10));
        v /= 10;
    }
    ht_w_raw(w, &tmp[i], sizeof(tmp) - (size_t)i);
}

static void w_hex(HtWriter *w, uintptr_t v)
{
    char tmp[2 + sizeof(uintptr_t) * 2];
    int i = (int)sizeof(tmp);
    if (!v)
        tmp[--i] = '0';
    while (v)
    {
        tmp[--i] = "0123456789abcdef"[v & 0xf];
        v >>= 4;
    }
    ht_w_raw(w, "0x", 2);
    ht_w_raw(w, &tmp[i], sizeof(tmp) - (size_t)i);
}

static void w_bytes(HtWriter *w, uint64_t v)
{
    static const char *units[] = {"B", "KB", "MB", "GB"};
    int u = 0;
    uint64_t whole = v, frac = 0;
    while (whole >= 1024 && u < 3)
    {
        frac = (whole % 1024) * 10 / 1024;
        whole /= 1024;
        u++;
    }
    w_u64(w, whole);
    if (u)
    {
        ht_w_raw(w, ".", 1);
        w_u64(w, frac);
    }
    ht_w_raw(w, " ", 1);
    ht_w_raw(w, units[u], strlen(units[u]));
}

void ht_w(HtWriter *w, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    const char *p = fmt;
    while (*p)
    {
        if (*p != '%')
        {
            const char *start = p;
            while (*p && *p != '%')
                p++;
            ht_w_raw(w, start, (size_t)(p - start));
            continue;
        }
        p++;
        switch (*p)
        {
        case 's':
        {
            const char *s = va_arg(ap, const char *);
            if (!s)
                s = "(null)";
            ht_w_raw(w, s, strlen(s));
            break;
        }
        case 'd':
            w_u64(w, (uint64_t)(int64_t)va_arg(ap, int));
            break;
        case 'u':
            w_u64(w, va_arg(ap, uint64_t));
            break;
        case 'x':
            w_hex(w, va_arg(ap, uintptr_t));
            break;
        case 'b':
            w_bytes(w, va_arg(ap, uint64_t));
            break;
        case '%':
            ht_w_raw(w, "%", 1);
            break;
        default:
            break;
        }
        if (*p)
            p++;
    }
    va_end(ap);
}

# Heaptrack — Usage Guide

`libheaptrack.so` is an in-process, scoped heap tracker and leak detector. Instead of
profiling a whole process, it attributes heap allocations to the **API you name**, keeps
tracking them after that API returns, and reports — on the device, in a single run — how
much each API allocated, how much it still holds, what leaked, which pointer variable was
involved and the call tree behind it.

It is built for components that ship **without debug symbols** on **32-bit embedded
targets** (RDK / ARM class), where Valgrind, ASan or host-side post-processing are not an
option.

---

## 1. Who this is for

| Audience | What they get |
|---|---|
| Component developers (player, DRM, demux, …) | "Which of my APIs leaks, and how much?" without instrumenting every allocation |
| Integrators / QA on a device | A single `/tmp/heaptrack_<pid>.log` after a soak run, no host tooling |
| Performance engineers | Per-API peak and outstanding heap, growth over time |

It is **not** a general-purpose profiler. If you can run Valgrind or ASan on a host build,
do that instead — they are more thorough. Heaptrack exists for the case where you cannot.

---

## 2. What you have to change in your code

Exactly one line per API you care about. Nothing else — no print statements, no manual
accounting, no report calls.

```cpp
#include <heaptrack.hpp>

void InterfacePlayerRDK::Tune(const std::string &url)
{
    HEAPTRACK_SCOPE_CPP("InterfacePlayerRDK::Tune");   // <- the only required line

    parseManifest();        // its allocations are attributed here too
    initializeStream();
}
```

Everything the report contains — counts, bytes, peak, leak classification, call tree,
symbol names, file output — is produced by the library.

Optionally, two more macros give richer findings:

```cpp
HEAPTRACK_ASSIGN(m_buffer, malloc(4096));   // report shows var=m_buffer, Player.cpp:88
                                            // and flags re-assignment without free
HEAPTRACK_HANDOFF(m_buffer, "Worker::run"); // ownership moved; leak names the new owner
```

### Why the scope macro is needed at all

The component is compiled without DWARF. There is no way for the library to know that the
code currently running is `InterfacePlayerRDK::Tune`, or that a pointer was stored in a
member called `m_buffer`. The macros supply those two names at compile time. Everything
else is discovered at runtime.

---

## 3. How it works

### 3.1 Interception

`libheaptrack.so` **exports** `malloc`, `calloc`, `realloc`, `free`, `memalign`,
`posix_memalign`, `aligned_alloc`, `valloc`, `pvalloc`, `malloc_usable_size` and the
`operator new` / `operator delete` family. Because your component links `heaptrack` ahead
of libc, the dynamic linker resolves those symbols to the tracker for every module in the
process — including allocations made *inside* libc (`strdup`, `asprintf`, `getline`,
`realpath`, `pthread_create`'s TLS block). No `LD_PRELOAD` and no `--wrap` are required.

The real allocator is reached through glibc's `__libc_malloc` family directly (no `dlsym`
on the hot path). On a non-glibc libc, the pointers are resolved with
`dlsym(RTLD_NEXT, …)`; because `dlsym` itself can allocate, a 16 KB static bootstrap arena
serves allocations made during that window.

`operator new` / `delete` do **not** register anything — they only set a thread-local
*kind hint* and then call `malloc` / `free`. Registration happens at the malloc layer
only, so nothing is counted twice. On free, the hint is compared with the recorded kind,
which is how `new[]` freed with `delete` is detected.

### 3.2 Attribution

Scopes form a **trie**, not one record per call:

```
root
 └─ Player::initialize        node 1
     └─ Manifest::parse       node 2
 └─ Player::tune              node 3
     └─ Decoder::allocBuffers node 4
```

The path `Player::tune -> Decoder::allocBuffers` is **one node**, reused on every call, so
memory does not grow with call count. Each thread keeps a small stack of active node ids.

An allocation is attributed to:

1. the innermost active node of the allocating thread, else
2. the global-mode node if `heaptrack_global_begin()` was called, else
3. nothing — the allocation is not tracked.

That is why a nested helper with no scope of its own still lands under its caller, and why
allocations before the first `heaptrack_begin` are ignored.

### 3.3 Lifetime and ownership

Nodes live for the whole process, so a record can always be attributed even after its API
returned. The registry is a **sharded hash table keyed by address** (64 shards, each with
its own lock, preallocated with `mmap`). Because it is keyed by address and global, a
pointer allocated on one thread and freed on another is matched correctly, and the stats
go back to the originating node.

On free, the record is **removed from the registry before** the real `free` runs. If it
were the other way round, another thread could receive the same address from `malloc` and
insert a new record before the stale removal happened.

When a block is freed by a different node than the one that allocated it, the pair is
recorded in a bounded **freed-by table**, which produces the
`Player::initialize -> Worker::run   1 allocs   32.0 KB` line in the report.

### 3.4 Call tree

For every tracked allocation the stack is unwound with `_Unwind_Backtrace` (default 24
frames). Raw PCs are hashed and deduplicated in a **stack depot**, so a record stores only
a 4-byte `stack_id` instead of ~96 bytes of frames.

Symbolization runs only at report time: `dl_iterate_phdr` maps each PC to a module, then
the tracker's own ELF reader opens that module, maps `.symtab`/`.strtab` (or
`.dynsym`/`.dynstr`) and finds the nearest preceding `FUNC` symbol. C++ names are
demangled with `abi::__cxa_demangle`. No DWARF, no host tool, no script.

### 3.5 Leak classification

A live allocation at report time is not automatically a leak:

| Class | Condition | Confidence |
|---|---|---|
| `IN_SCOPE` | The owning node still has an active invocation | Not a leak — work in progress |
| `OUTLIVED_SCOPE` | No active invocation, block still live | Suspected leak; may be an intentional long-lived object |
| `OVERWRITTEN` | A `HEAPTRACK_ASSIGN` re-bound the variable while the old block was still live | High confidence; reported with variable name and both file:line |

`OVERWRITTEN` is a strong signal but not a proof — another variable may still hold the old
pointer. The report wording says *"old value lost from the variable"* for that reason, and
`HEAPTRACK_HANDOFF` clears the flag.

### 3.6 Keeping out of its own way

The tracker never calls `malloc`. All of its state (registry, node table, stack depot,
string interner, report buffers) is `mmap`-ed at init with a fixed capacity. A thread-local
**recursion guard** is set while tracker code runs, so any allocation the tracker's own
dependencies make (for example `__cxa_demangle`) passes straight through untracked. TLS
uses `initial-exec` so that touching it never triggers an allocating `__tls_get_addr`.

When a bounded pool fills up, the allocation is passed through untracked and a counter is
printed in the report. The component never fails because of the tracker.

---

## 4. API reference

All of this is in `include/heaptrack.h` (C) and `include/heaptrack.hpp` (C++ RAII).

### Scopes

```c
void heaptrack_begin(const char *api_name);
void heaptrack_end(void);

HEAPTRACK_SCOPE("api_name")        /* C: closes on every return path (cleanup attribute) */
HEAPTRACK_SCOPE_CPP("api_name")    /* C++: RAII, exception safe */
```

`heaptrack_end()` pops the thread's stack. A mismatched end is counted and the stack is
unwound down to the matching node, so one missed `end` cannot corrupt tracking permanently.

### Component-wide mode

```c
void heaptrack_global_begin(const char *component_name);
void heaptrack_global_end(void);
```

Everything allocated by the process while this is active is attributed to the component
node, even outside any API scope. Useful for a first pass before you know where to look.

### Variable binding and ownership

```c
void heaptrack_bind(const void *old_ptr, const void *new_ptr,
                    const char *var_name, const char *file, int line);
void heaptrack_handoff(const void *ptr, const char *new_owner_api);

HEAPTRACK_ASSIGN(var, expr)        /* wraps heaptrack_bind, then assigns */
HEAPTRACK_HANDOFF(ptr, "owner")
```

`var` in `HEAPTRACK_ASSIGN` must be free of side effects — it is evaluated more than once.

### Asynchronous work

Thread-local state does not follow work into a thread pool, so propagate it explicitly:

```cpp
unsigned int ctx = heaptrack_ctx_capture();      // on the submitting thread
pool.submit([ctx] {
    heaptrack::AdoptContext adopt(ctx);          // on the worker
    doWork();                                    // attributed to the submitter's node
});
```

### Custom allocators

A pool or arena that does not go through `malloc` can join in:

```c
void heaptrack_note_alloc(const void *ptr, size_t size);
void heaptrack_note_free(const void *ptr);
```

### Reporting

```c
void heaptrack_report(void);                     /* on demand */
void heaptrack_set_sink(const HeaptrackSink *s); /* redirect the output */
```

You normally call neither: the library destructor writes a report at process exit, and
`HEAPTRACK_SIGNAL=1` adds a `SIGUSR2` trigger.

---

## 5. Linking it into a component

### 5.1 As a subdirectory of your project

```cmake
# component/CMakeLists.txt
add_subdirectory(test/HeapTrack)            # path to this directory

add_library(playerinterface SHARED ${SOURCES})

target_link_libraries(playerinterface PRIVATE heaptrack)
target_compile_options(playerinterface PRIVATE -fasynchronous-unwind-tables)
```

### 5.2 Against an installed copy

```bash
cmake -S test/HeapTrack -B /tmp/htbuild -DCMAKE_INSTALL_PREFIX=/usr -DHEAPTRACK_BUILD_TESTS=OFF
cmake --build /tmp/htbuild -j && cmake --install /tmp/htbuild
```

```cmake
find_library(HEAPTRACK_LIB heaptrack REQUIRED)
find_path(HEAPTRACK_INC heaptrack.h REQUIRED)

target_include_directories(playerinterface PRIVATE ${HEAPTRACK_INC})
target_link_libraries(playerinterface PRIVATE ${HEAPTRACK_LIB})
```

### 5.3 Making it switchable

Keep it out of production builds behind an option, with the macros compiled away:

```cmake
option(ENABLE_HEAPTRACK "Build with heap tracking" OFF)

if(ENABLE_HEAPTRACK)
    add_subdirectory(test/HeapTrack)
    target_link_libraries(playerinterface PRIVATE heaptrack)
    target_compile_definitions(playerinterface PRIVATE ENABLE_HEAPTRACK)
endif()
```

```cpp
// a small shim header in your component
#ifdef ENABLE_HEAPTRACK
#include <heaptrack.hpp>
#define PLAYER_HEAP_SCOPE(name) HEAPTRACK_SCOPE_CPP(name)
#else
#define PLAYER_HEAP_SCOPE(name) do {} while (0)
#endif
```

### 5.4 Build requirements for good reports

| Requirement | Why | Effect if missing |
|---|---|---|
| `-funwind-tables` or `-fasynchronous-unwind-tables` (on ARM EHABI: `.ARM.exidx` present) | The unwinder needs them | Call trees are short or empty |
| Keep `.symtab`: strip with `--strip-debug`, **not** `--strip-all`; or link with `-rdynamic` | The ELF reader resolves names from it | Frames print as `module+0xOFFSET` |
| Link `heaptrack` **before** libc (the default for `target_link_libraries`) | Symbol interposition | Allocations are not intercepted |

API names, variable names and `file:line` come from the macros, so they survive any level
of stripping. Only the call-tree frames depend on `.symtab`.

---

## 6. Building and running the tests

```bash
cd test/HeapTrack
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build -j

# full demo: prints the expected numbers, then the real report
HEAPTRACK_REPORT_AT_EXIT=0 ./build/tests/heaptrack_test

# whole suite (ctest hides stdout unless you pass -V)
cd build && ctest -V
```

| Test | Covers |
|---|---|
| `heaptrack_test` (`tests/test.cpp`) | Full mini-player demo with known sizes, expected vs actual |
| `test_basic` | Attribution, nesting, roll-up, allocation outliving its scope |
| `test_realloc` | `realloc(NULL,n)`, `realloc(p,0)`, grow, shrink, untracked pointer |
| `test_cpp` | `new`/`delete` kind hints, mismatch detection, overwrite, handoff |
| `test_threads` | Cross-thread free, `AdoptContext`, 4×20 000 alloc/free stress |
| `test_c_api` | Pure C, cleanup-attribute scope with early return, `HEAPTRACK_ASSIGN` |

Note on the test sources: they deliberately touch every block and are built with
`-fno-builtin-malloc -fno-builtin-free`. At `-O2` GCC deletes a `malloc`/`free` pair whose
pointer never escapes, and the tracker would then correctly report nothing at all.

---

## 7. Configuration

Read once at init from the environment.

| Variable | Default | Meaning |
|---|---|---|
| `HEAPTRACK_DEPTH` | `24` | Call-tree depth, max 32. `0` disables stack capture (cheapest mode) |
| `HEAPTRACK_MAX_RECORDS` | `262144` | Registry capacity. Overflow is counted, never fatal |
| `HEAPTRACK_MIN_SIZE` | `0` | Capture a stack only for allocations of at least this many bytes |
| `HEAPTRACK_MAX_FINDINGS` | `20` | Findings printed per API |
| `HEAPTRACK_SINK` | `/tmp/heaptrack_<pid>.log` | Output file path |
| `HEAPTRACK_REPORT_AT_EXIT` | `1` | Write a report from the library destructor |
| `HEAPTRACK_SIGNAL` | `0` | `1` installs a `SIGUSR2` handler plus a reporter thread |

With `HEAPTRACK_SIGNAL=1`, `kill -USR2 <pid>` produces a report at any time. The signal
handler itself does no locking and no formatting — it writes one byte to a pipe and a
dedicated reporter thread does the work.

Memory cost is dominated by the registry: roughly `HEAPTRACK_MAX_RECORDS × 48 × 2` bytes
of address space, demand-paged, so only the part actually used is resident. On a tight
32-bit target lower it to `65536`.

---

## 8. Reading the report

```text
API: Player::tune > Decoder::allocBuffers   (calls: 1)
  Total allocations     : 5
  Total deallocations   : 4
  Total allocated       : 1.2 MB
  Total freed           : 1.0 MB
  Current heap usage    : 256.0 KB     (rolled up incl. children: 256.0 KB)
  Peak heap usage       : 1.2 MB
  Live allocations      : 1
  Suspected leaks       : 1 allocs, 256.0 KB
  Mismatched frees      : 0
  Cross-context frees   : 0
  Findings              : IN_SCOPE=0 OUTLIVED_SCOPE=1 OVERWRITTEN=0

  Findings
    [OUTLIVED_SCOPE] var=<unnamed> size=262144 site=- addr=0x7a6f0f73d010
        #0 decoder_alloc_buffers()+0x5c
        #1 player_tune()+0x12
        #2 main+0x232
```

- **API** is the full scope path. `A > B` means B was entered while A was active.
- **Current heap usage** is what this node itself still holds; the rolled-up figure adds
  all descendants.
- **Cross-context frees** counts blocks from this node that were released under a
  different node; the pairs are listed in the freed-by summary.
- **`var=<unnamed>` / `site=-`** simply means the allocation did not go through
  `HEAPTRACK_ASSIGN`. Identify it by the call tree instead.

The `PER-API SUMMARY` table at the end is the quickest answer to "who allocated how much
and who leaked":

```text
API                                     allocs    allocated     freed         live    leaked
Player::initialize                      6         104.0 KB      40.0 KB       1       64.0 KB (1)
Player::tune > Decoder::allocBuffers    5         1.2 MB        1.0 MB        1       256.0 KB (1)
TOTAL                                   26        1.3 MB        1.0 MB        5       326.0 KB
```

The trailing counters (`Dropped records`, `Node table overflows`, `Stack depot drops`,
`Scope pairing errors`) must be `0` for the numbers above to be complete. If they are not,
raise the corresponding capacity or fix the unbalanced `begin`/`end`.

---

## 9. Limitations

1. **Pointer copies are not followed.** Ownership changes must be stated with
   `HEAPTRACK_HANDOFF`.
2. **`mmap`, shared memory and custom pools are invisible** unless routed through
   `heaptrack_note_alloc` / `heaptrack_note_free`.
3. **Inlining shortens call trees.** Without debug info there are no inline frames; at
   `-O2` a small static helper may disappear into its caller. The scope path in the `API:`
   line is unaffected.
4. **`OVERWRITTEN` can be a false positive** when another variable still aliases the old
   pointer.
5. **Allocations outside any scope** (and outside global mode) are not attributed at all.
6. **Bounded capacity.** Registry, node table and depot overflow gracefully and report it.
7. **Modules unloaded with `dlclose`** are not cached; frames inside them resolve to
   `??0x…` afterwards.
8. **Interposition scope.** Process-wide when linked into the executable or a library
   loaded at startup. A copy `dlopen`-ed with `RTLD_LOCAL` only interposes its own scope.

---

## 10. Extending it

### A new sink (for example the RDK logger)

```c
static void rdk_write(void *ctx, const char *data, size_t len)
{
    (void)ctx;
    RDK_LOG(RDK_LOG_WARN, "LOG.RDK.PLAYER", "%.*s", (int)len, data);
}

HeaptrackSink sink = { rdk_write, NULL };
heaptrack_set_sink(&sink);
```

The default file sink in `src/ht_sink.cpp` is just another implementation of the same
two-field interface. The formatter never uses stdio and writes through a fixed buffer, so
a sink must only be non-allocating and reentrancy-safe.

### A new leak class

Each class is one predicate in `src/ht_leak.cpp`:

```cpp
static bool is_huge_and_old(const HeapAllocRecord *r, const HeapTrackNode *)
{
    return r->size > (1u << 20) && age_of(r) > SOME_NS;
}

static const Classifier g_classifiers[] = {
    {HT_FIND_OVERWRITTEN, is_overwritten},
    {HT_FIND_HUGE_AND_OLD, is_huge_and_old},   // checked in order, first match wins
    {HT_FIND_IN_SCOPE,    is_in_scope},
    {HT_FIND_OUTLIVED_SCOPE, is_outlived},
};
```

Add the enum value in `src/ht_internal.h` and its name in `ht_finding_name()`. The report
picks it up automatically.

### Other hook points

| Want to | Touch |
|---|---|
| Add a tracked allocator entry point | `src/ht_interceptor.cpp` |
| Change the record layout / capacity | `HeapAllocRecord` in `src/ht_internal.h`, `ht_reg_init()` |
| Change what the report prints | `ht_report_run()` in `src/ht_report.cpp` |
| Port to a non-ELF or non-glibc target | `src/ht_bootstrap.c`, `src/ht_elf.cpp`, `src/ht_platform.h` |

---

## 11. Source map

```
include/heaptrack.h       C API and macros
include/heaptrack.hpp     C++ RAII wrappers
src/ht_platform.h         atomics, initial-exec TLS, mmap, 64-bit counter fallback
src/ht_internal.h         shared internal types and declarations
src/ht_bootstrap.{h,c}    bootstrap arena, real allocator resolution
src/ht_interceptor.cpp    malloc family, new/delete kind hints, alloc/free hooks
src/ht_context.cpp        string/site interner, node trie, scope stack, global mode
src/ht_registry.cpp       sharded address-keyed registry, freed-by table
src/ht_unwind.cpp         unwinder and warm-up
src/ht_stackdepot.cpp     stack deduplication
src/ht_elf.cpp            ELF symbol table reader
src/ht_symbolizer.cpp     module map, demangling
src/ht_leak.cpp           leak classification
src/ht_report.cpp         report formatting, reporter thread, SIGUSR2
src/ht_sink.cpp           sink interface, file sink, allocation-free writer
src/ht_api.cpp            public API, init, configuration, fork handling
tests/                    demo and regression programs
```

The design rationale behind each of these is in `heaptrack_design.md` at the repository
root.

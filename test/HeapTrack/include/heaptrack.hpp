#ifndef HEAPTRACK_HPP
#define HEAPTRACK_HPP

#include "heaptrack.h"

namespace heaptrack
{

class Scope
{
public:
    explicit Scope(const char *name) { heaptrack_begin(name); }
    ~Scope() { heaptrack_end(); }

private:
    Scope(const Scope &);
    Scope &operator=(const Scope &);
};

class AdoptContext
{
public:
    explicit AdoptContext(unsigned int ctx) { heaptrack_ctx_attach(ctx); }
    ~AdoptContext() { heaptrack_ctx_detach(); }

private:
    AdoptContext(const AdoptContext &);
    AdoptContext &operator=(const AdoptContext &);
};

inline unsigned int captureContext() { return heaptrack_ctx_capture(); }

} // namespace heaptrack

#define HEAPTRACK_SCOPE_CPP(name) ::heaptrack::Scope HT_CAT(_ht_scope_, __LINE__)(name)

#endif /* HEAPTRACK_HPP */

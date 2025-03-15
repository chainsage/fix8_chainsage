#ifndef __MACOS_ATOMIC_H
#define __MACOS_ATOMIC_H

#include <atomic>

typedef struct {
    std::atomic<long> counter;
} atomic_t;
typedef atomic_t atomic_long_t;

#define ATOMIC_INIT(i) {std::atomic<long>(i)}
#define ATOMIC_LONG_INIT(i) ATOMIC_INIT(i)

static inline long atomic_read(const atomic_t *v) {
    return v->counter.load(std::memory_order_relaxed);
}

static inline void atomic_set(atomic_t *v, long i) {
    v->counter.store(i, std::memory_order_relaxed);
}

static inline void atomic_inc(atomic_t *v) {
    v->counter.fetch_add(1, std::memory_order_relaxed);
}

static inline long atomic_inc_return(atomic_t *v) {
    return v->counter.fetch_add(1, std::memory_order_relaxed) + 1;
}

static inline void atomic_dec(atomic_t *v) {
    v->counter.fetch_sub(1, std::memory_order_relaxed);
}

static inline long atomic_dec_return(atomic_t *v) {
    return v->counter.fetch_sub(1, std::memory_order_relaxed) - 1;
}

static inline void atomic_add(long i, atomic_t *v) {
    v->counter.fetch_add(i, std::memory_order_relaxed);
}

static inline void atomic_sub(long i, atomic_t *v) {
    v->counter.fetch_sub(i, std::memory_order_relaxed);
}

static inline long atomic_add_unless(atomic_t *v, long a, long u) {
    long c, old;
    c = atomic_read(v);
    for (;;) {
        if (c == u) break;
        old = c;
        if (v->counter.compare_exchange_strong(old, c + a, std::memory_order_relaxed,
                                               std::memory_order_relaxed))
            break;
        c = old;
    }
    return c != u;
}

static inline unsigned long atomic_long_read(atomic_long_t *l) {
    return atomic_read((atomic_t *)l);
}

static inline void atomic_long_set(atomic_long_t *l, long i) {
    atomic_set((atomic_t *)l, i);
}

static inline void atomic_long_inc(atomic_long_t *l) { atomic_inc((atomic_t *)l); }

static inline unsigned long atomic_long_inc_return(atomic_long_t *l) {
    return atomic_inc_return((atomic_t *)l);
}

static inline void atomic_long_dec(atomic_long_t *l) { atomic_dec((atomic_t *)l); }

static inline unsigned long atomic_long_dec_return(atomic_long_t *l) {
    return atomic_dec_return((atomic_t *)l);
}

static inline void atomic_long_add(long i, atomic_long_t *l) {
    atomic_add(i, (atomic_t *)l);
}

static inline void atomic_long_sub(long i, atomic_long_t *l) {
    atomic_sub(i, (atomic_t *)l);
}

static inline unsigned long atomic_long_add_unless(atomic_long_t *l, long a, long u) {
    return atomic_add_unless((atomic_t *)l, a, u);
}

#define atomic_cmpxchg(v, old, new)                                            \
    ((v)->counter.compare_exchange_strong(old, new, std::memory_order_relaxed, \
                                          std::memory_order_relaxed)           \
         ? old                                                                 \
         : (v)->counter.load(std::memory_order_relaxed))

#define atomic_long_cmpxchg(l, old, new) (atomic_cmpxchg((atomic_t *)(l), (old), (new)))

#endif /* __MACOS_ATOMIC_H */
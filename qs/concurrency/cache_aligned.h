#ifndef QS_CONCURRENCY_CACHE_ALIGNED_H
#define QS_CONCURRENCY_CACHE_ALIGNED_H

#include <qs/config.h>

QS_NAMESPACE_BEGIN

template<class T>
class alignas(QS_CACHELINE_SIZE) cache_aligned : public T
{
public:
    using T::T;
};

QS_NAMESPACE_END

#endif // QS_CONCURRENCY_CACHE_ALIGNED_H

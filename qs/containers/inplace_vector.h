#ifndef QS_INPLACE_VECTOR_H_
#define QS_INPLACE_VECTOR_H_

#include <qs/base.h>

QS_NAMESPACE_BEGIN

template <typename T, std::size_t Capacity>
class inplace_vector
{
public:
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    inplace_vector();

private:
    alignas(T) unsigned char data_[Capacity * sizeof(T)];
    std::size_t size_;
};


QS_NAMESPACE_END

#endif // QS_INPLACE_VECTOR_H
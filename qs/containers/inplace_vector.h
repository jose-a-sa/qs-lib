#ifndef QS_INPLACE_VECTOR_H_
#define QS_INPLACE_VECTOR_H_

#include <qs/base.h>
#include <qs/concepts.h>

QS_NAMESPACE_BEGIN

// The class template `inplace_vector` is defined to store elements of type `T` with a fixed capacity `Capacity`.
// Emphasis on making the class API compatible with the standard library `std::vector` as much as possible to ease change between them.
// It follows closely the C++26 standard implementation, https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p0843r14.html
template<typename T, std::size_t Capacity>
class inplace_vector
{
public:
    // Type aliases for convenience and compatibility with standard library containers
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using iterator               = value_type*;
    using const_iterator         = value_type const*;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Default constructor
    QS_CONSTEXPR11 inplace_vector();

    // Additional constructors for various initialization scenarios
    QS_CONSTEXPR11 explicit inplace_vector(size_type n);
    QS_CONSTEXPR11 inplace_vector(size_type n, value_type const& value);

    // Constructor for initializing from a range of iterators
    template<class InputIterator,
             class = std::enable_if_t<qs::is_std_iterator<InputIterator, std::input_iterator_tag, value_type>::value>>
    QS_CONSTEXPR11 inplace_vector(InputIterator first, InputIterator last);

    // Copy and move constructors
    QS_CONSTEXPR11 inplace_vector(inplace_vector const& other);
    QS_CONSTEXPR11 inplace_vector(inplace_vector&& other) QS_NOEXCEPT;

    // Initializer list constructor
    QS_CONSTEXPR11 inplace_vector(std::initializer_list<value_type> il);

    // Destructor
    QS_CONSTEXPR20 ~inplace_vector();

    // Assignment operators
    QS_CONSTEXPR11 inplace_vector& operator=(inplace_vector const& other);
    QS_CONSTEXPR11 inplace_vector& operator=(inplace_vector&& other) QS_NOEXCEPT;
    QS_CONSTEXPR11 inplace_vector& operator=(std::initializer_list<value_type> il);

    // Assign functions for various scenarios
    template<class InputIterator,
             class = std::enable_if_t<qs::is_std_iterator<InputIterator, std::input_iterator_tag, value_type>::value>>
    QS_CONSTEXPR11 void assign(InputIterator first, InputIterator last);

    QS_CONSTEXPR11 void assign(size_type n, value_type const& x);
    QS_CONSTEXPR11 void assign(std::initializer_list<value_type> il);

    // Iterator functions
    QS_CONSTEXPR11 iterator       begin() QS_NOEXCEPT;
    QS_CONSTEXPR11 const_iterator begin() const QS_NOEXCEPT;
    QS_CONSTEXPR11 iterator       end() QS_NOEXCEPT;
    QS_CONSTEXPR11 const_iterator end() const QS_NOEXCEPT;

    QS_CONSTEXPR11 reverse_iterator       rbegin() QS_NOEXCEPT { return reverse_iterator(end()); };
    QS_CONSTEXPR11 const_reverse_iterator rbegin() const QS_NOEXCEPT { return const_reverse_iterator(end()); };
    QS_CONSTEXPR11 reverse_iterator       rend() QS_NOEXCEPT { return reverse_iterator(begin()); };
    QS_CONSTEXPR11 const_reverse_iterator rend() const QS_NOEXCEPT { return const_reverse_iterator(begin()); };

    QS_CONSTEXPR11 const_iterator         cbegin() const QS_NOEXCEPT { return begin(); };
    QS_CONSTEXPR11 const_iterator         cend() const QS_NOEXCEPT { return end(); };
    QS_CONSTEXPR11 const_reverse_iterator crbegin() const QS_NOEXCEPT { return rbegin(); };
    QS_CONSTEXPR11 const_reverse_iterator crend() const QS_NOEXCEPT { return rend(); };

    // Capacity-related functions
    QS_CONSTEXPR11 size_type size() const QS_NOEXCEPT;
    QS_CONSTEXPR11 size_type capacity() const QS_NOEXCEPT;
    QS_CONSTEXPR11 size_type max_size() const QS_NOEXCEPT;

    QS_CONSTEXPR11 bool empty() const QS_NOEXCEPT;
    QS_CONSTEXPR11 void reserve(size_type n); // only for api compatibility
    QS_CONSTEXPR11 void shrink_to_fit() QS_NOEXCEPT; // only for api compatibility

    // Element access functions
    QS_CONSTEXPR11 reference       operator[](size_type n);
    QS_CONSTEXPR11 const_reference operator[](size_type n) const;
    QS_CONSTEXPR11 reference       at(size_type n);
    QS_CONSTEXPR11 const_reference at(size_type n) const;

    QS_CONSTEXPR11 reference       front();
    QS_CONSTEXPR11 const_reference front() const;
    QS_CONSTEXPR11 reference       back();
    QS_CONSTEXPR11 const_reference back() const;

    QS_CONSTEXPR11 pointer       data() QS_NOEXCEPT;
    QS_CONSTEXPR11 const_pointer data() const QS_NOEXCEPT;

    // Modifiers
    QS_CONSTEXPR11 void push_back(value_type const& x);
    QS_CONSTEXPR11 void push_back(value_type&& x);

    // new in inplace_vector, does not throw, does nothing if the vector is full
    QS_CONSTEXPR11 void try_push_back(value_type const& x);
    QS_CONSTEXPR11 void try_push_back(value_type&& x);

    template<class... Args>
    QS_CONSTEXPR17 reference emplace_back(Args&&... args);

    // new in inplace_vector, does not throw, instead returns a pointer to the inserted element or nullptr
    template<class... Args>
    QS_CONSTEXPR17 pointer emplace_back(Args&&... args);

    QS_CONSTEXPR11 void pop_back();

    template<class... Args>
    QS_CONSTEXPR17 iterator emplace(const_iterator position, Args&&... args);

    QS_CONSTEXPR11 iterator insert(const_iterator position, value_type const& x);
    QS_CONSTEXPR11 iterator insert(const_iterator position, value_type&& x);
    QS_CONSTEXPR11 iterator insert(const_iterator position, size_type n, value_type const& x);

    template<class InputIterator,
             class = std::enable_if_t<is_std_iterator<InputIterator, std::input_iterator_tag, value_type>::value>>
    QS_CONSTEXPR11 iterator insert(const_iterator position, InputIterator first, InputIterator last);

    QS_CONSTEXPR11 iterator insert(const_iterator position, std::initializer_list<value_type> il);

    QS_CONSTEXPR11 iterator erase(const_iterator position);
    QS_CONSTEXPR11 iterator erase(const_iterator first, const_iterator last);

    QS_CONSTEXPR11 void clear() QS_NOEXCEPT;

    QS_CONSTEXPR11 void resize(size_type size);
    QS_CONSTEXPR11 void resize(size_type size, value_type const& x);

    QS_CONSTEXPR17 void swap(inplace_vector& other) QS_NOEXCEPT; // C++17

private:
    // Storage for elements, using a byte array with alignment for type T
    alignas(T) std::byte data_[Capacity * sizeof(T)];
    size_t size_;
};

QS_NAMESPACE_END

#endif // QS_INPLACE_VECTOR_H_

#ifndef QS_INPLACE_VECTOR_H_
#define QS_INPLACE_VECTOR_H_

#include <qs/concepts.h>
#include <qs/config.h>
#include <qs/exception_guard.h>
#include <qs/memory.h>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <new>
#include <stdexcept>

QS_NAMESPACE_BEGIN

// The class template `inplace_vector` is defined to store elements of type `T` with a fixed
// capacity `Capacity`. Emphasis on making the class API compatible with the standard library
// `std::vector` as much as possible to ease change between them. It follows closely the C++26
// standard implementation, https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2024/p0843r14.html
// template<typename T, std::size_t Capacity>
// class inplace_vector
// {
// public:
//     // Type aliases for convenience and compatibility with standard library containers
//     using value_type             = T;
//     using reference              = value_type&;
//     using const_reference        = value_type const&;
//     using pointer                = value_type*;
//     using const_pointer          = value_type const*;
//     using size_type              = size_t;
//     using difference_type        = ptrdiff_t;
//     using iterator               = value_type*;
//     using const_iterator         = value_type const*;
//     using reverse_iterator       = std::reverse_iterator<iterator>;
//     using const_reverse_iterator = std::reverse_iterator<const_iterator>;

//     // Default constructor
//     QS_CONSTEXPR11 inplace_vector();

//     // Additional constructors for various initialization scenarios
//     QS_CONSTEXPR11 explicit inplace_vector(size_type n);
//     QS_CONSTEXPR11 inplace_vector(size_type n, value_type const& value);

//     // Constructor for initializing from a range of iterators
//     template<class InputIterator,
//              class = std::enable_if_t<is_std_iterator<InputIterator, std::input_iterator_tag,
//              value_type>::value>>
//     QS_CONSTEXPR11 inplace_vector(InputIterator first, InputIterator last);

//     // Copy and move constructors
//     QS_CONSTEXPR11 inplace_vector(inplace_vector const& other);
//     QS_CONSTEXPR11 inplace_vector(inplace_vector&& other) noexcept;

//     // Initializer list constructor
//     QS_CONSTEXPR11 inplace_vector(std::initializer_list<value_type> il);

//     // Destructor
//     QS_CONSTEXPR20 ~inplace_vector();

//     // Assignment operators
//     QS_CONSTEXPR11 inplace_vector& operator=(inplace_vector const& other);
//     QS_CONSTEXPR11 inplace_vector& operator=(inplace_vector&& other) noexcept;
//     QS_CONSTEXPR11 inplace_vector& operator=(std::initializer_list<value_type> il);

//     // Assign functions for various scenarios
//     template<class InputIterator,
//              class = std::enable_if_t<is_std_iterator<InputIterator, std::input_iterator_tag,
//              value_type>::value>>
//     QS_CONSTEXPR11 void assign(InputIterator first, InputIterator last);

//     QS_CONSTEXPR11 void assign(size_type n, value_type const& x);
//     QS_CONSTEXPR11 void assign(std::initializer_list<value_type> il);

//     // Iterator functions
//     QS_CONSTEXPR11 iterator       begin() noexcept;
//     QS_CONSTEXPR11 const_iterator begin() const noexcept;
//     QS_CONSTEXPR11 iterator       end() noexcept;
//     QS_CONSTEXPR11 const_iterator end() const noexcept;

//     QS_CONSTEXPR11 reverse_iterator       rbegin() noexcept { return reverse_iterator(end());
//     }; QS_CONSTEXPR11 const_reverse_iterator rbegin() const noexcept { return
//     const_reverse_iterator(end()); }; QS_CONSTEXPR11 reverse_iterator       rend() noexcept {
//     return reverse_iterator(begin()); }; QS_CONSTEXPR11 const_reverse_iterator rend() const
//     noexcept { return const_reverse_iterator(begin()); };

//     QS_CONSTEXPR11 const_iterator         cbegin() const noexcept { return begin(); };
//     QS_CONSTEXPR11 const_iterator         cend() const noexcept { return end(); };
//     QS_CONSTEXPR11 const_reverse_iterator crbegin() const noexcept { return rbegin(); };
//     QS_CONSTEXPR11 const_reverse_iterator crend() const noexcept { return rend(); };

//     // Capacity-related functions
//     QS_CONSTEXPR11 size_type size() const noexcept;
//     QS_CONSTEXPR11 size_type capacity() const noexcept { return Capacity; };
//     QS_CONSTEXPR11 size_type max_size() const noexcept { return Capacity; };

//     QS_CONSTEXPR11 bool empty() const noexcept { return size() == 0; };
//     QS_CONSTEXPR11 void reserve(size_type n) /*const*/ // only for api compatibility
//     {
//         if(n > this->capacity())
//             this->throw_bad_alloc();
//     };
//     QS_CONSTEXPR11 void shrink_to_fit() noexcept { /*does nothing*/ }; // only for api
//     compatibility

//     // Element access functions
//     QS_CONSTEXPR11 reference       operator[](size_type n);
//     QS_CONSTEXPR11 const_reference operator[](size_type n) const;
//     QS_CONSTEXPR11 reference       at(size_type n);
//     QS_CONSTEXPR11 const_reference at(size_type n) const;

//     QS_CONSTEXPR11 reference       front();
//     QS_CONSTEXPR11 const_reference front() const;
//     QS_CONSTEXPR11 reference       back();
//     QS_CONSTEXPR11 const_reference back() const;

//     QS_CONSTEXPR11 pointer       data() noexcept;
//     QS_CONSTEXPR11 const_pointer data() const noexcept;

//     // Modifiers
//     QS_CONSTEXPR11 void push_back(value_type const& x);
//     QS_CONSTEXPR11 void push_back(value_type&& x);

//     // new in inplace_vector, does not throw, does nothing if the vector is full
//     QS_CONSTEXPR11 void try_push_back(value_type const& x);
//     QS_CONSTEXPR11 void try_push_back(value_type&& x);

//     template<class... Args>
//     QS_CONSTEXPR17 reference emplace_back(Args&&... args);

//     // new in inplace_vector, does not throw, instead returns a pointer to the inserted element
//     or nullptr template<class... Args> QS_CONSTEXPR17 pointer emplace_back(Args&&... args);

//     QS_CONSTEXPR11 void pop_back();

//     template<class... Args>
//     QS_CONSTEXPR17 iterator emplace(const_iterator position, Args&&... args);

//     QS_CONSTEXPR11 iterator insert(const_iterator position, value_type const& x);
//     QS_CONSTEXPR11 iterator insert(const_iterator position, value_type&& x);
//     QS_CONSTEXPR11 iterator insert(const_iterator position, size_type n, value_type const& x);

//     template<class InputIterator,
//              class = std::enable_if_t<is_std_iterator<InputIterator, std::input_iterator_tag,
//              value_type>::value>>
//     QS_CONSTEXPR11 iterator insert(const_iterator position, InputIterator first, InputIterator
//     last);

//     QS_CONSTEXPR11 iterator insert(const_iterator position, std::initializer_list<value_type>
//     il);

//     QS_CONSTEXPR11 iterator erase(const_iterator position);
//     QS_CONSTEXPR11 iterator erase(const_iterator first, const_iterator last);

//     QS_CONSTEXPR11 void clear() noexcept;

//     QS_CONSTEXPR11 void resize(size_type size);
//     QS_CONSTEXPR11 void resize(size_type size, value_type const& x);

//     QS_CONSTEXPR17 void swap(inplace_vector& other) noexcept; // C++17x


//     // [[noreturn]] void throw_out_of_range() const { throw std::out_of_range("inplace_vector");
//     }
//     // [[noreturn]] void throw_bad_alloc() const { throw std::bad_alloc(); }
// };


template<class EndPointer>
struct construction_transaction
{
    using value_type    = typename std::remove_pointer<EndPointer>::type;
    using pointer       = EndPointer;
    using const_pointer = typename std::add_const<pointer>::type;
    using size_type     = size_t;

    QS_CONSTEXPR14 construction_transaction(pointer& end, size_type n) noexcept
        : new_end(end + n),
          end_(end),
          curr_pos_(end)
    {}

    QS_CONSTEXPR14 construction_transaction(pointer& end) noexcept
        : new_end(nullptr),
          end_(end),
          curr_pos_(end)
    {}

    construction_transaction(construction_transaction const&)            = delete;
    construction_transaction& operator=(construction_transaction const&) = delete;

    QS_CONSTEXPR20 ~construction_transaction() noexcept { end_ = curr_pos_; }

    QS_CONSTEXPR14 pointer commit(size_type n = 1) noexcept { return curr_pos_ += n; }
    QS_CONSTEXPR14 pointer commit(pointer new_pos) noexcept { return curr_pos_ = new_pos; }

    const_pointer const new_end;

private:
    pointer& end_;
    pointer  curr_pos_;
};


template<class T, size_t Capacity, bool IsTrivial = (Capacity == 0) || std::is_trivially_copyable<T>::value>
struct inplace_vector_base;


template<class T>
struct inplace_vector_base<T, 0, true>
{
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    QS_CONSTEXPR14 inplace_vector_base() = default;

    QS_CONSTEXPR14 pointer       data_(size_type /*n*/ = 0) noexcept { return nullptr; }
    QS_CONSTEXPR11 const_pointer data_(size_type /*n*/ = 0) const noexcept { return nullptr; }
    QS_CONSTEXPR11 size_type     size_() const noexcept { return 0; }

    template<class... Args>
    QS_CONSTEXPR14 void construct_back_single_(Args&&...) noexcept
    {}

    QS_CONSTEXPR20 void construct_back_n_(size_type /*n*/) noexcept {}

    QS_CONSTEXPR20 void construct_back_n_(size_type /*n*/, const_reference /*x*/) noexcept {}

    template<class Iterator, class Sentinel>
    QS_CONSTEXPR20 void construct_back_range_(Iterator /*first*/, Sentinel /*last*/, size_type /*n*/) noexcept
    {}

    QS_CONSTEXPR20 void move_range_(pointer /*from_start*/, pointer /*from_end*/, pointer /*to*/) noexcept {}

    QS_CONSTEXPR20 void assign_n_(size_type /*n*/, const_reference /*x*/) noexcept {}

    template<class Iterator, class Sentinel>
    QS_CONSTEXPR20 void assign_range_(Iterator /*first*/, Sentinel /*last*/, size_type) noexcept
    {}

    QS_CONSTEXPR20 void destroy_back_(pointer /*new_end*/) noexcept {}

    QS_CONSTEXPR20 void clear_() noexcept {}
};


template<class T, size_t Capacity>
struct inplace_vector_base<T, Capacity, false>
{
    static_assert(Capacity > 0, "non-specialized inplace_vector_base must have Capacity greater than 0");
    static_assert(std::is_nothrow_destructible<T>::value, "value_type must be nothrow destructible");

    using value_type      = T;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    QS_CONSTEXPR14 inplace_vector_base() noexcept
        : end_(data_()) {};

    // Copy constructor
    QS_CONSTEXPR20 inplace_vector_base(inplace_vector_base const& other)
        : inplace_vector_base()
    {
        // auto guard = make_exception_guard([&]{ this->clear_(); });
        this->construct_back_range_(other.data_(), other.end_, other.size_());
        // guard.complete();
    }

    // Copy assignment operator
    QS_CONSTEXPR20 inplace_vector_base& operator=(inplace_vector_base const& rhs) noexcept
    {
        if(this != std::addressof(rhs))
        {
            auto guard = make_exception_guard([&] { this->clear_(); });
            this->assign_range_(rhs.data_(), rhs.end_, rhs.size_());
            guard.complete();
        }
        return *this;
    }

    // Move constructor
    QS_CONSTEXPR20 inplace_vector_base(inplace_vector_base&& other) noexcept
        : inplace_vector_base()
    {
        // auto guard = make_exception_guard([&]{ this->clear_(); });
        this->construct_back_range_(std::make_move_iterator(other.data_()),
                                    std::make_move_iterator(other.end_), other.size_());
        other.end_ = other.data_();
        // guard.complete();
    }

    // Move assignment operator
    QS_CONSTEXPR20 inplace_vector_base& operator=(inplace_vector_base&& rhs) noexcept
    {
        if(this != std::addressof(rhs))
        {
            auto guard = make_exception_guard([&] { this->clear_(); });
            this->assign_range_(std::make_move_iterator(rhs.data_()), std::make_move_iterator(rhs.end_),
                                rhs.size_());
            rhs.end_ = rhs.data_();
            guard.complete();
        }
        return *this;
    }

    // Destructor
    QS_CONSTEXPR20 ~inplace_vector_base() noexcept { this->clear_(); }


    QS_CONSTEXPR14 pointer       data_(size_type n = 0) noexcept { return this->buffer_->recast() + n; }
    QS_CONSTEXPR11 const_pointer data_(size_type n = 0) const noexcept { return this->buffer_->recast() + n; }
    QS_CONSTEXPR11 size_type     size_() const noexcept { return this->end_ - this->data_(); }


    template<class... Args>
    QS_CONSTEXPR20 void construct_back_single_(Args&&... args)
    {
        construction_transaction<pointer> tx(this->end_, 1);
        construct_at(this->end_, std::forward<Args>(args)...);
        tx.commit();
    }

    QS_CONSTEXPR20 void construct_back_n_(size_type n)
    {
        construction_transaction<pointer> tx(this->end_, n);
        for(pointer pos = this->end_; pos != tx.new_end; ++pos, tx.commit())
            construct_at(pos);
    }

    QS_CONSTEXPR20 void construct_back_n_(size_type n, const_reference x)
    {
        construction_transaction<pointer> tx(this->end_, n);
        for(pointer pos = this->end_; pos != tx.new_end; ++pos, tx.commit())
            construct_at(pos, x);
    }

    template<class Iterator, class Sentinel>
    QS_CONSTEXPR20 void construct_back_range_(Iterator first, Sentinel last, size_type n)
    {
        pointer const destroy_first = this->end_;
        auto          guard         = make_exception_guard([&] { this->destroy_back_(destroy_first); });

        construction_transaction<pointer> tx(this->end_, n);
        for(pointer pos = this->end_; first != last; ++pos, ++first, tx.commit())
            construct_at(pos, *first);

        guard.complete();
    }

    QS_CONSTEXPR20 void assign_n_(size_type n, const_reference x)
    {
        size_type const curr_size = this->size_();
        std::fill_n(this->data_(), std::min(n, curr_size), x);
        if(n > curr_size)
            this->construct_back_n_(n - curr_size, x);
        else
            this->destroy_back_(this->data_(n));
    }

    template<class Iterator, class Sentinel>
    QS_CONSTEXPR20 void assign_range_(Iterator first, Sentinel last, size_type n)
    {
        size_type const curr_size = this->size_();
        if(n > curr_size)
        {
            std::copy_n(first, curr_size, this->data_());
            this->construct_back_range_(std::next(first, curr_size), last, n - curr_size);
        }
        else
        {
            pointer new_end = std::copy(first, last, this->data_());
            this->destroy_back_(new_end);
        }
    }

    QS_CONSTEXPR20 void move_range_(pointer from_start, pointer from_end, pointer to)
    {
        pointer const old_last = this->end_;
        pointer const from_mid = from_start + (old_last - to);
        auto const    m        = static_cast<size_type>(std::max<difference_type>(from_end - from_mid, 0));
        {
            construction_transaction<pointer> tx(this->end_, m);
            for(pointer i = from_mid, pos = old_last; i < from_end; ++i, ++pos, tx.commit())
                construct_at(pos, std::move(*i));
        }
        std::move_backward(from_start, from_mid, old_last);
    }

    QS_CONSTEXPR20 void destroy_back_(pointer new_end) noexcept
    {
        reverse_destroy(new_end, this->end_);
        this->end_ = new_end;
    }

    QS_CONSTEXPR20 void clear_() noexcept { this->destroy_back_(this->data_()); }

    struct alignas(value_type) inner_element_t
    {
        byte                         elem_[sizeof(value_type)];
        QS_CONSTEXPR14 pointer       recast() { return launder(reinterpret_cast<pointer>(elem_)); }
        QS_CONSTEXPR14 const_pointer recast() const
        {
            return launder(reinterpret_cast<const_pointer>(elem_));
        }
    };
    static_assert(sizeof(inner_element_t) == sizeof(value_type) &&
                      alignof(inner_element_t) == alignof(value_type),
                  "inner_element_t is not the same size/alignment as value_type");

    inner_element_t buffer_[Capacity];
    pointer         end_;
};


template<class T, size_t Capacity>
struct inplace_vector_base<T, Capacity, true>
{
    static_assert(Capacity > 0, "non-specialized inplace_vector_base must have Capacity greater than 0");
    static_assert(std::is_nothrow_destructible<T>::value, "value_type must be nothrow destructible");

    using value_type      = T;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    QS_CONSTEXPR14 inplace_vector_base()
        : end_(data_()) {};

    QS_CONSTEXPR20 ~inplace_vector_base() noexcept { clear_(); }

    QS_CONSTEXPR14 pointer       data_(size_type n = 0) noexcept { return buffer_ + n; }
    QS_CONSTEXPR11 const_pointer data_(size_type n = 0) const noexcept { return buffer_ + n; }
    QS_CONSTEXPR11 size_type     size_() const noexcept { return end_ - data_(); }


    template<class... Args>
    QS_CONSTEXPR14 void construct_back_single_(Args&&... args)
    {
        *(this->end_++) = value_type{std::forward<Args>(args)...};
    }

    QS_CONSTEXPR20 void construct_back_n_(size_type n) { assign_n_(n, value_type{}); }

    QS_CONSTEXPR20 void construct_back_n_(size_type n, const_reference x) { assign_n_(n, x); }

    template<class Iterator, class Sentinel>
    QS_CONSTEXPR20 void construct_back_range_(Iterator first, Sentinel last, size_type n)
    {
        assign_range_(first, last, n);
    }

    QS_CONSTEXPR20 void move_range_(pointer from_start, pointer from_end, pointer to)
    {
        auto const n = static_cast<size_t>(from_end - from_start);
        std::memmove(to, from_start, n * sizeof(value_type));
    }

    QS_CONSTEXPR20 void assign_n_(size_type n, const_reference x)
    {
        std::fill_n(this->data_(), n, x);
        this->end_ = this->data_() + n;
    }

    template<class Iterator, class Sentinel>
    QS_CONSTEXPR20 void assign_range_(Iterator first, Sentinel last, size_type)
    {
        this->end_ = std::copy(first, last, this->end_);
    }

    QS_CONSTEXPR20 void destroy_back_(pointer new_end) noexcept { this->end_ = new_end; }

    QS_CONSTEXPR20 void clear_() noexcept { destroy_back_(data_()); }

    using inner_element_t = value_type;
    inner_element_t buffer_[Capacity];
    pointer         end_;
};


template<typename T, std::size_t Capacity>
class inplace_vector : private inplace_vector_base<T, Capacity>
{
    using base = inplace_vector_base<T, Capacity>;

public:
    // Type aliases for convenience and compatibility with standard library containers
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;
    using size_type              = size_t;
    using difference_type        = ptrdiff_t;
    using iterator               = value_type*;
    using const_iterator         = value_type const*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Default constructor
    QS_CONSTEXPR11 inplace_vector() = default;

    // Additional constructors for various initialization scenarios
    QS_CONSTEXPR11 explicit inplace_vector(size_type n)
        : inplace_vector()
    {
        auto guard = make_exception_guard([&] { clear(); });

        if(n > capacity())
            this->throw_bad_alloc_();
        base::construct_back_n_(n);

        guard.complete();
    };

    QS_CONSTEXPR11 inplace_vector(size_type n, const_reference x)
        : inplace_vector()
    {
        auto guard = make_exception_guard([&] { clear(); });

        if(n > capacity())
            this->throw_bad_alloc_();
        base::construct_back_n_(n, x);

        guard.complete();
    }

    // Constructor for initializing from a range of iterators
    template<
        class InputIterator,
        typename std::enable_if<
            is_std_iterator_exact<InputIterator, std::input_iterator_tag, value_type>::value, int>::type = 0>
    QS_CONSTEXPR11 inplace_vector(InputIterator first, InputIterator last)
        : inplace_vector()
    {
        auto guard = make_exception_guard([&] { clear(); });

        for(; first != last; ++first)
            emplace_back(*first);

        guard.complete();
    }

    template<
        class ForwardIterator,
        typename std::enable_if<
            is_std_iterator<ForwardIterator, std::forward_iterator_tag, value_type>::value, int>::type = 0>
    QS_CONSTEXPR11 inplace_vector(ForwardIterator first, ForwardIterator last)
        : inplace_vector()
    {
        auto guard = make_exception_guard([&] { clear(); });

        auto const n = static_cast<size_type>(std::distance(first, last));
        if(n > capacity())
            this->throw_bad_alloc_();
        base::construct_back_range_(first, last, n);

        guard.complete();
    }

    // initializer list constructor and assignment operator
    QS_CONSTEXPR11 inplace_vector& operator=(std::initializer_list<value_type> il)
    {
        assign(il.begin(), il.end());
        return *this;
    }

    // Assign functions for various scenarios

    template<
        class InputIterator,
        typename std::enable_if<
            is_std_iterator_exact<InputIterator, std::input_iterator_tag, value_type>::value, int>::type = 0>
    QS_CONSTEXPR11 void assign(InputIterator first, InputIterator last)
    {
        clear();
        for(; first != last; ++first)
            emplace_back(*first);
    }

    template<
        class ForwardIterator,
        typename std::enable_if<
            is_std_iterator<ForwardIterator, std::forward_iterator_tag, value_type>::value, int>::type = 0>
    QS_CONSTEXPR11 void assign(ForwardIterator first, ForwardIterator last)
    {
        auto const new_size = static_cast<size_type>(std::distance(first, last));
        if(new_size <= capacity()) // better branch prediction on MSVC if this is first
        {
            size_type const curr_size = size();
            if(new_size > curr_size)
            {
                std::copy_n(first, curr_size, base::data_());
                base::construct_back_range_(std::next(first, curr_size), last, new_size - curr_size);
            }
            else
            {
                pointer new_end = std::copy(first, last, base::data_());
                base::destruct_back_(new_end);
            }
        }
        else
            throw_length_error_();
    }

    QS_CONSTEXPR11 void assign(size_type n, const_reference x)
    {
        if(n <= capacity())
        {
            size_type const curr_size = size();
            std::fill_n(base::data_(), std::min(n, curr_size), x);
            if(n > curr_size)
                base::construct_back_n_(n - curr_size, x);
            else
                base::destroy_back_(base::data_(n));
        }
        else
            throw_length_error_();
    }

    QS_CONSTEXPR17 void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); };

    // Iterator functions
    QS_CONSTEXPR14 iterator       begin() noexcept { return wrap_iter(base::data_()); };
    QS_CONSTEXPR14 const_iterator begin() const noexcept { return wrap_iter(base::data_()); };
    QS_CONSTEXPR14 iterator       end() noexcept { return wrap_iter(base::end_); };
    QS_CONSTEXPR14 const_iterator end() const noexcept { return wrap_iter(base::end_); };

    QS_CONSTEXPR14 reverse_iterator       rbegin() noexcept { return reverse_iterator(end()); };
    QS_CONSTEXPR14 const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); };
    QS_CONSTEXPR14 reverse_iterator       rend() noexcept { return reverse_iterator(begin()); };
    QS_CONSTEXPR14 const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); };

    QS_CONSTEXPR14 const_iterator         cbegin() const noexcept { return begin(); };
    QS_CONSTEXPR14 const_iterator         cend() const noexcept { return end(); };
    QS_CONSTEXPR14 const_reverse_iterator crbegin() const noexcept { return rbegin(); };
    QS_CONSTEXPR14 const_reverse_iterator crend() const noexcept { return rend(); };

    // capacity()/size() related functions
    QS_CONSTEXPR11 size_type size() const noexcept { return base::size_(); };
    QS_CONSTEXPR11 size_type capacity() const noexcept { return Capacity; };
    QS_CONSTEXPR11 size_type max_size() const noexcept { return Capacity; };
    QS_CONSTEXPR11 bool      empty() const noexcept { return size() == 0; };

    QS_CONSTEXPR17 void reserve(size_type n) // only for api compatibility
    {
        if(n > capacity())
            throw_bad_alloc_();
    };
    QS_CONSTEXPR17 void shrink_to_fit() noexcept { /*does nothing*/ }; // only for api compatibility

    // Element access functions
    QS_CONSTEXPR14 reference operator[](size_type n) noexcept
    {
        return QS_ASSERT(n < size(), "inplace_vector[] index out of bounds"), *base::data_(n);
    };
    QS_CONSTEXPR14 const_reference operator[](size_type n) const noexcept
    {
        return QS_ASSERT(n < size(), "inplace_vector[] index out of bounds"), *base::data_(n);
    };
    QS_CONSTEXPR14 reference at(size_type n)
    {
        return (n >= size()) ? (throw_bad_alloc_(), *base::data_(0)) : *base::data_(n);
    };
    QS_CONSTEXPR14 const_reference at(size_type n) const
    {
        return (n >= size()) ? (throw_bad_alloc_(), *base::data_(0)) : *base::data_(n);
    };

    QS_CONSTEXPR14 reference front() noexcept
    {
        return QS_ASSERT(!empty(), "front() called on an empty inplace_vector"), *data();
    };
    QS_CONSTEXPR14 const_reference front() const noexcept
    {
        return QS_ASSERT(!empty(), "front() called on an empty inplace_vector"), *data();
    };
    QS_CONSTEXPR14 reference back() noexcept
    {
        return QS_ASSERT(!empty(), "back() called on an empty inplace_vector"), *(end() - 1);
    };
    QS_CONSTEXPR14 const_reference back() const noexcept
    {
        return QS_ASSERT(!empty(), "back() called on an empty inplace_vector"), *(end() - 1);
    };

    QS_CONSTEXPR14 pointer       data() noexcept { return base::data_(); };
    QS_CONSTEXPR14 const_pointer data() const noexcept { return base::data_(); };

    // Modifiers
    QS_CONSTEXPR11 void push_back(value_type const& x);
    QS_CONSTEXPR11 void push_back(value_type&& x);

    // new in inplace_vector, does not throw, does nothing if the vector is full
    QS_CONSTEXPR11 void try_push_back(value_type const& x);
    QS_CONSTEXPR11 void try_push_back(value_type&& x);

    QS_CONSTEXPR11 void unchecked_push_back(value_type const& x);
    QS_CONSTEXPR11 void unchecked_push_back(value_type&& x);

    template<class... Args>
    QS_CONSTEXPR17 reference emplace_back(Args&&... args);

    // new in inplace_vector, does not throw, instead returns a pointer to the inserted element
    template<class... Args>
    QS_CONSTEXPR17 pointer try_emplace_back(Args&&... args);

    template<class... Args>
    QS_CONSTEXPR17 pointer unchecked_emplace_back(Args&&... args);

    QS_CONSTEXPR11 void pop_back()
    {
        QS_ASSERT(!empty(), "inplace_vector::pop_back called on an empty vector");
        base::destroy_back_(base::end_ - 1);
    }

    template<class... Args>
    QS_CONSTEXPR17 iterator emplace(const_iterator position, Args&&... args);

    QS_CONSTEXPR11 iterator insert(const_iterator position, value_type const& x);
    QS_CONSTEXPR11 iterator insert(const_iterator position, value_type&& x);
    QS_CONSTEXPR11 iterator insert(const_iterator position, size_type n, value_type const& x);

    template<class InputIterator,
             typename std::enable_if<
                 is_std_iterator<InputIterator, std::input_iterator_tag, value_type>::value, int>::type = 0>
    QS_CONSTEXPR11 iterator insert(const_iterator position, InputIterator first, InputIterator last);

    QS_CONSTEXPR11 iterator insert(const_iterator position, std::initializer_list<value_type> il);

    QS_CONSTEXPR11 iterator erase(const_iterator position);
    QS_CONSTEXPR11 iterator erase(const_iterator first, const_iterator last);

    QS_CONSTEXPR11 void clear() noexcept { base::clear_(); }

    QS_CONSTEXPR11 void resize(size_type new_size)
    {
        size_type const curr_size = size();
        if(curr_size < new_size)
            this->append_(new_size - curr_size);
        else if(curr_size > new_size)
            base::destroy_back_(base::data_() + new_size);
    }

    QS_CONSTEXPR11 void resize(size_type new_size, const_reference x)
    {
        size_type const curr_size = size();
        if(curr_size < new_size)
            this->append_(new_size - curr_size, x);
        else if(curr_size > new_size)
            base::destroy_back_(base::data_() + new_size);
    }

    QS_CONSTEXPR17 void swap(inplace_vector& other) noexcept; // C++17x

private:
    // QS_CONSTEXPR11 iterator       wrap_iter(pointer pos) const noexcept { return pos; }
    // QS_CONSTEXPR11 const_iterator wrap_iter(const_pointer pos) const noexcept { return pos; }

    // QS_CONSTEXPR11 pointer        unwrap_iter(iterator it) const noexcept { return it; }
    // QS_CONSTEXPR11 const_iterator unwrap_iter(const_iterator it) const noexcept { return it; }

    QS_CONSTEXPR11 void throw_bad_alloc_() const { throw std::bad_alloc(); }
    QS_CONSTEXPR11 void throw_out_of_range_() const { throw std::out_of_range("inplace_vector"); }
    QS_CONSTEXPR11 void throw_length_error_() const { throw std::length_error("inplace_vector"); }

    QS_CONSTEXPR11 void append_(size_type n)
    {
        if(capacity() - size() >= n)
            base::construct_back_n_(n);
        else
            throw_bad_alloc_();
    }

    QS_CONSTEXPR11 void append_(size_type n, const_reference x)
    {
        if(capacity() - size() >= n)
            base::construct_back_n_(n, x);
        else
            throw_bad_alloc_();
    }
};


QS_NAMESPACE_END


#endif // QS_INPLACE_VECTOR_H_

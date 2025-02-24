#ifndef QS_EXCEPTION_GUARD_H_
#define QS_EXCEPTION_GUARD_H_


#include <qs/base.h>
#include <utility>


QS_NAMESPACE_BEGIN

namespace detail
{
    template<class Rollback, bool HasExceptions>
    struct exception_guard_impl;

    template<class Rollback> // HasExceptions = true
    struct exception_guard_impl<Rollback, true>
    {
        exception_guard_impl() = delete;

        QS_CONSTEXPR11 explicit exception_guard_impl(Rollback rollback)
            : rollback_(std::move(rollback)),
              complete_(false) {};

        QS_CONSTEXPR11 exception_guard_impl(exception_guard_impl&& other) QS_NOEXCEPT_B(
            std::is_nothrow_move_constructible<Rollback>::value)
            : rollback_(std::move(other.rollback_)),
              complete_(std::exchange(other.complete_, true))
        {}

        exception_guard_impl(exception_guard_impl const&)            = delete;
        exception_guard_impl& operator=(exception_guard_impl const&) = delete;
        exception_guard_impl& operator=(exception_guard_impl&&)      = delete;

        QS_CONSTEXPR14 void complete() QS_NOEXCEPT { complete_ = true; }

        QS_CONSTEXPR20 ~exception_guard_impl()
        {
            if(!complete_)
                rollback_();
        }

    private:
        Rollback rollback_;
        bool     complete_;
    };

    template<class Rollback> // HasExceptions = false
    struct exception_guard_impl<Rollback, false>
    {
        exception_guard_impl() = delete;

        QS_CONSTEXPR11 explicit exception_guard_impl(Rollback)
            : complete_(false) {};

        exception_guard_impl(exception_guard_impl const&)            = delete;
        exception_guard_impl& operator=(exception_guard_impl const&) = delete;

        QS_CONSTEXPR11 exception_guard_impl(exception_guard_impl&& other) QS_NOEXCEPT_B(
            std::is_nothrow_move_constructible<Rollback>::value)
            : complete_(std::exchange(other.complete_, true))
        {}
        exception_guard_impl& operator=(exception_guard_impl&&) = delete;

        QS_CONSTEXPR14 void complete() QS_NOEXCEPT { complete_ = true; }

        QS_CONSTEXPR20 ~exception_guard_impl()
        {
            QS_ASSERT(complete_, "exception_guard not completed with no exceptions");
        }

    private:
        bool complete_;
    };

} // namespace detail

template<class RollBackF>
struct exception_guard : detail::exception_guard_impl<RollBackF, config::has_noexcept>
{
    using detail::exception_guard_impl<RollBackF, config::has_noexcept>::exception_guard_impl;
};

template<class Rollback>
inline QS_CONSTEXPR11 exception_guard<Rollback> make_exception_guard(Rollback rollback)
{
    return exception_guard<Rollback>(std::move(rollback));
}

QS_NAMESPACE_END

#endif // QS_EXCEPTION_GUARD_H_

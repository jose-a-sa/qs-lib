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
              commit_(false) {};

        QS_CONSTEXPR11 exception_guard_impl(exception_guard_impl&& other) noexcept(
            std::is_nothrow_move_constructible<Rollback>::value)
            : rollback_(std::move(other.rollback_)),
              commit_(std::exchange(other.commit_, true))
        {}

        exception_guard_impl(exception_guard_impl const&)            = delete;
        exception_guard_impl& operator=(exception_guard_impl const&) = delete;
        exception_guard_impl& operator=(exception_guard_impl&&)      = delete;

        QS_CONSTEXPR14 void complete() noexcept { commit_ = true; }

        QS_CONSTEXPR20 ~exception_guard_impl()
        {
            if(!commit_)
                rollback_();
        }

    private:
        Rollback rollback_;
        bool     commit_;
    };

    template<class Rollback> // HasExceptions = false
    struct exception_guard_impl<Rollback, false>
    {
        exception_guard_impl() = delete;

        QS_CONSTEXPR11 explicit exception_guard_impl(Rollback)
            : commit_(false) {};

        exception_guard_impl(exception_guard_impl const&)            = delete;
        exception_guard_impl& operator=(exception_guard_impl const&) = delete;

        QS_CONSTEXPR11 exception_guard_impl(exception_guard_impl&& other) noexcept(
            std::is_nothrow_move_constructible<Rollback>::value)
            : commit_(std::exchange(other.commit_, true))
        {}
        exception_guard_impl& operator=(exception_guard_impl&&) = delete;

        QS_CONSTEXPR14 void complete() noexcept { commit_ = true; }

        QS_CONSTEXPR20 ~exception_guard_impl()
        {
            QS_ASSERT(commit_, "exception_guard not completed with no exceptions");
        }

    private:
        bool commit_;
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

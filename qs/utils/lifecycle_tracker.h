#ifndef QS_LIFECYCLE_TRACKER_H_
#define QS_LIFECYCLE_TRACKER_H_

#include <array>
#include <qs/base.h>
#include <qs/utils/demangler.h>
#include <string>
#include <string_view>
#include <tuple>

#if QS_HAS(FMTLIB)
#include <fmt/core.h>
#endif


QS_NAMESPACE_BEGIN


enum class lifecycle_event
{
    Constructor     = 0,
    CopyConstructor = 1,
    MoveConstructor = 2,
    CopyAssignment  = 3,
    MoveAssignment  = 4,
    Destructor      = 5
};


struct lifecycle_counters
{
    size_t constructor      = 0;
    size_t copy_constructor = 0;
    size_t move_constructor = 0;
    size_t copy_assignment  = 0;
    size_t move_assignment  = 0;
    size_t destructor       = 0;

    constexpr size_t total_constructed() const QS_NOEXCEPT
    {
        return constructor + copy_constructor + move_constructor;
    };

    constexpr size_t total_assigned() const QS_NOEXCEPT { return copy_assignment + move_assignment; };

    constexpr ptrdiff_t alive() const QS_NOEXCEPT
    {
        return static_cast<ptrdiff_t>(total_constructed()) - static_cast<ptrdiff_t>(destructor);
    };

    constexpr bool operator==(lifecycle_counters const& rhs) const QS_NOEXCEPT
    {
        return constructor == rhs.constructor && copy_constructor == rhs.copy_constructor &&
               move_constructor == rhs.move_constructor && copy_assignment == rhs.copy_assignment &&
               move_assignment == rhs.move_assignment && destructor == rhs.destructor;
    }
};


template<class T, size_t Uuid>
struct lifecycle_tracker_default_logger
{
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;

    static constexpr size_t uuid = Uuid;

    template<lifecycle_event Cnt>
    constexpr void log_event(const_reference /*self*/, lifecycle_counters const& /*cnts*/,
                             std::string_view const type_name) const
    {
        std::printf(log_event_format<Cnt>(), static_cast<int>(type_name.size()), type_name.data(),
                    static_cast<int>(type_name.size()), type_name.data());
        std::printf("\n");
    }

    constexpr void print_counters(lifecycle_counters const& cnts, std::string_view const type_name) const
    {
        std::printf(log_counters_format(), static_cast<int>(type_name.size()), type_name.data(), uuid,
                    cnts.total_constructed(), cnts.constructor, cnts.copy_constructor, cnts.move_constructor,
                    cnts.total_assigned(), cnts.copy_assignment, cnts.move_assignment, cnts.destructor, cnts.alive());
    }

protected:
    template<lifecycle_event Cnt>
    static const char* log_event_format()
    {
        static constexpr std::array<const char*, 6> event_fmt_map{"%.*s(...)",      "%.*s(%.*s const&)", "%.*s(%.*s&&)",
                                                                  "=(%.*s const&)", "=(%.*s&&)",         "~%.*s()"};
        return event_fmt_map[static_cast<size_t>(Cnt)];
    }

    static const char* log_counters_format()
    {
        return "Lifecycle tracker [type: %.*s, uuid: %zu]\n"
               " * constructor (ctor/copy/move) : %3zu (%zu/%zu/%zu)\n"
               " * assign (copy/move)           : %3zu (%zu/%zu)\n"
               " * destructor (alive)           : %3zu (%td)\n";
    }
};


#if QS_HAS(FMTLIB)

template<class T, size_t Uuid>
struct lifecycle_tracker_fmtlib_logger
{
    using value_type      = T;
    using reference       = value_type&;
    using const_reference = value_type const&;
    using pointer         = value_type*;
    using const_pointer   = value_type const*;

    constexpr size_t uuid = Uuid;

    template<lifecycle_event Cnt>
    constexpr void log_event(const_reference /*self*/, lifecycle_counters const& /*cnts*/,
                             std::string_view const type_name) const
    {
        fmt::print(log_event_format<Cnt>(), type_name, type_name);
        fmt::print("\n");
    }

    constexpr void print_counters(lifecycle_counters const& cnts, std::string_view const type_name) const
    {
        fmt::print(log_counters_format(), type_name, Uuid, cnts.total_constructed(), cnts.constructor,
                   cnts.copy_constructor, cnts.move_constructor, cnts.total_assigned(), cnts.copy_assignment,
                   cnts.move_assignment, cnts.destructor, cnts.alive());
    }

protected:
    template<lifecycle_event Cnt>
    static const char* log_event_format()
    {
        static constexpr std::array<const char*, 6> event_fmt_map{"{}(...)",      "{}({} const&)", "{}({}&&)",
                                                                  "=({} const&)", "=({}&&)",       "~{}()"};
        return event_fmt_map[static_cast<size_t>(Cnt)];
    }

    static const char* log_counters_format()
    {
        return "Lifecycle tracker [type: {}, uuid: {}]\n"
               " * constructor (ctor/copy/move) : {:>3} ({}/{}/{})\n"
               " * assign (copy/move)           : {:>3} ({}/{})\n"
               " * destructor (alive).          : {:>3} ({})\n";
    }
};

template<class T, size_t Uuid>
struct lifecycle_tracker_logger : lifecycle_tracker_fmtlib_logger<T, Uuid>
{};

#else

template<class T, size_t Uuid>
struct lifecycle_tracker_logger : lifecycle_tracker_default_logger<T, Uuid>
{};

#endif


namespace detail
{
    template<class Derived, class T, size_t Uuid>
    class lifecycle_tracker_base
    {
    public:
        lifecycle_tracker_base() { log_and_increment<lifecycle_event::Constructor>(); }

        lifecycle_tracker_base(lifecycle_tracker_base const&) { log_and_increment<lifecycle_event::CopyConstructor>(); }

        lifecycle_tracker_base& operator=(lifecycle_tracker_base const& other)
        {
            if(this != std::addressof(other))
                log_and_increment<lifecycle_event::CopyAssignment>();
            return *this;
        }

        lifecycle_tracker_base(lifecycle_tracker_base&&) QS_NOEXCEPT
        {
            log_and_increment<lifecycle_event::MoveConstructor>();
        }

        lifecycle_tracker_base& operator=(lifecycle_tracker_base&& other) QS_NOEXCEPT
        {
            if(this != std::addressof(other))
                log_and_increment<lifecycle_event::MoveAssignment>();
            return *this;
        }

        ~lifecycle_tracker_base() { log_and_increment<lifecycle_event::Destructor>(); }

        static constexpr void reset_counters() { counters_ = {}; }

        static constexpr void print_counters() { logger_.print_counters(get_counters(), get_type_name()); }

        static constexpr lifecycle_counters const& get_counters() { return counters_; }

        static constexpr void set_type_name(std::string_view const name) { type_name_ = name; }

        static constexpr std::string const& get_type_name()
        {
            if(type_name_.empty())
                set_type_name(demangler<T>::get());
            return type_name_;
        }

    protected:
        constexpr Derived*       self() { return static_cast<Derived*>(this); }
        constexpr Derived const* self() const { return static_cast<Derived const*>(this); }

    private:
        inline static lifecycle_counters                counters_{};
        inline static std::string                       type_name_{};
        inline static lifecycle_tracker_logger<T, Uuid> logger_{};

        template<lifecycle_event Cnt>
        static constexpr size_t& get_counter()
        {
            return std::get<static_cast<size_t>(Cnt)>(std::tie(counters_.constructor, counters_.copy_constructor,
                                                               counters_.move_constructor, counters_.copy_assignment,
                                                               counters_.move_assignment, counters_.destructor));
        }

        template<lifecycle_event Cnt>
        constexpr void log_and_increment() const
        {
            logger_.template log_event<Cnt>(*static_cast<T const*>(self()), get_counters(), get_type_name());
            ++get_counter<Cnt>();
        }
    };

} // namespace detail


template<class T, size_t Uuid = 0>
class lifecycle_tracker : public T, public detail::lifecycle_tracker_base<lifecycle_tracker<T, Uuid>, T, Uuid>
{
    using tracker = detail::lifecycle_tracker_base<lifecycle_tracker<T, Uuid>, T, Uuid>;

public:
    using T::T;
    using tracker::get_counters;
    using tracker::get_type_name;
    using tracker::print_counters;
    using tracker::reset_counters;
    using tracker::set_type_name;
};

#if QS_HAS(FMTLIB)
template<class T, size_t Uuid>
struct fmt::formatter<lifecycle_tracker<T, Uuid>> : fmt::formatter<T>
{};
#endif


QS_NAMESPACE_END


#endif // QS_LIFECYCLE_TRACKER_H_

#ifndef QS_CONTAINERS_ENUMMAP_H
#define QS_CONTAINERS_ENUMMAP_H

#include <qs/base.h>

#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

// This code is still in development

QS_NAMESPACE_BEGIN

template<typename T>
class enum_map
{
public:
    // Delete copy and move constructors and assignment operators
    enum_map(enum_map const&)            = delete;
    enum_map(enum_map&&)                 = delete;
    enum_map& operator=(enum_map const&) = delete;
    enum_map& operator=(enum_map&&)      = delete;
    ~enum_map()                          = default;

private:
    using key_list_impl_t = std::vector<std::string_view>; // List of keys
    using enum_map_impl_t = std::unordered_map<T, key_list_impl_t>; // Map from enum to keys
    using key_map_impl_t  = std::unordered_map<std::string_view, typename enum_map_impl_t::const_iterator>; // Map from key to enum

    class impl
    {
    public:
        impl() {};

        // Get the enum value associated with a key
        constexpr std::optional<T> get(std::string_view key) const
        {
            auto it = m_keyMap.find(key);
            if(it == m_keyMap.end())
                return {};
            return it->second->first;
        }

        // Get the list of keys associated with an enum value
        std::optional<std::reference_wrapper<key_list_impl_t const>> get_keys(T const& value) const
        {
            auto it = m_enumMap.find(value);
            if(it == m_enumMap.end())
                return {};
            return it->second;
        }

    private:
        enum_map_impl_t m_enumMap; // Map from enum to keys
        key_map_impl_t  m_keyMap;  // Map from key to enum
    };

    // Singleton instance of impl
    static impl& instance()
    {
        static impl instance;
        return instance;
    }

public:
    // Static method to get the enum value associated with a key
    static constexpr std::optional<T> get(std::string_view key) { return instance().get(key); }

    // Static method to get the list of keys associated with an enum value
    static std::optional<std::reference_wrapper<key_list_impl_t const>> get_keys(T const& value)
    {
        return instance().get_keys(value);
    }
};

QS_NAMESPACE_END

// Macro to begin enum definition
#define ENUM_BEGIN(EnumType)                                                                                           \
    template<>                                                                                                         \
    inline constexpr ::quasar::enum_map<EnumType>::impl::impl() noexcept                                               \
    {

// Macro to define enum values and their associated keys
#define ENUM_R(EnumV, ...)                                                                                             \
    {                                                                                                                  \
        std::vector<std::string_view> res{__VA_ARGS__};                                                                \
        auto [it, _] = m_enumMap.emplace(EnumV, res);                                                                  \
        for(auto&& s: res)                                                                                             \
            m_keyMap.emplace(s, it);                                                                                   \
    };

// Macro to end enum definition
#define ENUM_END(EnumType)                                                                                             \
    }                                                                                                                  \
    ;

// Macro to define ostream operator for enum
#define ENUM_OSTREAM(EnumType)                                                                                         \
    std::ostream& operator<<(std::ostream& out, EnumType const& val)                                                   \
    {                                                                                                                  \
        std::vector<std::string_view> vec = ::quasar::enum_map<EnumType>::get_keys(val);                               \
        if(vec.empty())                                                                                                \
            return out << static_cast<int>(val);                                                                       \
        return out << vec.front();                                                                                     \
    };

#endif // QS_CONTAINERS_ENUMMAP_H

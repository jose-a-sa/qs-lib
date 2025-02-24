
#include <qs/base.h>
#include <string>
#include <string_view>

#if defined(__GNUC__) || defined(__clang__)
#include <cxxabi.h>
#include <memory>
#elif defined(_MSC_VER) // MSVC
#include <DbgHelp.h>
#include <windows.h>
#pragma comment(lib, "Dbghelp.lib")
#else
#endif

QS_NAMESPACE_BEGIN

template<typename T>
class demangler
{
public:
    static std::string const& get()
    {
        if(type_name_.empty())
            set_default();
        return type_name_;
    }
    static void set(std::string_view const name) { type_name_ = name; }

private:
    inline static std::string type_name_{};

    static void set_default()
    {
#if defined(__GNUC__) || defined(__clang__)
        int status = 0;

        std::unique_ptr<char> demangled;
        demangled.reset(abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status));

        set(status == 0 ? demangled.get() : typeid(T).name());
#else
        set(typeid(T).name());
#endif
    }
};

QS_NAMESPACE_END

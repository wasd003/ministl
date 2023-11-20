#include <iostream>
#include <source_location>

template<typename... Ts>
static inline void print(const Ts&... args) {
    ((std::cout << args), ...);
}

#ifdef DEBUG
// https://stackoverflow.com/questions/14805192/c-variadic-template-function-parameter-with-default-value
// https://stackoverflow.com/questions/40951697/what-are-template-deduction-guides-and-when-should-we-use-them
template <typename... Ts>
struct debug
{
    debug(Ts&&... args, 
        const std::source_location& loc = std::source_location::current())
    {
        print(loc.function_name(), ":", loc.line());
        print(" [", args..., "]\n");
    }
};

template <typename... Ts>
debug(Ts&&...args) -> debug<Ts...>;
#else
template <typename... Ts>
static inline void debug(Ts&&... args) {}
#endif

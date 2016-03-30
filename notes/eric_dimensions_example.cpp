#include <cstddef>

struct Dim {
    constexpr Dim() = default;
    constexpr Dim(std::size_t d) : is_dyn_(false), dim_(d) {}
    constexpr bool is_dynamic() const { return is_dyn_; }
    constexpr bool value() const { return dim_; }
private:
    bool is_dyn_ = true;
    std::size_t dim_ = 0;
};

template<Dim... ds>
struct dimension {

};

constexpr Dim dyn {}; // see customization points paper for how to
                      // put this in a header without ODR violations.

int main() {
    dimension<dyn, 3, 42, dyn> d;
}


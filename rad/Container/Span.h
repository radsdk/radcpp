#pragma once

#include <rad/Core/Platform.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <optional>
#include <type_traits>
#include <vector>

namespace rad
{

/// Span refers to a contiguous sequence of objects, inspired by:
/// https://llvm.org/doxygen/ArrayRef_8h_source.html.
/// The design is more flexible with less restrictions compared to std::span/gsl::span.
template<typename T>
class [[nodiscard]] Span {
public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

private:
    /// The start of the array, in an external buffer.
    const T* m_data = nullptr;

    /// The number of elements.
    size_type m_count = 0;

public:
    /// @name Constructors
    /// @{

    /// Construct an empty Span.
    /*implicit*/ Span() = default;

    /// Construct an empty Span from std::nullopt.
    /*implicit*/ Span(std::nullopt_t) {}

    /// Construct an Span from a single element.
    /*implicit*/ Span(const T& t)
        : m_data(&t), m_count(1) {}

    /// Construct an Span from a pointer and count.
    constexpr /*implicit*/ Span(const T* data, size_t count)
        : m_data(data), m_count(count) {}

    /// Construct an Span from a range.
    constexpr Span(const T* begin, const T* end)
        : m_data(begin), m_count(end - begin)
    {
        assert(begin <= end);
    }

    /// Construct an Span from a std::vector.
    template<typename A>
    /*implicit*/ Span(const std::vector<T, A>& vec)
        : m_data(vec.data()), m_count(vec.size()) {}

    /// Construct an Span from a std::array
    template <size_t N>
    /*implicit*/ constexpr Span(const std::array<T, N>& arr)
        : m_data(arr.data()), m_count(N) {}

    /// Construct an Span from a C array.
    template <size_t N>
    /*implicit*/ constexpr Span(const T(&arr)[N]) : m_data(arr), m_count(N) {}

    /// Construct an Span from a std::initializer_list.
#if RAD_GNUC_PREREQ(9, 0, 0)
// Disable gcc's warning in this constructor.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winit-list-lifetime"
#endif
    constexpr /*implicit*/ Span(const std::initializer_list<T>& list)
        : m_data(list.begin() == list.end() ? (T*)nullptr : list.begin()),
        m_count(list.size()) {}
#if RAD_GNUC_PREREQ(9, 0, 0)
#pragma GCC diagnostic pop
#endif

    template<typename Container>
        requires std::ranges::contiguous_range<Container>
    Span(const Container& c) :
        m_data(std::ranges::data(c)),
        m_count(std::ranges::size(c))
    {}

    /// Construct an Span<const T*> from Span<T*>. This uses SFINAE to
    /// ensure that only ArrayRefs of pointers can be converted.
    template <typename U>
    Span(const Span<U*>& other,
        std::enable_if_t<std::is_convertible<U* const*, T const*>::value>
        * = nullptr)
        : m_data(other.data()), m_count(other.size()) {}

    /// Construct an Span<const T*> from std::vector<T*>. This uses SFINAE
    /// to ensure that only vectors of pointers can be converted.
    template <typename U, typename Allocator>
    Span(const std::vector<U*, Allocator>& vec,
        std::enable_if_t<std::is_convertible<U* const*, T const*>::value>
        * = nullptr)
        : m_data(vec.data()), m_count(vec.size()) {}

    /// @}
    /// @name Simple Operations
    /// @{

    iterator begin() const { return m_data; }
    iterator end() const { return m_data + m_count; }

    reverse_iterator rbegin() const { return reverse_iterator(end()); }
    reverse_iterator rend() const { return reverse_iterator(begin()); }

    /// empty - Check if the array is empty.
    bool empty() const { return m_count == 0; }

    const T* data() const { return m_data; }

    /// size - Get the array size.
    size_t size() const { return m_count; }
    size_t size32() const { return static_cast<uint32_t>(m_count); }

    /// front - Get the first element.
    const T& front() const {
        assert(!empty());
        return m_data[0];
    }

    /// back - Get the last element.
    const T& back() const {
        assert(!empty());
        return m_data[m_count - 1];
    }

    /// equals - Check for element-wise equality.
    bool equals(Span rhs) const {
        if (m_count != rhs.m_count) {
            return false;
        }
        return std::equal(begin(), end(), rhs.begin());
    }

    /// slice(n, m) - Chop off the first n elements of the array, and keep m
    /// elements in the array.
    Span<T> slice(size_t n, size_t m) const {
        assert(n + m <= size());
        return Span<T>(data() + n, m);
    }

    /// slice(n) - Chop off the first n elements of the array.
    Span<T> slice(size_t n) const { return slice(n, size() - n); }

    /// Drop the first \p n elements of the array.
    Span<T> drop_front(size_t n = 1) const {
        assert(size() >= n && "Dropping more elements than exist");
        return slice(n, size() - n);
    }

    /// Drop the last \p n elements of the array.
    Span<T> drop_back(size_t n = 1) const {
        assert(size() >= n && "Dropping more elements than exist");
        return slice(0, size() - n);
    }

    /// Return a copy of *this with the first n elements satisfying the
    /// given predicate removed.
    template <class Predicate> Span<T> drop_while(Predicate pred) const {
        return Span<T>(std::find_if_not(begin(), end(), pred), end());
    }

    /// Return a copy of *this with the first n elements not satisfying
    /// the given predicate removed.
    template <class Predicate> Span<T> drop_until(Predicate pred) const {
        return Span<T>(std::find_if(begin(), end(), pred), end());
    }

    /// Return a copy of *this with only the first \p n elements.
    Span<T> take_front(size_t n = 1) const {
        if (n >= size())
            return *this;
        return drop_back(size() - n);
    }

    /// Return a copy of *this with only the last \p n elements.
    Span<T> take_back(size_t n = 1) const {
        if (n >= size())
            return *this;
        return drop_front(size() - n);
    }

    /// Return the first n elements of this Array that satisfy the given
    /// predicate.
    template <class Predicate> Span<T> take_while(Predicate pred) const {
        return Span<T>(begin(), std::find_if_not(begin(), end(), pred));
    }

    /// Return the first n elements of this Array that don't satisfy the
    /// given predicate.
    template <class Predicate> Span<T> take_until(Predicate pred) const {
        return Span<T>(begin(), std::find_if(begin(), end(), pred));
    }

    /// @}
    /// @name Operator Overloads
    /// @{
    const T& operator[](size_t index) const {
        assert(index < m_count);
        return m_data[index];
    }

    /// Disallow accidental assignment from a temporary.
    ///
    /// The declaration here is extra complicated so that "arrayRef = {}"
    /// continues to select the move assignment operator.
    template <typename U>
    std::enable_if_t<std::is_same<U, T>::value, Span<T>>&
        operator=(U&&) = delete;

    /// Disallow accidental assignment from a temporary.
    ///
    /// The declaration here is extra complicated so that "arrayRef = {}"
    /// continues to select the move assignment operator.
    template <typename U>
    std::enable_if_t<std::is_same<U, T>::value, Span<T>>&
        operator=(std::initializer_list<U>) = delete;

    /// @}
    /// @name Conversion operators
    /// @{
    operator std::vector<T>() const {
        return std::vector<T>(m_data, m_data + m_count);
    }

    /// @}
}; // class Span

} // namespace rad

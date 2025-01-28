#ifndef QS_INPLACE_GRAPH_H_
#define QS_INPLACE_GRAPH_H_

#include <qs/config.h>

#include <array>


QS_NAMESPACE_BEGIN


template<class Adj, std::size_t Nodes, std::size_t Edges>
class inplace_graph
{
public:
    using value_type = Adj;
    using index_type = int32_t;

    constexpr inplace_graph() { head_.fill(-1); }

    constexpr void clear()
    {
        size_ = {};
        head_.fill(-1);
    }

    template<class V, class = std::enable_if_t<std::is_convertible<V, value_type>::value>>
    constexpr void add_edge(index_type u, V&& v)
    {
        edge_to_[size_] = std::forward<V>(v);
        prev_[size_]    = head_[u];
        head_[u]        = size_++;
    }

    struct vertex_adjacency_list
    {
        class const_iterator
        {
        public:
            constexpr const_iterator(inplace_graph const& g, index_type head)
                : g_(g),
                  current_{head}
            {}
            constexpr bool operator!=(const_iterator const& other) const { return current_ != other.current_; }
            constexpr value_type const& operator*() const { return g_.edge_to_[current_]; }
            constexpr const_iterator&   operator++()
            {
                current_ = g_.prev_[current_];
                return *this;
            }

        private:
            inplace_graph const& g_;
            index_type          current_;
        };

        constexpr vertex_adjacency_list(inplace_graph const& g, index_type vertex)
            : g_(g),
              vertex_{vertex}
        {}
        constexpr const_iterator begin() const { return const_iterator(g_, g_.head_[vertex_]); }
        constexpr const_iterator end() const { return const_iterator(g_, -1); }

    private:
        inplace_graph const& g_;
        index_type          vertex_;
    };

    constexpr vertex_adjacency_list edges(int from) const { return vertex_adjacency_list(*this, from); }
    constexpr auto             operator()(int from) const { return edges(from); }

private:
    std::array<value_type, Edges> edge_to_{};
    std::array<index_type, Edges> prev_{};
    std::array<index_type, Nodes> head_{};
    index_type                    size_{};
};


QS_NAMESPACE_END

#endif // QS_INPLACE_GRAPH_H_

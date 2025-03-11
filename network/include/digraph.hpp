//
// Digraph.hpp
//
// Directional Graph Data Structure
//

#ifndef __GPW_FOUNDATION_DIGRAPH__
#define __GPW_FOUNDATION_DIGRAPH__

#include "node.hpp"

#include <iterator>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>

namespace gpw::foundation {

/*******************************************************************************
 *
 * @class digraph
 *
 */
template <typename T> class digraph {
private:
    using node_ptr = node<T>*;

    std::list<node<T>> _nodes;

    using node_itr       = typename std::list<node<T>>::iterator;
    using const_node_itr = typename std::list<node<T>>::const_iterator;

public:
    digraph () {}
    virtual ~digraph () {}

    void
    create_node (const std::string& label, const T& data = T()) {
        // Ignore if the label already exists in the list of nodes.
        if (node_with_label (label) != _nodes.end()) return;

        _nodes.emplace_back (node<T>{label, data});
    }

    void
    remove_node (const std::string& label) {
        // Remove all connections to this node
        for (auto& node : _nodes) {
            node.disconnect (label);
        }

        _nodes.remove_if ([&label] (auto& node) { return node.label() == label; });
    }

    void
    connect_node (const std::string& hl, const std::string& tl) {
        auto head = node_with_label (hl);
        auto tail = node_with_label (tl);

        if (head == _nodes.end() || tail == _nodes.end()) return;

        head->connect (*tail);
    }

    bool
    is_connected (const std::string& hl, const std::string& tl) const {
        auto head = std::as_const (*this).node_with_label (hl);
        auto tail = node_with_label (tl);

        if (head == _nodes.end() || tail == _nodes.end()) return false;

        return head->is_connected (*tail);
    }

    size_t
    size () const {
        return _nodes.size();
    }

    size_t
    count_connections () const {
        return std::accumulate (
            _nodes.cbegin(),
            _nodes.cend(),
            0,
            [] (const size_t& acc, const auto& node) { return node.count_connections() + acc; }
        );
    }

    std::string
    description () const {
        std::stringstream strm;

        for (const auto& node : _nodes) {
            strm << node.label() << " : ";
            for (const auto& edge : node.edges()) {
                strm << edge->label() << ", ";
            }
            strm << '\n';
        }

        return strm.str();
    }

    // Find strongly connected components using depth-first forest
    //
    // The algorithm was proposed by R.E.Tarjan and the implementation is based on
    // the Christmas lecture by D.E.Knuth in 2024.
    //
    using arc  = std::pair<std::string, std::string>;
    using arcs = std::vector<arc>;

    std::list<std::list<node<T>>>
    strongly_connected_components () const {
        std::list<std::list<node<T>>> scc;

        std::list<node<T>> nodes = _nodes;
        arcs               tree_arcs;
        arcs               back_arcs;
        arcs               loops;
        arcs               fwd_arcs;
        arcs               cross_arcs;

        find_strongly_connected_components (
            scc, nodes, tree_arcs, back_arcs, loops, fwd_arcs, cross_arcs
        );

        return scc;
    }

private:
    node_itr
    node_with_label (const std::string& label) {
        auto cit = static_cast<const digraph*> (this)->node_with_label (label);
        return std::next (_nodes.begin(), std::distance (_nodes.cbegin(), cit));
    }

    const_node_itr
    node_with_label (const std::string& label) const {
        return std::find_if (_nodes.cbegin(), _nodes.cend(), [&label] (const auto& node) {
            return node.label() == label;
        });
    }

    // Depth-first forest algorithm
    //
    // Begining from
    void
    find_strongly_connected_components (
        std::list<std::list<node<T>>>& scc,
        std::list<node<T>>&            nodes,
        arcs&                          tree_arcs,
        arcs&                          back_arcs,
        arcs&                          loops,
        arcs&                          fwd_arcs,
        arcs&                          cross_arcs
    ) const {}
};

}  // namespace gpw::foundation

#endif

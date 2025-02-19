//
// Digraph.hpp
//
// Directional Graph Data Structure
//

#ifndef __GPW_FOUNDATION_DIGRAPH__
#define __GPW_FOUNDATION_DIGRAPH__

#include "node.hpp"

#include <memory>
#include <numeric>
#include <string>

namespace gpw::foundation {

/*******************************************************************************
 *
 * @class digraph
 *
 */
template <typename T> class digraph {
private:
    using node_ptr = node<T>*;

    // Because this graph is responsible for the resource management for all of
    // its nodes, this class has a list of unique_ptrs of the nodes.
    // The connections between the nodes are saved using raw pointers, which
    // do not state the ownership between them.
    // Each node has pointers to other nodes connected to it.
    std::list<std::unique_ptr<node<T>>> _nodes;

public:
    digraph () {}
    virtual ~digraph () {}

    void
    create_node (const std::string& label, const T& data = T()) {
        // Ignore if the label already exists in the list of nodes.
        if (node_with_label (label) != nullptr) return;

        _nodes.emplace_back (std::make_unique<node<T>> (label, data));
    }

    void
    remove_node (const std::string& label) {
        // Remove all connections to this node
        for (auto& ptr : _nodes) {
            ptr->disconnect (label);
        }

        _nodes.remove_if ([&label] (auto& node) { return node->label() == label; });
    }

    void
    connect_node (const std::string& hl, const std::string& tl) {
        auto head_ptr = node_with_label (hl);
        auto tail_ptr = node_with_label (tl);

        if (head_ptr == nullptr || tail_ptr == nullptr) return;

        head_ptr->connect (*tail_ptr);
    }

    bool
    is_connected (const std::string& hl, const std::string& tl) const {
        auto head_ptr = node_with_label (hl);
        auto tail_ptr = node_with_label (tl);

        if (head_ptr == nullptr || tail_ptr == nullptr) return false;

        return head_ptr->is_connected (*tail_ptr);
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
            [] (const size_t& acc, const auto& node) { return node->count_connections() + acc; }
        );
    }

private:
    node_ptr
    node_with_label (const std::string& label) {
        return const_cast<node_ptr> (static_cast<const digraph*> (this)->node_with_label (label));
    }

    const node_ptr
    node_with_label (const std::string& label) const {
        auto iter = std::find_if (_nodes.cbegin(), _nodes.cend(), [&label] (const auto& ptr) {
            return ptr->label() == label;
        });

        if (iter == _nodes.cend()) return nullptr;

        return iter->get();
    }
};

}  // namespace gpw::foundation

#endif

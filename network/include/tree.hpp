//
// tree.hpp
//
// Tree Data structure
//

#ifndef __GPW_FOUNDATION_TREE__
#define __GPW_FOUNDATION_TREE__

#include "node.hpp"

#include <memory>
#include <vector>

namespace gpw::foundation {

/*******************************************************************************
 *
 * @class tree
 *
 */
template <typename T> class tree {
    using node_ptr = node<T>*;

    node_ptr _root;

    // Because this tree is responsible for the resource management for all of
    // its nodes, this class has a list of unique_ptrs of the nodes.
    // The connections between the nodes are saved using raw pointers, which
    // do not state the ownership between them.
    // Each node has pointers to other nodes (childrens) connected to it.
    std::list<std::unique_ptr<node<T>>> _nodes;

public:
    enum class search_method { depth, breath };

    tree () = delete;

    tree (const std::string& label, const T& data = T()) {
        _create_node (label, data);
        _root = _nodes.begin()->get();
    }

    virtual ~tree () {}

    size_t
    size () const {
        return _nodes.size();
    }

    void
    append_node (const std::string& parent_label, const std::string& label, const T& data = T()) {
        // If the node already exists in the tree, do nothing.
        if (_find_node (label)) return;

        auto parent_ptr = _find_node (parent_label);
        if (parent_ptr == nullptr) return;

        auto new_node_ptr = _create_node (label, data);
        parent_ptr->connect (*new_node_ptr);
    }

    std::vector<std::string>
    path (const std::string& dst, const search_method method = search_method::depth) const {
        // TODO: Implement breath-first search and connect to this interface.
        auto dst_ptr = _find_node (dst);
        if (dst_ptr == nullptr) return {};

        return _depth_first_search (dst_ptr);
    }

private:
    node_ptr
    _create_node (const std::string& label, const T& data) {
        _nodes.emplace_back (std::make_unique<node<T>> (label, data));
        return _nodes.back().get();
    }

    node_ptr
    _find_node (const std::string& label) const {
        auto iter = std::find_if (_nodes.begin(), _nodes.end(), [&label] (auto& ptr) {
            return ptr->label() == label;
        });

        if (iter == _nodes.end()) return nullptr;

        return iter->get();
    }

    std::vector<std::string>
    _depth_first_search (const node_ptr dst) const {
        std::vector<std::string> path;
        std::list<node_ptr>      stack;
        stack.push_back (_root);

        if (_depth_first_search (_root, dst, stack)) {
            std::transform (
                stack.cbegin(),
                stack.cend(),
                std::back_inserter (path),
                [] (const auto& node) { return node->label(); }
            );
            return path;
        }
        // The leaf node is not found in this tree.
        return {};
    }

    bool
    _depth_first_search (const node_ptr root, const node_ptr leaf, std::list<node_ptr>& stack)
        const {
        if (root == leaf) return true;

        for (const auto& child : root->edges()) {
            stack.push_back (child);
            if (_depth_first_search (child, leaf, stack)) return true;

            stack.pop_back();
        }

        return false;
    }
};

}  // namespace gpw::foundation

#endif

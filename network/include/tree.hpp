//
// tree.hpp
//
// Tree Data structure
//

#ifndef __GPW_FOUNDATION_TREE__
#define __GPW_FOUNDATION_TREE__

#include "node.hpp"

#define __DEBUG_LOGGING__

#ifdef __DEBUG_LOGGING__
#include <iostream>
#endif

#include <deque>
#include <list>
#include <sstream>
#include <utility>
#include <vector>

namespace gpw::foundation {

/*******************************************************************************
 *
 * @class tree
 *
 */
template <typename T> class tree {
    using node_ptr       = node<T>*;
    using const_node_ptr = const node<T>*;

    // node_ptr _root;

    // Each node has pointers to other nodes (childrens) connected to it.
    // The connections between the nodes are saved using raw pointers, which
    // do not state the ownership between them.
    std::list<node<T>> _nodes;

public:
    enum class search_method { depth, breath };

    tree () = delete;

    tree (const std::string& label, const T& data = T()) {
        _create_node (label, data);
        // _root = &(*_nodes.begin());

#ifdef __DEBUG_LOGGING__
        std::cout << "New tree is created\n";
        std::cout << " - Root label: " << root().label() << '\n';
#endif
    }

    virtual ~tree () {}

    size_t
    size () const noexcept {
        return _nodes.size();
    }

    const node<T>&
    root () const noexcept {
        return _nodes.front();
    }

    node<T>&
    root () noexcept {
        return _nodes.front();
    }

    void
    append_node (
        const std::string& parent_label,
        const std::string& label,
        const T&           data = T()
    ) noexcept {
        // If the node already exists in the tree, do nothing.
        if (_find_node (label)) return;

        auto parent_ptr = _find_node (parent_label);
        if (parent_ptr == nullptr) return;

        auto new_node_ptr = _create_node (label, data);
        parent_ptr->connect (*new_node_ptr);

#ifdef __DEBUG_LOGGING__
        std::cout << "New node added: " << parent_ptr->label() << " -> " << new_node_ptr->label()
                  << '\n';
#endif
    }

    std::vector<std::string>
    path (const std::string& dst, const search_method method = search_method::depth)
        const noexcept {
        auto dst_ptr = _find_node (dst);
        if (dst_ptr == nullptr) return {};

        switch (method) {
        case search_method::depth: return _depth_first_search (dst_ptr);

        case search_method::breath: return _breath_first_search (dst_ptr);
        }
    }

    bool
    is_ancestor_of (const std::string& label, const std::string& current_node_label)
        const noexcept {
        return is_descendent_of (current_node_label, label);
    }

    bool
    is_descendent_of (const std::string& label, const std::string& current_node_label)
        const noexcept {
        auto current_node_ptr = _find_node (current_node_label);
        if (current_node_ptr == nullptr) return false;

        auto descendent_ptr = _find_node (label);
        if (descendent_ptr == nullptr) return false;

        std::list<const_node_ptr> stack;
        stack.push_back (current_node_ptr);

        return _depth_first_search (current_node_ptr, descendent_ptr, stack);
    }

    std::string
    description () const noexcept {
        std::stringstream strm;

        strm << "# nodes: " << size() << '\n';
        strm << "Root: " << root().label() << '\n';
        strm << root().description (true) << '\n';

        return strm.str();
    }

    bool
    contains_node (const std::string& label) const noexcept {
        return _find_node (label) != nullptr;
    }

private:
    node_ptr
    _create_node (const std::string& label, const T& data) noexcept {
        _nodes.emplace_back (node<T>{label, data});
        return &_nodes.back();
    }

    node_ptr
    _find_node (const std::string& label) noexcept {
        return const_cast<node_ptr> (static_cast<const tree&> (*this)._find_node (label));
    }

    const_node_ptr
    _find_node (const std::string& label) const noexcept {
        auto iter = std::find_if (_nodes.begin(), _nodes.end(), [&label] (auto& node) {
            return node.label() == label;
        });

        if (iter == _nodes.end()) return nullptr;

        return &(*iter);
    }

    std::vector<std::string>
    _depth_first_search (const_node_ptr dst) const noexcept {
        std::vector<std::string>  path;
        std::list<const_node_ptr> stack;
        const_node_ptr            root_ptr = &root();
        stack.push_back (root_ptr);

        if (_depth_first_search (root_ptr, dst, stack)) {
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
    _depth_first_search (const_node_ptr root, const_node_ptr dst, std::list<const_node_ptr>& stack)
        const noexcept {
        if (root == dst) return true;

        for (const auto& child : root->edges()) {
            stack.push_back (child);
            if (_depth_first_search (child, dst, stack)) return true;

            stack.pop_back();
        }

        return false;
    }

    using backtrackable = std::pair<const_node_ptr, const_node_ptr>;

    std::vector<std::string>
    _breath_first_search (const_node_ptr dst) const noexcept {
        std::vector<std::string>   path;
        std::deque<const_node_ptr> queue;
        std::list<backtrackable>   node_pairs;
        const_node_ptr             root_ptr = &root();

        queue.push_front (root_ptr);
        // Root node does not have its parent.
        node_pairs.push_back (std::make_pair (root_ptr, nullptr));

        if (_breath_first_search (dst, queue, node_pairs)) {
            // Backtrack and build path
            std::vector<std::string> backtrack;

            const_node_ptr current = dst;
            while (current != nullptr) {
                backtrack.push_back (current->label());
                auto backstep = std::find_if (
                    node_pairs.cbegin(),
                    node_pairs.cend(),
                    [&current] (const auto& node_pair) {
                        return std::get<0> (node_pair) == current;
                    }
                );

                current = std::get<1> (*backstep);
            }

            std::copy (backtrack.crbegin(), backtrack.crend(), std::back_inserter (path));
            return path;
        }
        return {};
    }

    bool
    _breath_first_search (
        const_node_ptr              dst,
        std::deque<const_node_ptr>& queue,
        std::list<backtrackable>&   node_pairs
    ) const noexcept {
        auto current = queue.front();

        if (current == dst) return true;

        queue.pop_front();

        for (const auto& child : current->edges()) {
            node_pairs.push_back (std::make_pair (child, current));
            queue.push_back (child);
        }

        if (queue.size() == 0) return false;

        return _breath_first_search (dst, queue, node_pairs);
    }
};

}  // namespace gpw::foundation

#endif

//
// Digraph.hpp
//
// Directional Graph Data Structure
//

#ifndef __GPW_FOUNDATION_DIGRAPH__
#define __GPW_FOUNDATION_DIGRAPH__

#include "node.hpp"
#include "tree.hpp"

#ifdef __DEBUG_LOGGING__
#include <iostream>
#endif

#include <array>
#include <iterator>
#include <list>
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
    using arc  = std::array<std::string, 2>;
    using arcs = std::vector<arc>;

    struct forest {
        std::list<tree<T>> trees;
        std::vector<arc>   tree_arcs;
        std::vector<arc>   back_arcs;
        std::vector<arc>   fwd_arcs;
        std::vector<arc>   cross_arcs;
        std::vector<arc>   loops;
    };

    // Depth-first forest algorithm
    //
    // 1. Set H <- the last node in the list (L).
    // 2. Call find_strongly_connected_components (recursion)
    // 3. If there no connected node to visit, group strongly connected components.
    //
    forest
    make_depth_first_forest () const {
        forest dfforest;

        auto labeled_nodes         = make_labeled_graph();
        auto current_node_itr      = labeled_nodes.end() - 1;
        int  count_completed_nodes = 0;

        do {
            search_for_new_tree (labeled_nodes, dfforest);
            count_completed_nodes =
                std::count_if (labeled_nodes.cbegin(), labeled_nodes.cend(), [] (const auto& node) {
                    return node.completed == true;
                });
        } while (_nodes.size() != count_completed_nodes);

#ifdef __DEBUG_LOGGING__
        for (const auto& tree : dfforest.trees) {
            std::cout << tree.description() << '\n';
        }
#endif

        return dfforest;
    }

private:
    std::list<std::list<node<T>>>
    scc_from_forest (forest& dfforest) const {
        // From the first tree
    }

    struct labeled_node {
        bool                     completed;
        std::string              label;
        std::vector<std::string> edges;
    };

    std::vector<labeled_node>
    make_labeled_graph () const {
        std::vector<labeled_node> labeled_graph;

        for (const auto& node : _nodes) {
            std::vector<std::string> edges;
            for (const auto& edge : node.edges()) {
                edges.push_back (edge->label());
            }
            labeled_graph.emplace_back (labeled_node{false, node.label(), std::move (edges)});
        }

        return labeled_graph;
    }

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

    bool
    is_node_visited (const std::string& label, const forest& dff) const noexcept {
        for (const auto& tree : dff.trees) {
            if (tree.contains_node (label)) return true;
        }
        return false;
    }

    using lnode_itr = typename std::vector<labeled_node>::iterator;

    void
    explore_depth_first (std::vector<labeled_node>& nodes, forest& dfforest, lnode_itr current_itr)
        const {
        if (!current_itr->completed) {
            for (auto& edge_label : current_itr->edges) {
                auto itr =
                    std::find_if (nodes.begin(), nodes.end(), [&edge_label] (const auto& node) {
                        return node.label == edge_label;
                    });

                bool need_exploration = classify_arc (nodes, dfforest, itr, current_itr);
                if (need_exploration) explore_depth_first (nodes, dfforest, itr);
            }
            current_itr->completed = true;
        }
    }

    void
    search_for_new_tree (std::vector<labeled_node>& nodes, forest& dfforest) const {
        auto incomplete = std::find_if (nodes.rbegin(), nodes.rend(), [] (const auto& node) {
            return node.completed == false;
        });
        if (incomplete == nodes.rend()) return;

// Create a new tree
#ifdef __DEBUG_LOGGING__
        if (dfforest.trees.size() > 0)
            std::cout << "The previous tree info:\n" << dfforest.trees.back().description() << '\n';
#endif
        dfforest.trees.emplace_back (tree<T>{incomplete->label});
#ifdef __DEBUG_LOGGING__
        std::cout << "New tree with root: " << incomplete->label << '\n';
#endif
        lnode_itr itr = incomplete.base();
        explore_depth_first (nodes, dfforest, itr - 1);
    }

    //   Mark the next connected node (T) as 'visited'
    //   Classify the new arc:
    //     If the node (T) is a new visit, it is "tree arc."
    //     Otherwise,
    //       If the node (T) is an ancestor of the node (H), it is "back arc."
    //       If the node (T) is the same as the node (H), it is "loop arc."
    //       If the node (T) is a descendant of the node (H), it is "forward arc."
    //       Otherwise, it is "cross arc."
    bool
    classify_arc (
        std::vector<labeled_node>& nodes,
        forest&                    dfforest,
        lnode_itr                  itr,
        lnode_itr                  current_itr
    ) const {
        // New visit, tree arc.
        if (!is_node_visited (itr->label, dfforest)) {
            dfforest.tree_arcs.emplace_back (
                std::array<std::string, 2>{current_itr->label, itr->label}
            );
            dfforest.trees.back().append_node (current_itr->label, itr->label);
#ifdef __DEBUG_LOGGING__
            std::cout << "New edge in tree: " << current_itr->label << " -> " << itr->label << '\n';
#endif
            return true;
        } else {
            if (dfforest.trees.back().is_ancestor_of (itr->label, current_itr->label)) {
                dfforest.back_arcs.emplace_back (
                    std::array<std::string, 2>{current_itr->label, itr->label}
                );
            } else if (current_itr == itr) {
                dfforest.loops.emplace_back (
                    std::array<std::string, 2>{current_itr->label, itr->label}
                );
            } else if (dfforest.trees.back().is_descendent_of (itr->label, current_itr->label)) {
                dfforest.fwd_arcs.emplace_back (
                    std::array<std::string, 2>{current_itr->label, itr->label}
                );
            } else {
                dfforest.cross_arcs.emplace_back (
                    std::array<std::string, 2>{current_itr->label, itr->label}
                );
            }
        }
        return false;
    }
};

}  // namespace gpw::foundation

#endif

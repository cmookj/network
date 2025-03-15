//
// Digraph.hpp
//
// Directional Graph Data Structure
//

#ifndef __GPW_FOUNDATION_DIGRAPH__
#define __GPW_FOUNDATION_DIGRAPH__

#include "node.hpp"
#include "tree.hpp"

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
    using arc    = std::pair<std::string, std::string>;
    using arcs   = std::vector<arc>;
    using forest = std::list<tree<T>>;

    // Depth-first forest algorithm
    //
    // 1. Set H <- the last node in the list (L).
    // 2. Call find_strongly_connected_components (recursion)
    // 3. If there no connected node to visit, group strongly connected components.
    //
    forest
    make_depth_first_forest () const {
        forest depth_first_forest;

        auto labeled_graph    = make_labeled_graph();
        auto current_node_itr = labeled_graph.end() - 1;
        arcs tree_arcs;
        arcs back_arcs;
        arcs loops;
        arcs fwd_arcs;
        arcs cross_arcs;
        depth_first_forest.emplace_back (tree<T>{current_node_itr->label});

        make_depth_first_forest (
            labeled_graph,
            depth_first_forest,
            current_node_itr,
            tree_arcs,
            back_arcs,
            loops,
            fwd_arcs,
            cross_arcs
        );

        return depth_first_forest;
    }

private:
    std::list<std::list<node<T>>>
    scc_from_forest (
        forest& depth_first_forest,
        arcs&   tree_arcs,
        arcs&   back_arcs,
        arcs&   loops,
        arcs&   fwd_arcs,
        arcs&   cross_arcs
    ) const {
        // From the first tree
    }

    enum class node_status { pristine, visited, completed };

    struct labeled_node {
        node_status              status;
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
            labeled_graph.emplace_back (
                labeled_node{node_status::pristine, node.label(), std::move (edges)}
            );
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

    //
    // If current node's status is 'completed'
    //   If there exists 'visited' node in the node list
    //     Current node <- 'visited' node found, and recurse
    //   Otherwise,
    //     If there is no 'pristine' node in the list
    //       Terminate this algorithm
    //     Otherwise,
    //       Change the `pristine` nodeh to 'visited' and create a new tree
    //
    // Otherwise,
    //   Mark the next connected node (T) as 'visited'
    //   Classify the new arc:
    //     If the node (T) is a new visit, it is "tree arc."
    //     Otherwise,
    //       If the node (T) is an ancestor of the node (H), it is "back arc."
    //       If the node (T) is the same as the node (H), it is "loop arc."
    //       If the node (T) is a descendant of the node (H), it is "forward arc."
    //       Otherwise, it is "cross arc."
    //  Set current_node <- T, and go to 2.
    //
    using lnode_itr = typename std::vector<labeled_node>::iterator;

    void
    make_depth_first_forest (
        std::vector<labeled_node>& nodes,
        forest&                    depth_first_forest,
        lnode_itr                  current_node_itr,
        arcs&                      tree_arcs,
        arcs&                      back_arcs,
        arcs&                      loops,
        arcs&                      fwd_arcs,
        arcs&                      cross_arcs
    ) const {
        // If current node is completed, find the next incomplete from the back.
        // If all the nodes are completed, return.
        if (current_node_itr->status == node_status::completed) {
            auto visited_itr = std::find_if (nodes.rbegin(), nodes.rend(), [] (const auto& node) {
                return node.status == node_status::visited;
            });

            if (visited_itr != nodes.rend())
                make_depth_first_forest (
                    nodes,
                    depth_first_forest,
                    visited_itr.base() - 1,
                    tree_arcs,
                    back_arcs,
                    loops,
                    fwd_arcs,
                    cross_arcs
                );

            auto pristine_itr = std::find_if (nodes.begin(), nodes.end(), [] (const auto& node) {
                return node.status == node_status::pristine;
            });

            if (pristine_itr == nodes.end()) return;

            pristine_itr->status = node_status::visited;
            // Create a new tree
            depth_first_forest.emplace_back (tree<T>{pristine_itr->label});
        } else {
            for (auto& edge_label : current_node_itr->edges) {
                auto itr =
                    std::find_if (nodes.begin(), nodes.end(), [&edge_label] (const auto& node) {
                        return node.label == edge_label;
                    });
                // New visit, tree arc.
                if (itr->status == node_status::pristine) {
                    tree_arcs.emplace_back (std::make_pair (current_node_itr->label, itr->label));
                    depth_first_forest.back().append_node (current_node_itr->label, itr->label);
                } else {
                    if (depth_first_forest.back().is_ancestor_of (
                            itr->label, current_node_itr->label
                        )) {
                        back_arcs.emplace_back (std::make_pair (current_node_itr->label, itr->label)
                        );
                    } else if (current_node_itr == itr) {
                        loops.emplace_back (std::make_pair (current_node_itr->label, itr->label));
                    } else if (depth_first_forest.back().is_descendent_of (
                                   itr->label, current_node_itr->label
                               )) {
                        fwd_arcs.emplace_back (std::make_pair (current_node_itr->label, itr->label)
                        );
                    } else {
                        cross_arcs.emplace_back (
                            std::make_pair (current_node_itr->label, itr->label)
                        );
                    }
                }
                make_depth_first_forest (
                    nodes,
                    depth_first_forest,
                    itr,
                    tree_arcs,
                    back_arcs,
                    loops,
                    fwd_arcs,
                    cross_arcs
                );
            }
        }
    }
};

}  // namespace gpw::foundation

#endif

#ifndef __GPW_FOUNDATION_NODE__
#define __GPW_FOUNDATION_NODE__

#include <algorithm>
#include <list>
#include <string>

namespace gpw::foundation {

/*******************************************************************************
 *
 * @class node
 *
 */
template <typename T> class node {
    // Note that this `node` type does not manage memory resources.
    // Connection or disconnection to other nodes only adds or removes raw pointer
    // to the nodes WITHOUT creation or deletion of the object.
    using node_ptr = node<T>*;

    std::string         _label;
    T                   _data;
    std::list<node_ptr> _edges;

public:
    node () = delete;
    node (const std::string_view lb, const T& dt = T())
        : _label{lb}
        , _data{dt} {}

    std::optional<T>
    data () const {
        return _data;
    }

    std::string
    label () const {
        return _label;
    }

    const std::list<node_ptr>&
    edges () const {
        return _edges;
    }

    void
    connect (node<T>& ch) {
        if (is_connected (ch)) return;

        _edges.push_back (&ch);
    }

    void
    disconnect (node<T>& ch) {
        _edges.remove (&ch);
    }

    void
    disconnect (const std::string& label) {
        _edges.remove_if ([&label] (auto& node_ptr) { return node_ptr->_label == label; });
    }

    size_t
    count_connections () const {
        return _edges.size();
    }

    bool
    is_connected (const node<T>& node) const {
        if (std::find (_edges.cbegin(), _edges.cend(), &node) == _edges.cend()) return false;

        return true;
    }
};

}  // namespace gpw::foundation

#endif

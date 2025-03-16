#ifndef __GPW_FOUNDATION_NODE__
#define __GPW_FOUNDATION_NODE__

#include <algorithm>
#include <list>
#include <sstream>
#include <string>
#include <vector>

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
        std::erase_if (_edges, [&label] (auto& node_ptr) { return node_ptr->_label == label; });
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

    std::string
    description (bool recursion = false) const noexcept {
        std::vector<std::string> connected_labels;
        std::transform (
            _edges.cbegin(),
            _edges.cend(),
            std::back_inserter (connected_labels),
            [] (const auto& ptr) { return ptr->label(); }
        );

        std::stringstream strm;
        strm << _label << " : ";
        strm << '{';
        if (connected_labels.cbegin() != connected_labels.cend()) {
            auto last = connected_labels.cend() - 1;
            std::copy (
                connected_labels.cbegin(), last, std::ostream_iterator<std::string> (strm, ", ")
            );
            strm << *last;
        }
        strm << "}\n";

        if (recursion) {
            for (const auto& ptr : _edges) {
                strm << ptr->description (recursion);
            }
        }

        return strm.str();
    }
};

}  // namespace gpw::foundation

#endif

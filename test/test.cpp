#include "digraph.hpp"
#include "tree.hpp"

#include <gtest/gtest.h>

using namespace gpw::foundation;

TEST (Node, ConnectionDisconnection) {
    node<int> a ("a");
    EXPECT_EQ (a.count_connections(), 0);
    EXPECT_EQ (a.data(), 0);

    // Self-connection
    a.connect (a);
    EXPECT_EQ (a.count_connections(), 1);

    node<int> b ("b");
    a.connect (b);
    EXPECT_EQ (a.count_connections(), 2);

    // Duplicate connection: should be rejected
    a.connect (b);
    EXPECT_EQ (a.count_connections(), 2);

    node<int> c ("c");
    a.disconnect (c);
    EXPECT_EQ (a.count_connections(), 2);

    a.connect (c);
    EXPECT_EQ (a.count_connections(), 3);

    // Removal: should be independent of the order of connection
    a.disconnect ("a");
    EXPECT_EQ (a.count_connections(), 2);

    a.disconnect ("b");
    EXPECT_EQ (a.count_connections(), 1);
}

TEST (Node, ConnectionDisconnectionStringData) {
    node<std::string> a ("a", "AAA");
    EXPECT_EQ (a.count_connections(), 0);
    EXPECT_EQ (a.data(), "AAA");

    // Self-connection
    a.connect (a);
    EXPECT_EQ (a.count_connections(), 1);

    node<std::string> b ("b", "Bbb");
    EXPECT_EQ (b.data(), "Bbb");
    a.connect (b);
    EXPECT_EQ (a.count_connections(), 2);

    // Duplicate connection: should be rejected
    a.connect (b);
    EXPECT_EQ (a.count_connections(), 2);

    node<std::string> c ("c", "Ceeee");
    EXPECT_EQ (c.data(), "Ceeee");
    a.disconnect (c);
    EXPECT_EQ (a.count_connections(), 2);

    a.connect (c);
    EXPECT_EQ (a.count_connections(), 3);

    // Removal: should be independent of the order of connection
    a.disconnect ("a");
    EXPECT_EQ (a.count_connections(), 2);

    a.disconnect ("b");
    EXPECT_EQ (a.count_connections(), 1);
}

TEST (Digraph, NodeAdditionRemoval) {
    digraph<int> gr;
    EXPECT_EQ (gr.size(), 0);

    gr.create_node ("A");
    gr.create_node ("B");
    gr.create_node ("C");
    gr.create_node ("D");
    gr.create_node ("E");
    gr.create_node ("F");
    gr.create_node ("G");
    gr.create_node ("H");
    gr.create_node ("I");
    gr.create_node ("J");
    gr.create_node ("K");
    gr.create_node ("L");
    gr.create_node ("M");
    gr.create_node ("N");
    gr.create_node ("O");

    EXPECT_EQ (gr.size(), 15);

    gr.remove_node ("A");
    EXPECT_EQ (gr.size(), 14);

    // Duplicate, invalid removal
    gr.remove_node ("A");
    EXPECT_EQ (gr.size(), 14);

    gr.remove_node ("B");
    EXPECT_EQ (gr.size(), 13);

    gr.remove_node ("C");
    EXPECT_EQ (gr.size(), 12);

    // Invalid node.  The size should not change.
    gr.remove_node ("Z");
    EXPECT_EQ (gr.size(), 12);
}

TEST (Digraph, NodeConnection) {
    digraph<int> gr;

    gr.create_node ("A");
    gr.create_node ("B");
    gr.create_node ("C");
    gr.create_node ("D");
    gr.create_node ("E");
    gr.create_node ("F");
    gr.create_node ("G");
    gr.create_node ("H");
    gr.create_node ("I");
    gr.create_node ("J");
    gr.create_node ("K");
    gr.create_node ("L");
    gr.create_node ("M");
    gr.create_node ("N");
    gr.create_node ("O");

    gr.connect_node ("A", "M");
    gr.connect_node ("C", "F");
    gr.connect_node ("D", "M");
    gr.connect_node ("E", "I");
    gr.connect_node ("F", "C");
    gr.connect_node ("F", "L");
    gr.connect_node ("G", "K");
    gr.connect_node ("H", "G");
    gr.connect_node ("H", "C");
    gr.connect_node ("I", "I");
    gr.connect_node ("J", "L");
    gr.connect_node ("J", "B");
    gr.connect_node ("K", "H");
    gr.connect_node ("K", "N");
    gr.connect_node ("L", "J");
    gr.connect_node ("L", "E");
    gr.connect_node ("M", "A");
    gr.connect_node ("M", "D");
    gr.connect_node ("M", "O");
    gr.connect_node ("N", "J");
    gr.connect_node ("O", "N");
    gr.connect_node ("O", "B");
    gr.connect_node ("O", "F");

    EXPECT_EQ (1, 2);
    std::cout << gr.description();

    EXPECT_TRUE (gr.is_connected ("A", "M"));
    EXPECT_TRUE (gr.is_connected ("C", "F"));
    EXPECT_TRUE (gr.is_connected ("D", "M"));
    EXPECT_TRUE (gr.is_connected ("E", "I"));
    EXPECT_TRUE (gr.is_connected ("F", "C"));
    EXPECT_TRUE (gr.is_connected ("F", "L"));
    EXPECT_TRUE (gr.is_connected ("G", "K"));
    EXPECT_TRUE (gr.is_connected ("H", "G"));
    EXPECT_TRUE (gr.is_connected ("H", "C"));
    EXPECT_TRUE (gr.is_connected ("I", "I"));
    EXPECT_TRUE (gr.is_connected ("J", "L"));
    EXPECT_TRUE (gr.is_connected ("J", "B"));
    EXPECT_TRUE (gr.is_connected ("K", "H"));
    EXPECT_TRUE (gr.is_connected ("K", "N"));
    EXPECT_TRUE (gr.is_connected ("L", "J"));
    EXPECT_TRUE (gr.is_connected ("L", "E"));
    EXPECT_TRUE (gr.is_connected ("M", "A"));
    EXPECT_TRUE (gr.is_connected ("M", "D"));
    EXPECT_TRUE (gr.is_connected ("M", "O"));
    EXPECT_TRUE (gr.is_connected ("N", "J"));
    EXPECT_TRUE (gr.is_connected ("O", "N"));
    EXPECT_TRUE (gr.is_connected ("O", "B"));
    EXPECT_TRUE (gr.is_connected ("O", "F"));

    EXPECT_EQ (gr.count_connections(), 23);
}

TEST (Tree, Creation) {
    tree<int> tr{"O"};

    EXPECT_EQ (tr.size(), 1);

    tr.append_node ("O", "N");
    EXPECT_EQ (tr.size(), 2);

    tr.append_node ("N", "J");
    EXPECT_EQ (tr.size(), 3);

    tr.append_node ("J", "L");
    tr.append_node ("L", "E");
    tr.append_node ("E", "I");
    EXPECT_EQ (tr.size(), 6);

    tr.append_node ("I", "I");

    EXPECT_EQ (tr.size(), 6);
}

TEST (Tree, Search) {
    // O
    // |
    // N
    // |
    // J
    // |
    // L
    // |
    // E
    // |
    // I
    tree<int> tr{"O"};

    tr.append_node ("O", "N");
    tr.append_node ("N", "J");
    tr.append_node ("J", "L");
    tr.append_node ("L", "E");
    tr.append_node ("E", "I");

    auto path = tr.path ("I");

    EXPECT_EQ (path.size(), 6);

    EXPECT_EQ (path[0], "O");
    EXPECT_EQ (path[1], "N");
    EXPECT_EQ (path[2], "J");
    EXPECT_EQ (path[3], "L");
    EXPECT_EQ (path[4], "E");
    EXPECT_EQ (path[5], "I");

    auto path_b = tr.path ("I", tree<int>::search_method::breath);

    EXPECT_EQ (path_b.size(), 6);

    EXPECT_EQ (path_b[0], "O");
    EXPECT_EQ (path_b[1], "N");
    EXPECT_EQ (path_b[2], "J");
    EXPECT_EQ (path_b[3], "L");
    EXPECT_EQ (path_b[4], "E");
    EXPECT_EQ (path_b[5], "I");

    EXPECT_TRUE (tr.is_ancestor_of ("N", "E"));
    EXPECT_TRUE (tr.is_descendent_of ("E", "N"));

    //
    // A
    // |
    // .-.-.--.
    // B C D  E
    // | |    |
    // | |    .-.
    // F G    H I
    // |      |
    // .-.-.  |
    // J K L  M
    //        |
    //        N
    tree<int> tr2{"A"};

    tr2.append_node ("A", "B");
    tr2.append_node ("A", "C");
    tr2.append_node ("A", "D");
    tr2.append_node ("A", "E");

    tr2.append_node ("B", "F");

    tr2.append_node ("C", "G");

    tr2.append_node ("E", "H");
    tr2.append_node ("E", "I");

    tr2.append_node ("F", "J");
    tr2.append_node ("F", "K");
    tr2.append_node ("F", "L");

    tr2.append_node ("H", "M");

    tr2.append_node ("M", "N");

    auto path2 = tr2.path ("K");

    EXPECT_EQ (path2.size(), 4);

    EXPECT_EQ (path2[0], "A");
    EXPECT_EQ (path2[1], "B");
    EXPECT_EQ (path2[2], "F");
    EXPECT_EQ (path2[3], "K");

    auto path2_b = tr2.path ("K", tree<int>::search_method::breath);

    EXPECT_EQ (path2_b.size(), 4);

    EXPECT_EQ (path2_b[0], "A");
    EXPECT_EQ (path2_b[1], "B");
    EXPECT_EQ (path2_b[2], "F");
    EXPECT_EQ (path2_b[3], "K");

    auto path3 = tr2.path ("M");

    EXPECT_EQ (path3.size(), 4);

    EXPECT_EQ (path3[0], "A");
    EXPECT_EQ (path3[1], "E");
    EXPECT_EQ (path3[2], "H");
    EXPECT_EQ (path3[3], "M");

    auto path3_b = tr2.path ("M", tree<int>::search_method::breath);

    EXPECT_EQ (path3_b.size(), 4);

    EXPECT_EQ (path3_b[0], "A");
    EXPECT_EQ (path3_b[1], "E");
    EXPECT_EQ (path3_b[2], "H");
    EXPECT_EQ (path3_b[3], "M");
}

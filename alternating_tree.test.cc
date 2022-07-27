#include <gtest/gtest.h>
#include "alternating_tree.h"
#include "graph_fill_region.h"

namespace att {

    std::vector<pm::GraphFillRegion>& getGfrs(){
        static std::vector<pm::GraphFillRegion> gfrs(10);
        return gfrs;
    }

    pm::Graph& getGraph() {
        static pm::Graph g(10);
        return g;
    }


    pm::AltTreeEdge t(
            const std::vector<pm::AltTreeEdge>& children,
            size_t inner_region_id,
            size_t outer_region_id,
            bool root,
            size_t io_loc_from_id,
            size_t io_loc_to_id,
            size_t parent_loc_from_id,
            size_t parent_loc_to_id
            ) {
        pm::AltTreeNode* node;
        pm::CompressedEdge parent_ce;
        if (root) {
            node = new pm::AltTreeNode(&getGfrs()[outer_region_id]);
        } else {
            node = new pm::AltTreeNode(
                    &getGfrs()[inner_region_id],
                    &getGfrs()[outer_region_id],
                    {
                        &getGraph().nodes[io_loc_from_id],
                        &getGraph().nodes[io_loc_to_id],
                        0
                    }
                    );
            parent_ce = {
                    &getGraph().nodes[parent_loc_from_id],
                    &getGraph().nodes[parent_loc_to_id],
                    0
            };
        }
        auto edge = pm::AltTreeEdge(node, parent_ce);
        for (auto& child : children) {
            edge.alt_tree_node->add_child(child);
        }
        return edge;
    }


    pm::AltTreeEdge example_tree() {
        return t(
                {
                        t(
                                {
                                        t(
                                                {},
                                                3,
                                                4,
                                                false,
                                                3,
                                                4,
                                                2,
                                                3
                                        )
                                },
                                1,
                                2,
                                false,
                                1,
                                2,
                                0,
                                1
                        )
                },
                -1,
                0,
                true,
                -1,
                0,
                -1,
                -1
        );
    }


    void delete_alternating_tree(pm::AltTreeNode* root) {
        for (auto child: root->children) {
            delete_alternating_tree(child.alt_tree_node);
        }
        delete root;
    }


    TEST(AlternatingTree, FindRoot) {
        auto n = example_tree();
        ASSERT_EQ(
                n.alt_tree_node->children[0].alt_tree_node->children[0].alt_tree_node->find_root(), n.alt_tree_node);
        ASSERT_EQ(n.alt_tree_node->find_root(), n.alt_tree_node);
        delete_alternating_tree(n.alt_tree_node);
    }





}


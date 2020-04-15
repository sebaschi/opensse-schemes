#include <sse/schemes/oceanus/details/tethys.hpp>

#include <iostream>

#include <gtest/gtest.h>


namespace sse {
namespace tethys {

namespace details {
namespace test {

TEST(tethys_graph, dfs_1)
{
    TethysGraph graph(3);

    graph.add_edge_from_source(0, 2, 0, 0);
    graph.add_edge(1, 2, 0, 0, ForcedRight);

    graph.add_edge(2, 1, 0, 1, ForcedLeft);
    graph.add_edge_to_sink(3, 1, 1, 0);

    graph.add_edge(4, 1, 0, 2, ForcedLeft);
    graph.add_edge(5, 1, 2, 1, ForcedRight);
    graph.add_edge_to_sink(6, 1, 1, 1);

    size_t cap  = 0;
    auto   path = graph.find_source_sink_path(&cap);

    std::vector<std::size_t> path_index;
    std::transform(path.begin(),
                   path.end(),
                   std::back_inserter(path_index),
                   [&graph](const EdgePtr& e) -> std::size_t {
                       return graph.get_edge(e).value_index;
                   });


    EXPECT_EQ(cap, 1);
    EXPECT_EQ(path_index, std::vector<size_t>({0, 1, 4, 5, 6}));
}

TEST(tethys_graph, dfs_2)
{
    TethysGraph graph(3);

    graph.add_edge_from_source(0, 2, 0, 0);
    graph.add_edge(1, 2, 0, 0, ForcedRight);

    graph.add_edge(4, 1, 0, 2, ForcedLeft);
    graph.add_edge(5, 1, 2, 1, ForcedRight);
    graph.add_edge_to_sink(6, 1, 1, 1);

    graph.add_edge(2, 1, 0, 1, ForcedLeft);
    graph.add_edge_to_sink(3, 1, 1, 0);


    size_t cap  = 0;
    auto   path = graph.find_source_sink_path(&cap);

    std::vector<std::size_t> path_index;
    std::transform(path.begin(),
                   path.end(),
                   std::back_inserter(path_index),
                   [&graph](const EdgePtr& e) -> std::size_t {
                       return graph.get_edge(e).value_index;
                   });


    EXPECT_EQ(cap, 1);
    EXPECT_EQ(path_index, std::vector<size_t>({0, 1, 2, 3}));
}


TEST(tethys_graph, maxflow_1)
{
    TethysGraph graph(3);

    graph.add_edge_from_source(0, 2, 0, 0);
    graph.add_edge(1, 2, 0, 0, ForcedRight);

    graph.add_edge(2, 1, 0, 1, ForcedLeft);
    graph.add_edge_to_sink(3, 1, 1, 0);

    graph.add_edge(4, 1, 0, 2, ForcedLeft);
    graph.add_edge(5, 1, 2, 1, ForcedRight);
    graph.add_edge_to_sink(6, 1, 1, 1);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    TethysGraph expected_graph(3);

    expected_graph.add_edge_from_source(0, 2, 0, 0);
    expected_graph.add_edge(1, 2, 0, 0, ForcedRight);

    expected_graph.add_edge(2, 1, 0, 1, ForcedLeft);
    expected_graph.add_edge_to_sink(3, 1, 1, 0);

    expected_graph.add_edge(4, 1, 0, 2, ForcedLeft);
    expected_graph.add_edge(5, 1, 2, 1, ForcedRight);
    expected_graph.add_edge_to_sink(6, 1, 1, 1);

    EXPECT_EQ(graph, expected_graph);
}

TEST(tethys_graph, maxflow_2)
{
    TethysGraph graph(3);

    graph.add_edge_from_source(0, 1, 0, 0);
    graph.add_edge(1, 1, 0, 0, ForcedRight);

    graph.add_edge(2, 1, 0, 1, ForcedLeft);
    graph.add_edge_to_sink(3, 1, 1, 0);

    graph.add_edge(4, 1, 0, 2, ForcedLeft);
    graph.add_edge(5, 1, 2, 1, ForcedRight);
    graph.add_edge_to_sink(6, 1, 1, 1);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    TethysGraph expected_graph(3);

    expected_graph.add_edge_from_source(0, 1, 0, 0);
    expected_graph.add_edge(1, 1, 0, 0, ForcedRight);

    expected_graph.add_edge(2, 0, 0, 1, ForcedLeft);
    expected_graph.add_edge_to_sink(3, 0, 1, 0);

    expected_graph.add_edge(4, 1, 0, 2, ForcedLeft);
    expected_graph.add_edge(5, 1, 2, 1, ForcedRight);
    expected_graph.add_edge_to_sink(6, 1, 1, 1);

    EXPECT_EQ(graph, expected_graph);
}

TEST(tethys_graph, maxflow_3)
{
    TethysGraph graph(3);

    graph.add_edge_from_source(0, 1, 0, 0);
    graph.add_edge(1, 1, 0, 0, ForcedRight);

    graph.add_edge(4, 1, 0, 2, ForcedLeft);
    graph.add_edge(5, 1, 2, 1, ForcedRight);
    graph.add_edge_to_sink(6, 1, 1, 1);

    graph.add_edge(2, 1, 0, 1, ForcedLeft);
    graph.add_edge_to_sink(3, 1, 1, 0);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    TethysGraph expected_graph(3);

    expected_graph.add_edge_from_source(0, 1, 0, 0);
    expected_graph.add_edge(1, 1, 0, 0, ForcedRight);


    expected_graph.add_edge(4, 0, 0, 2, ForcedLeft);
    expected_graph.add_edge(5, 0, 2, 1, ForcedRight);
    expected_graph.add_edge_to_sink(6, 0, 1, 1);

    expected_graph.add_edge(2, 1, 0, 1, ForcedLeft);
    expected_graph.add_edge_to_sink(3, 1, 1, 0);

    EXPECT_EQ(graph, expected_graph);
}


TEST(tethys_graph, maxflow_4)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 15, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 10, 9, 0);

    graph.add_edge(3, 20, 1, 8, ForcedRight);

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 12, 8, 1);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 12);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 12);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 0);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 12);
}

TEST(tethys_graph, maxflow_5)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 15, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 10, 9, 0);

    graph.add_edge(3, 20, 1, 8, ForcedRight);

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(
        15, 30, 8, 1); // <-- change this capacity from 12 to 30

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 15);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 15);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 0);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 15);
}

TEST(tethys_graph, maxflow_6)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 15, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 10, 9, 0);

    EdgePtr sat_edge
        = graph.add_edge(3, 20, 1, 8, ForcedRight); // will saturate here

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 30, 8, 1);


    // add these edges
    graph.add_edge(7, 9, 9, 3, ForcedRight);
    graph.add_edge(11, 9, 3, 3, ForcedLeft);
    graph.add_edge(5, 7, 3, 6, ForcedRight);
    graph.add_edge(14, 9, 6, 1, ForcedLeft);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 20);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 15);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 5);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 20);

    EXPECT_EQ(graph.get_edge_flow(sat_edge), 20);
}


TEST(tethys_graph, maxflow_7)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 15, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 10, 9, 0);

    graph.add_edge(3, 30, 1, 8, ForcedRight); // change this capacity

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 30, 8, 1);


    graph.add_edge(7, 9, 9, 3, ForcedRight);
    graph.add_edge(11, 9, 3, 3, ForcedLeft);
    EdgePtr sat_edge = graph.add_edge(5, 7, 3, 6, ForcedRight); // saturate here
    graph.add_edge(14, 9, 6, 1, ForcedLeft);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 22);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 15);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 7);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 22);

    EXPECT_EQ(graph.get_edge_flow(sat_edge), 7);
}


TEST(tethys_graph, maxflow_8)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 15, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 10, 9, 0);

    graph.add_edge(3, 30, 1, 8, ForcedRight);

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 30, 8, 1);


    EdgePtr sat_edge_1
        = graph.add_edge(7, 9, 9, 3, ForcedRight); // saturate here
    EdgePtr sat_edge_2
        = graph.add_edge(11, 9, 3, 3, ForcedLeft); // ... here ...
    graph.add_edge(5, 7, 3, 6, ForcedRight);
    EdgePtr sat_edge_3 = graph.add_edge(14, 9, 6, 1, ForcedLeft); // ..and here

    // add these edges
    graph.add_edge(4, 7, 3, 4, ForcedRight);
    graph.add_edge(12, 10, 4, 6, ForcedLeft);
    graph.add_edge(6, 10, 6, 6, ForcedRight);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 24);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 15);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 9);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 24);

    EXPECT_EQ(graph.get_edge_flow(sat_edge_1), 9);
    EXPECT_EQ(graph.get_edge_flow(sat_edge_2), 9);
    EXPECT_EQ(graph.get_edge_flow(sat_edge_3), 9);
}

TEST(tethys_graph, maxflow_9)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 15, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 10, 9, 0);

    graph.add_edge(3, 30, 1, 8, ForcedRight);

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 30, 8, 1);


    EdgePtr sat_edge_1
        = graph.add_edge(7, 15, 9, 3, ForcedRight); // change this capacity
    EdgePtr sat_edge_2
        = graph.add_edge(11, 15, 3, 3, ForcedLeft); // change this capacity
    graph.add_edge(5, 7, 3, 6, ForcedRight);
    EdgePtr sat_edge_3
        = graph.add_edge(14, 15, 6, 1, ForcedLeft); // change this capacity

    graph.add_edge(4, 7, 3, 4, ForcedRight);
    graph.add_edge(12, 10, 4, 6, ForcedLeft);
    graph.add_edge(6, 10, 6, 6, ForcedRight);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 25);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 15);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 10);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 25);

    EXPECT_EQ(graph.get_edge_flow(sat_edge_1), 10);
    EXPECT_EQ(graph.get_edge_flow(sat_edge_2), 10);
    EXPECT_EQ(graph.get_edge_flow(sat_edge_3), 10);
}

TEST(tethys_graph, maxflow_10)
{
    TethysGraph graph(10);

    EdgePtr e_source_1
        = graph.add_edge_from_source(0, 10, 1, 0); // change this capacity
    EdgePtr e_source_2
        = graph.add_edge_from_source(1, 45, 9, 0); // change this capacity

    graph.add_edge(3, 30, 1, 8, ForcedRight);

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 30, 8, 1);


    graph.add_edge(7, 15, 9, 3, ForcedRight);
    graph.add_edge(11, 15, 3, 3, ForcedLeft);
    EdgePtr sat_edge_1
        = graph.add_edge(5, 7, 3, 6, ForcedRight); // saturate here
    graph.add_edge(14, 15, 6, 1, ForcedLeft);

    EdgePtr sat_edge_2
        = graph.add_edge(4, 7, 3, 4, ForcedRight); // saturate here
    graph.add_edge(12, 10, 4, 6, ForcedLeft);
    graph.add_edge(6, 10, 6, 6, ForcedRight);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 24);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 10);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 14);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 24);

    EXPECT_EQ(graph.get_edge_flow(sat_edge_1), 7);
    EXPECT_EQ(graph.get_edge_flow(sat_edge_2), 7);
}


TEST(tethys_graph, maxflow_11)
{
    TethysGraph graph(10);

    EdgePtr e_source_1 = graph.add_edge_from_source(0, 10, 1, 0);
    EdgePtr e_source_2 = graph.add_edge_from_source(1, 45, 9, 0);

    graph.add_edge(3, 30, 1, 8, ForcedRight);

    EdgePtr e_sink_1 = graph.add_edge_to_sink(8, 10, 7, 0);
    EdgePtr e_sink_2 = graph.add_edge_to_sink(15, 30, 8, 1);


    graph.add_edge(7, 15, 9, 3, ForcedRight);
    graph.add_edge(11, 15, 3, 3, ForcedLeft);
    graph.add_edge(5, 7, 3, 6, ForcedRight);
    graph.add_edge(14, 15, 6, 1, ForcedLeft);

    graph.add_edge(4, 7, 3, 4, ForcedRight);
    graph.add_edge(12, 10, 4, 6, ForcedLeft);
    graph.add_edge(6, 10, 6, 6, ForcedRight);

    // add these (useless) edges
    EdgePtr useless_edge_1 = graph.add_edge(10, 5, 2, 7, ForcedLeft);
    EdgePtr useless_edge_2 = graph.add_edge(13, 2, 5, 7, ForcedLeft);

    graph.compute_residual_maxflow();
    graph.transform_residual_to_flow();


    size_t flow = graph.get_flow();

    EXPECT_EQ(flow, 24);

    EXPECT_EQ(graph.get_edge_flow(e_source_1), 10);
    EXPECT_EQ(graph.get_edge_flow(e_source_2), 14);

    EXPECT_EQ(graph.get_edge_flow(e_sink_1), 0);
    EXPECT_EQ(graph.get_edge_flow(e_sink_2), 24);

    EXPECT_EQ(graph.get_edge_flow(useless_edge_1), 0);
    EXPECT_EQ(graph.get_edge_flow(useless_edge_2), 0);
}


} // namespace test
} // namespace details
} // namespace tethys
} // namespace sse
#include <gtest/gtest.h>
#include "../include/Grid.hpp"


class GridTest : public ::testing::Test {
protected:
    GridParameters params;

    void SetUp() override {
        params.x_start = 0.0;
        params.x_end = 1.0;
        params.growth_factor = 1.0;
        params.attraction_force_constant = 1.0;
        params.integ_ord = 5;
        params.nb_elem = 10;
        params.outputfile_path = "./output";
    }
};
using GridDeathTest = GridTest;


TEST_F(GridTest, ConstructorCreatesCorrectNumberOfElements) {
    Grid grid(params);
    EXPECT_EQ(grid.get_elementNumber(), 10);
}


TEST_F(GridTest, ConstructrFailsWhenX_endLessThanX_start) {
    params.x_start = 1.0;
    params.x_end = 0.0;
    EXPECT_EXIT({
        Grid grid(params);
    }, ::testing::ExitedWithCode(1), ".*x_end must be greater than x_start.*");
}

TEST_F(GridTest, ConstructrFailsWhenX_endEqualToX_start) {
    params.x_start = 1.0;
    params.x_end = 1.0;
    EXPECT_EXIT({
        Grid grid(params);
    }, ::testing::ExitedWithCode(1), ".*x_end must be greater than x_start.*");
}

TEST_F(GridTest, ConstructorFailsWhenWrongIntegrationOrder) {
    params.integ_ord = 0;
    EXPECT_EXIT({
        Grid grid(params);
    }, ::testing::ExitedWithCode(1), ".*Grid Integration Order must be greater than 0.*");
}

TEST_F(GridDeathTest, ConstructorFailsWhenZeroElements) {
    params.nb_elem = 0;
    EXPECT_EXIT({
        Grid grid(params); 
    }, ::testing::ExitedWithCode(1), ".*Number of elements must be greater than 0.*");
}

TEST_F(GridTest, GetNodePositionReturnsCorrectValue) {
    Grid grid(params);
    grid.buildGrid();
    EXPECT_DOUBLE_EQ(grid.get_nodePosition(0), 0.0);
    EXPECT_DOUBLE_EQ(grid.get_nodePosition(10), 1.0);
}

TEST_F(GridTest, NodesAreSctrictlyMonotonic) {
    Grid grid(params);
    grid.buildGrid();
    
    for (size_t i=1; i<params.nb_elem; i++){
        EXPECT_GT(grid.get_nodePosition(i), grid.get_nodePosition(i-1)) 
        << "Node " << i << " is not greater than Node " << i-1;
    }
}

TEST_F(GridTest, ZeroAtractionForceConstantLeadsToUniformGrid) {
    params.attraction_force_constant = 0.0;
    Grid grid(params);
    grid.buildGrid();
    
    double expected_dx = (params.x_end - params.x_start) / params.nb_elem;
    
    for (size_t i=1; i<=params.nb_elem; i++){
        EXPECT_NEAR(grid.get_nodePosition(i) - grid.get_nodePosition(i-1), expected_dx, 1e-12)
        << "Node spacing is not uniform at Node " << i;
    }
}

TEST_F(GridTest, near_NonZeroAttractionForceRefinesGridNearBoundary){
    params.attraction_force_constant = 1.0;
    params.nb_elem=10;
    BoundaryConditions bc1{0, 1}; // Dirichlet at node 0

    Grid grid(params);
    grid.set_bc(bc1);
    grid.buildGrid();

    

    double expected_dx = (params.x_end - params.x_start) / params.nb_elem;
    double real_dx = grid.get_nodePosition(1) - grid.get_nodePosition(0);
    EXPECT_LT(real_dx, expected_dx) << "Node spacing is not less than uniform spacing at Node 1";
}

TEST_F(GridTest, far_NonZeroAttractionForceRefinesGridNearBoundary){
    params.attraction_force_constant = 1.0;
    BoundaryConditions bc1{0, 1}; // Dirichlet at node 0
    params.nb_elem=10;
    Grid grid(params);
    grid.set_bc(bc1);
    grid.buildGrid();

    double expected_dx = (params.x_end - params.x_start) / params.nb_elem;
    double real_dx = grid.get_nodePosition(params.nb_elem) - grid.get_nodePosition(params.nb_elem - 1);
    EXPECT_GT(real_dx, expected_dx) << "Node spacing is not less than uniform spacing at Node 10";
}
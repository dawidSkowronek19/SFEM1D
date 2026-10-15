#ifndef GRID_HPP
#define GRIP_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_splinalg.h>

#include "./bc.hpp"

struct GridParameters{
    double x_start, x_end;
    double growth_factor;
    double attraction_force_constant;
    int integ_ord;
    int nb_elem;
    std::string outputfile_path;
};



class Grid{

    public:
        Grid(const GridParameters &parameters);
        
        void set_bc(const BoundaryConditions bc);
        void buildGrid();
        void safeGrid();


        double get_nodePosition(int node_id) const;
        double get_elementNumber() const;


        private:

        std::vector<double> m_nodes;
        std::vector<BoundaryConditions> m_boundaryConditions;
        GridParameters m_gridParameters;

};

#endif
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <functional>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>

#include "../include/Grid.hpp"
#include "../include/ShapeFunction.hpp"
#include "../include/Quadrature.hpp"
#include "../include/Physics.hpp"
#include "../include/Assembler.hpp"

int main() {
    GridParameters params;
    params.x_start = -1.0;
    params.x_end = 1.0;
    params.nb_elem = 50;
    params.growth_factor = 1.0;
    params.attraction_force_constant = 10.0;
    params.integ_ord = 2;
    params.outputfile_path = "grid_output.txt";

    Grid grid(params);
    grid.buildGrid();

    int num_elements = static_cast<int>(grid.get_elementNumber());
    int p = 1; 
    int total_dofs = num_elements * p + 1; 

    ShapeFunction sf(p);
    Quadrature quad = QuadratureFactory::refLine(p + 1);
    
    sf.set_cache_val(quad.get_points());

    auto A_func = [](double x) { return 1.0; };
    auto B_func = [](double x) { return exp(-50.0*x*x); };
    auto f_func = [](double x) { return 0.0; };

    LaplaceIntegrator laplace(sf, A_func);
    ReactionIntegrator free_term(sf, B_func);
    SourceIntegrator source(sf, f_func);

    Assembler assembler(total_dofs);
    assembler.reserve_triplets(num_elements, p);

    for (int e = 0; e < num_elements; ++e) {
        ElementGeometry1D geom;
        geom.x1 = grid.get_nodePosition(e);
        geom.x2 = grid.get_nodePosition(e + 1);

        laplace.S_clear();
        laplace.S_loc(geom, quad);

        free_term.S_clear();
        free_term.S_loc(geom, quad);

        source.F_clear();
        source.F_loc(geom, quad);

        std::vector<int> dofs(p + 1);
        for (int i = 0; i <= p; ++i) {
            dofs[i] = e * p + i;
        }

        assembler.include_Smatrix(laplace.get_S()+free_term.get_S(), dofs);
        assembler.include_Fvector(source.get_F(), dofs);
    }

    assembler.assemble();

    std::map<int, double> bc_values;
    bc_values[0] = 0.0;
    bc_values[total_dofs - 1] = 1.0;

    assembler.apply_DirBC(bc_values);

    Eigen::SparseLU<Eigen::SparseMatrix<double>> solver;
    solver.compute(assembler.get_stiffnessMatrix());
    
    if (solver.info() != Eigen::Success) {
        std::cerr << "Błąd dekompozycji macierzy!" << std::endl;
        return -1;
    }

    Eigen::VectorXd solution = solver.solve(assembler.get_loadVector());
    
    if (solver.info() != Eigen::Success) {
        std::cerr << "Błąd podczas rozwiązywania układu równań!" << std::endl;
        return -1;
    }

    std::ofstream out_file("solution.csv");
    if (!out_file.is_open()) {
        std::cerr << "Nie udało się otworzyć pliku do zapisu!" << std::endl;
        return -1;
    }

    out_file << "x,u\n";
    for (int i = 0; i < total_dofs; ++i) {
        out_file << std::setprecision(10) << grid.get_nodePosition(i) << "," << solution(i) << "\n";
    }
    out_file.close();

    std::cout << "Wyniki zostały pomyślnie zapisane do pliku solution.csv" << std::endl;

    return 0;
}
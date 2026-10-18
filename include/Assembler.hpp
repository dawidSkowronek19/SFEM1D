#ifndef ASSEMBLER
#define ASSEMBLER

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <Eigen/Sparse>

class Assembler{

    public:
        explicit Assembler(int total_dofs);

        //add local elements to global matrix/vec
        void include_Smatrix(const Eigen::MatrixXd& S_local, const std::vector<int>& dof_indices);
        void include_Fvector(const Eigen::VectorXd& F_local, const std::vector<int>& dof_indices);

        void assemble();
        void apply_DirBC(const std::map<int, double>& boundary_values);
        void reserve_triplets(int num_elements, int p);

        //getters
        const Eigen::SparseMatrix<double>& get_stiffnessMatrix() const;
        const Eigen::VectorXd& get_loadVector() const;

    private:
        int m_total_dofs;
        Eigen::SparseMatrix<double> m_S;
        Eigen::VectorXd m_F;
        std::vector<Eigen::Triplet<double>> m_triplets;

};


#endif
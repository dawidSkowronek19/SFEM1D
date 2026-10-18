#include "../include/Assembler.hpp"

Assembler::Assembler(int total_dofs) : m_total_dofs(total_dofs){

    m_F = Eigen::VectorXd::Zero(m_total_dofs);
    m_S = Eigen::SparseMatrix<double>(m_total_dofs, m_total_dofs);
    
}

void Assembler::include_Smatrix(const Eigen::MatrixXd& S_loc, const std::vector<int>& dof_indices){

    const int localSize = static_cast<int>(S_loc.rows());
    for (int i = 0; i < localSize; ++i)
    {
        const int global_I = dof_indices[i];
        for (int j = 0; j < localSize; ++j)
        {
            const int global_J = dof_indices[j];
            m_triplets.push_back(Eigen::Triplet<double>(global_I, global_J, S_loc(i, j)));
        }
    }
}

void Assembler::include_Fvector(const Eigen::VectorXd& F_loc, const std::vector<int>& dof_indices){
    const int localSize = static_cast<int>(F_loc.rows());
    for (int i = 0; i < localSize; ++i)
    {
        const int global_I = dof_indices[i];
        m_F(global_I) += F_loc[i];
    }
}

void Assembler::assemble(){


    m_S.resize(m_total_dofs, m_total_dofs);
    m_S.setFromTriplets(m_triplets.begin(), m_triplets.end());
}

void Assembler::reserve_triplets(int num_elements, int p){
    
    int dofs_per_element = p + 1;
    size_t estimated_triplets = static_cast<size_t>(num_elements) * dofs_per_element * dofs_per_element;
    
    m_triplets.reserve(estimated_triplets);
}

void Assembler::apply_DirBC(const std::map<int, double>& boundary_values)
{
    for (int col = 0; col < m_S.outerSize(); ++col)
    {
        for (Eigen::SparseMatrix<double>::InnerIterator it(m_S, col); it; ++it)
        {
            if (boundary_values.count(it.row()))
            {
                it.valueRef() = 0.0;
            }
        }
    }
    for (const auto& [dof_idx, dirichlet_val] : boundary_values)
    {
        m_S.coeffRef(dof_idx, dof_idx) = 1.0;
        m_F(dof_idx) = dirichlet_val;
    }

    m_S.makeCompressed();
}

const Eigen::SparseMatrix<double>& Assembler::get_stiffnessMatrix() const 
{ 
    return m_S; 
}

const Eigen::VectorXd& Assembler::get_loadVector() const 
{ 
    return m_F; 
}
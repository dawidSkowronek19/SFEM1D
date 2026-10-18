#include "../include/Physics.hpp"

// ==================== OPERATORY BAZOWE ====================

BilinearOperator::BilinearOperator(const ShapeFunction& sh_func) 
    : m_shfunc(sh_func) 
{
    int sh_nb = m_shfunc.get_num_shape_functions(); // Dla 1D wynosi (p + 1)
    m_S = Eigen::MatrixXd::Zero(sh_nb, sh_nb);
}

LinearOperator::LinearOperator(const ShapeFunction& sh_func) 
    : m_shfunc(sh_func) 
{
    int sh_nb = m_shfunc.get_num_shape_functions(); // Dla 1D wynosi (p + 1)
    m_F = Eigen::VectorXd::Zero(sh_nb);
}

void BilinearOperator::S_clear() { m_S.setZero(); }
void LinearOperator::F_clear() { m_F.setZero(); }

const Eigen::MatrixXd& BilinearOperator::get_S() const { return m_S; }
const Eigen::VectorXd& LinearOperator::get_F() const { return m_F; }

// ==================== KONSTRUKTORY DZIECI ====================

LaplaceIntegrator::LaplaceIntegrator(const ShapeFunction& sh_func, 
                                     const std::function<double(double)>& A)
    : BilinearOperator(sh_func), m_A(A) {}

AdvectionIntegrator::AdvectionIntegrator(const ShapeFunction& sh_func, 
                                         const std::function<double(double)>& v)
    : BilinearOperator(sh_func), m_v(v) {}

SimpleIntegrator::SimpleIntegrator(const ShapeFunction& sh_func, 
                                   const std::function<double(double)>& A, 
                                   const double multi)
    : BilinearOperator(sh_func), m_A(A), m_multi(multi) {}

ReactionIntegrator::ReactionIntegrator(const ShapeFunction& sh_func, 
                                       const std::function<double(double)>& A)
    : SimpleIntegrator(sh_func, A, -1.0) {}

MassDampingIntegrator::MassDampingIntegrator(const ShapeFunction& sh_func, 
                                             const std::function<double(double)>& A)
    : SimpleIntegrator(sh_func, A, 1.0) {}

SourceIntegrator::SourceIntegrator(const ShapeFunction& sh_func, 
                                   const std::function<double(double)>& A)
    : LinearOperator(sh_func), m_A(A) {}


void LaplaceIntegrator::S_loc(const ElementGeometry1D& geom, const Quadrature& quad)
{
    const double detJ = geom.detJ();
    const double invJ = geom.invJ();
    const int sh_nb = m_shfunc.get_num_shape_functions();

    const double factor = invJ;

    for (int q = 0; q < quad.get_point_numb(); ++q)
    {
        const double ksi = quad.get_ksi(q);
        const double weight = quad.get_weight(q);
        const double x = geom.to_global(ksi);
        const double A_val = m_A(x);

        const double w_A_factor = weight * A_val * factor;

        for (int i = 0; i < sh_nb; ++i)
        {
            const double dphi_i = m_shfunc.get_cached_dphi(q, i);

            for (int j = i; j < sh_nb; ++j)
            {
                const double dphi_j = m_shfunc.get_cached_dphi(q, j);
                const double val = w_A_factor * dphi_i * dphi_j;

                m_S(i, j) += val;
                if (i != j) m_S(j, i) += val;
            }
        }
    }
}


void AdvectionIntegrator::S_loc(const ElementGeometry1D& geom, const Quadrature& quad)
{
    const int sh_nb = m_shfunc.get_num_shape_functions();

    for (int q = 0; q < quad.get_point_numb(); ++q)
    {
        const double ksi = quad.get_ksi(q);
        const double weight = quad.get_weight(q);
        const double x = geom.to_global(ksi);
        const double v_val = m_v(x);

        const double w_v = weight * v_val;

        for (int i = 0; i < sh_nb; ++i)
        {
            const double phi_i = m_shfunc.get_cached_phi(q, i);

            for (int j = 0; j < sh_nb; ++j)
            {
                const double dphi_j = m_shfunc.get_cached_dphi(q, j);
                m_S(i, j) += w_v * phi_i * dphi_j;
            }
        }
    }
}


void SimpleIntegrator::S_loc(const ElementGeometry1D& geom, const Quadrature& quad)
{
    const double detJ = geom.detJ();
    const int sh_nb = m_shfunc.get_num_shape_functions();

    for (int q = 0; q < quad.get_point_numb(); ++q)
    {
        const double ksi = quad.get_ksi(q);
        const double weight = quad.get_weight(q);
        const double x = geom.to_global(ksi);
        const double A_val = m_A(x);

        const double w_detJ_A = m_multi * weight * detJ * A_val;

        for (int i = 0; i < sh_nb; ++i)
        {
            const double phi_i = m_shfunc.get_cached_phi(q, i);

            for (int j = i; j < sh_nb; ++j)
            {
                const double phi_j = m_shfunc.get_cached_phi(q, j);
                const double val = w_detJ_A * phi_i * phi_j;

                m_S(i, j) += val;
                if (i != j) m_S(j, i) += val;
            }
        }
    }
}

void SourceIntegrator::F_loc(const ElementGeometry1D& geom, const Quadrature& quad)
{
    const double detJ = geom.detJ(); // h_e
    const int sh_nb = m_shfunc.get_num_shape_functions();

    for (int q = 0; q < quad.get_point_numb(); ++q)
    {
        const double ksi = quad.get_ksi(q);
        const double weight = quad.get_weight(q);
        const double x = geom.to_global(ksi);
        const double f_val = m_A(x);

        const double w_detJ_f = weight * detJ * f_val;

        for (int i = 0; i < sh_nb; ++i)
        {
            const double phi_i = m_shfunc.get_cached_phi(q, i);
            m_F(i) += w_detJ_f * phi_i;
        }
    }
}
#include "../include/ShapeFunction.hpp"

ShapeFunction::ShapeFunction(int p) : m_p(p){
    m_IJ.push_back({m_p, 0});
    m_IJ.push_back({0, m_p});
    if (p>1){
        for (int idx = m_p-1; idx>=1; idx--){
            m_IJ.push_back({idx, m_p-idx});
        }
    }
}

std::pair<double, double> ShapeFunction::lambda_generator(const double ksi) const{
    return {1.0-ksi, ksi};
}

double ShapeFunction::Silvester(const int i, const double lambda) const{
    if (i==0) return 1.0;

    double value=1.0;
    for (int idx=0; idx<i; idx++){
        double T = m_p*lambda-idx;
        double B=i-idx;
        value*=(T/B);
    }

    return value;
}

double ShapeFunction::div_Silvester(const int i, const double lambda) const
{
    if (i == 0) return 0.0;

    double div = 0.0;
    for (int sum_idx = 0; sum_idx < i; sum_idx++)
    {
        double mult = 1.0;
        for (int mult_idx = 0; mult_idx < i; mult_idx++)
        {
            if (mult_idx == sum_idx) continue;
            mult *= (m_p * lambda - mult_idx) / (i - mult_idx);
        }

        div += (static_cast<double>(m_p) / (i - sum_idx)) * mult;
    }

    return div;
}

//=================== getters ===============

double ShapeFunction::compute_phi(double ksi, int idx) const{
    auto [lambda1, lambda2] = lambda_generator(ksi);
    auto[I,J]=m_IJ[idx];

    return Silvester(I,lambda1)*Silvester(J,lambda2);
}

double ShapeFunction::compute_dphi(double ksi, int idx) const
{
    auto [lambda1, lambda2] = lambda_generator(ksi);
    auto [I, J] = m_IJ[idx];

    double S_I = Silvester(I, lambda1);
    double S_J = Silvester(J, lambda2);

    constexpr double dlambda1_dksi = -1.0;
    constexpr double dlambda2_dksi = 1.0;


    double dphi_dksi = div_Silvester(I, lambda1) * dlambda1_dksi * S_J +
                       S_I * div_Silvester(J, lambda2) * dlambda2_dksi;

    return dphi_dksi;
}


double ShapeFunction::get_cached_phi(const int point_idx, const int sh_nb) const 
{ 
    return m_phi(point_idx, sh_nb); 
}

double ShapeFunction::get_cached_dphi(const int point_idx, const int sh_nb) const 
{ 
    return m_dphi_dksi(point_idx, sh_nb); 
}

int ShapeFunction::get_p() const 
{ 
    return m_p; 
}


// =============== setters ==============
void ShapeFunction::set_cache_val(const std::vector<double>& integration_points)
{
    const int shfunc_nb = static_cast<int>(m_IJ.size());
    const int num_points = static_cast<int>(integration_points.size());

    m_phi = Eigen::MatrixXd::Zero(num_points, shfunc_nb);
    m_dphi_dksi = Eigen::MatrixXd::Zero(num_points, shfunc_nb);

    for (int shfunc_idx = 0; shfunc_idx < shfunc_nb; shfunc_idx++)
    {
        for (int point_idx = 0; point_idx < num_points; point_idx++)
        {
            const double ksi = integration_points[point_idx];
            
            m_phi(point_idx, shfunc_idx) = compute_phi(ksi, shfunc_idx);
            m_dphi_dksi(point_idx, shfunc_idx) = compute_dphi(ksi, shfunc_idx);
        }
    }
}


// =========== save shape functions ============

void ShapeFunction::save_shape_functions(const std::vector<double>& ksi_vec, std::string outdir) const{
    std::ofstream file_path(outdir);
    
    const int shfunc_nb = static_cast<int>(m_IJ.size());
    const int ksi_points_nb = static_cast<int>(ksi_vec.size());
    file_path<<"# ";
    for (int i=0; i<shfunc_nb; i++){
        file_path<<"phi_"<<i<<"\t";
    }

    for (const auto& ksi:ksi_vec)
    {
        for (int i=0; i<shfunc_nb; i++){
            file_path<<'['<<compute_phi(ksi, i)<<','<<compute_dphi(ksi, i)<<']'<<" ";
        }
        file_path<<"\n";
    }
}

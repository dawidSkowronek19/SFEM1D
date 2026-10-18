#include "../include/Quadrature.hpp"


double Quadrature::get_ksi(const int idx) const {

    return m_points[idx]; 
}

double Quadrature::get_weight(const int idx) const {

    return m_weights[idx]; 
}

int Quadrature::get_point_numb() const {

    return static_cast<int>(m_weights.size()); 
}

const std::vector<double>& Quadrature::get_points() const { 

    return m_points; 
}

const std::vector<double>& Quadrature::get_weights() const { 

    return m_weights; 
}

void Quadrature::set_point(double ksi, double weight){

    m_points.push_back(ksi);
    m_weights.push_back(weight);
}

Quadrature QuadratureFactory::refLine(int num_points)
{
    if (num_points < 1)
    {
        throw std::invalid_argument("[Error] # quad points must be >=1");
    }

    Quadrature quad;

    if (num_points == 1)
    {
        quad.set_point(0.5, 1.0);
        return quad;
    }

    // Algorithm: Golub-Welsch
    Eigen::MatrixXd J = Eigen::MatrixXd::Zero(num_points, num_points);

    for (int k = 1; k < num_points; ++k)
    {
        double beta = static_cast<double>(k)/std::sqrt(4.0 * k * k - 1.0);
        J(k - 1, k) = beta;
        J(k, k - 1) = beta;
    }

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(J);
    
    const Eigen::VectorXd& nodes = solver.eigenvalues();
    const Eigen::MatrixXd& eigenvectors = solver.eigenvectors();

    for (int i = 0; i < num_points; ++i)
    {
        //[-1, 1] -> [0, 1]
        double ksi = 0.5 * (nodes(i) + 1.0);
        double v0 = eigenvectors(0, i);
        double weight = v0 * v0;

        quad.set_point(ksi, weight);
    }

    return quad;
}
#ifndef QUADRATURE
#define QUADRATURE

#include <vector>
#include <Eigen/Dense>
#include <cmath>
#include <stdexcept>

class Quadrature {
public:
    Quadrature() = default;

    double get_ksi(int idx) const;
    double get_weight(int idx) const;

    const std::vector<double>& get_points() const;
    const std::vector<double>& get_weights() const;
    int get_point_numb() const;

    void set_point(double ksi, double weight);

private:
    std::vector<double> m_points;
    std::vector<double> m_weights;
};

class QuadratureFactory {
public:
    static Quadrature refLine(int num_points);
};

#endif
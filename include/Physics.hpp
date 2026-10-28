#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <vector>
#include <functional>
#include <Eigen/Dense>

#include "ShapeFunction.hpp"
#include "Quadrature.hpp"


struct ElementGeometry1D {
    double x1;
    double x2;

    double length() const { return x2 - x1; }
    double detJ() const { return length(); }
    double invJ() const { return 1.0 / length(); }

    double to_global(double ksi) const {
        return x1 + ksi * length();
    }
};


class BilinearOperator {
public:
    explicit BilinearOperator(const ShapeFunction& sh_func);
    virtual ~BilinearOperator() = default;

    virtual void S_loc(const ElementGeometry1D& geom, const Quadrature& quad) = 0;
    void S_clear();
    const Eigen::MatrixXd& get_S() const;

protected:
    const ShapeFunction& m_shfunc;
    Eigen::MatrixXd m_S;
};

class LinearOperator {
public:
    explicit LinearOperator(const ShapeFunction& sh_func);
    virtual ~LinearOperator() = default;

    virtual void F_loc(const ElementGeometry1D& geom, const Quadrature& quad) = 0;
    void F_clear();
    const Eigen::VectorXd& get_F() const;

protected:
    const ShapeFunction& m_shfunc;
    Eigen::VectorXd m_F;
};


class SimpleIntegrator : public BilinearOperator {
public:
    SimpleIntegrator(const ShapeFunction& sh_func, 
                     const std::function<double(double)>& A, 
                     double multi);

    void S_loc(const ElementGeometry1D& geom, const Quadrature& quad) override;

protected:
    std::function<double(double)> m_A;
    double m_multi;
};


class LaplaceIntegrator : public BilinearOperator {
public:
    LaplaceIntegrator(const ShapeFunction& sh_func, 
                      const std::function<double(double)>& A);

    void S_loc(const ElementGeometry1D& geom, const Quadrature& quad) override;

private:
    std::function<double(double)> m_A;
};
class AdvectionIntegrator : public BilinearOperator {
public:
    AdvectionIntegrator(const ShapeFunction& sh_func, 
                        const std::function<double(double)>& v);

    void S_loc(const ElementGeometry1D& geom, const Quadrature& quad) override;

private:
    std::function<double(double)> m_v;
};


class SourceIntegrator : public LinearOperator {
public:
    SourceIntegrator(const ShapeFunction& sh_func, 
                     const std::function<double(double)>& A);

    void F_loc(const ElementGeometry1D& geom, const Quadrature& quad) override;

private:
    std::function<double(double)> m_A;
};


class ReactionIntegrator : public SimpleIntegrator {
public:
    ReactionIntegrator(const ShapeFunction& sh_func, 
                       const std::function<double(double)>& A);
};

class MassDampingIntegrator : public SimpleIntegrator {
public:
    MassDampingIntegrator(const ShapeFunction& sh_func, 
                          const std::function<double(double)>& A);
};

#endif
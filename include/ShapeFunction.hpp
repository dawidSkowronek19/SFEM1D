#ifndef SHAPEFUNCTION
#define SHAPEFUNCTION

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <tuple>
#include <string>
#include <fstream>

class ShapeFunction{

    public:
        explicit ShapeFunction(int p);

        //getters
        double get_cached_phi(const int idx, const int shape_function_nb) const;
        double get_cached_dphi(const int idx, const int shape_function_nb) const;
        double compute_phi(double ksi, int shape_function_nb) const;
        double compute_dphi(double ksi, int shape_function_nb) const;
        int get_p() const;
        int get_num_shape_functions() const { return static_cast<int>(m_IJ.size()); }

        //setters
        void set_cache_val(const std::vector<double>&integration_points);

        void save_shape_functions(const std::vector<double>& ksi_vec, std::string outdir="./output") const;
        
    private:
        const int m_p;
        std::vector<std::pair<int, int>> m_IJ;
        Eigen::MatrixXd m_phi, m_dphi_dksi;


        std::pair<double, double> lambda_generator(const double ksi) const;
        double Silvester(const int i, const double lambda) const;
        double div_Silvester(const int i, const double lambda) const;


};

#endif
#include "./Grid.hpp"

//========== Constructor ==========
Grid::Grid(const GridParameters& parameters) : m_gridParameters(parameters){
    std::cout<<std::string(60, '=');
                
    std::cout<<"\n\n\n \t\t\tGRID SECTION \n\n\n";
                
    std::cout<<"\t\t"<<std::string(4, '#')<<" GRID PARAMETERS "<<std::string(4, '#')<<"\n\n";
    std::cout<<"\tx_start = "<<m_gridParameters.x_start<<"\n";
    std::cout<<"\tx_end = "<<m_gridParameters.x_end<<"\n";
    if (m_gridParameters.x_end <= m_gridParameters.x_start || std::abs(m_gridParameters.x_end - m_gridParameters.x_start) < 1e-12){
        std::cerr<<"\t[ ERROR ] x_end must be greater than x_start\n";
        std::exit(1);
    }
    std::cout<<"\tGrowth Factor = "<<m_gridParameters.growth_factor<<"\n";
    std::cout<<"\tAttraction Force Constant = "<<m_gridParameters.attraction_force_constant<<"\n";
    std::cout<<"\tGrid Integration Order = "<<m_gridParameters.integ_ord<<"\n";
    if (m_gridParameters.integ_ord < 1){
        std::cerr<<"\t[ ERROR ] Grid Integration Order must be greater than 0\n";
        std::exit(1);
    }
    std::cout<<"\tNumber of elements = "<<m_gridParameters.nb_elem<<"\n\n";
    if (m_gridParameters.nb_elem < 1){
        std::cerr<<"\t[ ERROR ] Number of elements must be greater than 0\n";
        std::exit(1);
    }
    std::cout<<"\tOutput File Path = "<<m_gridParameters.outputfile_path<<"\n\n";
    std::cout<<"\t\t"<<"# GRID PARAMETERS END #\n\n";


    m_nodes.resize(m_gridParameters.nb_elem+1, 0.0);
}

void Grid::set_bc(const BoundaryConditions bc){
    m_boundaryConditions.push_back(bc);

    std::cout<<std::string(4, '#')<<" BOUNDARY CONDITIONS "<<std::string(4, '#')<<"\n\n";
    
    if (bc.type==1){
        std::cout<<"\t Dirichlet Condition ";
    }
    else if (bc.type==2){
        std::cout<<"\t Neumann Condition ";
    }
    else{
        std::cerr<<"\t [ ERROR ] Unknown Condition "; // This should not happen
        std::exit(1);
    }

}


void Grid::buildGrid(){
    std::cout<<"\n\n\t\t"<<std::string(4, '#')<<" BUILDING GRID "<<std::string(4, '#')<<"\n\n";

    int bc_nb = m_boundaryConditions.size();
    double dx = (m_gridParameters.x_end - m_gridParameters.x_start) / m_gridParameters.nb_elem;
    double sigma = m_gridParameters.growth_factor;
    double A = m_gridParameters.attraction_force_constant;
    int integ_ord = m_gridParameters.integ_ord;

    std::vector<double> bc_x;

    for (auto&bc : m_boundaryConditions){
        double initial_x = m_gridParameters.x_start + bc.node_id * dx;
        bc_x.push_back(initial_x);
    }

    auto node_need = [bc_nb, &bc_x, sigma, A](double x) {
        double omega=1.0;
        for (int i=0; i<bc_nb; i++){
            double power = (x-bc_x[i])*(x-bc_x[i])/(2*sigma*sigma);
            omega+=A*exp(-power);
        }
        return omega;    
    };


    const gsl_integration_fixed_type *T = gsl_integration_fixed_legendre;
    gsl_integration_fixed_workspace *work = gsl_integration_fixed_alloc(T, integ_ord, 0, dx, 0, 0);
    double *int_nods = gsl_integration_fixed_nodes(work);
    double *int_w = gsl_integration_fixed_weights(work);

    std::vector<double> F(m_gridParameters.nb_elem+1, 0.0);
    for (int m=1; m<=m_gridParameters.nb_elem; m++){
        double x_prev = m_gridParameters.x_start + (m-1)*dx;
 

        for (int i=0; i<integ_ord; i++){
            F[m] += int_w[i]*node_need(x_prev + int_nods[i]);
        }

        F[m]+=F[m-1];
    }

    gsl_integration_fixed_free(work);

    double F_L=F.back();
    for (int j=0; j<m_gridParameters.nb_elem+1; j++)
    {
        double targetF=j*F_L/m_gridParameters.nb_elem;
        auto it = std::lower_bound(F.begin(), F.end(), targetF);

        if (it==F.begin())
        {
            m_nodes[j]=m_gridParameters.x_start;
        }
        else
        {
            size_t idx1 = std::distance(F.begin(), it);
            size_t idx0 = idx1 - 1;

            double F0 = F[idx0], F1 = F[idx1];
            double x0 = m_gridParameters.x_start + idx0 * dx;
            double x1 = m_gridParameters.x_start + idx1 * dx;

            double t = (targetF - F0) / (F1 - F0);
            m_nodes[j] = x0 + t * (x1 - x0);
        }
    }

    std::cout<<"\t\t"<<std::string(4, '#')<<" GRID BUILDING END "<<std::string(4, '#')<<"\n\n";
}

void Grid::safeGrid(){
    std::cout<<"\n\n\t\t"<<std::string(4, '#')<<" SAVING GRID "<<std::string(4, '#')<<"\n\n";

    std::ofstream outfile(m_gridParameters.outputfile_path + "/grid_nodes.txt");
    for (const auto& node : m_nodes) {
        outfile << node << "\n";
    }
    outfile.close();

    std::cout<<"\t\t"<<std::string(4, '#')<<" GRID SAVED "<<std::string(4, '#')<<"\n\n";
}

double Grid::get_nodePosition(int node_id) const {
    if (node_id < 0 || node_id >= m_nodes.size()) {
        std::cerr << "[ ERROR ] Node ID out of bounds\n";
        std::exit(1);
    }
    return m_nodes[node_id];
}

double Grid::get_elementNumber() const {
    return m_nodes.size() - 1;
}
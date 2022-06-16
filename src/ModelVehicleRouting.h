
#ifndef MODELVEHICLEROUTING_H
#define MODELVEHICLEROUTING_H

#include "Model.h"
#include "Solution.h"

class ModelVehicleRouting : public Model {

    private:
    
        // Variable names
        string x;
        string y;
        
        // Solution values
        int V;
        int K;
        vector<vector<vector<double>>> sol_x;
        vector<double> sol_y;

        virtual void reserveSolutionSpace(const Data* data);
        virtual void readSolution        (const Data* data);
        virtual void assignWarmStart     (const Data* data);
        virtual void createModel         (const Data* data);
        
        void printSolutionVariables(int digits = 5, int decimals = 2);
        vector<SolverCut> separationAlgorithm(vector<double> sol);
        void traverseGraph(int vehicleId, int vertex, vector<bool> visited);
        bool graphIsConnected(int vehicleIndex);
        
    public:
        
        ModelVehicleRouting();

        virtual ~ModelVehicleRouting();

        virtual void execute(const Data *data);


};    

#endif 



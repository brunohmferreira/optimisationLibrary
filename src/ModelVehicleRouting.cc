
#include "ModelVehicleRouting.h"
#include "Options.h"
#include "DataVehicleRouting.h"

ModelVehicleRouting::ModelVehicleRouting() : Model(){
    V = 0;
    K = 0;
    x = "x";
    y = "y";
}

ModelVehicleRouting::~ModelVehicleRouting() {
}

void ModelVehicleRouting::execute(const Data* data) {

    float startTime = Util::getTime();

    if (debug > 1) solver->printSolverName();

    createModel(data);
    reserveSolutionSpace(data);
    assignWarmStart(data);
    setSolverParameters(0);

    solver->addInfoCallback(this);
    solver->addLazyCallback(this);

    solve(data);

    totalTime = Util::getTime() - startTime;
    printSolutionVariables();
} 

void ModelVehicleRouting::printSolutionVariables(int digits, int decimals) {
    if (debug) {
        if (solver->solutionExists() && !solver->isInfeasible() && !solver->isUnbounded()) {
            vector<string> route;
            int indexRoute;
            printf("\nSolution: \n");
            for (int k = 0; k < K; k++) {
                route.resize(V);
                std::fill(route.begin(), route.end(), "");
                indexRoute = 0;
                printf("Vehicle %d \n", k);
                for (int j = 0; j < V; j++) {
                    printf("\t Xi%d", j);
                }
                printf("\n");
                for (int i = 0; i < V; i++) {
                    printf("X%dj", i);
                    for (int j = 0; j < V; j++) {
                        if(i == j)
                            printf("\t 0");
                        else
                            printf("\t %.0f", sol_x[k][i][j]);

                        if(round(sol_x[k][i][j]) == 1) {
                            route[indexRoute] = "(" + lex(i) + ", " + lex(j) + ") "; 
                            indexRoute++;
                        }

                    }
                    printf("\n");
                }
                printf("\n");
                printf("Route (Vehicle %d): ", k);
                for (int i = 0; i < V; i++) {
                    printf("%s", route[i].c_str());
                }
                printf("\n\n");
            }
            printf("\n");
            for (int k = 0; k < K; k++) {
                printf("y%d: %.0f\n", k, sol_y[k]);
            }
        }
    }

}

void ModelVehicleRouting::reserveSolutionSpace(const Data* data) {
    sol_x.resize(K,vector<vector<double>>(V, vector<double>(V)));
    sol_y.resize(K);
}

void ModelVehicleRouting::readSolution(const Data* data) {
    totalNodes = solver->getNodeCount();
    solution->resetSolution();
    solution->setSolutionStatus(solver->solutionExists(), solver->isOptimal(),  solver->isInfeasible(), solver->isUnbounded());
    if (!solver->solutionExists()) {
        if (debug) printf("Solution does not exist\n");        
    } else {
        solution->setValue    (solver->getObjValue() );
        solution->setBestBound(solver->getBestBound());

        for (int k = 0; k < K; k++) {
            for (int i = 0; i < V; i++) {
                for (int j = 0; j < V; j++) {
                    if (i == j)
                        sol_x[k][i][j] = 0;
                    else
                        sol_x[k][i][j] = solver->getColValue(x + lex(k) + UND + lex(i) + UND + lex(j));
                }
            }
        }

        for (int k = 0; k < K; k++) {
            sol_y[k] = solver->getColValue(y + lex(k));
        }
    }
}


void ModelVehicleRouting::createModel(const Data* data) {
    
    const DataVehicleRouting* dataCMR = dynamic_cast<const DataVehicleRouting*>(data);
    V = (dataCMR->getNumberOfClients()) + 1;
    K = dataCMR->getVehicleFleet();
    solver->changeObjectiveSense(0);

    // x variable
    for (int k = 0; k < K; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (i != j)
                    solver->addBinaryVariable(dataCMR->getDistance(i, j), x + lex(k) + UND + lex(i) + UND + lex(j));
            }
        }
    }

    // y variable
    for (int k = 0; k < K; k++)
        solver->addBinaryVariable(dataCMR->getFixedCost(), y + lex(k));


    vector<string> colNames;
    vector<double> elements;

    int indexAux;
    
    // at least one vehicle attends a client (1b and 1c)
    colNames.resize(K * V - K);
    elements.resize(K * V - K);
    for (int j = 1; j < V; j++) {
        indexAux = 0;
        for (int k = 0; k < K; k++) {
            for (int i = 0; i < V; i++) {
                if (j != i) {
                    colNames[indexAux] = x + lex(k) + UND + lex(i) + UND + lex(j);
                    elements[indexAux] = 1;
                    indexAux++;
                }
            }
        }
        solver->addRow(colNames, elements, 1, 'E', "constraint1b_" + lex(j));
    }

    colNames.resize(K * V - K);
    elements.resize(K * V - K);
    for (int i = 1; i < V; i++) {
        indexAux = 0;
        for (int k = 0; k < K; k++) {
            for (int j = 0; j < V; j++) {
                if (i != j) {
                    colNames[indexAux] = x + lex(k) + UND + lex(i) + UND + lex(j);
                    elements[indexAux] = 1;
                    indexAux++;
                }
            }
        }
        solver->addRow(colNames, elements, 1, 'E', "constraint1c_" + lex(i));
    }

    // relationship between x and y (1d)
    colNames.resize(V);
    elements.resize(V);
    for (int k = 0; k < K; k++) {
        indexAux = 0;

        for (int j = 1; j < V; j++) {
            colNames[indexAux] = x + lex(k) + UND + '0' + UND + lex(j);
            elements[indexAux] = 1;
            indexAux++;
        }

        colNames[indexAux] = y + lex(k);
        elements[indexAux] = -1;

        solver->addRow(colNames, elements, 0, 'L', "constraint1d_" + lex(k));
    }

    // truck sequence (truck queue) constraint (1e)
    colNames.resize(V + V - 2);
    elements.resize(V + V - 2);
    for (int k = 1; k < K; k++) {
        indexAux = 0;

        for (int j = 1; j < V; j++) {
            colNames[indexAux] = x + lex(k-1) + UND + '0' + UND + lex(j);
            elements[indexAux] = 1;
            indexAux++;
            colNames[indexAux] = x + lex(k) + UND + '0' + UND + lex(j);
            elements[indexAux] = -1;
            indexAux++;
        }

        solver->addRow(colNames, elements, 0, 'G', "constraint1e_" + lex(k));
    }

    // enter and leave constraint (1f)
    colNames.resize((V - 1) + (V - 1));
    elements.resize((V - 1) + (V - 1));
    for (int k = 0; k < K; k++) {
        for (int h = 0; h < V; h++) {
            indexAux = 0;

            for (int i = 0; i < V; i++) {
                if (h != i) {
                    colNames[indexAux] = x + lex(k) + UND + lex(i) + UND + lex(h);
                    elements[indexAux] = 1;
                    indexAux++;
                }
            }

            for (int j = 0; j < V; j++) {
                if (h != j) {
                    colNames[indexAux] = x + lex(k) + UND + lex(h) + UND + lex(j);
                    elements[indexAux] = -1;
                    indexAux++;
                }
            }

            solver->addRow(colNames, elements, 0, 'E', "constraint1f_" + lex(k) + UND + lex(h));
        } 
    }

    // capacity constraint (1h)
    colNames.resize(V * V - V);
    elements.resize(V * V - V);
    for (int k = 0; k < K; k++) {
        indexAux = 0;

        for (int j = 0; j < V; j++) {
            for (int i = 0; i < V; i++) {
                if (i != j) {
                    colNames[indexAux] = x + lex(k) + UND + lex(i) + UND + lex(j);
                    elements[indexAux] = dataCMR->getDemand(j).getQuantity(dataCMR->getDemand(j).clientId);
                    indexAux++;
                }
            }
        }

        solver->addRow(colNames, elements, dataCMR->getVehicleCapacity(), 'L', "constraint1h_" + lex(k));
    }
}

void ModelVehicleRouting::assignWarmStart(const Data* data) { }

// Cutting planes

vector<SolverCut> ModelVehicleRouting::separationAlgorithm(vector<double> sol) {
    vector<SolverCut> cuts;
    bool routeIsConnected;

    for (int k = 0; k < K; k++) {

        routeIsConnected = graphIsConnected(k);

        if (!routeIsConnected) {
            SolverCut cut;
            for (int i = 1; i < V; i++) {
                for (int j = 1; j < V; j++) {
                    if (i != j) {
                        double currentSolIndex = solver->getColIndex(x + lex(k) + UND + lex(i) + UND + lex(j));
                        if (sol[currentSolIndex] == 1) {
                            cut.addCoef(currentSolIndex, 1);
                        }
                    }
                }
            }

            if (cut.getCoefs().size() > 1) {
                cut.setName("cut1g_" + lex(k));
                cut.setSense('L');
                cut.setRHS(cut.getCoefs().size() - 1);
                cuts.push_back(cut);
            }
        }
    }
    
    return cuts;
}

void ModelVehicleRouting::traverseGraph(int vehicleId, int vertex, vector<bool> visited) {
    visited[vertex] = true; //mark as visited
    for (int v = 0; v < V; v++) {
        if (sol_x[vehicleId][vertex][v] == 1) {
            if (!visited[v])
                traverseGraph(vehicleId, v, visited);
        }
    }
}

bool ModelVehicleRouting::graphIsConnected(int vehicleIndex) { // passar alreadyVisited como parametro
    vector<bool> alreadyVisited;
    alreadyVisited.resize(V);
    //for all vertex as start point, check whether all nodes are visible or not
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++)
            alreadyVisited[j] = false; //initialize as no node is visited
        traverseGraph(vehicleIndex, i, alreadyVisited);
        for (int j = 0; j < V; j++) {
            if (!alreadyVisited[i]) //if there is a node, not visited by traversal, graph is not connected
                return false;
        }
    }
    return true;
}
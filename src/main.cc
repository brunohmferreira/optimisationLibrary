/**
 * main.cc
 *
 * Copyright(c) 2018
 * Cristiano Arbex Valle
 * All rights reserved.
 */

#include "Options.h"
#include "Execute.h"

void finalise() {
    Options::finalise();
}

int main(int argc, char *argv[]) {
    // string str = "";
    // char requestString[10000];

    try {
        // Read and parse options
        Options::getInstance()->factory();
        Options::getInstance()->parseOptions(argc, argv);
        Options::getInstance()->print();

        Execute execute;
        execute.execute();

        /*strcpy(requestString, "");
        strcat(requestString, "numberOfConstructions: ");
        strcat(requestString, "4 ");
        strcat(requestString, "concreteMixerTruckFleet: ");
        strcat(requestString, "1 ");
        strcat(requestString, "numberOfTypesOfConcrete: ");
        strcat(requestString, "1 ");
        strcat(requestString, "fixedCostOfUsingTheConcreteMixerTruck: ");
        strcat(requestString, "10 ");
        strcat(requestString, "concreteMixerTruckCapacity: ");
        strcat(requestString, "8 ");
        strcat(requestString, "demands: ");
        strcat(requestString, "0 0 0 ");

        
        strcat(requestString, "1 2 0 ");
        strcat(requestString, "2 2 0 ");
        strcat(requestString, "3 2 0 ");
        strcat(requestString, "4 2 0 ");
        

        strcat(requestString, "distances: ");
        strcat(requestString, "0	2	4	6	8 ");
        strcat(requestString, "10	0	12	14	16 ");
        strcat(requestString, "18	20	0	22	24 ");
        strcat(requestString, "26	28	30	0	32 ");
        strcat(requestString, "34	36	48	40	0 ");

        char* request = new char[strlen(requestString) + 1];
        strcpy(request, requestString);

        Execute execute;
        ResponseObject responseObject = execute.execute(request);

        str += "{";
        str += "obj:";
        str += std::to_string(responseObject.solutionResponse.obj);
        str += ",bestBound:";
        str += std::to_string(responseObject.solutionResponse.bestBound);
        str += ",gap:";
        str += std::to_string(responseObject.solutionResponse.gap);
        str += ",solutionExists:";
        str += std::to_string(responseObject.solutionResponse.solutionExists);
        str += ",solutionIsOptimal:";
        str += std::to_string(responseObject.solutionResponse.solutionIsOptimal);
        str += ",solutionIsInfeasible:";
        str += std::to_string(responseObject.solutionResponse.solutionIsInfeasible);
        str += ",solutionsIsUnbounded:";
        str += std::to_string(responseObject.solutionResponse.solutionsIsUnbounded);


        str += ",sol_y: [";
        for (int i = 0; i < responseObject.modelResponse.sol_y.size(); i++) {
            str += std::to_string(responseObject.modelResponse.sol_y[i]);

            if (i != responseObject.modelResponse.sol_y.size() - 1)
                str += ",";
        }
        str += "]";

        str += ",sol_z: [";
        for (int i = 0; i < responseObject.modelResponse.sol_z.size(); i++) {
            str += "[";
            for (int j = 0; j < responseObject.modelResponse.sol_z[i].size(); j++) {
                str += std::to_string(responseObject.modelResponse.sol_z[i][j]);

                if (j != responseObject.modelResponse.sol_z[i].size() - 1)
                    str += ",";
            }
            str += "]";

            if (i != responseObject.modelResponse.sol_z.size() - 1)
                str += ",";
        }
        str += "]";

        str += ",sol_x: [";
        for (int i = 0; i < responseObject.modelResponse.sol_x.size(); i++) {
            str += "[";
            for (int j = 0; j < responseObject.modelResponse.sol_x[i].size(); j++) {
                str += "[";
                for (int k = 0; k < responseObject.modelResponse.sol_x[i][j].size(); k++) {
                    str += std::to_string(responseObject.modelResponse.sol_x[i][j][k]);

                    if (k != responseObject.modelResponse.sol_x[i][j].size() - 1)
                        str += ",";
                }
                str += "]";

                if (j != responseObject.modelResponse.sol_x[i].size() - 1)
                    str += ",";
            }
            str += "]";

            if (i != responseObject.modelResponse.sol_x.size() - 1)
                str += ",";
        }
        str += "]";
        str += "}";*/
        
    } catch (std::invalid_argument& e) {
        printf("%s\n", e.what());
    }
    
    finalise();

    return 0;
}


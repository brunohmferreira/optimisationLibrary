/**
 * Execute.cc
 *
 * Copyright(c) 2018
 * Cristiano Arbex Valle
 * All rights reserved.
 */

#include "Execute.h"
#include "Options.h"
#include "DataCapitalBudgeting.h"
#include "ModelCapitalBudgeting.h"
#include "DataMotivatingProblem.h"
#include "ModelMotivatingProblem.h"
#include "DataKnapsackProblem.h"
#include "ModelKnapsackProblem.h"
#include "DataAssignmentProblem.h"
#include "ModelAssignmentProblem.h"
#include "DataConcreteMixerTruckRouting.h"
#include "ModelConcreteMixerTruckRouting.h"

Execute::Execute() {
    totalTime = 0;
    model = NULL;
    data  = NULL;

}

Execute::~Execute() {
    delete(model);
    delete(data);
}

void Execute::execute() {
    float startTime = Util::getTime();

    if (Options::getInstance()->getStringOption("model").compare("toy") == 0) {
        data  = new DataCapitalBudgeting();
        model = new ModelCapitalBudgeting();
    }
    else if (Options::getInstance()->getStringOption("model").compare("motivating") == 0) {
        data  = new DataMotivatingProblem();
        model = new ModelMotivatingProblem();
    }
    else if (Options::getInstance()->getStringOption("model").compare("knapsackProblem") == 0) {
        data  = new DataKnapsackProblem();
        model = new ModelKnapsackProblem();
    }
    else if (Options::getInstance()->getStringOption("model").compare("assignmentProblem") == 0) {
        data  = new DataAssignmentProblem();
        model = new ModelAssignmentProblem();
    }
    else if (Options::getInstance()->getStringOption("model").compare("concreteMixerTruckRouting") == 0) {
        data = new DataConcreteMixerTruckRouting();
        model = new ModelConcreteMixerTruckRouting();
    }
    else {
        data  = new Data();
        model = new Model();
    }

    data->readData();
    data->print();

    model->execute(data);
    model->printSolution();

    totalTime = Util::getTime() - startTime;

    /*
    if (Options::getInstance()->getIntOption("debug")) {
        printf("\n");
        printf("Solution value:           %8.2f\n",   model->getSolution().getValue());
        printf("Final lower bound:        %8.2f\n",   model->getSolution().getBestBound());
        printf("Gap:                      %8.2f%%\n", model->getSolution().getGap()*100);
        printf("Bound at first node:      %8.2f\n\n", model->getFirstNodeBound());
        printf("Number of nodes solved:   %8d\n",     model->getTotalNodes());
        if (model->getCallbackCalls() > 0) {
            printf("Callback time              %7.3fs (%d calls)\n", model->getCallbackTime(), model->getCallbackCalls());
            printf("   Adding cuts             %7.3fs (%d cuts added)\n", model->getCallbackCutsTime(), model->getCutsAdded());
            printf("   Data processing         %7.3fs\n", model->getCallbackDataTime());
            printf("   Max flow                %7.3fs (%d calls, %d solved)\n",model->getMaxFlowTime(),model->getMaxFlowCalls(),model->getMaxFlowsSolved());
            printf("   BFS                     %7.3fs\n", model->getBfsTime());
        }
        printf("Solver time:               %7.3fs\n",   model->getSolvingTime());
        printf("   First node solved in    %7.3fs\n",   model->getFirstNodeTime());
        printf("   Best solution time      %7.3fs\n",   model->getBestSolutionTime());
        printf("BC time:                   %7.3fs\n",   model->getTotalTime());
        printf("\nEverything finalised in:   %7.3fs\n", totalTime);
    }
    */
}

ResponseObject Execute::execute(char* request) {
    float startTime = Util::getTime();

    std::string                         requestString(request);
    std::stringstream                   requestStringStream(requestString);
    std::istream_iterator<std::string>  begin(requestStringStream);
    std::istream_iterator<std::string>  end;
    std::vector<std::string>            requestSplitted(begin, end);

    std::copy(requestSplitted.begin(), requestSplitted.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

    int     index                   = 0;
    int     numberOfConstructions   = stoi(requestSplitted[1]);
    int     fleet                   = stoi(requestSplitted[3]);
    int     concreteTypes           = stoi(requestSplitted[5]);
    double  cost                    = stod(requestSplitted[7]);
    double  capacity                = stod(requestSplitted[9]);

    vector<ClientDemand> clientDemands;
    clientDemands.resize(numberOfConstructions + 1);

    clientDemands[0].constructionId = stoi(requestSplitted[11]);
    clientDemands[0].setQuantity(stod(requestSplitted[12]));
    clientDemands[0].setConcreteTypeId(stoi(requestSplitted[13]));

    index = 14;
    for (int i = 0; i < numberOfConstructions; i++) {
        clientDemands[i+1].constructionId = stoi(requestSplitted[index]);
        index++;
        clientDemands[i+1].setQuantity(stod(requestSplitted[index]));
        index++;
        clientDemands[i+1].setConcreteTypeId(stod(requestSplitted[index]));
        index++;
    }

    vector<vector<double>> distances;
    distances.resize(numberOfConstructions + 1, vector<double>(numberOfConstructions + 1));

    index++;
    for (int i = 0; i < numberOfConstructions + 1; i++) {
        for (int j = 0; j < numberOfConstructions + 1; j++) {
            distances[i][j] = stod(requestSplitted[index]);
            index++;
        }
    }

    RequestObject* requestObject = new RequestObject(numberOfConstructions, fleet, concreteTypes, cost, capacity, clientDemands, distances);

    data  = new DataConcreteMixerTruckRouting(requestObject);
    model = new ModelConcreteMixerTruckRouting();

    data->readObjectData();
    data->print();

    model->execute(data);
    model->printSolution();

    ResponseObject* response    = new ResponseObject();
    response->modelResponse     = model->reportSolutionVariables();
    response->solutionResponse  = model->reportSolution();
     
    totalTime = Util::getTime() - startTime;

    return *response;
}

void finaliseProcess() {
    Options::finalise();
}

void ExecuteProcess(char* response, char* request) {

    string responseString = "";

    try {
        Execute execute;
        ResponseObject responseObject = execute.execute(request);

        responseString += "{";
        responseString += "obj:";
        responseString += std::to_string(responseObject.solutionResponse.obj);
        responseString += ",bestBound:";
        responseString += std::to_string(responseObject.solutionResponse.bestBound);
        responseString += ",gap:";
        responseString += std::to_string(responseObject.solutionResponse.gap);
        responseString += ",solutionExists:";
        responseString += std::to_string(responseObject.solutionResponse.solutionExists);
        responseString += ",solutionIsOptimal:";
        responseString += std::to_string(responseObject.solutionResponse.solutionIsOptimal);
        responseString += ",solutionIsInfeasible:";
        responseString += std::to_string(responseObject.solutionResponse.solutionIsInfeasible);
        responseString += ",solutionsIsUnbounded:";
        responseString += std::to_string(responseObject.solutionResponse.solutionsIsUnbounded);


        responseString += ",sol_y: [";
        for (int i = 0; i < responseObject.modelResponse.sol_y.size(); i++) {
            responseString += std::to_string(responseObject.modelResponse.sol_y[i]);

            if (i != responseObject.modelResponse.sol_y.size() - 1)
                responseString += ",";
        }
        responseString += "]";

        responseString += ",sol_z: [";
        for (int i = 0; i < responseObject.modelResponse.sol_z.size(); i++) {
            responseString += "[";
            for (int j = 0; j < responseObject.modelResponse.sol_z[i].size(); j++) {
                responseString += std::to_string(responseObject.modelResponse.sol_z[i][j]);

                if (j != responseObject.modelResponse.sol_z[i].size() - 1)
                    responseString += ",";
            }
            responseString += "]";

            if (i != responseObject.modelResponse.sol_z.size() - 1)
                responseString += ",";
        }
        responseString += "]";

        responseString += ",sol_x: [";
        for (int i = 0; i < responseObject.modelResponse.sol_x.size(); i++) {
            responseString += "[";
            for (int j = 0; j < responseObject.modelResponse.sol_x[i].size(); j++) {
                responseString += "[";
                for (int k = 0; k < responseObject.modelResponse.sol_x[i][j].size(); k++) {
                    responseString += std::to_string(responseObject.modelResponse.sol_x[i][j][k]);

                    if (k != responseObject.modelResponse.sol_x[i][j].size() - 1)
                        responseString += ",";
                }
                responseString += "]";

                if (j != responseObject.modelResponse.sol_x[i].size() - 1)
                    responseString += ",";
            }
            responseString += "]";

            if (i != responseObject.modelResponse.sol_x.size() - 1)
                responseString += ",";
        }
        responseString += "]";
        responseString += "}";

    }
    catch (std::invalid_argument& e) {
        Util::throwInvalidArgument(e.what());
    }

    strcpy(response, responseString.c_str());
}
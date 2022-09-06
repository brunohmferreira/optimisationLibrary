
#pragma once

#ifdef OPTIMISATIONLIBRARY_EXPORTS
#define OPTIMISATIONLIBRARY_API __declspec(dllexport)
#else
#define OPTIMISATIONLIBRARY_API __declspec(dllimport)
#endif

extern "C" OPTIMISATIONLIBRARY_API void ExecuteProcess(char* response, char* request);

#ifndef Execute_H
#define Execute_H

#include "Data.h"
#include "Model.h"

class ResponseObject {

public:

    ResponseObject() {}
    ~ResponseObject() {}

    SoltuionResponseObject solutionResponse;
    ModelResponseObject modelResponse;
};

class Execute {

    private:

        Data* data;

        Model* model;

        double totalTime;

    public:
   
        Execute();
        ~Execute();

        void execute();
        ResponseObject execute(char* requestString);
};

#endif 

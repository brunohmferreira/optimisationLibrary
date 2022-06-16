

#include "DataVehicleRouting.h"
#include "Options.h"

DataVehicleRouting::DataVehicleRouting() : Data(){
    numberOfClients = 0;
    vehicleFleet = 0;
}

DataVehicleRouting::~DataVehicleRouting() {
}

void DataVehicleRouting::readData() {
    if (debug) printf("Initializing data for Vehicle Routing \n\n");

    FILE* file;
    string inputFilename = Options::getInstance()->getStringOption("input");
    string fourLastCharacters = inputFilename.substr(inputFilename.length() - 4);
    string availableSuffix = ".txt";

    // empty filename
    if (inputFilename.c_str() == NULL || inputFilename.find_first_not_of(' ') == std::string::npos) Util::throwInvalidArgument("Error: Input file '%s' has an  invalid name.", inputFilename.c_str());

    // input format
    if (std::count(fourLastCharacters.begin(), fourLastCharacters.end(), '.') == 0) inputFilename.append(availableSuffix);
    if (inputFilename.compare(inputFilename.size() - availableSuffix.size(), availableSuffix.size(), availableSuffix) != 0) Util::throwInvalidArgument("Error: Input file '%s' is in invalid format.", inputFilename.c_str());
 
    // opening file
    if (!Util::openFile(&file, inputFilename.c_str(), "r")) Util::throwInvalidArgument("Error: Input file '%s' was not found or could not be opened.", inputFilename.c_str());

    try {
        char buffer[50];
        double quantity;
        // number of clients
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%d", &numberOfClients             ) != 1) throw std::invalid_argument("");

        // vehicle fleet
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%d", &vehicleFleet                ) != 1) throw std::invalid_argument("");

        // fixed cost of using the vehicle
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%lf", &fixedCost                  ) != 1) throw std::invalid_argument("");

        // vehicle capacity
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%s", buffer                       ) != 1) throw std::invalid_argument("");
        if (fscanf(file, "%lf", &vehicleCapacity            ) != 1) throw std::invalid_argument("");

        demands   .resize(numberOfClients + 1                                     );
        distances .resize(numberOfClients + 1, vector<double>(numberOfClients + 1));

        // demands
        if (fscanf(file, "%s", buffer                           ) != 1) throw std::invalid_argument("");
        for (int i = 0; i <= numberOfClients; i++) {
            if (fscanf(file, "%d", &demands[i].clientId         ) != 1) throw std::invalid_argument("");
            if (fscanf(file, "%lf", &quantity                   ) != 1) throw std::invalid_argument("");

            demands[demands[i].clientId].setQuantity(quantity);
        }

        // distances
        if (fscanf(file, "%s", buffer                    ) != 1) throw std::invalid_argument("");
        for (int i = 0; i <= numberOfClients; i++) {
            for (int j = 0; j <= numberOfClients; j++) {
                if (fscanf(file, "%lf", &distances[i][j] ) != 1) throw std::invalid_argument("");
            }
        }

    } catch ( const std::invalid_argument& e) {
        if (!Util::closeFile(&file)) Util::throwInvalidArgument("Error: Instances file %s could not be closed.", inputFilename.c_str());
        Util::throwInvalidArgument("Error: Instances file '%s' is invalid.", inputFilename.c_str());
    }

    // close file
    if (!Util::closeFile(&file)) Util::throwInvalidArgument("Error: Instances file %s could not be closed.", inputFilename.c_str());
}

void DataVehicleRouting::print() {
    if (debug) {
        printf("number of clients\n");
        printf("%d\n\n", numberOfClients);
        printf("vehicle fleet\n");
        printf("%d\n\n", vehicleFleet);
        printf("fixed cost of using the vehicle\n");
        printf("R$%.2f\n\n", fixedCost);
        printf("vehicle capacity\n");
        printf("%.2f\n\n", vehicleCapacity);
        printf("demands\n");
        for (int i = 0; i <= numberOfClients; i++) {
            printf("demandId: %d\t quantity: %.2f\n", demands[i].clientId, demands[i].getQuantity(demands[i].clientId));
        }
        printf("\ndistances\n");
        for (int i = 0; i <= numberOfClients; i++) {
            for (int j = 0; j <= numberOfClients; j++) {
                printf("%.2f\t", distances[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

}

ClientDemand DataVehicleRouting::getDemand(int i) const {
    if (i >= numberOfClients + 1) Util::throwInvalidArgument("Error: Out of range parameter i in getDemand");
    return demands[i];
}

double DataVehicleRouting::getDistance(int i, int j) const {
    if (i >= numberOfClients + 1) Util::throwInvalidArgument("Error: Out of range parameter i in getDistance");
    if (j >= numberOfClients + 1) Util::throwInvalidArgument("Error: Out of range parameter j in getDistance");
    return distances[i][j];
}

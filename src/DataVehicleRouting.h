
#ifndef DATAVEHICLEROUTING_H
#define DATAVEHICLEROUTING_H

#include "Util.h"
#include "Data.h"

// Client demand
class ClientDemand {

    private:
        double  quantity;

    public:
        int clientId;

        ClientDemand() {}

        ~ClientDemand() {}

        double getQuantity      (int client) const { return quantity; }

        void setQuantity(double value) {
            quantity = value;
        }
};

class DataVehicleRouting : public Data {

    private:
        int numberOfClients;
        int vehicleFleet;
        double fixedCost;
        double vehicleCapacity;
        vector<ClientDemand> demands;
        vector<vector<double>> distances;

    public:

        DataVehicleRouting();
        virtual ~DataVehicleRouting();

        virtual void readData();
        virtual void print();

        int          getNumberOfClients     (            ) const {return numberOfClients  ;}
        int          getVehicleFleet        (            ) const {return vehicleFleet     ;}
        double       getFixedCost           (            ) const {return fixedCost        ;}
        double       getVehicleCapacity     (            ) const {return vehicleCapacity  ;}
        ClientDemand getDemand              (int i       ) const ;
        double       getDistance            (int i, int j) const ;
};

#endif

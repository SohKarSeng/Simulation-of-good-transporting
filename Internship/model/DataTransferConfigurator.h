#ifndef DATATRANSFERCONFIGURATOR_H
#define DATATRANSFERCONFIGURATOR_H

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

namespace ns3 {

    class DataTransferConfigurator {
    public:
        DataTransferConfigurator(Ipv4Address destinationAddress);

        void ConfigureApplications(Ptr<Node> nodeId, const std::string& dataToSend);

    private:
        Ipv4Address destAddress;
    };

}

#endif // DATA_TRANSFER_CONFIGURATOR_H
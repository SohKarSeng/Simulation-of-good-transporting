#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"

#include "DataTransferConfigurator.h"
#include "DataTransfer.h"

NS_LOG_COMPONENT_DEFINE("DataTransferConfigurator");

namespace ns3 {

    DataTransferConfigurator::DataTransferConfigurator(Ipv4Address destinationAddress)
        : destAddress(destinationAddress) {}

    void DataTransferConfigurator::ConfigureApplications(Ptr<Node> nodeId, const std::string& dataToSend) {
        Ptr<DataTransfer> app = CreateObject<DataTransfer>();
            
        NS_LOG_INFO("Data from Node " << nodeId->GetId());
        app->SetDataToSend(dataToSend);
        app->SetDestinationIpv4Address(destAddress);
        app->SetNode(nodeId);

        app->StartApp();
    }

}
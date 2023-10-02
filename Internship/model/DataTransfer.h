#ifndef DATATRANSFER_H 
#define DATATRANSFER_H 


#include "ns3/application.h"
#include "ns3/object.h"
#include "ns3/ipv4-address.h"
#include "ns3/simulator.h"
#include "ns3/node.h"
#include "ns3/socket.h"

namespace ns3
{

  class DataTransfer : public Application {
  public:
    static TypeId GetTypeId();
    
    DataTransfer();

    void StartApp();
    void SetDataToSend(const std::string& data);
    void SetDestinationIpv4Address(Ipv4Address address);
    void SetNode(Ptr<Node> node);
    void SendData();

  private:
    void StartApplication();
    void StopApplication();
    virtual void DoDispose();
    void ReceiveData(Ptr<Socket> socket);

    Ptr<Node> m_node;
    Ptr<Socket> m_socket;
    std::string m_dataToSend;
    Ipv4Address m_destinationIpv4Address;
    uint16_t m_destinationPort = 9; // You can change the port number if needed
  };

}

#endif /* DATATRANSFER_H  */
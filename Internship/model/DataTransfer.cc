#include "DataTransfer.h"
#include "ns3/packet.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/ipv4.h"



NS_LOG_COMPONENT_DEFINE("DataTransfer");


namespace ns3 {


  TypeId DataTransfer::GetTypeId() {
    static TypeId tid = TypeId("DataTransfer")
                            .SetParent<Application>(); // Inherit from Application instead of Object
    return tid;
  }

  DataTransfer::DataTransfer() {
  }

  void DataTransfer::SetDataToSend(const std::string& data) {
    m_dataToSend = data;
  }

  void DataTransfer::SetDestinationIpv4Address(Ipv4Address address) {
    m_destinationIpv4Address = address;
  }

  void DataTransfer::SetNode(Ptr<Node> node) {
    m_node = node;
    m_socket = Socket::CreateSocket(m_node, UdpSocketFactory::GetTypeId()); // Create the socket
    Ipv4Address localAddress = m_node->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();
    m_socket->Bind(InetSocketAddress(localAddress, 0));
    m_socket->SetRecvCallback(MakeCallback(&DataTransfer::ReceiveData, this));
  }

  void DataTransfer::StartApp() {
    StartApplication();
  }


  void DataTransfer::StartApplication() {
    // Schedule the first packet transmission
    SendData();
    StopApplication();
  }

  void DataTransfer::StopApplication() {
    m_socket->Close();
  }

  void DataTransfer::SendData() {
    // Create a packet with the desired content
    Ptr<Packet> packet = Create<Packet>((uint8_t*)m_dataToSend.c_str(), m_dataToSend.size());

    // Send the packet to the destination address
    InetSocketAddress remoteAddress = InetSocketAddress(m_destinationIpv4Address, m_destinationPort);
    m_socket->SendTo(packet, 0, remoteAddress);

    // Log message to indicate that a packet has been sent
    NS_LOG_INFO("Sent packet with data: " << m_dataToSend);
  }

  void DataTransfer::DoDispose() {
    m_socket = 0;
    Application::DoDispose();
  }

  void DataTransfer::ReceiveData(Ptr<Socket> socket) {
    Ptr<Packet> packet;
    Address from;
    while ((packet = socket->RecvFrom(from))) {
      // Process the received data here
      uint8_t* buffer = new uint8_t[packet->GetSize()];
      packet->CopyData(buffer, packet->GetSize());
      std::string receivedData((char*)buffer, packet->GetSize());
      delete[] buffer;

      // Print the received data to the console
      NS_LOG_INFO("Received data: " << receivedData);
    }
  }

} // namespace ns3


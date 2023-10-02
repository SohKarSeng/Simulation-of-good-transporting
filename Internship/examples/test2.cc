
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/point-to-point-module.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/math.h"

#include "ns3/DataTransfer.h"
#include "ns3/DataTransferConfigurator.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("MySimulation");

void CheckpointDetectionCallback(Ptr<Node> wifiNode, Ptr<Node> tempNode, Ptr<Node> oilNode, Ptr<Node> longNode, Ptr<Node> latiNode, Ptr<Node> timeStamp) {
  Ptr<MobilityModel> mobility = wifiNode->GetObject<MobilityModel>();
  Vector position = mobility->GetPosition();
   
  double x = position.x;
  double y = position.y;
  double z = position.z;

  NS_LOG_INFO("------Node " << wifiNode->GetId() << "------");
  NS_LOG_INFO("Current position of Node " << wifiNode->GetId() << ": x=" << x << ", y=" << y << ", z=" << z);
  NS_LOG_INFO("------------------");

  // Set up location of checkpoint
  Vector checkpoint1Position(3.0, 3.0, 0.0);
  Vector checkpoint2Position(7.0, 7.0, 0.0);
  double proximityThreshold = 0.5; // Distance threshold for checkpoint proximity


  double distance1 = CalculateDistance(position, checkpoint1Position);
  double distance2 = CalculateDistance(position, checkpoint2Position);

  Ipv4Address destinationAddress("10.0.0.2");

  if (distance1 <= proximityThreshold) {
    NS_LOG_INFO("------Casual Transmit------");
    NS_LOG_INFO("First Checkpoint detected for Node " << wifiNode->GetId());

    DataTransferConfigurator tempConfigurator(destinationAddress);
    tempConfigurator.ConfigureApplications(tempNode, "[Insert Temp Data]");

    DataTransferConfigurator oilConfigurator(destinationAddress);
    oilConfigurator.ConfigureApplications(oilNode, "[Insert Oil Data]");

    DataTransferConfigurator longConfigurator(destinationAddress);
    tempConfigurator.ConfigureApplications(longNode, "[Insert Longitude Data]");

    DataTransferConfigurator latiConfigurator(destinationAddress);
    oilConfigurator.ConfigureApplications(latiNode, "[Insert Latitude Data]");
    
    DataTransferConfigurator timeConfigurator(destinationAddress);
    oilConfigurator.ConfigureApplications(timeStamp, "[Insert Time Stamp]");

    NS_LOG_INFO("---------------------------");
  }
  else if (distance2 <= proximityThreshold) {
    NS_LOG_INFO("------Casual Transmit------");
    NS_LOG_INFO("Second Checkpoint detected for Node " << wifiNode->GetId());

    DataTransferConfigurator tempConfigurator(destinationAddress);
    tempConfigurator.ConfigureApplications(tempNode, "[Insert Temp Data]");

    DataTransferConfigurator oilConfigurator(destinationAddress);
    oilConfigurator.ConfigureApplications(oilNode, "[Insert Oil Data]");

    DataTransferConfigurator longConfigurator(destinationAddress);
    tempConfigurator.ConfigureApplications(longNode, "[Insert Longitude Data]");

    DataTransferConfigurator latiConfigurator(destinationAddress);
    oilConfigurator.ConfigureApplications(latiNode, "[Insert Latitude Data]");
    
    DataTransferConfigurator timeConfigurator(destinationAddress);
    timeConfigurator.ConfigureApplications(timeStamp, "[Insert Time Stamp]");

    NS_LOG_INFO("---------------------------");
  }

  double interval = 1.0;


  // Looping the function
  Simulator::Schedule(Seconds(interval), &CheckpointDetectionCallback, wifiNode, tempNode, oilNode, longNode, latiNode, timeStamp);
}




void ChangeSpeedCallback(Ptr<Node> node, Vector newVelocity) {
  // Get the ConstantVelocityMobilityModel of the node
  Ptr<ConstantVelocityMobilityModel> mobility = node->GetObject<ConstantVelocityMobilityModel>();

  // Set the new velocity
  mobility->SetVelocity(newVelocity);
}



void CheckNodeStop(Ptr<Node> node) {

  // Get the model of the node
  Ptr<ConstantVelocityMobilityModel> mobility = node->GetObject<ConstantVelocityMobilityModel>();

  // Get velocity
  Vector velocity = mobility->GetVelocity();
  Vector position = mobility->GetPosition();

  // Set up location of checkpoint
  Vector checkpoint1Position(3.0, 3.0, 0.0);
  Vector checkpoint2Position(7.0, 7.0, 0.0);
  double proximityThreshold = 0.5; // Distance threshold for checkpoint proximity

  double distance1 = CalculateDistance(position, checkpoint1Position);
  double distance2 = CalculateDistance(position, checkpoint2Position);

  // Check if the node has stopped (velocity close to zero)
  static map<uint32_t, double> stopTimes; // Store stop times for each node
  static map<uint32_t, double> totalStoppedDuration;
  double currentTime = Simulator::Now().GetSeconds();

  if (velocity.x == 0.0 && velocity.y == 0.0 && velocity.z == 0.0) {
    if (stopTimes.find(node->GetId()) == stopTimes.end()) {
      // Record the time when the node first stopped
      stopTimes[node->GetId()] = currentTime;
    } else {
      // Check if the node has been stopped for more than the desired duration
      double stoppedDuration = currentTime - stopTimes[node->GetId()];
      if (stoppedDuration >= 3.0) {
        totalStoppedDuration[node->GetId()] += 1;
        // Perform actions when the node has stopped for the desired duration
      }
    }
  } else {
    // The node is moving, so reset the stop time
    stopTimes.erase(node->GetId());
  }

  if (distance1 <= proximityThreshold) {
    Ipv4Address destinationAddress("10.0.0.2");

    DataTransferConfigurator pausedTimeConfigurator(destinationAddress);
    pausedTimeConfigurator.ConfigureApplications(node, to_string(totalStoppedDuration[node->GetId()]));
  }
  else if (distance2 <= proximityThreshold) {
    Ipv4Address destinationAddress("10.0.0.2");

    DataTransferConfigurator pausedTimeConfigurator(destinationAddress);
    pausedTimeConfigurator.ConfigureApplications(node, to_string(totalStoppedDuration[node->GetId()]));
  }

  // Schedule the next check
  double checkInterval = 1.0; // Time interval between checks
  Simulator::Schedule(Seconds(checkInterval), &CheckNodeStop, node);
}




void GetCurrentPosition(Ptr<Node> node) {
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel>();
  Vector position = mobility->GetPosition();

  double x = position.x;
  double y = position.y;
  double z = position.z;

  NS_LOG_INFO("------Node " << node->GetId() << "------");
  NS_LOG_INFO("Current position of Node " << node->GetId() << ": x=" << x << ", y=" << y << ", z=" << z);
  NS_LOG_INFO("------------------");

  // Schedule the next position update
  double positionInterval = 2.0;

  Simulator::Schedule(Seconds(positionInterval), &GetCurrentPosition, node);
}





int main(int argc, char* argv[]) {

  CommandLine cmd(__FILE__);
  cmd.Parse(argc, argv);

  uint32_t nWifi = 6;

  // Code for enabling the log for each componment defined
  Time::SetResolution(Time::NS);
  LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO); 
  LogComponentEnable("DataTransferConfigurator", LOG_LEVEL_INFO);
  LogComponentEnable("MySimulation", LOG_LEVEL_INFO);
  LogComponentEnable("DataTransfer", LOG_LEVEL_INFO);


  // Creating node for Server, node 0
  NodeContainer nodes;
  nodes.Create(1);

  // Node X Wireless setting - this cover node 1, 2, 3, 4, 5 and 6.
  NodeContainer wifiNodes1;
  wifiNodes1.Create(nWifi);

  // Node Y Wireless setting - this cover node 7, 8, 9, 10, 11 and 12.
  NodeContainer wifiNodes2;
  wifiNodes2.Create(nWifi);

  // Setting up YansWifiChannel for Node X
  YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default();
  YansWifiPhyHelper phy1;
  phy1.SetChannel(channel1.Create());

  // Setting up YansWifiChannel for Node Y
  YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default();
  YansWifiPhyHelper phy2;
  phy2.SetChannel(channel2.Create());

  WifiMacHelper mac;
  Ssid ssid = Ssid("ns-3-ssid");

  WifiHelper wifi;

  // Setting up Device for both node X and Y
  NetDeviceContainer wifiNode1Device;
  mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
  wifiNode1Device = wifi.Install(phy1, mac, wifiNodes1);

  NetDeviceContainer wifiNode2Device;
  mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
  wifiNode2Device = wifi.Install(phy2, mac, wifiNodes2);

  // Install network stack on IoT devices
  InternetStackHelper stack;
  stack.Install(nodes);
  stack.Install(wifiNodes1);
  stack.Install(wifiNodes2);

  /* Create a point-to-point link between node 0 and node 2
  PointToPointHelper p2p2;
  p2p2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
  p2p2.SetChannelAttribute("Delay", StringValue("2ms"));
  NetDeviceContainer devices2 = p2p2.Install(nodes.Get(0), nodes.Get(1));*/


  // Assign IP addresses to IoT devices
  Ipv4AddressHelper address;
  address.SetBase("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces1 = address.Assign(wifiNode1Device);

  //address.SetBase("10.1.0.0", "255.255.255.0");
  //Ipv4InterfaceContainer interfaces2 = address.Assign(wifiNode2Device);

  address.SetBase("10.2.0.0", "255.255.255.0");
  Ipv4InterfaceContainer interface3 = address.Assign(wifiNode2Device);


  // 1st node
  // Set mobility for first IoT devices
  MobilityHelper mobility1;
  mobility1.SetPositionAllocator("ns3::GridPositionAllocator",
                                "MinX", DoubleValue(0.0),
                                "MinY", DoubleValue(0.0),
                                "DeltaX", DoubleValue(10.0),
                                "DeltaY", DoubleValue(10.0),
                                "GridWidth", UintegerValue(2),
                                "LayoutType", StringValue("RowFirst"));
  
  //ConstantPOsitionMobilityModel - to fix server location
  mobility1.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility1.Install(nodes.Get(0));



  // 2nd node
  // Create a mobility model with ConstantVelocityMobilityModel
  Ptr<ConstantVelocityMobilityModel> mobility2 = CreateObject<ConstantVelocityMobilityModel>();

  // Set initial position and velocity
  Vector initialPosition(0.0, 0.0, 0.0);
  Vector velocity(1.0, 1.0, 0.0);
  mobility2->SetPosition(initialPosition);
  mobility2->SetVelocity(velocity);

  wifiNodes1.Get(0)->AggregateObject(mobility2);



  // 3rd node
  // Same as node 2
  Ptr<ConstantVelocityMobilityModel> mobility3 = CreateObject<ConstantVelocityMobilityModel>();
  wifiNodes2.Get(0)->AggregateObject(mobility3);

  // Set initial position and velocity
  mobility3->SetPosition(initialPosition);
  mobility3->SetVelocity(velocity);


  // Create a UDP server on one IoT device
  UdpEchoServerHelper echoServer(9);
  ApplicationContainer serverApp = echoServer.Install(nodes.Get(0));
  serverApp.Start(Seconds(1.0));
  serverApp.Stop(Seconds(10.0));


  /* Create a UDP client on the other IoT device
  UdpEchoClientHelper echoClient1(interfaces1.GetAddress(0), 9);
  echoClient1.SetAttribute("MaxPackets", UintegerValue(2));
  echoClient1.SetAttribute("Interval", TimeValue(Seconds(2.0)));
  echoClient1.SetAttribute("PacketSize", UintegerValue(1024));

  // Create a UDP client on the other IoT device
  UdpEchoClientHelper echoClient2(interfaces2.GetAddress(0), 9);
  echoClient2.SetAttribute("MaxPackets", UintegerValue(2));
  echoClient2.SetAttribute("Interval", TimeValue(Seconds(2.0)));
  echoClient2.SetAttribute("PacketSize", UintegerValue(1024));

  ApplicationContainer clientApp1 = echoClient1.Install(nodes.Get(1));
  clientApp1.Start(Seconds(1.0));
  clientApp1.Stop(Seconds(10.0));

  ApplicationContainer clientApp2 = echoClient2.Install(nodes.Get(2));
  clientApp2.Start(Seconds(1.0));
  clientApp2.Stop(Seconds(10.0));*/


  // Enable routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();




  // Speed changer - 2nd node
  Vector newVelocity1(0.0, 0.0, 0.0);
  double timeOnSpeedChange1 = 5.0;
  double timeOnSpeedChange2 = 10.0;

  // Speed changing - 2nd node
  Simulator::Schedule(Seconds(timeOnSpeedChange1), &ChangeSpeedCallback, wifiNodes1.Get(0), newVelocity1);
  Simulator::Schedule(Seconds(timeOnSpeedChange2), &ChangeSpeedCallback, wifiNodes1.Get(0), velocity);





  // Set up checkpoint detection - 2nd node
  double checkpointInterval = 1.0;
  Simulator::Schedule(Seconds(checkpointInterval), &CheckpointDetectionCallback, wifiNodes1.Get(0), wifiNodes1.Get(1), wifiNodes1.Get(2), wifiNodes1.Get(3), wifiNodes1.Get(4), wifiNodes1.Get(5));

  // Set up checkpoint detection - 3rd node
  Simulator::Schedule(Seconds(checkpointInterval), &CheckpointDetectionCallback, wifiNodes2.Get(0), wifiNodes2.Get(1), wifiNodes2.Get(2), wifiNodes2.Get(3), wifiNodes2.Get(4), wifiNodes2.Get(5));



  // Run the GetCurrentPosition function periodically - 2nd node
  //double positionInterval = 2.0;
  //Simulator::Schedule(Seconds(positionInterval), &GetCurrentPosition, wifiNodes1.Get(0));

  // Run the GetCurrentPosition function periodically - 3rd node
  //Simulator::Schedule(Seconds(positionInterval), &GetCurrentPosition, nodes.Get(2));

  // Check node stops
  CheckNodeStop(wifiNodes1.Get(0));
  CheckNodeStop(wifiNodes2.Get(0));



  // Run the simulation
  NS_LOG_INFO("Starting the simulation...");

  Simulator::Stop(Seconds(15.0));
  Simulator::Run();
  Simulator::Destroy();

  NS_LOG_INFO("Simulation completed.");
  return 0;
}
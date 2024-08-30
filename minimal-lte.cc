
#include <utility>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/pf-ff-mac-scheduler.h"

using namespace ns3;

typedef std::pair<int, int> Pos;
typedef std::vector<Pos> Coords ;
typedef std::pair<ApplicationContainer, ApplicationContainer> ClientServerPair;


ApplicationContainer udpFullBufferReceiver (Ptr<Node> node,
                                            uint16_t port)
{
    UdpServerHelper ulPacketSinkHelper(port);
    return ulPacketSinkHelper.Install(node);
}

ApplicationContainer udpFullBufferSender(Ptr<Node> sender,
                                         Ipv4Address receiverIP,
                                         uint16_t port,
                                         int maxPackets = 100000000,
                                         int packetSize = 2048)
{

    UdpClientHelper ulClient(receiverIP, port);

    ulClient.SetAttribute("MaxPackets", UintegerValue(maxPackets));
    ulClient.SetAttribute("Interval", TimeValue(MilliSeconds(1)));
    ulClient.SetAttribute("PacketSize", UintegerValue(packetSize));

    return ulClient.Install(sender);


}


ClientServerPair
setupUdpFullBufferFor2Users(NodeContainer ueNodes,
                            NodeContainer enbNodes,
                            Ptr<Node> pgw,
                            Ipv4InterfaceContainer ueIpIface,
                            int maxPackets,
                            int packetSize)
{

    // Up Link Port
    uint16_t ulPort = 2000;
    // Down Link Port
    uint16_t dlPort = 1000;

    ApplicationContainer clientApps;
    ApplicationContainer serverApps;


    int bobN = 0;
    Ptr<Node> bob = ueNodes.Get(bobN);
    Ipv4Address bobIP =  ueIpIface.GetAddress(bobN);

    int aliceN = 1;
    Ptr<Node> alice = ueNodes.Get(aliceN);
    Ipv4Address aliceIP =  ueIpIface.GetAddress(aliceN);


    // Alice Sending
    // UE Client Sending Uplink Traffic from Alice
    // UE Alice sending packets to PGW listening on port UL
    clientApps.Add(udpFullBufferSender(alice, bobIP, ulPort, maxPackets, packetSize));

    // Bob Sending
    clientApps.Add(udpFullBufferSender(bob, aliceIP, ulPort, maxPackets, packetSize));

    // Server Receiving
    // PGW Server Receiving Uplink Traffic from Alice
    // PGW listening on port UL, receiving packets from UE Alice
    serverApps.Add(udpFullBufferReceiver(pgw, ulPort));


    // Server Sending
    // PGW Client Sending Downlink Traffic from PGW to Bob
    // PGW is sending packets to UE Bob listening on port DL
    clientApps.Add(udpFullBufferSender(pgw, bobIP, dlPort, maxPackets, packetSize));
    // To Alice
    clientApps.Add(udpFullBufferSender(pgw, aliceIP, dlPort, maxPackets, packetSize));

    // Bob Receiving
    // UE Server Receiving Downlink Traffic from PGW
    // UE Bob receiving packets from PGW listening on port DL
    serverApps.Add(udpFullBufferReceiver(bob, dlPort));
    //Alice Receiving
    serverApps.Add(udpFullBufferReceiver(alice, dlPort));

    return std::make_pair(serverApps, clientApps);
}


// Setup Internet Stack for LTE devices and asigning IP addresses
// IPs to be returned
Ipv4InterfaceContainer setupInternetStack(Ptr<LteHelper> lteHelper,
                                          Ptr<PointToPointEpcHelper> epcHelper,
                                          NodeContainer enbNodes,
                                          NodeContainer ueNodes)
{

    InternetStackHelper internet;
    internet.Install(ueNodes);

    // Setting up LTE Devices on all nodes
    NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice(enbNodes);
    NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice(ueNodes);


    // Setting up users IPs
    Ipv4InterfaceContainer ueIpIface;
    ueIpIface = epcHelper->AssignUeIpv4Address(NetDeviceContainer(ueLteDevs));

    // Attaching users to eNB
    lteHelper->Attach(ueLteDevs, enbLteDevs.Get(0));

    return ueIpIface;


}

// Attaching coords to nodes
void setCoords(NodeContainer nodes,
               Coords * coords)
{

    uint32_t nodesCount = nodes.GetN();
    uint32_t coordsCount = coords -> size();

    if (nodesCount != coordsCount)
        throw std::runtime_error("Nodes count not equal to coordinates count");

    uint32_t i;
    for(i=0; i < coordsCount; i++){
        Pos pos = coords -> at(i);
        std::cout << "Coords: " << pos.first << " " << pos.second << std::endl;
        Ptr<Node> node = nodes.Get(i);
        node -> GetObject<MobilityModel>()->SetPosition(Vector(pos.first, pos.second, 0.0));
    }
}

void setUpMobility(MobilityHelper * mobility,
                   NodeContainer ueNodes,
                   Coords * ueCoords ,
                   NodeContainer enbNodes,
                   Coords * enbCoords)
{


    // Positions of all nodes are constant
    mobility -> SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility -> Install(ueNodes);
    mobility -> Install(enbNodes);

    setCoords(ueNodes, ueCoords);
    setCoords(enbNodes, enbCoords);


}

int main(int argc, char *argv[])
{

    double simulationTime = 10.0; // 10s
    int maxPackets = 0; // means infinity
    int packetSize  = 1024; // 1 KB

    CommandLine cmd;
    cmd.AddValue ("simulationTime", "Total duration of the simulation in seconds", simulationTime);
    cmd.AddValue ("maxPackets", "maximum number of packets to be sent", maxPackets);
    cmd.AddValue ("packetSize", "one packet size in bytes", packetSize);
    cmd.Parse (argc, argv);


    Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

    // Setting up EPC
    Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper>();
    lteHelper->SetEpcHelper(epcHelper);

    // setting up Scheduler
    lteHelper->SetSchedulerType("ns3::PfFfMacScheduler");

    //E-UTRAN
    NodeContainer enbNodes;
    enbNodes.Create(1);

    //Packet Data-Network Gatewey
    Ptr<Node> pgw = epcHelper->GetPgwNode();

    //User Equipment
    NodeContainer ueNodes;
    int ueNodesCount = 2;
    ueNodes.Create(ueNodesCount);



    // Coordinates of Nodes
    //
    Coords enbCoords = {
        { 0.0, 0.0 },
    };

    Coords ueCoords = {
        { 100.0, 0.0 },
        { 0.0, 100.0 }
    };

    // Set up nodes location
    MobilityHelper mobility;
    setUpMobility(&mobility, ueNodes, &ueCoords, enbNodes, &enbCoords);


    // Set up Internet Stack
    Ipv4InterfaceContainer ueIpIface = setupInternetStack(lteHelper, epcHelper, enbNodes, ueNodes);

    // Set up UDP Client-Server and FullBuffer Traffic
    ClientServerPair applications = setupUdpFullBufferFor2Users(ueNodes, enbNodes, pgw, ueIpIface, maxPackets, packetSize);



    // Starting Applications
    applications.first.Start(Seconds(0.1));
    applications.second.Start(Seconds(0.1));


    // Enabling Traces
    lteHelper->EnableRlcTraces();
    lteHelper->EnableMacTraces();


    // Simulation Time
    Simulator::Stop(Seconds(simulationTime));

    // Running Simulation
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}

#include "ns3/NetworkController.h"

using namespace ns3;

NetworkController::NetworkController(Constellation* newConstellation) {
    constellation = newConstellation;
    time = 0;
}

NetworkController::NetworkController(Constellation* newConstellation, int startingTime) {
    constellation = newConstellation;
    time = startingTime;
}

NetworkController::NetworkController(int startingTime) {
    constellation = new Constellation(100);
    time = startingTime;
}

NetworkController::NetworkController() {
    constellation = new Constellation(100);
    time = 0;
}

NetworkController::~NetworkController() {
    delete constellation;
}

std::vector<std::vector<int>> NetworkController::nodesCommonChannel(NanoSatellite* satellite) {
    std::vector<std::vector<int>> nodes;
    return nodes;
}

void NetworkController::setDefaultParameters() {
    Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));
    Config::SetDefault ("ns3::RangePropagationLossModel::MaxRange",DoubleValue (20000));
    Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("60Mb/s"));
    Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (1000));
    Time::SetResolution(Time::NS);
    nodeContainer.Create(constellation->constellationSize);
    MobilityHelper nodesCoordinates;
    nodesCoordinates.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    nodesCoordinates.Install(nodeContainer);
    InternetStackHelper protocolStack;
    protocolStack.Install(nodeContainer);
    ipv4.SetBase("110.110.110.0","255.255.255.0");
}

void NetworkController::initConstellation() {
     constellation->initSatellites();
     constellation->setRandomGroups();
     constellation->createLinks();
     constellation->updateCoordinates(time);
     constellation->setLinks(time);
}

void NetworkController::setNetworkNodesPos() {
    for(NanoSatellite* s : constellation->satellites) {
        std::vector<float> coordsVector = s->getTimeCoordinates()->getCoordinates();
        Vector satellitePos(coordsVector[0]/1000000,coordsVector[1]/1000000,coordsVector[2]/10000000);
        nodeContainer.Get(s->satelliteID)->GetObject<MobilityModel>()->SetPosition(satellitePos);
        //std::cout << nodeContainer.Get(s->satelliteID)->GetObject<MobilityModel>()->GetPosition() << "\n";
    }
}


void NetworkController::setNetworkNodesChannels() {
    //CsmaHelper csmaChannel;
    //deviceContainer = csmaChannel.Install(nodeContainer);
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phyLayer = YansWifiPhyHelper::Default();
    channel.AddPropagationLoss("ns3::RangePropagationLossModel");
    phyLayer.SetChannel(channel.Create());
    WifiMacHelper macLayer;
    Ssid ssid = Ssid ("N7_CONSTELLATION");
    macLayer.SetType ("ns3::StaWifiMac",
                      "Ssid", SsidValue (ssid),
                      "ActiveProbing", BooleanValue (false));
    WifiHelper wirelessChannel;
    deviceContainer = wirelessChannel.Install(phyLayer,macLayer,nodeContainer);
    ipAddressContainer = ipv4.Assign(deviceContainer);
    OnOffHelper dataApp("ns3::UdpSocketFactory",InetSocketAddress(ipAddressContainer.GetAddress(14)));
    ApplicationContainer appContainer = dataApp.Install(nodeContainer);
    appContainer.Start(Seconds(5));
    appContainer.Stop(Seconds(7));
    Ipv4GlobalRoutingHelper routingController;
    routingController.PopulateRoutingTables();
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("aodv.routes", std::ios::out);
    routingController.PrintRoutingTableAllAt (Seconds (8), routingStream);

    /*
    for(NanoSatellite* s : constellation -> satellites) {
        PointToPointHelper p2pChannel;
        s = s;
    }
    */
}

/*
void NetworkController::setNetworkNodesChannels() {
    for(NanoSatellite* s : constellation -> satellites) {
        NodeContainer localNodes;
        localNodes.Add(nodeContainer.Get(s->satelliteID));
        for(int k : s->reachableNodes) {
            localNodes.Add(nodeContainer.Get(k));
        }
        CsmaHelper csmaChannel;
        csmaChannel.SetChannelAttribute("Delay",StringValue("0.08ms"));
        csmaChannel.SetChannelAttribute("DataRate",StringValue("60Mb/s"));
        NetDeviceContainer nodeDevice = csmaChannel.Install(localNodes);
        ipAddressContainer.push_back(ipv4.Assign(nodeDevice));
    }
    Ipv4GlobalRoutingHelper routing;
    routing.PopulateRoutingTables ();
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("aodv.routes", std::ios::out);
    routing.PrintRoutingTableAllAt (Seconds (8), routingStream);
}

void NetworkController::dataTransmission() {
    for(NanoSatellite * s : constellation -> satellites) {
        if(!s->reachableNodes.empty()) {
            OnOffHelper transmissionApp("ns3::UdpSocketFactory",InetSocketAddress(nodeContainer.Get(s->satelliteID)->GetObject<Ipv4>()->GetAddress(1,0).GetLocal()));
            for(int k : s -> getGroup()) {
                ApplicationContainer appContainer = transmissionApp.Install(nodeContainer.Get(k));
                appContainer.Start(Seconds(1));
                appContainer.Stop(Seconds(2));
            }
        }
    }
}

void NetworkController::updateNetworkNodes(int timeCounter) {
    std::cout<< timeCounter << "\n";
    constellation->updateCoordinates(timeCounter);
    constellation->updateLinks(timeCounter);
    setNetworkNodesChannels();
}




void NetworkController::dataTransmission() {
    OnOffHelper dataApp("ns3::UdpSocketFactory",InetSocketAddress(ipAddressContainer.GetAddress(14)));
    ApplicationContainer appContainer = dataApp.Install(nodeContainer);
    appContainer.Start(Seconds(5));
    appContainer.Stop(Seconds(7));
}


void NetworkController::setNetworkNodesIP() {
    Ipv4AddressHelper ipv4;
    AodvHelper aodv;
    InternetStackHelper protocolStack;
    protocolStack.SetRoutingHelper(aodv);
    protocolStack.Install(nodeContainer);
    ipv4.SetBase("110.110.110.0","255.255.255.0");
    ipAddressContainer = ipv4.Assign(deviceContainer);
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("aodv.routes", std::ios::out);
    aodv.PrintRoutingTableAllAt (Seconds (11), routingStream);
} 


                                        VERSION AODC, NON TERMINEE



void NetworkController::setDefaultParameters() {
    //Config::SetDefault ("ns3::Ipv4GlobalRouting::RespondToInterfaceEvents", BooleanValue (true));
    Config::SetDefault("ns3::RangePropagationLossModel::MaxRange",DoubleValue(2*10e3));
    //Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("60Mb/s"));
    //Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (1000));
    Time::SetResolution(Time::NS);
    MobilityHelper nodesCoordinates;
    nodeContainer.Create(constellation->constellationSize);
    nodesCoordinates.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    nodesCoordinates.Install(nodeContainer);
}

void NetworkController::initConstellation() {
     constellation->initSatellites();
     constellation->setRandomGroups();
     constellation->createLinks();
     constellation->updateCoordinates(time);
     constellation->setLinks(time);
}


void NetworkController::setNetworkNodesPos() {
    for(NanoSatellite* s : constellation->satellites) {
        std::vector<float> coordsVector = s->getTimeCoordinates()->getCoordinates();
        Vector satellitePos(coordsVector[0],coordsVector[1],coordsVector[2]);
        nodeContainer.Get(s->satelliteID)->GetObject<MobilityModel>()->SetPosition(satellitePos);
        //std::cout << nodeContainer.Get(s->satelliteID)->GetObject<MobilityModel>()->GetPosition() << "\n";
    }
}

void NetworkController::setNetworkNodesChannels() {
    WifiHelper wifiProtocol;
    wifiProtocol.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);
    wifiProtocol.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("HtMcs7"), "ControlMode", StringValue ("HtMcs0"));
    WifiMacHelper macLayer;
    YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default();
    wifiChannel.AddPropagationLoss("ns3::RangePropagationLossModel");
    macLayer.SetType("ns3::AdhocWifiMac");
    Ssid ssid = Ssid ("simple-mpdu-aggregation");
    macLayer.SetType ("ns3::StaWifiMac",
                 "Ssid", SsidValue (ssid),
                 "BE_MaxAmpduSize", UintegerValue (2000));
    YansWifiPhyHelper phyLayer = YansWifiPhyHelper::Default();
    phyLayer.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
    phyLayer.SetChannel (wifiChannel.Create ());
    deviceContainer = wifiProtocol.Install(phyLayer,macLayer,nodeContainer);
}

void NetworkController::setNetworkNodesIP() {
    Ipv4AddressHelper ipv4;
    AodvHelper aodv;
    InternetStackHelper protocolStack;
    protocolStack.SetRoutingHelper(aodv);
    protocolStack.Install(nodeContainer);
    ipv4.SetBase("110.110.110.0","255.255.255.0");
    ipAddressContainer = ipv4.Assign(deviceContainer);
    Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("aodv.routes", std::ios::out);
    aodv.PrintRoutingTableAllAt (Seconds (11), routingStream);
}

void NetworkController::dataTransmission() {
    OnOffHelper dataApp("ns3::UdpSocketFactory",InetSocketAddress(ipAddressContainer.GetAddress(14)));
    ApplicationContainer appContainer = dataApp.Install(nodeContainer);
    appContainer.Start(Seconds(5));
    appContainer.Stop(Seconds(7));
}


void NetworkController::initNetworkNodes() {
    NodeContainer nodeContainer;
    InternetStackHelper networkProtocols;
    Ipv4AddressHelper ip;
    nodeContainer.Create(constellation->constellationSize);
    networkProtocols.Install(nodeContainer);
    ip.SetBase("110.110.0.0","255.255.0.0");
    // Initial channels initialization 
    for(NanoSatellite* s : constellation->satellites) {
        std::vector<NodeContainer> nodeVector;
        if(!s->reachableNodes.empty()) {
            NodeContainer nodes;
            for(int k : s->reachableNodes) {
                nodes.Add(nodeContainer.Get(s->satelliteID));
                nodes.Add(nodeContainer.Get(k));
                float delay = s->getPropagationTime(constellation->satellites[k]);
                CsmaHelper csmaChannel;
                csmaChannel.SetChannelAttribute("DataRate",StringValue("64Mbps"));
                csmaChannel.SetChannelAttribute("Delay",StringValue(std::to_string((int) (delay*10e6)) + "ms"));
                NetDeviceContainer nodeDevice = csmaChannel.Install(nodes);
                ip.Assign(nodeDevice);
                //std::cout << nodes.Get(0)->GetObject<Ipv4>()->GetAddress(1,0).GetLocal() << "\n";
                nodeVector.push_back(nodes);
                std::cout << s->satelliteID << " : " << k << "\n";
            }
        }
        satelliteNodes[s] = nodeVector;
    }
}

void NetworkController::updateNetworkNodes() {

}

*/
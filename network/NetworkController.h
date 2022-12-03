#include "ns3/Constellation.h"
#include <ns3/core-module.h>
#include "ns3/network-module.h"
#include "ns3/aodv-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/v4ping-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/mobility-module.h"

class NetworkController {
    public: 
        
        Constellation* constellation;

        std::map<NanoSatellite*,std::vector<ns3::NodeContainer>> satelliteNodes;
        ns3::NodeContainer nodeContainer;
        ns3::NetDeviceContainer deviceContainer;
        ns3::Ipv4AddressHelper ipv4;
        ns3::Ipv4InterfaceContainer ipAddressContainer;
        int time = 0;

        NetworkController(Constellation* constellation);
        NetworkController(int startingTime);
        NetworkController(Constellation* newConstellation, int startingTime);
        NetworkController();
        ~NetworkController();
        
        std::vector<std::vector<int>> nodesCommonChannel(NanoSatellite* satellite);
        void setDefaultParameters();
        void setNetworkNodesPos();
        void setNetworkNodesChannels();
        void setNetworkNodesIP();
        void initConstellation();
        void scheduleUpdates();
        void dataTransmission();
        void configureNodes();
        void updateNetworkNodes(int timeCounter);
        void setApplications();
        void setTime();
        void updateTime();

};  
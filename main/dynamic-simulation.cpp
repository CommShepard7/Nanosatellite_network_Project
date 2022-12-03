#include <ns3/NetworkController.h>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DynamicSimulation");


NetworkController networkController(0);
int timeCounter = 0;
/*
void updateNodes() {
  networkController.updateNetworkNodes(timeCounter);
  timeCounter++;
}
*/
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    networkController.initConstellation();
    networkController.setDefaultParameters();
    networkController.setNetworkNodesPos();
    networkController.setNetworkNodesChannels();
    //networkController.setNetworkNodesIP();
    //networkController.dataTransmission();
    
    for(int k = 0; k <= 0; k++) {
      networkController.constellation->updateCoordinates(k);
      //networkController.setNetworkNodesChannels();
      networkController.constellation->setLinks(k);
      std::cout << networkController.constellation->satellites[k%99] << " \n";
    }


    /*
    for(int k = 0; k <= 9998; k++) {
       Simulator::Schedule(Seconds(k*10),&updateNodes);
    }
    */
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Execution time : " << std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count() << "\n\n";
    NS_LOG_UNCOND("Scheduling complete.\n");
    NS_LOG_UNCOND("Launching simulation.\n");
    Simulator::Stop(Seconds(1000));
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_UNCOND("Simulation complete.\n");
    return 0;
}
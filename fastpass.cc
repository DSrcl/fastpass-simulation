#include <vector>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-address.h"
#include "edge-coloring/euler-coloring.h"
#include "lan/lan-helper.h"
#include "utils.h"


#define RACK_SIZE  500
#define RACK_COUNT 12
#define AGGR_COUNT 10

using namespace ns3;



int main()
{
	vector<NodeContainer> terminalNodes(RACK_COUNT);
	for (int i = 0; i < RACK_COUNT; i++) {
		NodeContainer rackNodes;
		rackNodes.Create(RACK_SIZE);
		terminalNodes[i] = rackNodes;
	}

	NodeContainer torSwitchNodes;
	torSwitchNodes.Create(RACK_COUNT);

	NodeContainer aggrSwitchNodes;
	aggrSwitchNodes.Create(AGGR_COUNT);
	
	SimpleNetDeviceHelper deviceHelper;
	deviceHelper.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
	deviceHelper.SetChannelAttribute("Delay", StringValue("0ms"));	

	LanHelper<SimpleNetDeviceHelper> lanHelper(deviceHelper);
	
}

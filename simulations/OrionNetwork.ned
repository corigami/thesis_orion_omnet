//
// Copyright (C) 2014 OpenSim Ltd.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

//package inet.applications.orionapp;
package orion.simulations;

import inet.base.LifecycleController;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.nodes.aodv.AODVRouter;
import inet.nodes.inet.AdhocHost;
import inet.world.radio.IdealChannelModel;
import inet.world.radio.ChannelControl;
//import inet.world.scenario.ScenarioManager;

network OrionNetwork
{
    parameters:
        int numHosts;
        
    submodules:
      //  channelControl: IdealChannelModel {
         channelControl: ChannelControl {
            parameters:
                @display("p=50,50");
        }
        configurator: IPv4NetworkConfigurator {
            parameters:
               	addDefaultRoutes = false;
                addStaticRoutes = false;
                addSubnetRoutes = false;
                config = xml("<config><interface hosts='*' address='10.0.x.x' netmask='255.255.0.0'/></config>");
                @display("p=50,100");
        }
        routingTableRecorder: RoutingTableRecorder {
            parameters:
                @display("p=50,150");
        }
        lifecycleController: LifecycleController {
            parameters:
                @display("p=50,200");
        }

//    host[numHosts]: AODVRouter{
//            parameters:
//                @display("i=device/pocketpc_s;r=,,#707070");
//        }
   host[numHosts]: AdhocHost{
                   parameters:
                @display("i=device/pocketpc_s;r=,,#707070");
        }
    connections allowunconnected:

}

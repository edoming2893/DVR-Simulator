

#include <stdio.h>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>

using namespace std;

int const GRAPHSIZE = 100;

void runsimulation(std::string filename){
    
    cout << endl;
    
    struct Node{
        // node ID
        int name;
        // identifying edge of node or neightbors this array represents edge to our graph
        int neighbors[GRAPHSIZE];
        // creating space for routing table
        int destination[GRAPHSIZE];
        int cost[GRAPHSIZE];
        int nexthop[GRAPHSIZE];
    };

    struct Dvpacket{
        int dest[GRAPHSIZE];
        int travelcost[GRAPHSIZE];
    };
    
    // create graph will contain all nodes
    Node graph[GRAPHSIZE];
    
    // assingning node its name as well as intitializing everything to -1
    // any slot with -1 is considered to be empty
    
    
    for (int i = 0; i < GRAPHSIZE ; i ++) {
        graph[i].name = i;
        for(int j = 0 ; j < GRAPHSIZE ;  j++){
            graph[i].destination[j] = -1;
            graph[i].cost[j] = -1;
            graph[i].nexthop[j] = -1;
            graph[i].neighbors[j] = -1;
            
        }
    }
    
    ifstream inFile;
    inFile.open(filename.c_str());
    
    int src, dest, cost;
    
    int numnodes = 0;

    // intializing nodes with the correct information for their neighbors
    // information is provided from topology file
    
    while( inFile >> src >> dest >> cost) {
        
        graph[src].destination[dest] = dest;
        graph[src].cost[dest] = cost;
        graph[src].nexthop[dest] = dest;
        graph[src].neighbors[dest] = dest;
        
        graph[dest].destination[src] = src;
        graph[dest].cost[src] = cost;
        graph[dest].nexthop[src] = src;
        graph[dest].neighbors[src] = src;
        
        if (src > numnodes){
            numnodes = src + 1;
        }
        else if(dest > numnodes){
            numnodes = dest + 1;
        }
            

        }

    //including the poisoning scheme
    // since we cant do infinity we will put cost to 100,000
    for(int k = 0 ; k<numnodes ; k++){
        for (int j=0 ; j<numnodes ; j++ ){
            if(graph[k].destination[j] == -1){
                graph[k].destination[j] = j;
                graph[k].cost[j] = 100000;
                graph[k].nexthop[j] = -1;
                
            }

            if(graph[k].name == graph[k].destination[j]){
                graph[k].cost[j] = 0;
                graph[k].nexthop[j] = k;
            }
        }
    }
    
    //print out intial routing tables
    // making out file
    string resultsouput = filename.substr(0, 9) + "_RESULTS.txt";
    ofstream Outfile;
    Outfile.open(resultsouput.c_str());
    
    
    for (int i=0; i<numnodes ; i++){
        Outfile << "****** Intial Routing Table: "<< graph[i].name<< "******"<<endl;
        Outfile << "Poisoning Nodes Cost (∞): 100,000 : Unknown Edge/Next Hop represented by -1"<< endl;
        Outfile << left << setw(6) << "Dest"<< setw(10) <<"cost" << setw(6)<< "N.H" << endl;
        
        
        for (int j = 0; j<numnodes ; j++){
            Outfile << left << setw(6) << graph[i].destination[j]
                    << setw(10) << graph[i].cost[j] << setw(6)
                    << graph[i].nexthop[j] << endl;
        }
    }

    Outfile << ".................................."<<endl;
    Outfile << ".................................."<<endl;
    Outfile << "............RESULTS..............."<<endl;
    Outfile << ".................................."<<endl;
    Outfile << ".................................."<<endl<<endl;;
    //create the DV packets
    
    // counter for number of packets sent
    int packetcount = 0;
    
    // will update to the last node update
    // everytimes node is updated it will be replaced with updated node
    int recentupdatednode = -2;
    
    
    //flag to keep loop for updating
    bool flag;
    int round = 1;
    
    do{
        flag = false;
        // Beginning of a Round

        // For every node make a dv packet and send it to neighbors
        for(int i=0; i<numnodes ; i++){
            // initializing dv packet
            
            Dvpacket packet;
            
            //for every node make a dv packet
            // here we go through all of node 'n' to build packet
            for(int k=0 ; k<numnodes ; k++){
                // getting information that is known
                if(graph[i].cost[k] > 0 && graph[i].destination[k] != graph[i].name ){
                    packet.dest[k] = graph[i].destination[k];
                    packet.travelcost[k] = graph[i].cost[k];
                }
                // setting cost from self to 0
                else{
                    packet.travelcost[k] = 0;
                }
            }

            // Dv Packet is made so we will send it to neighbors
            // here we go thorugh node 'n' same as above
            for (int z=0 ; z<numnodes ; z++){
    
            // selecting neighbors to send packet
                //cout << "Neighbor of " << graph[i].name<< endl;
                // Neighbors print out correctly
                if(graph[i].neighbors[z] != -1){
                    //cout << "This is a Neighbor: " << graph[i].neighbors[z]<<endl;
                    //we will send the DV packet to that neighbor
                    // z is the neightbor so we will now update based on the dv packet
                    // we will compare the packe cost to receipeint of dv packet
                    // go through the dv packet to get information
                    packetcount++;
                    for (int t=0; t<numnodes ; t++){
                        
                        
                        // here the neighbor 'z' has received the packet and is now reading it
                        // it will determine wheter or not to update based on information provided
                        // inside the packet
                        //NOTE: Neighbors is detrmening update based on information it has and DVPACKET ONLY
                        if (graph[z].cost[t] > packet.travelcost[t] + graph[z].cost[i] ){
                            
                            graph[z].cost[t] = packet.travelcost[t] + graph[z].cost[i];
                            graph[z].nexthop[t] = graph[i].name;
                            
                            // if update occurred then we will go to next round
                            flag=true;
                            // increment round number to track how many rounds its going through
                            //round++;
                            // keep track of most recent updated node
                            recentupdatednode = graph[z].name;
                        }
                        
                    }
                }
            

            }
            
        }
         round++;

    }while(flag);
    
    

    //print out intial routing tables
    Outfile << "TOTAL NUMBER OF ROUNDS FOR CONVERGENCE: "<< round<<endl;
    Outfile << "TOTAL NUMBER OF PACKETS SENT FOR CONVERGENCE: "<< packetcount<<endl;
    Outfile << "LAST NODE TO CONVERGE: "<< recentupdatednode <<endl;
    
    for (int i=0; i<numnodes ; i++){
        Outfile << "****** Final Routing Tables: "<< graph[i].name<< "******"<<endl;
        
        for (int j = 0; j<numnodes ; j++){
            Outfile << left << setw(6) << graph[i].destination[j]
                    << setw(10) << graph[i].cost[j] << setw(6)
                    << graph[i].nexthop[j] << setw(5)<<  endl;
        }
    }
    
    string answer;
    int source;
    int destined;
    
    cout << "Would you like to simulate routing a specific packet for this topology, "<< filename<<",? " << endl;
    cout << "(Type in all Uppercase : YES or NO): ";
    cin >> answer;
    
    cout <<endl<<endl;
    
    
    cout << "What is the source node (Answer with the specific source node id like 0 or 3) :  ";
    cin >> source;
    
    cout << endl<<endl;
    
    cout << "What is the destine/receiving node (Answer with the specific source node id like 0 or 3) :  ";
    cin >> destined;
    
    cout << endl<< endl;
    
    
    // Routing the Packets
    
    // Route Node 0 sends packet to node 3 route the packet
 
    if( answer == "YES"){
        Outfile << endl << endl;
        Outfile <<"Simulating Packet Delivery:   " << endl;
        Outfile <<"From: " << source << endl;
        Outfile <<"To: " << destined << endl;
        
        Outfile <<"*** Routing Process ***" << endl;
        
        int currently_located = source,
            take_to = destined,
            next_location = -1,
            totalcost = graph[currently_located].cost[take_to];
    
        bool sendagain = true;
    
        do{

            // start at source node and look for entry destination of packer
            //note this will give use the next node to go to
            Outfile <<"Current Location: " << currently_located<<endl;
            next_location = graph[currently_located].nexthop[take_to];
            Outfile << "Next Location:  "<< next_location<<endl;
            //
            currently_located = next_location;
            next_location = graph[currently_located].nexthop[take_to];
            //
            if(currently_located == next_location ){
                Outfile <<"Current Location: " << currently_located<<endl;
                Outfile << "Next Location:  "<< next_location<<endl;
                Outfile << "Congratulations: Packet Delivery Complete"<<endl;
                Outfile << "Total Cost: " << totalcost<< endl;
                sendagain = false;
            }
        }while(sendagain);
    }
    
    cout << "All Done, Check Out Output file, "<< resultsouput << " ,for Results." << endl;
    cout << "Thanks for Trying Out the Simulator."<<endl;
    
}

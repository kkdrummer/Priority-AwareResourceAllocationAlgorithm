#include<bits/stdc++.h>
using namespace std;


// structure of Vm
struct vm{
	int vmId;
	int resourceDemand; 
	int applicationPriorityLevel; // 1-Normal, 2-Critical, 3-Highly Critical
	int arrivalTime;
	int canAllocateInLowerHosts;
	void read(){
		cin>>vmId>>resourceDemand>>applicationPriorityLevel>>arrivalTime>>canAllocateInLowerHosts;
	}
	void print(){
		cout<<"VM Information of vmId "<<vmId<<" : \n";
		cout<<"resourceDemand = "<<resourceDemand<<"\n";
		cout<<"applicationPriorityLevel = "<<applicationPriorityLevel<<"\n";
		cout<<"arrivalTime = "<<arrivalTime<<"\n";
		cout<<"\n";
	}
	bool operator < (const vm& other) const{
		// arrange in order of lesser arrival time, then greater priority, then lower host allocation possibility, then greater resource demand
		return make_pair(make_pair(arrivalTime,-applicationPriorityLevel),make_pair(canAllocateInLowerHosts,-resourceDemand))<make_pair(make_pair(other.arrivalTime,-other.applicationPriorityLevel),make_pair(other.canAllocateInLowerHosts,-other.resourceDemand));
	}
};


// structure of host
struct host{
	int hostId;
	int hostPriorityLevel; // 1-Normal, 2-Critical, 3-Highly Critical
	int capacity;
	void read(){
		cin>>hostId>>hostPriorityLevel>>capacity;
	}
	void print(){
		cout<<"Host Information of hostId "<<hostId<<" : \n";
		cout<<"hostPriorityLevel = "<<hostPriorityLevel<<"\n";
		cout<<"capacity = "<<capacity<<"\n";
		cout<<"\n";
	}
	bool operator < (const host& other) const{
		// arrange in order of higher priority level, then greater capacity
		return make_pair(-hostPriorityLevel,-capacity)<make_pair(-other.hostPriorityLevel,-other.capacity);
	}
};


// Globally stored vm and host information
vm vmList[101];
host hostList[11];
int numOfVM;
int numOfHost;
int vmHostMap[101];

void init();
void allocate();
void printMapping();

int main()
{
	init();

	allocate();

	printMapping();

	return 0;
}

// initializes the global info of VMs and Hosts
void init(){
	freopen("input5.txt","r",stdin); // open input file
	freopen("output5.txt","w",stdout); // open output file

	//---vm input ---//
	cin>>numOfVM;
	for(int i=0;i<numOfVM;i++){
		vmList[i].read();
	}

	// sort the VMs in increasing order of arrival time
	sort(vmList,vmList+numOfVM);

	cout<<"-----VM requests In order of processing:-----\n\n";
	for(int i=0;i<numOfVM;i++){
		vmList[i].print();
	}
	cout<<"\n\n";
	//--- vm input end ---//


	//--- host input ---//
	cin>>numOfHost;

	for(int i=0;i<numOfHost;i++){
		hostList[i].read();
	}
	// sort hosts in decreasing order of priority
	sort(hostList,hostList+numOfHost);

	cout<<"-----Hosts Available:-----\n\n";	
	for(int i=0;i<numOfHost;i++){
		hostList[i].print();
	}
	//--- host input end ---//

}

int startHost(int priorityLevel){
	int l=0,r=numOfHost-1;
	int ret;
	while(l<=r){
		int m=(l+r)/2;
		if(hostList[m].hostPriorityLevel<=priorityLevel){
			ret=m;
			r=m-1;
		}
		else l=m+1;
	}
	return ret;
}

// Allocates VMs in Hosts
void allocate()
{
	for(int i=0;i<numOfVM;i++){

		int id=vmList[i].vmId;
		int resDemand=vmList[i].resourceDemand;
		int priorityLevel=vmList[i].applicationPriorityLevel;
		int canAllocateInLower=vmList[i].canAllocateInLowerHosts;
		int allocatedHostIndex=-1;

		// starts checking from the host with same priority level and keep checking until allocated
		for(int j=startHost(priorityLevel);j<numOfHost;j++){
			
			// lower priority level reached and not possible to allocate in lower level hosts
			if(hostList[j].hostPriorityLevel < priorityLevel && !canAllocateInLower)break;

			if(hostList[j].capacity >= resDemand){
					hostList[j].capacity -= resDemand;
					allocatedHostIndex=j;
					break;
			}

		}

		if(allocatedHostIndex!=-1){ // host allocated

			vmHostMap[id]=hostList[allocatedHostIndex].hostId;

			// move the allocated host down the list to maintain the sorted order of hosts
			for(int j=allocatedHostIndex+1;j<numOfHost;j++){
				if(hostList[j].hostPriorityLevel<hostList[allocatedHostIndex].hostPriorityLevel||hostList[j].capacity<hostList[allocatedHostIndex].capacity)break;
				swap(hostList[j],hostList[j-1]);
			}
		}

		else{
			vmHostMap[id]=-1;
		}
	}
}


// Prints the allocation Mapping
void printMapping(){
	cout<<"\n\n-----VM to Host Mapping-----\n\n";
	vector<int> unAllocatedVmIds;
	for(int i=0;i<numOfVM;i++){
		cout<<"vmId - "<<i<<" "<<"Allocated hostID - "<<vmHostMap[i]<<"\n";
		
		if(vmHostMap[i]==-1){
			unAllocatedVmIds.push_back(i);
		}
	}
	cout<<"\n\n";
	cout<<"Unallocated VMs are:\n";

	for(auto vmId:unAllocatedVmIds){
		cout<<vmId<<"\n";
	}
}
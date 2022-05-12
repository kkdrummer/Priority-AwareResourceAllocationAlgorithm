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
		cin>>vmId>>resourceDemand>>applicationPriorityLevel>>arrivalTime;//>>canAllocateInLowerHosts;
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
		return make_pair(make_pair(arrivalTime,0),make_pair(canAllocateInLowerHosts,-resourceDemand))<make_pair(make_pair(other.arrivalTime,0),make_pair(other.canAllocateInLowerHosts,-other.resourceDemand));
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
		// arrange in order of higher priority level, then smaller capacity capacity
		return make_pair(-hostPriorityLevel,capacity)<make_pair(-other.hostPriorityLevel,other.capacity);
	}
};

struct Metric{
	int time;
	double utilization[4]; // 1-Normal, 2-Critical, 3-Highly Critical
};


// Globally stored vm and host information
vm vmList[100001];
host hostList[4][101];
int numOfVM;
int totalNumOfHost;
int numOfHost[4]={0};
int vmHostMap[100001];
int totalCapacity[4];
int occupiedCapacity[4]={0};
int allocatedRequests[4]={0};
int totalRequests[4];
int weight[4]={0,1,3,9};
int currentTime;
double utilization[3001][4];
string priorityNames[4]={"", "Normal", "Critical", "Highly-Critical"};
int algorithmDisabled=1;

void init();
int startHost(int priorityLevel);
void calculateUtilization(int uptoTime);
double calculatePower(double util);
void allocate();
void printMapping();
void printMetrics();

int main()
{
	init();

	allocate();

	//printMapping();

	printMetrics();

	return 0;
}

// initializes the global info of VMs and Hosts
void init(){
	freopen("tests/in/sample_input1.txt","r",stdin); // open input file
	freopen("tests/out/b/sample_output1.txt","w",stdout); // open output file

	currentTime=1; // in seconds

	cout<<"----Simulation Started-----\n\n";
	
	if(algorithmDisabled){
		cout<<"-----Algorithm Disabled-----\n\n";
	}

	else{
		cout<<"-----Algorithm Enabled-----\n\n";
	}

	//---vm input ---//
	cin>>numOfVM;
	int minResourceDemand=-1,maxResourceDemand=-1;
	for(int i=0;i<numOfVM;i++){
		vmList[i].read();
		vmList[i].canAllocateInLowerHosts=1;
		if(minResourceDemand==-1 || minResourceDemand>vmList[i].resourceDemand) minResourceDemand=vmList[i].resourceDemand;
		if(maxResourceDemand==-1 || maxResourceDemand<vmList[i].resourceDemand) maxResourceDemand=vmList[i].resourceDemand;
	}
	cout<<"Total VM Requests = "<<numOfVM<<"\n\n";
	cout<<"Range of Resource Demand = "<<minResourceDemand<<" - "<<maxResourceDemand<<"\n\n";

	// sort the VMs in increasing order of arrival time
	sort(vmList,vmList+numOfVM);

	/*
	cout<<"-----VM requests In order of processing:-----\n\n";
	for(int i=0;i<numOfVM;i++){
		vmList[i].print();
	}
	cout<<"\n\n";
	//--- vm input end ---//
	*/
	
	//--- host input ---//
	cin>>totalNumOfHost;

	for(int i=0;i<totalNumOfHost;i++){
		host tempHost;
		tempHost.read();
		int pLevel=tempHost.hostPriorityLevel;

		if(algorithmDisabled){
			pLevel=1;
		}

		hostList[pLevel][numOfHost[pLevel]]=tempHost;
		numOfHost[pLevel]++;
		totalCapacity[pLevel]+=tempHost.capacity;	
	}

	// sort hosts in decreasing order of priority
	for(int i=1;i<=3;i++)sort(hostList[i],hostList[i]+numOfHost[i]);

	cout<<"Total Host = "<<totalNumOfHost<<"\n\n";
	cout<<"Host Capacity = "<<hostList[1][0].capacity<<"\n";

/*
	cout<<"-----Hosts Available:-----\n\n";	
	for(int i=0;i<numOfHost;i++){
		hostList[i].print();
	}
	//--- host input end ---//
*/

}

// Finds the id of the host from which allocation checking has to start 
int idealHost(int pLevel, int rDemand){
	// if no hosts of pLevel priority or host with maximum capacity is also not sufficient
	if(numOfHost[pLevel]==0||hostList[pLevel][numOfHost[pLevel]-1].capacity<rDemand)return -1;

	int l=0,r=numOfHost[pLevel]-1;
	int ret = -1;

	while(l<=r){
		int m=(l+r)/2;
		if(hostList[pLevel][m].capacity>=rDemand){
			ret=m;
			r=m-1;
		}
		else l=m+1;
	}
	
	return ret;
}

// Calculates Utilization At each second from currentTime to uptoTime
void calculateUtilization(int uptoTime){
	
	for(int time=currentTime;time<=uptoTime;time++){
		for(int priorityLevel=1;priorityLevel<=3;priorityLevel++){
			utilization[time][priorityLevel]=((double)occupiedCapacity[priorityLevel]/totalCapacity[priorityLevel]);
		}
	}
}

double calculatePower(double util){
	return 10.00*util + 20.00;
}

int score(){
	int s=0;
	for(int pLevel=1;pLevel<=3;pLevel++){
		s+=weight[pLevel]*(double)(allocatedRequests[pLevel]);
	}
	return s;
}


// Allocates VMs in Hosts
void allocate()
{
	for(int i=0;i<numOfVM;i++){
		
		int arrTime=vmList[i].arrivalTime;
		int id=vmList[i].vmId;
		int resDemand=vmList[i].resourceDemand;
		int priorityLevel=vmList[i].applicationPriorityLevel;
		int canAllocateInLower=vmList[i].canAllocateInLowerHosts;
		int allocatedHostLevel=-1;
		int allocatedHostIndex=-1;
		
		totalRequests[priorityLevel]++;

		calculateUtilization(vmList[i].arrivalTime-1); // Calculate Utilization For Times before Current Request's Arrival Time

		// starts checking from the host with same priority level and keep checking until allocated
		for(int pLevel=priorityLevel;pLevel>=1;pLevel--){
			
			// lower priority level reached and not possible to allocate in lower level hosts
			if(pLevel < priorityLevel && !canAllocateInLower)break;

			int id=idealHost(pLevel,resDemand);

			if(id!=-1){
					hostList[pLevel][id].capacity -= resDemand;
					allocatedHostLevel=pLevel;
					allocatedHostIndex=id;
					break;
			}

		}

		if(allocatedHostIndex!=-1){ // host allocated

			vmHostMap[id]=hostList[allocatedHostLevel][allocatedHostIndex].hostId;
			occupiedCapacity[allocatedHostLevel]+=resDemand;
			allocatedRequests[priorityLevel]++;

			// move the allocated host down the list to maintain the sorted order of hosts
			for(int j=allocatedHostIndex-1;j>=0;j--){
				if(hostList[allocatedHostLevel][j].capacity<hostList[allocatedHostLevel][j+1].capacity)break;
				swap(hostList[allocatedHostLevel][j],hostList[allocatedHostLevel][j+1]);
			}
		}

		else{ // no host allocated
			vmHostMap[id]=-1;
		}

		currentTime=arrTime;
	}

	calculateUtilization(currentTime); // Calculate Utilization For the last second, after which it remains constant
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

	cout<<"\n\n";
}

//Prints the Performance Metrics
void printMetrics(){
	
	cout<<"\n\n-----Performance Metrics At Each second-----\n\n";
	double averageUtilization[4]={0.00},averagePowerConsumption[4]={0.00};

	for(int time=1;time<=currentTime;time++){
		cout<<"At Time "<<time<<": \n";
		for(int priorityLevel=1;priorityLevel<=3;priorityLevel++){
			
			if(algorithmDisabled&&priorityLevel>1)break;
			
			cout<<priorityNames[priorityLevel]<<":  ";

			double util=utilization[time][priorityLevel];
			cout<<"Utilization = "<<util<<"    ";
			averageUtilization[priorityLevel]+=util;
			
			double power=calculatePower(util);
			cout<<"Power Consumption = "<<power<<"\n\n\n";
			averagePowerConsumption[priorityLevel]+=power;
		}
	}

	cout<<"\n\n-----Average Values:-----\n\n";
	for(int priorityLevel=1;priorityLevel<=3;priorityLevel++){
		if(algorithmDisabled&&priorityLevel>1)break;
		cout<<"Average Utilization for "<<priorityNames[priorityLevel]<<" hosts = "<<averageUtilization[priorityLevel]/(double)currentTime<<"\n\n";
		cout<<"Average Power Consumption for "<<priorityNames[priorityLevel]<<" hosts = "<<averagePowerConsumption[priorityLevel]/(double)currentTime<<"\n\n";
	}

	cout<<"\n\n-----VM Application Allocation Rates:-----\n\n";
	for(int priorityLevel=1;priorityLevel<=3;priorityLevel++){
		cout<<priorityNames[priorityLevel]<<": \n";
		cout<<"Total Requests = "<<totalRequests[priorityLevel]<<"\n";
		cout<<"Allocated Requests = "<<allocatedRequests[priorityLevel]<<"\n";
		cout<<"Allocation Rate ="<<(double)((double)allocatedRequests[priorityLevel]/(double)totalRequests[priorityLevel])*100<<"%\n\n";
	}

	cout<<"\n\n-----Final Results-----\n\n";
	int totalAllocatedRequests=0;
	for(int pLevel=1;pLevel<=3;pLevel++)totalAllocatedRequests+=allocatedRequests[pLevel];
	cout<<"Total Allocated Requests = "<<totalAllocatedRequests<<" out of "<<numOfVM<<"\n";
	cout<<"SCORE = "<<score()<<"\n";

}
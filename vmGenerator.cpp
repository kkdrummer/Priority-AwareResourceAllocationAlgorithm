//----AUTHOR:kkdrummer----/
#include<bits/stdc++.h>
using namespace std;

int rn(int base){ // returns a number in the range 1 to base
	return (rand()%base) + 1;
}

int main(){
	srand(time(0));
	int numOfVM=100;
	
	freopen("input5.txt","w",stdout);
	cout<<numOfVM<<"\n";
	
	for(int i=0;i<numOfVM;i++){
		cout<<i<<" "; // id
		cout<<500+rn(500)<<" "; // resource demand
		cout<<rn(3)<<" "; // priority level
		cout<<rn(30)<<" "; // arrival time
		cout<<rn(2)-1<<"\n"; // can allocate in lower level hosts 
	} 
	
	
	return 0;
}

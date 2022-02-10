//----AUTHOR:kkdrummer----/
#include<bits/stdc++.h>
using namespace std;

int rn(int base){ // returns a number in the range 1 to base
	return (rand()%base) + 1;
}

int main(){
	srand(time(0));
	int numOfVM=30000;
	
	freopen("tests/in/input20.txt","w",stdout);
	cout<<numOfVM<<"\n";
	
	for(int i=0;i<numOfVM;i++){
		cout<<i<<" "; // id
		int rd=rn(400)+rn(400)+rn(300)+rn(20)+rn(20)+rn(20)+rn(20)+rn(20); // resource demand
		int pl=3;
		if(rd%5==0)pl=1;
		else if(rd%5<2)pl=2;
		cout<<rd<<" ";
		cout<<pl<<" "; // priority level
		cout<<rn(30)<<" "; // arrival time
		cout<<rn(2)-1<<"\n"; // can allocate in lower level hosts 
	} 
	
	
	return 0;
}

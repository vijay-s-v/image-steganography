#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// Embed a hidden message into a given image
int main(int argc, char * argv []){
	char c;
	int lineCounter = 1;
	ifstream f("owl.png", ios::binary | ios::in);
	while(f.get(c)){
		for(int i = 7; i >= 0; i--){
			cout << ((c >> i) & 1);
		}
		cout << " ";
		if(lineCounter % 6 == 0){ cout << endl; }
		lineCounter++;
	}
	return 0;
}

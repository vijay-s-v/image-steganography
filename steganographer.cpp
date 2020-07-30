#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/*
Embed a hidden message into a given image
Depending on usage, this program will either encrypt a file into an image or decrypt a file from an image
This program uses LSB steganography to perform the encryption

Encryption
Argument 1: string - "encrypt"
Argument 2: string - name of image file to use
Argument 3: string - name of file to be encrypted
Argument 4: int - number of trailing bits in the image to use
Output: a file named secret whose type will be the type of the image file used
Usage example: ./steganographer encrypt owl.png secret.txt 2
Note: if it is not possible to encrypt the file into the given image, an error will be thrown

Decryption
Argument 1: string - "decrypt"
Argument 2: string - name of image file to decrypt
Argumemt 3: string - file extension for the output file
Argument 4: int - number of trailing bits to extract from the image
Output: a file named secret using the file extension given  
Usage example: ./steganographer decrypt secret.png txt 2
*/

void encrypt(){}

void decrypt(){}

int main(int argc, char * argv []){
	// Check for correct number of arguments, exit if not called properly
	if(argc != 5){ 
		cout << "Error: expecting 4 arguments, please see readme" << endl;
		return 1;
	}

	// Get filetype of original image
	string original = argv[2];
	string encryptedImage = "secret" + original.substr(original.find_last_of("."), original.length());
	
	// Open required files
	ifstream originalImage(argv[2], ios::binary | ios::in);
	ifstream targetFile(argv[3], ios::binary | ios::in);
	ofstream outputFile("duplicate.png", ios::binary | ios::out);

	// Get size of original image in bytes
	originalImage.seekg(0, originalImage.end);
	int imageSize = originalImage.tellg();
	originalImage.seekg(0, originalImage.beg);

	// ofstream outputFile(encryptedImage, ios::binary | ios::out);
	
	char inChar, outChar;
	int byte [8];
	while(originalImage.get(inChar)){
		int j = 0;
		for(int i = 7; i >= 0; i--){
			byte[j] = ((inChar >> i) & 1);
			j++;
		}
		for(int j = 0; j < 8; j++){
			outChar <<= 1;
			outChar += byte[j];
		}
		outputFile.write(&outChar, sizeof(outChar));
	}

	/*
	while(originalImage.get(c)){
		cout << c;
		outputFile.write(&c, sizeof(c));
	}*/

	// Close open files
	originalImage.close();
	targetFile.close();
	outputFile.close();
	return 0;
}

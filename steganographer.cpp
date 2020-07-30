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

Exit codes
1 - Incorrect usage
2 - Unable to open file
3 - Image too small to encrypt the target file in
*/

// Function for encryption
// Inputs:
// org - name of the image to be used in the encryption
// file - name of the file to be encrypted
// factor - number of trailing bits to change in each byte in the image
int encrypt(string org, string file, int factor){
	char inChar, fileChar;
	int fileByte[8], outByte[8];

	// Get filetype of original image
	string encryptedImage = "secret" + org.substr(org.find_last_of("."), org.length());
	
	// Open required files
	ifstream originalImage(org, ios::binary | ios::in);
	ifstream targetFile(file, ios::binary | ios::in);
	ofstream outputFile(encryptedImage, ios::binary | ios::out);

	// Check that the files were opened properly
	if(!originalImage.is_open()){
		cout << "Error: unable to open " << org << endl;
		return 2;
	}
	if(!targetFile.is_open()){
		cout << "Error: unable to open " << file << endl;
		return 2;
	}

	// Get size of original image and target file in bytes
	originalImage.seekg(0, originalImage.end);
	int imageSize = originalImage.tellg();
	originalImage.seekg(0, originalImage.beg);

	targetFile.seekg(0, targetFile.end);
	int targetSize = targetFile.tellg();
	targetFile.seekg(0, targetFile.beg);

	// Check if encryption is possible
	if((targetSize / factor * 8) > imageSize){
		cout << "Error: file is too large to be encrypted with the given encryption factor" << endl;
		return 3;
	}
	
	// Perform encryption
	// Could probably be done more efficiently
	while(targetFile.get(fileChar)){
		int bitCounter = 0, outBitCounter = 0;

		// Get bits of the byte from file
		for(int i = 7; i >= 0; i--){
			fileByte[bitCounter] = (fileChar >> i) & 1;
			bitCounter++;
		}

		// Get bytes from image to encrypt
		for(int i = 0; i < (8 / factor); i++){
			bitCounter = 0;
			char outChar;
			originalImage.get(inChar);

			// Load outgoing byte with image data
			for(int j = 7; j >= factor; j--){
				outByte[bitCounter] = (inChar >> i) & 1;
				bitCounter++;
			}

			// Put file bits in the last few bits of the outgoing byte
			for(int j = factor - 1; j >= 0; j--){
				outByte[bitCounter] = fileByte[outBitCounter];
				bitCounter++;
				outBitCounter++;
			}

			// Prepare the byte and write to file
			for(int j = 0; j < 8; j++){
				outChar <<= 1;
				outChar += outByte[j]; 
			}
			outputFile.write(&outChar, sizeof(outChar));
		}
		cout << outBitCounter << endl;
	}
	
	// Close files
	originalImage.close();
	targetFile.close();
	outputFile.close();
	return 0;
}

// Function for decryption
// Inputs:
// file - image to be decrypted
// ext - file extension of the output file
// factor - number of trailing bits used in encryption
int decrypt(string file, string ext, int factor){ 
	// Perform decryption
	return 0;
}

int main(int argc, char * argv []){
	// Check for correct number of arguments, exit if not called properly
	if(argc != 5){ 
		cout << "Error: expecting 4 arguments, please see readme" << endl;
		return 1;
	}

	int hideFactor = stoi(argv[4]);
	// Check that the hide factor is a power of 2 less than 8
	if(hideFactor > 4 || hideFactor < 1 || hideFactor == 3){
		cout << "Error: please choose an encryption factor of 1, 2, or 4" << endl;
		return 1;
	}
	
	string operation = argv[1];
	if(operation == "encrypt"){ return encrypt(argv[2], argv[3], hideFactor); }
	else if(operation == "decrypt"){ return decrypt(argv[2], argv[3], hideFactor); }
	else{
		cout << "Error: invalid first argument, please see readme" << endl;
		return 1;
	}
	
	/* Testing code
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
	*/

	return 0;
}

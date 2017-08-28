#include<iostream>
#include<string>
#include<sstream>//int to string
#include<fstream>//input and output files
#include<algorithm>//reverse()
#include<stdlib.h>//atoi()
#include<cmath>//pow()

using namespace std;

//Converts int value to 8 bit binary string
string toBinary(int x){
	stringstream ss;//to convert int to string
	string sBin="";
	
	for(int i=0;i<8;i++){
		ss<<x%2;
		x=x/2;
	}
	
	ss>>sBin;
	reverse(sBin.begin(),sBin.end());//inverts string
	
	return sBin;
}

//Returns ascii character (string) for 8 bit binary string received
string toString(string bin)
{
	string sOut="";
	int x=0;
	char c;
	
	for(int i=0;i<8;i++)
	{
		if(bin[i] == '1')
		{
			x = x + pow(2,7-i);
		}
	}
	
	c=x;
	sOut = sOut+c;
	
	return sOut;
	
}

//Shifts string "ShiftBy" places to the left
string shiftLeft (string myarray, int size, int shiftBy)
{
    char temp;
    
	if(shiftBy > size){
        shiftBy = shiftBy - size;
    }
    else if(shiftBy < 0)
    {
    	shiftBy = size + shiftBy;
	}

    if(size != 1 && shiftBy > 0)
	{    
        for(int i=0;i<shiftBy;i++)
        {
        	temp=myarray[0];
        	for(int j=0;j<size-1;j++)
        	{
				myarray[j] = myarray[j+1];
			}
			myarray[size-1]=temp;
		}
    }
    return myarray;
}

//Generates 8 bit keys (K1 and K2) and returns string with K1 and K2 concatenated (K1 + K2)
string keyGeneration(string kkk)
{
	string k1 = "", k2 = "", l = "", r = "", sP10 = "";
	int iP10[10] = {3,5,2,7,4,10,1,9,8,6};//P10 order
	int iP8[8] = {6,3,7,4,8,5,10,9};
	
	//P10
	for(int i=0;i<10;i++)
	{
		sP10=sP10 + kkk[iP10[i]-1];
	}
	
	l = sP10.substr(0,5);//L from p10[0] to p10[4]
	r = sP10.substr(5,5);//R from p10[5] to p10[9]
	
	l = shiftLeft(l,l.size(),1);
	r = shiftLeft(r,r.size(),1);
	
	sP10 = l + r; // LS-1
	
	////////////////////////////////////////K1
	for(int i=0;i<8;i++)
	{
		k1 = k1 + sP10[iP8[i]-1];
	}
	////////////////////////////////////////K1
	
	l = sP10.substr(0,5);//L from p10[0] to p10[4]
	r = sP10.substr(5,5);//R from p10[5] to p10[9]
	
	l = shiftLeft(l,l.size(),2);
	r = shiftLeft(r,r.size(),2);
	
	sP10 = l + r; // LS-2
	
	////////////////////////////////////////K2
	for(int i=0;i<8;i++)
	{
		k2 = k2 + sP10[iP8[i]-1];
	}
	////////////////////////////////////////K2
	
	
	return k1 + k2;
}

//fk function for SDES, takes message/cipher and key as size 8 strings
string fk(string sMessage,string k1)
{
	int iAux = 0;
	int iEP[8] = {4,1,2,3,2,3,4,1};
	int iP4[4] = {2,4,3,1};
	int S0[4][4] = {
		{1,0,3,2},
		{3,2,1,0},
		{0,2,1,3},
		{3,1,3,2}
	};
	int S1[4][4] = {
		{0,1,2,3},
		{2,0,1,3},
		{3,0,1,0},
		{2,1,0,3}
	};
	
	string l = "", r = "", sEP = "", sXOR = "", ll = "", rr = "", sAux = "", sP4 = "", sOut = "";
	
	l = sMessage.substr(0,4);//L from sMessage[0] to sMessage[3]
	r = sMessage.substr(4,4);//R from sMessage[4] to sMessage[7]
	
	/////////////////////////// start small fk(r,k1)
	//EP
	for(int i=0;i<8;i++)
	{
		sEP = sEP + r[iEP[i]-1];
	}
	
	//EP xor k1
	for(int i=0;i<8;i++)
	{
		if(sEP[i] == k1[i])
		{
			sXOR = sXOR + "0";
		}
		else
		{
			sXOR = sXOR + "1";
		}
	}
	
	ll = sXOR.substr(0,4);//LL from sXOR[0] to sXOR[3]
	rr = sXOR.substr(4,4);//RR from sXOR[4] to sXOR[7]
	
	//S0 with LL
	iAux = S0[atoi(ll.substr(0,1).c_str())*2 + atoi(ll.substr(3,1).c_str())][atoi(ll.substr(1,1).c_str())*2 + atoi(ll.substr(2,1).c_str())];
	
	switch (iAux)
	{
		case 0:
			sAux="00";
			break;
		case 1:
			sAux="01";
			break;
		case 2:
			sAux="10";
			break;
		case 3:
			sAux="11";
			break;
		default:
			break;
	}
	
	//S1 with RR
	iAux = S1[atoi(rr.substr(0,1).c_str())*2 + atoi(rr.substr(3,1).c_str())][atoi(rr.substr(1,1).c_str())*2 + atoi(rr.substr(2,1).c_str())];
	
	switch (iAux)
	{
		case 0:
			sAux=sAux+"00";
			break;
		case 1:
			sAux=sAux+"01";
			break;
		case 2:
			sAux=sAux+"10";
			break;
		case 3:
			sAux=sAux+"11";
			break;
		default:
			break;
	}
	
	//P4
	for(int i=0;i<4;i++)
	{
		sP4 = sP4 + sAux[iP4[i]-1];
	}
	
	/////////////////////////// end small fk(r,k1)
	
	//L xor fk(r,k1)
	for(int i=0;i<4;i++)
	{
		if(l[i] == sP4[i])
		{
			sOut = sOut + "0";
		}
		else
		{
			sOut = sOut + "1";
		}
	}
	
	//output value is sOut concatenated with R original
	return sOut + r;
}

//Switches left half with right half of a string of an even n size
string SW(string x, int n)
{
	return x.substr(n/2,n/2) + x.substr(0,n/2);
}

//Applies IP
string IP(string sOriginal)
{
	string sOut="";
	int iIP[8] = {2,6,3,1,4,8,5,7};
	for(int i=0;i<8;i++)
	{
		sOut = sOut + sOriginal[iIP[i]-1];
	}
	return sOut;
}

//Applies IP-1
string IPx(string sOriginal)
{
	string sOut="";
	int iIPx[8] = {4,1,3,5,7,2,8,6};
	for(int i=0;i<8;i++)
	{
		sOut = sOut + sOriginal[iIPx[i]-1];
	}
	return sOut;
}

string SDES_Enc(string sPlainText, string sKeys)
{
	string temp="",sIP="", sOut="";
	sIP=IP(sPlainText);
	
	temp=fk(sIP,sKeys.substr(0,8));
	
	temp = SW(temp,temp.size());
	
	temp=fk(temp,sKeys.substr(8,8));
	
	sOut=IPx(temp);
	
	return sOut;
}

string SDES_Dec(string sCipher, string sKeys)
{
	string temp="", sIP="", sOut="";
	
	sIP=IP(sCipher);
	
	temp=fk(sIP,sKeys.substr(8,8));
	
	temp = SW(temp,temp.size());
	
	temp=fk(temp,sKeys.substr(0,8));
	
	sOut=IPx(temp);
	
	return sOut;
}

int main(){
	char c;
	int iValue;
	string x="",sAux="", sFinal="", sKeys="", sPlainText="",sCipher="", sMessage="";
	
	//x="1111111111";
	
	ifstream iFile;//input file
	ofstream oEnc,oDec;//output files
	
	//input two characters that will be key for generating K1 and K2
	while(sAux.size()!=2)
	{
		cout<<"Input key (two characters from keyboard): ";
		getline(cin,sAux);
	}
	
	x= toBinary((int)sAux[0]) + toBinary((int)sAux[1]);//transform characters to binary
	
	x = x.substr(4,4) + x.substr(8,4);//Use last 4 bits from 1st character and 1st 4 bits of 2nd character as key
	
	sKeys=keyGeneration(x);//generate keys K1 and K2
	
	iFile.open("SDESTest.txt");//open input file
	oEnc.open("SDESEncrypted.txt");//open encrypted output file
	oDec.open("SDESDecrypted.txt");//open decrypted output file
	
	while(!iFile.eof())
	{
		//iFile >> sMessage;
		getline(iFile,sMessage);//get line from input file
		
		for(int i=0;i<sMessage.size();i++)
		{
			c=sMessage[i];//get character
			iValue=c;//integer value of character
			sPlainText = toBinary(iValue);//character to binary
			
			//ENCRYPTION
			sCipher=SDES_Enc(sPlainText,sKeys);
			oEnc << toString(sCipher);//output character to encrypted file
			//DECRYPTION
			sFinal=SDES_Dec(sCipher,sKeys);
			oDec << toString(sFinal);//output character to decrypted file
		}
		if(!iFile.eof())
		{		
			oEnc << endl;
			oDec << endl;
		}
	}
	
	//close all files
	iFile.close();
	oEnc.close();
	oDec.close();

	cout<<endl<<"~~DONE~~"<<endl;
	
	system("PAUSE");
	return 0;
}

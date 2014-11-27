#include "stdafx.h"
#include "UDPSockets.h"

SocketPack::SocketPack(int size){
		sendBytes = new Byte[size];
}

UDPSockets::UDPSockets(void){
	sendSocket = new Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	rcvSocket = new Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
}

IPAddress * UDPSockets::GetMyIP(){
	String __gc * strHostName = Dns::GetHostName();
	IPHostEntry __gc * hostEntry = Dns::GetHostByName(strHostName);
	IEnumerator __gc * myEnum = hostEntry->AddressList->GetEnumerator();
	IPAddress * myIP = 0;
	while (myEnum->MoveNext()) 
	{
		myIP = __try_cast<IPAddress*>(myEnum->Current);
		return myIP;
	}
	return myIP;
}

bool UDPSockets::ConnectTo(String * serverIP, int sndPort, int rcvPort){
	IPEndPoint __gc * sendIPEP = new IPEndPoint(IPAddress::Parse(serverIP), sndPort);
	IPEndPoint __gc * rcvIPEP = new IPEndPoint(this->GetMyIP(), rcvPort);
	
	sendEP = sendIPEP;
	rcvEP = rcvIPEP;

	rcvSocket->Bind(rcvEP);
	return true;
}

void UDPSockets::SendMsg(String * message){
	Byte sendBytes __gc[] = Encoding::ASCII->GetBytes(message);
	sendSocket->SendTo(sendBytes, sendEP);
}

SocketPack * UDPSockets::RecieveMsg(int dataBufferSize){
	Byte tempBuffer __gc[] = new Byte[dataBufferSize];
	int iRx = rcvSocket->ReceiveFrom(tempBuffer, &rcvEP);
	SocketPack * resultPack = new SocketPack(iRx);
	resultPack->socket = this->rcvSocket;
	if(iRx>0){
		for(int counter=0; counter < iRx; counter++)
		{
			resultPack->sendBytes[counter] = tempBuffer[counter];
		}
	}
	else{
		
	}
	return resultPack;
}

/*
void UDPSockets::WaitForData(){
	if  ( this->fncCallBack == 0 ) 
	{
		fncCallBack	= new AsyncCallback(this, &UDPSockets::WaitForDataCallback);
	}
	SocketPack * socPak = new SocketPack(1024);
	socPak->socket = this->rcvSocket;
	IAsyncResult __gc * dataResult = rcvSocket->BeginReceiveFrom(socPak->sendBytes,
		0,
		socPak->sendBytes->get_Length(),
		SocketFlags::None,
		&this->rcvEP,
		new System::AsyncCallback(&UDPSockets::WaitForDataCallback, WaitForDataCallback ),
		socPak);
}
*/

void UDPSockets::WaitForDataCallback(IAsyncResult * async){
	socPakResult = __try_cast<SocketPack*>(async->AsyncState);
	int iRx = socPakResult->socket->EndReceiveFrom(async, &rcvEP);
	if(iRx>0){
		SocketPack * callBackPack = new SocketPack(iRx);	//repack the data that arrived into a new SocketPack
		callBackPack->socket = socPakResult->socket;
		for(int counter=0; counter < iRx; counter++)
		{
			callBackPack->sendBytes[counter] = socPakResult->sendBytes[counter];
		}
		OnDataReceived(callBackPack);
	}
}

void UDPSockets::Close(){
	sendSocket->Close();
	rcvSocket->Close();
}
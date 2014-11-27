// UDPSockets.h
#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Collections;

public __gc class SocketPack
{
public:
	Byte sendBytes __gc[];
	Socket * socket;
	SocketPack(int size);
};


public __gc class UDPSockets
{
//Properties
private: Socket __gc * sendSocket;
private: Socket __gc * rcvSocket;
private: EndPoint __gc * sendEP;
private: EndPoint __gc * rcvEP;
private: AsyncCallback * fncCallBack ;
private: SocketPack __gc * socPakResult;

public: __delegate void DataReceivedEventHandler(System::Object *  sender, SocketPack * socData);
public: __event DataReceivedEventHandler * DataReceived;
protected: virtual void OnDataReceived(SocketPack * socData)
	{
		DataReceived(this, socData);
	}

//Methods
public: UDPSockets();
public: IPAddress * GetMyIP();
public: bool ConnectTo(String * serverIP, int sndPort, int rcvPort);
public: void SendMsg(String * message);
public: SocketPack * RecieveMsg(int dataBufferSize);
private: void WaitForDataCallback(IAsyncResult * async);
public: void Close();
};
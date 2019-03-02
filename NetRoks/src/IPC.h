#pragma once
#include <string>
#include <unistd.h>
#include <string.h>
#include "Erroring.h"
#include "utils.h"

enum IPCCommand
{
	IPC_ERROR = 0,
	IPC_CONTINUE,
	IPC_ABORT,
	IPC_IS_BROKEN = 10,
	IPC_GET_MODE,
	IPC_GET_SIZE,
	IPC_FILE_DELETE,
	IPC_DIRECTORY_DELETE,
	IPC_RENAME,
	IPC_DIRECTORY_CREATE,
	IPC_DIRECTORY_ENUM,
	IPC_FILE_GET,
	IPC_FILE_PUT
};

class IPCSender
{
	int _fd;

protected:
	int SetFD(int fd);

public:
	IPCSender(int fd = -1);
	~IPCSender();

	void Send(const void *data, size_t len) throw(IPCError);
	void SendString(const char *s) throw(IPCError);
	void SendString(const std::string &s) throw(IPCError);
	template <class POD_T>
		inline void SendPOD(const POD_T &pod) throw(IPCError)
	{
		Send(&pod, sizeof(pod));
	}
	inline void SendCommand(IPCCommand cmd) throw(IPCError)
	{
		SendPOD(cmd);
	}
};

class IPCRecver
{
	int _fd;

protected:
	int SetFD(int fd);

public:
	IPCRecver(int fd = -1);
	~IPCRecver();

	void Recv(void *data, size_t len) throw(IPCError);
	void RecvString(std::string &s) throw(IPCError);

	template <class POD_T>
		inline void RecvPOD(POD_T &pod) throw(IPCError)
	{
		Recv(&pod, sizeof(pod));
	}

	inline IPCCommand RecvCommand() throw(IPCError)
	{
		IPCCommand out;
		RecvPOD(out);
		return out;
	}
};

class IPCEndpoint : public IPCRecver, public IPCSender
{
	public:
	IPCEndpoint(int fd_recv, int fd_send);
};
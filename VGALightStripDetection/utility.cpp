
#include <Windows.h>
#include <tchar.h>
#include "utility.h"
#include <fstream>
#include <io.h>
#include <direct.h>
#include "PreDefine.h"
#include "SpdMultipleSinks.h"

int getVGAInfo(char* ppid, size_t size)
{
	DWORD exitCode = 0;
	TCHAR cmd[MAX_PATH] = { 0 };
	TCHAR args[MAX_PATH] = { 0 };
	TCHAR directory[MAX_PATH] = { 0 };
	SHELLEXECUTEINFO ShExecInfo = { 0 };

	_tremove(L".\\GetVGAINFO\\PPID.log");

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.lpDirectory = NULL;
#if _DEBUG
	ShExecInfo.nShow = SW_SHOW;
#else 
	ShExecInfo.nShow = SW_HIDE;
#endif
	ShExecInfo.hInstApp = NULL;

	_stprintf_s(args, MAX_PATH, L"ppid");
	ShExecInfo.lpParameters = args;

	_stprintf_s(cmd, MAX_PATH, L".\\GetVGAINFO\\GetVGAINFO.exe");
	ShExecInfo.lpFile = cmd;

	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	GetExitCodeProcess(ShExecInfo.hProcess, &exitCode);

	// 等8秒
	//_tsystem(L"choice /t 8 /d y /n > nul");

	//unsigned int ppid = 0;
	std::fstream file(".\\GetVGAINFO\\PPID.log", std::fstream::in);
	if (file.good())
	{
		//char buf[MAXCHAR] = { 0 };
		file.getline(ppid, size);
		//sscanf_s(buf, "%u", &ppid);		
	}
	file.close();
	SPDLOG_SINKS_DEBUG("================PPID:{}================", ppid);
	return exitCode;
}

typedef int(*lpLoadVenderDLL)();
typedef bool(*lpVGA_Read_IC_I2C)(UCHAR ucAddress, UCHAR reg_address, BYTE &rData, UINT iCardNumber, Ul32 ulDDCPort, UCHAR regSize, UCHAR DataSize, Ul32 flags);
typedef bool(*lpVGA_Write_IC_I2C)(UCHAR ucAddress, UCHAR reg_address, UCHAR *rData, UINT iCardNumber, Ul32 ulDDCPort, UCHAR regSize, UCHAR DataSize, Ul32 flags);

lpLoadVenderDLL  LOAD_VENDOR_DLL;
lpVGA_Read_IC_I2C    VGA_READ_IC_I2C;
lpVGA_Write_IC_I2C   VGA_WRITE_IC_I2C;

// LED 灯的地址
BYTE REG[22] = { 0x60, 0x63, 0x66, 0x69, 0x6c, 0x6f, 0x72, 0x75, 0x78, 0x7b, 0x7e
				, 0x81, 0x84, 0x87, 0x8a, 0x8d, 0x90, 0x93, 0x96, 0x99, 0x9c, 0x9f };

BYTE uOffset[12] = { 0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00,0xFF,0x00,0x00 };

void initVGA()
{
	HINSTANCE hDLL;		// Handle to DLL
	hDLL = LoadLibrary(L"VGA_Extra_x64.dll");
	LOAD_VENDOR_DLL = (lpLoadVenderDLL)GetProcAddress(hDLL, "LoadVenderDLL");
	VGA_READ_IC_I2C = (lpVGA_Read_IC_I2C)GetProcAddress(hDLL, "VGA_Read_IC_I2C");
	VGA_WRITE_IC_I2C = (lpVGA_Write_IC_I2C)GetProcAddress(hDLL, "VGA_Write_IC_I2C");
	SPDLOG_SINKS_DEBUG("LOAD_VENDOR_DLL:{}", LOAD_VENDOR_DLL == NULL? "NULL" : "NOT NULL");
	SPDLOG_SINKS_DEBUG("VGA_READ_IC_I2C:{}", VGA_READ_IC_I2C == NULL ? "NULL" : "NOT NULL");
	SPDLOG_SINKS_DEBUG("VGA_WRITE_IC_I2C:{}", VGA_WRITE_IC_I2C == NULL ? "NULL" : "NOT NULL");
	// 载入dll
	LOAD_VENDOR_DLL();
}

void setSignleColor(int led, BYTE r, BYTE g, BYTE b)
{
	bool result = false;
	//Set Start Address
	uOffset[0] = 0x81;
	uOffset[1] = REG[led];
	result = VGA_WRITE_IC_I2C(0xCE, 0x0, (BYTE*)uOffset, 0, 1, 1, 2, 1);	//set address	
	result == false ? SPDLOG_SINKS_ERROR("VGA_WRITE_IC_I2C_01:{}", result) : "";

	uOffset[0] = 3;	//rgb size
	uOffset[1] = r;
	uOffset[2] = b;
	uOffset[3] = g;
	//(UCHAR ucAddress, UCHAR reg_address, UCHAR *rData, UINT iCardNumber, Ul32 ulDDCPort, UCHAR regSize, UCHAR DataSize, Ul32 flags)
	result = VGA_WRITE_IC_I2C((BYTE)0xCE, (BYTE)0x03, (BYTE*)uOffset, 0, 1, 1, 4, 1);
	result == false ? SPDLOG_SINKS_ERROR("VGA_WRITE_IC_I2C_02:{}", result) : "";

	uOffset[0] = 0x80;
	uOffset[1] = 0x21;
	result = VGA_WRITE_IC_I2C(0xCE, 0x0, (BYTE*)uOffset, 0, 1, 1, 2, 1);	//set address
	result == false ? SPDLOG_SINKS_ERROR("VGA_WRITE_IC_I2C_03:{}", result) : "";

	uOffset[0] = 0x01;
	result = VGA_WRITE_IC_I2C(0xCE, 0x1, (BYTE*)uOffset, 0, 1, 1, 1, 1);	//write data
	result == false ? SPDLOG_SINKS_ERROR("VGA_WRITE_IC_I2C_04:{}", result) : "";

	uOffset[0] = 0x80;
	uOffset[1] = 0x2F;
	result = VGA_WRITE_IC_I2C(0xCE, 0x0, (BYTE*)uOffset, 0, 1, 1, 2, 1);	//set address
	result == false ? SPDLOG_SINKS_ERROR("VGA_WRITE_IC_I2C_05:{}", result) : "";

	uOffset[0] = 0x01;
	result = VGA_WRITE_IC_I2C(0xCE, 0x01, (BYTE*)uOffset, 0, 1, 1, 1, 1);	//write data
	result == false ? SPDLOG_SINKS_ERROR("VGA_WRITE_IC_I2C_06:{}", result) : "";
}

void resetColor(int count, BYTE r, BYTE g, BYTE b)
{
	for (int i = 0; i < count; i++)
	{
		setSignleColor(i, r, g, b);
	}
}

void createPPIDFolder(const char* ppid)
{
	int ret = 0;
	if (0 != _access(AgingFolder, 0))
	{
		ret = _mkdir(AgingFolder);   // 返回 0 表示创建成功，-1 表示失败	
		if (ret != 0)
		{
			printf("\ncreate %s dir fail-%ld\n", AgingFolder, GetLastError());
		}
	}
	char path[MAX_PATH] = { 0 };
	sprintf_s(path, MAX_PATH, "%s/%s", AgingFolder, ppid);
	ret = _mkdir(path);
	if (ret != 0)
	{
		printf("\ncreate %s dir fail-%ld\n", path, GetLastError());

	}
}

void createFakePPID(char* ppid, unsigned int len)
{
	GUID guid;
	CoCreateGuid(&guid);

	snprintf(ppid, len, "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X", 
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return;
}
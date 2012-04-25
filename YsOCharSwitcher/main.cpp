#include <stdio.h>
#include <Windows.h>

int main()
{
	HWND hWndYsO = FindWindow( "falcom", 0 );
	if( !hWndYsO )
	{
		::MessageBox( 0, "请确认您已经开启了YSO！",0,0 );
		return 0;
	}
	DWORD pidYsO = 0;
	GetWindowThreadProcessId( hWndYsO, &pidYsO ); // get yso pid

	if( !pidYsO )
	{
		::MessageBox( 0, "获取PID失败", 0, 0 );
	}

	HANDLE hRemoteProcess = OpenProcess( PROCESS_ALL_ACCESS, false, pidYsO );

	int pt = 0x0065b3cc;
	int*** pBase = ( int*** )pt;

	int address2 = 0;		//*(pBase+1*4)

	::ReadProcessMemory( hRemoteProcess, (void*)(pBase+0*4), (void*)&address2, 4, 0 );// read the memory of pBase+4

	int address3 = 0;		//*(*(pBase+1*4)) + 0x97C;

	::ReadProcessMemory( hRemoteProcess, (void*)address2,  (void*)&address3, 4, 0 );

	address3 = address3 + 0x97C;

	int npcNum = 0;		// npc Num;

	::ReadProcessMemory( hRemoteProcess, (void*)address3, (void*)&npcNum, 4, 0 );

	if( !npcNum )
	{
		::MessageBox( 0, "读取失败~", 0, 0 );
	}else{
		char numString[100];
		sprintf( numString, "当前操纵的猪脚编号为%d", npcNum );
		::MessageBox( 0, numString, 0, 0 );
	}

	int characterToExchange = 0;
	printf("请输入要替换的NPC编号");
	scanf( "%d", &characterToExchange );

	while(true)
	{
		if( !WriteProcessMemory( hRemoteProcess, (void*)address3, (void*)&characterToExchange, 4, 0 ) )
		{
			::MessageBox( 0, "写入错误", 0, 0 );
			break;
		}
	}
}
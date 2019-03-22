/*
Copyright (C) 2015 Felipe Izzo
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "quakedef.h"
#include "errno.h"

#include <stdint.h>

int __stacksize__ = 256 * 1024;


#define TICKS_PER_SEC 268123480.0

qboolean		isDedicated;

uint64_t initialTime = 0;
int hostInitialized = 0;

char gameFolder[100];

//static aptHookCookie sysAptCookie;

/*
===============================================================================

FILE IO

===============================================================================
*/

#define MAX_HANDLES             10
FILE    *sys_handles[MAX_HANDLES];

int             findhandle (void)
{
	int             i;

	for (i=1 ; i<MAX_HANDLES ; i++)
		if (!sys_handles[i])
			return i;
	Sys_Error ("out of handles");
	return -1;
}

/*
================
filelength
================
*/
int filelength (FILE *f)
{
	int             pos;
	int             end;

	pos = ftell (f);
	fseek (f, 0, SEEK_END);
	end = ftell (f);
	fseek (f, pos, SEEK_SET);

	return end;
}

int Sys_FileOpenRead (char *path, int *hndl)
{
	FILE    *f;
	int             i;

	i = findhandle ();

	f = fopen(path, "rb");
	if (!f)
	{
		*hndl = -1;
		return -1;
	}
	sys_handles[i] = f;
	*hndl = i;

	return filelength(f);
}

int Sys_FileOpenWrite (char *path)
{
	FILE    *f;
	int             i;

	i = findhandle ();

	f = fopen(path, "wb");
	if (!f)
		Sys_Error ("Error opening %s: %s", path,strerror(errno));
	sys_handles[i] = f;

	return i;
}

void Sys_FileClose (int handle)
{
	fclose (sys_handles[handle]);
	sys_handles[handle] = NULL;
}

void Sys_FileSeek (int handle, int position)
{
	fseek (sys_handles[handle], position, SEEK_SET);
}

int Sys_FileRead (int handle, void *dest, int count)
{
	return fread (dest, 1, count, sys_handles[handle]);
}

int Sys_FileWrite (int handle, void *data, int count)
{
	return fwrite (data, 1, count, sys_handles[handle]);
}

int     Sys_FileTime (char *path)
{
	FILE    *f;

	f = fopen(path, "rb");
	if (f)
	{
		fclose(f);
		return 1;
	}

	return -1;
}

void Sys_mkdir (char *path)
{
}


/*
===============================================================================

SYSTEM IO

===============================================================================
*/

void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length)
{
}


void Sys_Error (char *error, ...)
{
	va_list         argptr;

	printf ("Sys_Error: ");
	va_start (argptr,error);
	vprintf (error,argptr);
	va_end (argptr);
	printf ("\n");
	while(1){
	}
	if(hostInitialized)
		Sys_Quit();

	else
	{
		exit (0);
	}
}

void Sys_Printf (char *fmt, ...)
{
	if(hostInitialized)
		return;

	va_list         argptr;

	va_start (argptr,fmt);
	vprintf (fmt,argptr);
	va_end (argptr);
}

void Sys_Quit (void)
{
	exit (0);
}

#include <sys/time.h>
#if defined(PC)
uint64_t get_time(void){
  struct timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

#endif
uint64_t get_time(void);

double Sys_FloatTime (void)
{
	if(!initialTime){
		initialTime = get_time();
	}
	uint64_t curTime = get_time();
	return (curTime - initialTime)/1000000.;
}

char *Sys_ConsoleInput (void)
{
	return NULL;
}

void Sys_Sleep (void)
{
}

void CTR_SetKeys(uint32_t keys, uint32_t state){
  //todo
	/* if( keys & KEY_SELECT) */
	/* 	Key_Event(K_ESCAPE, state); */
	/* if( keys & KEY_START) */
	/* 	Key_Event(K_ENTER, state); */
	/* if( keys & KEY_DUP) */
	/* 	Key_Event(K_UPARROW, state); */
	/* if( keys & KEY_DDOWN) */
	/* 	Key_Event(K_DOWNARROW, state); */
	/* if( keys & KEY_DLEFT) */
	/* 	Key_Event(K_LEFTARROW, state); */
	/* if( keys & KEY_DRIGHT) */
	/* 	Key_Event(K_RIGHTARROW, state); */
	/* if( keys & KEY_Y) */
	/* 	Key_Event(K_AUX4, state); */
	/* if( keys & KEY_X) */
	/* 	Key_Event(K_AUX3, state); */
	/* if( keys & KEY_B) */
	/* 	Key_Event(K_AUX2, state); */
	/* if( keys & KEY_A) */
	/* 	Key_Event(K_AUX1, state); */
	/* if( keys & KEY_L) */
	/* 	Key_Event(K_AUX5, state); */
	/* if( keys & KEY_R) */
	/* 	Key_Event(K_AUX7, state); */
	/* if( keys & KEY_ZL) */
	/* 	Key_Event(K_AUX6, state); */
	/* if( keys & KEY_ZR) */
	/* 	Key_Event(K_AUX8, state); */
}


void Sys_SendKeyEvents (void)
{
	/* hidScanInput(); */
	
	/* u32 kDown = hidKeysDown(); */

	/* u32 kUp = hidKeysUp(); */

	/* if(kDown) */
	/* 	CTR_SetKeys(kDown, true); */
	/* if(kUp) */
	/* 	CTR_SetKeys(kUp, false); */

	/* Touch_Update(); */
}

void Sys_HighFPPrecision (void)
{
}

void Sys_LowFPPrecision (void)
{
}

//=============================================================================


void Sys_DefaultConfig(void)
{
	Cbuf_AddText ("bind ABUTTON +right\n");
	Cbuf_AddText ("bind BBUTTON +lookdown\n");
	Cbuf_AddText ("bind XBUTTON +lookup\n");
	Cbuf_AddText ("bind YBUTTON +left\n");
	Cbuf_AddText ("bind LTRIGGER +jump\n");
	Cbuf_AddText ("bind RTRIGGER +attack\n");
	Cbuf_AddText ("bind PADUP \"impulse 10\"\n");
	Cbuf_AddText ("bind PADDOWN \"impulse 12\"\n");
	Cbuf_AddText ("lookstrafe \"1.000000\"\n");
	Cbuf_AddText ("lookspring \"0.000000\"\n");
	Cbuf_AddText ("gamma \"0.700000\"\n");

}

void Sys_Init(void)
{
}

#if !defined(PC)
int quake_main (int argc, char **argv)
#else
int main (int argc, char **argv)
#endif
{
	float		time, oldtime;

	static quakeparms_t    parms;

	printf("quake will begin\n");

	parms.memsize = 5.5*1024*1024;/*at minimum requirement, thus this is needed ai ram memory :( */
	parms.membase = calloc (1,parms.memsize);
	if(parms.membase==NULL)printf("allocation failed\n");
	parms.basedir = ".";
	printf("allocated\n");

	COM_InitArgv (argc, argv);

	parms.argc = com_argc;
	parms.argv = com_argv;
	Host_Init (&parms);
	printf("host init end\n");
	Sys_Init();

	oldtime = Sys_FloatTime() -0.1;
	while (1)
	{
		time = Sys_FloatTime();
		Host_Frame (time - oldtime);
		oldtime = time;
	}
	return 0;
}

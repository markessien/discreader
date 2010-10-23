/*
 * Extremely simple (and small) example of using AKRip32.dll in a command-
 * line application.  This sample:
 *   1. Finds and gets a handle to the first CD unit
 *   2. Reads the TOC
 *   3. Extracts the first track into a WAV file, "track1.wav"
 */

#include <windows.h>
#include <stdio.h>
#include "akrip32.h"


/*
 * WAV file header format
 */
typedef struct
{
  BYTE  riff[4];            /* must be "RIFF"                */
  DWORD len;                /* #bytes + 44 - 8               */
  BYTE  cWavFmt[8];         /* must be "WAVEfmt"             */
  DWORD dwHdrLen;
  WORD  wFormat;
  WORD  wNumChannels;
  DWORD dwSampleRate;
  DWORD dwBytesPerSec;
  WORD  wBlockAlign;
  WORD  wBitsPerSample;
  BYTE  cData[4];            /* must be "data"               */
  DWORD dwDataLen;           /* #bytes                       */
} PACKED WAVHDR, *PWAVHDR, *LPWAVHDR;


HCDROM GetFirstCDHandle( void );
void MSB2DWORD( DWORD *d, BYTE *b );
void RipAudio( HCDROM hCD, DWORD dwStart, DWORD dwLen );
LPTRACKBUF NewTrackBuf( DWORD numFrames );
void writeWavHeader( FILE *fp, DWORD len );


int main( void )
{
  HCDROM hCD;
  TOC toc;
  DWORD dwStart, dwLen;

  hCD = GetFirstCDHandle();
  if ( !hCD )
    {
      printf( "Could not get handle to CD\n" );
      return 0;
    }

  ZeroMemory( &toc, sizeof(TOC) );
  ModifyCDParms( hCD, CDP_MSF, FALSE );
  if ( ReadTOC( hCD, &toc ) != SS_COMP )
    {
      printf( "Error reading TOC\n" );
      return 0;
    }

  MSB2DWORD( &dwStart, toc.tracks[0].addr );
  MSB2DWORD( &dwLen, toc.tracks[1].addr );
  dwLen -= dwStart;

  RipAudio( hCD, dwStart, dwLen );

  CloseCDHandle( hCD );

  return 0;
}



HCDROM GetFirstCDHandle( void )
{
  CDLIST cdlist;
  GETCDHAND cdh;

  ZeroMemory( &cdlist, sizeof(cdlist) );
  GetCDList( &cdlist );

  if ( cdlist.num == 0 )
    return NULL;

  ZeroMemory( &cdh, sizeof(cdh) );
  cdh.size     = sizeof(GETCDHAND);
  cdh.ver      = 1;
  cdh.ha       = cdlist.cd[0].ha;
  cdh.tgt      = cdlist.cd[0].tgt;
  cdh.lun      = cdlist.cd[0].lun;
  cdh.readType  = CDR_ANY;      // set for autodetect

  return GetCDHandle( &cdh );
}


void MSB2DWORD( DWORD *d, BYTE *b )
{
  DWORD retVal;

  retVal = (DWORD)b[0];
  retVal = (retVal<<8) + (DWORD)b[1];
  retVal = (retVal<<8) + (DWORD)b[2];
  retVal = (retVal<<8) + (DWORD)b[3];

  *d = retVal;
}



void RipAudio( HCDROM hCD, DWORD dwStart, DWORD dwLen )
{
  DWORD dwStatus;
  FILE *fp;
  DWORD num2rip = 26;
  int retries;
  LPTRACKBUF t;
  int c = 0;
  DWORD numWritten = 0;

  t = NewTrackBuf( 26 );
  if ( !t )
    return;

  fp = fopen( "track1.wav", "w+b" );

  writeWavHeader( fp, 0 );

  printf( "Beginning to rip %d sectors starting at sector %d\n", dwLen, dwStart );

  while( dwLen )
    {
      if ( !( c++ % 5) )
	printf( "%d: %d\n", dwStart, dwLen );

      if ( dwLen < num2rip )
	num2rip = dwLen;

      retries = 3;
      dwStatus = SS_ERR;
      while ( retries-- && (dwStatus != SS_COMP) )
	{
	  t->numFrames = num2rip;
	  t->startOffset = 0;
	  t->len = 0;
	  t->startFrame = dwStart;
	  dwStatus = ReadCDAudioLBA( hCD, t );
	}
      if ( dwStatus == SS_COMP )
	{
	  fwrite( t->buf + t->startOffset, 1, t->len, fp );
	  numWritten += t->len;
	}
      else
	{
	  printf( "Error at %d (%d:%d)\n", dwStart, GetAspiLibError(), GetAspiLibAspiError() );
	}
      dwStart += num2rip;
      dwLen -= num2rip;
    }

  writeWavHeader( fp, numWritten );
  
  fclose( fp );
  GlobalFree( (HGLOBAL)t );
}


LPTRACKBUF NewTrackBuf( DWORD numFrames )
{
  LPTRACKBUF t;
  int numAlloc;

  numAlloc = (((int)numFrames)*2352) + TRACKBUFEXTRA;

  t = (LPTRACKBUF)GlobalAlloc( GPTR, numAlloc );

  if ( !t )
    return NULL;

  t->startFrame = 0;
  t->numFrames = 0;
  t->maxLen = numFrames * 2352;
  t->len = 0;
  t->status = 0;
  t->startOffset = 0;

  return t;
}


void writeWavHeader( FILE *fp, DWORD len )
{
  WAVHDR wav;

  if ( !fp )
    return;

  memcpy( wav.riff, "RIFF", 4 );
  wav.len = len + 44 - 8;
  memcpy( wav.cWavFmt, "WAVEfmt ", 8 );
  wav.dwHdrLen = 16;
  wav.wFormat = 1;
  wav.wNumChannels = 2;
  wav.dwSampleRate = 44100;
  wav.dwBytesPerSec = 44100*2*2;
  wav.wBlockAlign = 4;
  wav.wBitsPerSample = 16;
  memcpy( wav.cData, "data", 4 );
  wav.dwDataLen = len;
  fseek( fp, 0, SEEK_SET );

  fwrite( &wav, 1, sizeof(wav), fp );
}

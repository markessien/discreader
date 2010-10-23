// AKRip.cpp: implementation of the CAKRip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AKRip.h"
#include <dbt.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAKRip::CAKRip(CRipCallback * pCallback)
{
	m_pCallback = pCallback;
	m_bInCDDBQuery = FALSE;
	m_bUseProxy = FALSE;
	m_iProxyPort = 0;
	m_iCurrentQueryCDIndex = 0;
	m_lps = NULL;
	m_bAbort = FALSE;
	m_bInExtractThread = FALSE;

	// Create our notification window
	if (m_wndNotify.IsWindow() == FALSE)
	{
		m_wndNotify.SetDrainFunction(NotifyFunc, (long)this);
			
		RECT rect;
		rect.left = 0; rect.top = 0; rect.right = 5; rect.bottom = 5;
		m_wndNotify.Create(NULL, rect, _T("NotifyWnd"), WS_POPUP);
	}
	
	_ASSERT(m_wndNotify.IsWindow());

}

CAKRip::~CAKRip()
{
	if (m_wndNotify.IsWindow())
		m_wndNotify.DestroyWindow();
}

CDLIST CAKRip::GetCDListFromAK()
{
	CDLIST cd;
	memset( &cd, 0, sizeof(cd) );
    cd.max = MAXCDLIST;

    GetCDList( &cd );

	return cd;
}

void CAKRip::MSB2DWORD( DWORD *d, BYTE *b )
{
  DWORD retVal;

  retVal = (DWORD)b[0];
  retVal = (retVal<<8) + (DWORD)b[1];
  retVal = (retVal<<8) + (DWORD)b[2];
  retVal = (retVal<<8) + (DWORD)b[3];

  *d = retVal;
}

void CAKRip::RefreshTrackList(int nCDIndex)
{

	CDTRACK at;
	TOC toc;
	int i;
	DWORD dwStatus;
	
	// Get the Drive
  	CDLIST cdlist;
	GETCDHAND cdh;
	memset( &cdlist, 0, sizeof(cdlist) );
    cdlist.max = MAXCDLIST;

    GetCDList( &cdlist );
	
	if ( cdlist.num == 0 )
		return;

	ZeroMemory( &cdh, sizeof(cdh) );
	cdh.size     = sizeof(GETCDHAND);
	cdh.ver      = 1;
	cdh.ha       = cdlist.cd[nCDIndex].ha;
	cdh.tgt      = cdlist.cd[nCDIndex].tgt;
	cdh.lun      = cdlist.cd[nCDIndex].lun;
	cdh.readType  = CDR_ANY;      // set for autodetect

	HCDROM hCD = GetCDHandle( &cdh );
	_ASSERT(hCD);

	if (hCD == NULL)
		return;

	ZeroMemory( &toc, sizeof(TOC) );
	ModifyCDParms( hCD, CDP_MSF, (DWORD)FALSE );
	dwStatus = ReadTOC( hCD, &toc );

	if ( dwStatus != SS_COMP )
	{
		CloseCDHandle( hCD );
		return;
	}

	for( i = toc.firstTrack; i <= toc.lastTrack; i++ )
    {
		TOCTRACK *t;
		int idx;

		idx = i - toc.firstTrack;

		ZeroMemory( &at, sizeof(at) );
		wsprintf( at.name, "Audio Track %d", i );

		t = &(toc.tracks[idx]);
		MSB2DWORD( &(at.start), t->addr );
		if ( t->ADR & 0x04 )
		{
			// will need to fix the leadout detection code here
			at.bData = TRUE;
			lstrcpy( at.name, "Data Track" );
		}

		t = &(toc.tracks[idx+1]);
		MSB2DWORD( &(at.len), t->addr );
		at.len -= at.start;

		#define LEADOUT  (150*75)

		// if the next track is a data track, subtract 150 seconds of leadout
		if ( (t->ADR & 0x04) && (at.len > LEADOUT) )
			at.len -= LEADOUT;

		m_pCallback->CDTrackListed(at.name, at.len / 75); // i don't know why 75!
    }

	CloseCDHandle( hCD );
}

void CAKRip::DoCDDBQuery(int iCDIndex)
{
	m_bInCDDBQuery = TRUE;
	m_iCurrentQueryCDIndex = iCDIndex;

	DWORD dwThreadID;
	CreateThread( NULL, 0, 
				  (LPTHREAD_START_ROUTINE)DoCDDBQueryThread,
				  (LPVOID)this, 
				  0, &dwThreadID );
}

DWORD CAKRip::DoCDDBQueryThread( LPVOID lpParam )
{
	CAKRip* pThis = (CAKRip*) lpParam;

	CDDBQUERYITEM cdq[5];
	CDDBQUERY query;
	char linebuf[81];
	int i;
	BOOL bCDDB = FALSE;
	char *buf, *p;

	ZeroMemory( cdq, sizeof(cdq) );

	buf = (char *)GlobalAlloc( GPTR, 32000 );

#if 1
	CDDBSetOption( CDDB_OPT_USECDPLAYERINI, "", TRUE );
#endif
	
	CDDBSetOption( CDDB_OPT_SERVER, (char*)pThis->m_strCDDBServer.c_str(), 0 );
	CDDBSetOption( CDDB_OPT_CGI, (char*)pThis->m_strCDDBCGI.c_str(), 0 );
	CDDBSetOption( CDDB_OPT_USEPROXY, "", pThis->m_bUseProxy );

	if ( pThis->m_bUseProxy )
	{
		CDDBSetOption( CDDB_OPT_PROXY, (char*)pThis->m_strProxyAddr.c_str(), 0 );
		CDDBSetOption( CDDB_OPT_PROXYPORT, "", pThis->m_iProxyPort );
	}

	CDDBSetOption( CDDB_OPT_USER, "user@carillonbells.com", 0 );
	CDDBSetOption( CDDB_OPT_AGENT, "CarillonBellsRipper", 0 );
	
	pThis->m_strCDDBStatus = "Contacting CDDB Server...";
	pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 0);
	
	// Get the Drive
  	CDLIST cdlist;
	GETCDHAND cdh;
	memset( &cdlist, 0, sizeof(cdlist) );
    cdlist.max = MAXCDLIST;

    GetCDList( &cdlist );
	
	if ( cdlist.num == 0 )
		return E_FAIL;

	ZeroMemory( &cdh, sizeof(cdh) );
	cdh.size     = sizeof(GETCDHAND);
	cdh.ver      = 1;
	cdh.ha       = cdlist.cd[pThis->m_iCurrentQueryCDIndex].ha;
	cdh.tgt      = cdlist.cd[pThis->m_iCurrentQueryCDIndex].tgt;
	cdh.lun      = cdlist.cd[pThis->m_iCurrentQueryCDIndex].lun;
	cdh.readType  = CDR_ANY;      // set for autodetect

	HCDROM hCD = GetCDHandle( &cdh );
	_ASSERT(hCD);

	if (hCD == NULL)
		return E_FAIL;

	query.num = 5;
	query.q = cdq;
	bCDDB = CDDBQuery( hCD, &query ) == SS_COMP;

	if (bCDDB && (query.num == 0))
	{
		pThis->m_strCDDBStatus = "No match for this CD in CDDB database";
		pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 0);
	}

	bool bOK = false;
	if ( query.num )
    {
      // If we're in the middle of a rip operation, just use the first one
      // as a default.  Otherwise, pop up a dialog to let the user choose
      // which one to use
		bOK = true;
#if 0 // uncomment to pop up multiple item box.
		if ( query.num == 1 )
		{
#endif
			pThis->m_strCDDBStatus = "Retrieving CD info";
			pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 0);

			pThis->m_iCurrentQueryCDIndex = 0;

			bCDDB = (CDDBGetDiskInfo( &cdq[0], buf, 32000 ) == SS_COMP);
			if ( !bCDDB )
			{
				pThis->m_strCDDBStatus = "Error retrieving CDDB info for CD";
				pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 0);
			}
#if 0
		}
		else
		{
			pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 4); // notify of multiple items

			for (int i=0;i<query.num;i++)
			{
				pThis->m_cdqCurrent = cdq[i];
				pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 5); // send item text
			}
			
			pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 6); // inform of item send complete

			if ( pThis->m_iCurrentQueryCDIndex < 0 )
			{
				bCDDB = FALSE;
			}
			else
			{
				bCDDB = (CDDBGetDiskInfo( &cdq[pThis->m_iCurrentQueryCDIndex], buf, 32000 ) == SS_COMP);
				if ( !bCDDB )
				{
					pThis->m_strCDDBStatus = "Error retrieving CDDB info for CD";
					pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 0);
				}
			}
		}
		
#endif
    }

	if ( !bCDDB )
	{
		pThis->m_bInCDDBQuery = FALSE;
		GlobalFree( (HGLOBAL)buf ); 
		CloseCDHandle( hCD );
		return 0;
	}
	
	pThis->m_cdqCurrent = cdq[pThis->m_iCurrentQueryCDIndex];
	pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 1);
	
	if ( bOK )
	{	
		pThis->m_strCDDBStatus = "CDDB Query complete";
		pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 0);
	}

	p = buf;
	i = 0;
	while( p && *p )
	{
		GetLineFromBuf( linebuf, &p, 81 );
		if ( !strncmp( linebuf, "TTITLE", 6 ) )
		{
			char* p = strstr( linebuf, "=" );
			if (p && (*++p))
			{
				pThis->m_strCurrentCDDBName = p;
				pThis->m_iCurrentCDDBIndex = i;
				pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 2);
			}

			i++;
		}
		else if ( linebuf[0] == '.' )
			p = NULL;     // last line read
	}

	GlobalFree( (HGLOBAL)buf );  
	pThis->m_bInCDDBQuery = FALSE;
	
	CloseCDHandle( hCD );

	return 0;
}

char FirstDriveFromMask (ULONG unitmask)
{
  char i;

  for (i = 0; i < 26; ++i)
  {
     if (unitmask & 0x1)
        break;
     unitmask = unitmask >> 1;
  }

  return (i + 'A');
}

BOOL CAKRip::NotifyFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, long pClass)
{
	CAKRip* pThis = (CAKRip*)pClass;
	switch (uMsg)
	{
	case WM_EVENTFIRE:
		switch (wParam)
		{
		case 0:
			pThis->m_pCallback->CDDBStatusUpdate((char*)pThis->m_strCDDBStatus.c_str());
			break;
		case 1:
			pThis->m_pCallback->CDDBCDInfoRetrieved(pThis->m_cdqCurrent.artist, pThis->m_cdqCurrent.title, pThis->CDDBCatToID3Genre(pThis->m_cdqCurrent.categ));
			break;
		case 2:
			pThis->m_pCallback->CDDBTrackNameRetrieved(pThis->m_iCurrentCDDBIndex, (char*)pThis->m_strCurrentCDDBName.c_str());
			break;
		case 3: // serverlist retrieved
			pThis->FinishCDDBSitesQuery();
			break;
		case 4: // 
			pThis->m_pCallback->CDDBMultipleMatches();
			break;
		case 5:
			pThis->m_pCallback->CDDBMultipleMatchItem(pThis->m_cdqCurrent.title);
			break;
		case 6:
			pThis->m_pCallback->CDDBMultipleMatchNotifyComplete();
			break;
		case 7:
			pThis->m_pCallback->TrackExtractionComplete(pThis->m_iCurrentExtractIndex);
			break;
		}
		break;
	case WM_DEVICECHANGE:
		{
			PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;
			char szMsg[80];
			switch(wParam)
			{
			case DBT_DEVICEARRIVAL: // See if a CD-ROM or DVD was inserted into a drive.
				{
					 if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
					 {
						PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

						if (lpdbv -> dbcv_flags & DBTF_MEDIA)
						{
							wsprintf(szMsg, "%c:", FirstDriveFromMask(lpdbv ->dbcv_unitmask));
							pThis->m_pCallback->CDInserted(szMsg);
						}
					 }
				}
				break;
			 case DBT_DEVICEREMOVECOMPLETE:
				 {
					// See if a CD-ROM was removed from a drive.
					if (lpdb -> dbch_devicetype == DBT_DEVTYP_VOLUME)
					{
					   PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;

					   if (lpdbv -> dbcv_flags & DBTF_MEDIA)
					   {
						  wsprintf(szMsg, "%c:", FirstDriveFromMask(lpdbv ->dbcv_unitmask));
						  pThis->m_pCallback->CDRemoved(szMsg);
					   }
					}
				 }
				 break;
			} // switch 
		}
		break;

	}
	return FALSE;
}

int CAKRip::CDDBCatToID3Genre( char *sCategory )
{
	static char *szCat[] = 
	{
		"blues", "classical", "country",
		"data", "folk", "jazz", "misc",
		"newage", "reggae", "rock", 
		"soundtrack", NULL
	};

	static BYTE genre[12] =
	{ 
		0, 32, 2, 17, 53, 8, 17, 
		10, 16, 17, 24, 17 
	};

	int iGenre = 17;  // default to "rock"
	for( int i = 0; i < 11; i++ )
	{
		if ( lstrcmpi( sCategory, szCat[i] ) == 0)
		{
			iGenre = genre[i];
		}
	}

	return iGenre;
}

void CAKRip::GetLineFromBuf( char *tgt, char **src, int len )
{
  char *p;

  if ( !src || !*src || !tgt )
    return;

  p = *src;

  ZeroMemory( tgt, len );
  len--;

  while( *p && len )
    {
      if ( *p == '\n' )
        break;
      *tgt++ = *p;
      p++;
      len--;
    }

  // advance *src to end of last read string
  if ( *p == '\n' )
    p++;
  *src = p;

  len = lstrlen( tgt );
  if ( tgt[len-2] == '\r' )
    tgt[len-2] = '\0';
  if ( (tgt[len-1] == '\r') || (tgt[len-1] == '\n') )
    tgt[len-1] = '\0';
}

void CAKRip::SetCDDBOptions(char *pszServer, char *pszCGI, bool bUseProxy, char *pszProxyAddress, long nProxyPort)
{
	m_strCDDBServer = pszServer;
	m_strCDDBCGI = pszCGI;
	m_bUseProxy = bUseProxy;
	m_strProxyAddr = pszProxyAddress;
	m_iProxyPort = (int)nProxyPort;
}

void CAKRip::DoCDDBSitesQuery()
{
	DWORD dwThreadID;
	CreateThread(NULL, 0, 
				 (LPTHREAD_START_ROUTINE)DoCDDBSitesQueryThread,
				 (LPVOID)this,
				 0, &dwThreadID );
}


DWORD CAKRip::DoCDDBSitesQueryThread( LPVOID lpParam )
{
	CAKRip* pThis = (CAKRip*)lpParam;

  	pThis->m_lps = (LPCDDBSITELIST)GlobalAlloc( GPTR, sizeof(CDDBSITELIST) );
	pThis->m_lps->s = (LPCDDBSITE)GlobalAlloc( GPTR, sizeof(CDDBSITE) * 8 );
	pThis->m_lps->num = 8;

	CDDBSetOption( CDDB_OPT_SERVER, (char*)pThis->m_strCDDBServer.c_str(), 0 );
	CDDBSetOption( CDDB_OPT_CGI, (char*)pThis->m_strCDDBCGI.c_str(), 0 );
	CDDBSetOption( CDDB_OPT_USEPROXY, "", pThis->m_bUseProxy );

	if ( pThis->m_bUseProxy )
	{
		CDDBSetOption( CDDB_OPT_PROXY, (char*)pThis->m_strProxyAddr.c_str(), 0 );
		CDDBSetOption( CDDB_OPT_PROXYPORT, "", pThis->m_iProxyPort );
	}

	CDDBSetOption( CDDB_OPT_USER, "cddb@essienrd.com", 0 );
	CDDBSetOption( CDDB_OPT_AGENT, "EssienRD", 0 );
	CDDBSetOption( CDDB_OPT_HTTPPORT, "", 80 );
	
	CDDBGetServerList( pThis->m_lps );
	pThis->m_wndNotify.SendMessage(WM_EVENTFIRE, 3);

	return 0;
}


void CAKRip::FinishCDDBSitesQuery()
{
    // add the rest
    for( int i = 0; i < m_lps->num; i++ )
	{
		if (i == 8)
			break;

	  if ( m_lps->s[i].bHTTP &&
	       lstrcmp( m_lps->s[i].szServer, "www.cddb.com" ) &&
	       lstrcmp( m_lps->s[i].szServer, "www.freedb.org" ) )
	    {
	      m_pCallback->CDDBServerRetrieved(m_lps->s[i]);
	    }
	}

	GlobalFree( (HGLOBAL)m_lps->s );
	GlobalFree( (HGLOBAL)m_lps );
}

void CAKRip::SetCDDBItemFromMultiple(int iIndex)
{
	m_iCurrentCDDBIndex = iIndex;
}

void CAKRip::ExtractTrack(int iCDDrive, int iIndex, char *pstrTargetFile)
{
	m_bAbort = FALSE;
	m_bInExtractThread = TRUE;
	m_iCurrentExtractCDDrive = iCDDrive;
	m_iCurrentExtractIndex = iIndex;
	m_strCurrentExtractFile = pstrTargetFile;
	
	DWORD dwThreadID;
	CreateThread(NULL, 0, 
				 (LPTHREAD_START_ROUTINE)ExtractTrackThread,
				 (LPVOID)this,
				 0, &dwThreadID );
}

DWORD CAKRip::ExtractTrackThread( LPVOID lpParam )
{
	
	CAKRip* pThis = (CAKRip*) lpParam;
	pThis->PerformExtractTrack(pThis->m_iCurrentExtractCDDrive, pThis->m_iCurrentExtractIndex, (char*)pThis->m_strCurrentExtractFile.c_str());
	return 0;
}


void CAKRip::WriteWavHeader( FILE *fp, DWORD len )
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

void CAKRip::PerformExtractTrack(int iCDDrive, int iIndex, char *pstrTargetFile)
{
//	CDTRACK at;
	TOC toc;
//	int i;
	DWORD dwStatus;
	
	// Get the Drive
  	CDLIST cdlist;
	GETCDHAND cdh;
	memset( &cdlist, 0, sizeof(cdlist) );
    cdlist.max = MAXCDLIST;

    GetCDList( &cdlist );
	
	if ( cdlist.num == 0 )
	{
		m_bInExtractThread = FALSE;
		return;
	}

	ZeroMemory( &cdh, sizeof(cdh) );
	cdh.size     = sizeof(GETCDHAND);
	cdh.ver      = 1;
	cdh.ha       = cdlist.cd[iCDDrive].ha;
	cdh.tgt      = cdlist.cd[iCDDrive].tgt;
	cdh.lun      = cdlist.cd[iCDDrive].lun;
	cdh.readType  = CDR_ANY;      // set for autodetect

	HCDROM hCD = GetCDHandle( &cdh );
	_ASSERT(hCD);

	if (hCD == NULL)
	{
		m_bInExtractThread = FALSE;
		return;
	}

	ZeroMemory( &toc, sizeof(TOC) );
	ModifyCDParms( hCD, CDP_MSF, (DWORD)FALSE );
	dwStatus = ReadTOC( hCD, &toc );

	if ( dwStatus != SS_COMP )
	{
		CloseCDHandle( hCD );
		m_bInExtractThread = FALSE;
		return;
	}
	
	DWORD dwStart;
	DWORD dwLen;
	MSB2DWORD( &dwStart, toc.tracks[iIndex].addr );
	MSB2DWORD( &dwLen, toc.tracks[iIndex + 1].addr );
	dwLen -= dwStart;
	
	DWORD dwTrackStart = dwStart;
	m_dwCurrentExtractLen = dwLen;
	m_dwCurrentExtractProgress = 0;

//	DWORD dwStatus;
	FILE *fp;
	DWORD num2rip = 26;
	int retries;
	LPTRACKBUF t;
	int c = 0;
	DWORD numWritten = 0;

	t = NewTrackBuf( 26 );
	if ( !t )
	{
		CloseCDHandle( hCD );
		m_bInExtractThread = FALSE;
	    return;
	}
	
	fp = fopen( pstrTargetFile, "w+b" );
	_ASSERT(fp);

	if (fp == NULL)
		m_bAbort = TRUE;
	
	if (m_bAbort)
	{
		CloseCDHandle( hCD );
		m_bInExtractThread = FALSE;
		return;
	}

	WriteWavHeader( fp, 0 );

	printf( "Beginning to rip %d sectors starting at sector %d\n", dwLen, dwStart );

	while (dwLen)
    {
		if (m_bAbort)
			break;

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
	
		m_dwCurrentExtractProgress = dwStart - dwTrackStart;
	}

	WriteWavHeader( fp, numWritten );

	fclose( fp );
	GlobalFree( (HGLOBAL)t );
	CloseCDHandle( hCD );

	m_bInExtractThread = FALSE;
	m_wndNotify.SendMessage(WM_EVENTFIRE, 7);
}


LPTRACKBUF CAKRip::NewTrackBuf( DWORD numFrames )
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

void CAKRip::GetCurrentExtractInfo(DWORD * dwProgress, DWORD * dwLen)
{
	if (dwLen)
		*dwLen = m_dwCurrentExtractLen;

	if (dwProgress)
		*dwProgress =  m_dwCurrentExtractProgress;
}

void CAKRip::AbortExtraction()
{
	m_bAbort = TRUE;
	while (m_bInExtractThread)
		Sleep(100);
}

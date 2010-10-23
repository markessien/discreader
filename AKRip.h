// AKRip.h: interface for the CAKRip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AKRIP_H__BBAF4FAB_D2CB_4FA2_BD5B_71E0ECFBE852__INCLUDED_)
#define AFX_AKRIP_H__BBAF4FAB_D2CB_4FA2_BD5B_71E0ECFBE852__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "akrip\akrip32.h"
#include <string>
#include "notifywindow.h"

using namespace std;

class CRipCallback
{
public:
	virtual void CDTrackListed(char* pstrName, double Length) = 0;
	virtual void CDDBStatusUpdate(char* pstrName) = 0;
	virtual void CDDBCDInfoRetrieved(char* Artist, char* Title, long Genre) = 0;
	virtual void CDDBTrackNameRetrieved(long Index, char* Name) = 0;
	virtual void CDInserted(char* DriveLetter) = 0;
	virtual void CDRemoved(char* DriveLetter) = 0;
	virtual void CDDBServerRetrieved(CDDBSITE serverInfo) = 0;
	virtual void CDDBMultipleMatches() = 0;
	virtual void CDDBMultipleMatchItem(char* pstrName) = 0;
	virtual void CDDBMultipleMatchNotifyComplete() = 0;
	virtual void TrackExtractionComplete(long Index) = 0;
};

#define WM_EVENTFIRE WM_APP + 13

class CAKRip  
{
public:
	void AbortExtraction();
	void GetCurrentExtractInfo(DWORD * dwProgress, DWORD * dwLen);
	void ExtractTrack(int iCDDrive, int iIndex, char* pstrTargetFile);
	void SetCDDBItemFromMultiple(int iIndex);
	void SetCDDBOptions(char* pszServer, char* pszCGI, bool bUseProxy, char* pszProxyAddress, long nProxyPort);
	

	CAKRip(CRipCallback* pCallback);
	virtual ~CAKRip();


	// WAV file header format
	typedef struct tagWAVHDR
	{
	  BYTE  riff[4];            // must be "RIFF"
	  DWORD len;                // #bytes + 44 - 8
	  BYTE  cWavFmt[8];         // must be "WAVEfmt"
	  DWORD dwHdrLen;
	  WORD  wFormat;
	  WORD  wNumChannels;
	  DWORD dwSampleRate;
	  DWORD dwBytesPerSec;
	  WORD  wBlockAlign;
	  WORD  wBitsPerSample;
	  BYTE  cData[4];            // must be "data"
	  DWORD dwDataLen;           // #bytes
	} PACKED WAVHDR, *PWAVHDR, *LPWAVHDR;

	typedef struct tagCDTRACK
	{
	  char name[MAX_PATH+1];
	  char status[61];
	  DWORD start;
	  DWORD len;
	  BOOL  bData;
	  BOOL  bChecked;          // display the check mark?
	  BOOL  bComplete;         // is it a full track, or just a segment?
	} CDTRACK;

	void RefreshTrackList(int nCDIndex);
	CDLIST GetCDListFromAK();
	void DoCDDBQuery(int iCDIndex);
	void DoCDDBSitesQuery();

private:
	void MSB2DWORD( DWORD *d, BYTE *b );
	static DWORD DoCDDBQueryThread(LPVOID lpParam);
	static BOOL NotifyFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, long pClass);
	int CDDBCatToID3Genre( char *sCategory );
	static void GetLineFromBuf( char *tgt, char **src, int len );
	static DWORD DoCDDBSitesQueryThread( LPVOID lpParam );
	void FinishCDDBSitesQuery();
	
	static DWORD ExtractTrackThread( LPVOID lpParam );
	void PerformExtractTrack(int iCDDrive, int iIndex, char *pstrTargetFile);
	void WriteWavHeader( FILE *fp, DWORD len );
	LPTRACKBUF NewTrackBuf( DWORD numFrames );

private:
	BOOL m_bAbort;
	BOOL m_bInExtractThread;
	DWORD m_dwCurrentExtractLen;
	DWORD m_dwCurrentExtractProgress;
	int m_iCurrentExtractCDDrive;
	int m_iCurrentExtractIndex;
	string m_strCurrentExtractFile;
	LPCDDBSITELIST m_lps;
	int m_iCurrentCDDBQueryNum;
	string m_strCurrentCDDBName;
	int m_iCurrentCDDBIndex;
	CDDBQUERYITEM m_cdqCurrent;
	int m_iCurrentQueryCDIndex;
	CRipCallback * m_pCallback;
	BOOL m_bInCDDBQuery;
	string m_strCDDBServer;
	string m_strCDDBCGI;
	BOOL m_bUseProxy;
	int m_iProxyPort;
	string m_strProxyAddr;
	string m_strCDDBStatus;
	CNotifyWindow m_wndNotify;
};

#endif // !defined(AFX_AKRIP_H__BBAF4FAB_D2CB_4FA2_BD5B_71E0ECFBE852__INCLUDED_)

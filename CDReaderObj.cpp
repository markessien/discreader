// CDReaderObj.cpp : Implementation of CCDReaderObj

#include "stdafx.h"
#include "CDReader.h"
#include "CDReaderObj.h"

/////////////////////////////////////////////////////////////////////////////
// CCDReaderObj


STDMETHODIMP CCDReaderObj::ListCDDrives()
{

	CDLIST cdlist = m_akRip.GetCDListFromAK();
	
	for( int i = 0; i < cdlist.num; i++ )
	{
		CComBSTR strInfo = cdlist.cd[i].info.vendor;
		strInfo.Append(cdlist.cd[i].info.prodId);
		strInfo.Append(cdlist.cd[i].info.rev);
		strInfo.Append(cdlist.cd[i].info.vendSpec);
		
		Fire_CDDriveListed(strInfo);
	}

	return S_OK;
}

STDMETHODIMP CCDReaderObj::ListSongsOnCD(long Index)
{
	m_akRip.RefreshTrackList(Index);

	return S_OK;
}

STDMETHODIMP CCDReaderObj::GetCDDBNamesOfCD(long DriveIndex)
{
	m_akRip.DoCDDBQuery(DriveIndex);

	return S_OK;
}

STDMETHODIMP CCDReaderObj::CDDBServerInfo(BSTR Server, BSTR CGI, VARIANT_BOOL UseProxy, BSTR ProxyAddress, long ProxyPort)
{
	USES_CONVERSION;
	m_akRip.SetCDDBOptions(W2A(Server), W2A(CGI), UseProxy == VARIANT_TRUE, W2A(ProxyAddress), ProxyPort);

	return S_OK;
}

STDMETHODIMP CCDReaderObj::CDDBGetServerList(VARIANT_BOOL OnlyDefault)
{
	Fire_CDDBServerListed(L"cddb.cddb.com", L"/~cddb/cddb.cgi", 80);
	Fire_CDDBServerListed(L"freedb.freedb.org", L"/~cddb/cddb.cgi", 80);
	
	if (OnlyDefault == VARIANT_FALSE)
		m_akRip.DoCDDBSitesQuery();

	return S_OK;
}

STDMETHODIMP CCDReaderObj::SelectCDDBCDInfoFromMultiple(long Index)
{
	m_akRip.SetCDDBItemFromMultiple(Index);

	return S_OK;
}

STDMETHODIMP CCDReaderObj::get_CurrentlyExtractedTrack(long *pVal)
{
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CCDReaderObj::get_ExtractionProgress(double *pVal)
{
	DWORD dwProgress = 0;
	m_akRip.GetCurrentExtractInfo(&dwProgress, NULL);
	*pVal = (double)dwProgress / 75.0;

	return S_OK;
}

STDMETHODIMP CCDReaderObj::ExtractTrack(long CDIndex, long Index, BSTR TargetFilePath)
{
	USES_CONVERSION;
	m_akRip.ExtractTrack(CDIndex, Index, W2A(TargetFilePath));

	return S_OK;
}

STDMETHODIMP CCDReaderObj::AbortExtraction()
{
	try {
	m_akRip.AbortExtraction();
	} catch(...){
	}

	return S_OK;
}

STDMETHODIMP CCDReaderObj::get_ExtractedFileLength(double *pVal)
{
	DWORD dwLen = 0;
	m_akRip.GetCurrentExtractInfo(NULL, &dwLen);
	*pVal = (double)dwLen / 75.0;

	return S_OK;
}

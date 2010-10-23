// CDReaderObj.h : Declaration of the CCDReaderObj

#ifndef __CDREADEROBJ_H_
#define __CDREADEROBJ_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "akrip.h"
#include "CDReaderCP.h"

/////////////////////////////////////////////////////////////////////////////
// CCDReaderObj
class ATL_NO_VTABLE CCDReaderObj : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICDReaderObj, &IID_ICDReaderObj, &LIBID_CDREADERLib>,
	public CComControl<CCDReaderObj>,
	public IPersistStreamInitImpl<CCDReaderObj>,
	public IOleControlImpl<CCDReaderObj>,
	public IOleObjectImpl<CCDReaderObj>,
	public IOleInPlaceActiveObjectImpl<CCDReaderObj>,
	public IViewObjectExImpl<CCDReaderObj>,
	public IOleInPlaceObjectWindowlessImpl<CCDReaderObj>,
	public IConnectionPointContainerImpl<CCDReaderObj>,
	public IPersistStorageImpl<CCDReaderObj>,
	public ISpecifyPropertyPagesImpl<CCDReaderObj>,
	public IQuickActivateImpl<CCDReaderObj>,
	public IDataObjectImpl<CCDReaderObj>,
	public IProvideClassInfo2Impl<&CLSID_CDReader, &DIID__ICDReaderObjEvents, &LIBID_CDREADERLib>,
	public IPropertyNotifySinkCP<CCDReaderObj>,
	public CComCoClass<CCDReaderObj, &CLSID_CDReader>,
	public CProxy_ICDReaderObjEvents< CCDReaderObj >,
	public CRipCallback
{
public:
	CCDReaderObj() : m_akRip(this)
	{
		m_hIcon = LoadIcon(_Module.m_hInstResource, (LPCTSTR) IDI_CDREADER);
	}
	
	HRESULT FinalConstruct( )
	{
		SIZEL size = {32, 32};
		AtlPixelToHiMetric( &size, &m_sizeExtent );
		m_sizeNatural = m_sizeExtent;

		SIZEL szlPixels, szlMetric;
		szlPixels.cx = 32;
		szlPixels.cy = 32;

		AtlPixelToHiMetric(&szlPixels, &szlMetric);

		// IOleObjectImpl
		SetExtent(DVASPECT_CONTENT, &szlMetric);

		// update control sizing...
		m_rcPos.right= m_rcPos.left + 32;
		m_rcPos.bottom= m_rcPos.top + 32;

		m_bAutoSize = TRUE;

		return S_OK;
	}


DECLARE_REGISTRY_RESOURCEID(IDR_CDREADEROBJ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CCDReaderObj)
	COM_INTERFACE_ENTRY(ICDReaderObj)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()

BEGIN_PROP_MAP(CCDReaderObj)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CCDReaderObj)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__ICDReaderObjEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CCDReaderObj)
	CHAIN_MSG_MAP(CComControl<CCDReaderObj>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ICDReaderObj
public:
	STDMETHOD(get_ExtractedFileLength)(/*[out, retval]*/ double *pVal);
	STDMETHOD(AbortExtraction)();
	STDMETHOD(ExtractTrack)(long CDIndex, /*[in]*/ long Index, /*[in]*/ BSTR TargetFilePath);
	STDMETHOD(get_ExtractionProgress)(/*[out, retval]*/ double *pVal);
	STDMETHOD(get_CurrentlyExtractedTrack)(/*[out, retval]*/ long *pVal);
	STDMETHOD(SelectCDDBCDInfoFromMultiple)(/*[in]*/ long Index);
	STDMETHOD(CDDBGetServerList)(VARIANT_BOOL OnlyDefault);
	STDMETHOD(CDDBServerInfo)(/*[in]*/ BSTR Server, /*[in]*/ BSTR CGI, /*[in]*/ VARIANT_BOOL UseProxy, /*[in]*/ BSTR ProxyAddress, /*[in]*/ long ProxyPort);
	STDMETHOD(GetCDDBNamesOfCD)(/*[in]*/ long DriveIndex);
	STDMETHOD(ListSongsOnCD)(/*[in]*/ long Index);
	STDMETHOD(ListCDDrives)();

private:
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		DrawIcon(di.hdcDraw, rc.left, rc.top, m_hIcon);

		return S_OK;
	}
	
	void CDTrackListed(char* pstrName, double Length)
	{
		USES_CONVERSION;
		Fire_TrackListed(A2W(pstrName), Length);
	}

	void CDDBStatusUpdate(char* pstrName)
	{
		USES_CONVERSION;
		Fire_CDDBQueryStatusUpdate(A2W(pstrName));
	}
	
	void CDDBCDInfoRetrieved(char* Artist, char* Title, long Genre)
	{
		CComBSTR strArtist = Artist ? Artist : "";
		CComBSTR strTitle = Title ? Title : "";

		Fire_CDDBCDInfoRetrieved(strArtist, strTitle, Genre);
	}

	void CDDBTrackNameRetrieved(long Index, char* Name)
	{
		USES_CONVERSION;
		Fire_TrackNameUpdate(Index, A2W(Name));
	}

	void CDInserted(char* DriveLetter)
	{	
		USES_CONVERSION;
		Fire_CDInserted(A2W(DriveLetter));
	}

	void CDRemoved(char* DriveLetter)
	{
		USES_CONVERSION;
		Fire_CDRemoved(A2W(DriveLetter));
	}
	
	void CDDBServerRetrieved(CDDBSITE serverInfo)
	{
		USES_CONVERSION;
		Fire_CDDBServerListed(A2W(serverInfo.szServer), A2W(serverInfo.szCGI), serverInfo.iPort);
	}
	
	void CDDBMultipleMatches()
	{
		Fire_CDDBMultipleMatches();
	}

	void CDDBMultipleMatchItem(char* pstrName)
	{
		Fire_CDDBMultipleMatchItem(CComBSTR(pstrName));
	}

	void CDDBMultipleMatchNotifyComplete()
	{
		Fire_CDDBMultipleMatchNotifyComplete();
	}
	
	void TrackExtractionComplete(long Index)
	{
		Fire_TrackExtractionComplete(Index);
	}

private:
	CAKRip m_akRip;
	HICON	 m_hIcon;
};

#endif //__CDREADEROBJ_H_

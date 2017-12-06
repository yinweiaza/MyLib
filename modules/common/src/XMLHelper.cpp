#include "StdAfx.h"
#include ".\XMLHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

CXMLHelper::CXMLHelper(void)
{
	m_pByte	   = NULL;
	m_pIXMLDoc = NULL;
}

CXMLHelper::~CXMLHelper(void)
{
	if (m_pIXMLDoc != NULL)
		Close();
}

HRESULT CXMLHelper::Load(LPCTSTR lpFileName, IXMLDOMElement** ppIXMLRoot)
{
	if (m_pIXMLDoc != NULL)
		return E_FAIL;

	if (lpFileName == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;
	hr = ::CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
       IID_IXMLDOMDocument, reinterpret_cast<void**>(&m_pIXMLDoc));
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->put_async(VARIANT_FALSE);
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->put_preserveWhiteSpace(VARIANT_TRUE);	//½âÎöÊ±±£Áô¿Õ°×
	FAILED_RETURN_hr;


	VARIANT_BOOL varbResult = VARIANT_FALSE;
	hr = m_pIXMLDoc->load(CComVariant(lpFileName), &varbResult);
	if (hr != S_OK)
		return hr;

	if (ppIXMLRoot != NULL)
	{
		hr = m_pIXMLDoc->get_documentElement(ppIXMLRoot);
		if (hr != S_OK)
			return hr;
	}

	return S_OK;
}

HRESULT CXMLHelper::Save(LPCTSTR lpFileName)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (lpFileName == NULL)
		return E_POINTER;

	return m_pIXMLDoc->save(CComVariant(lpFileName));
}

HRESULT CXMLHelper::LoadXMLString(LPCTSTR lpXMLIn, IXMLDOMElement** ppIXMLRoot)
{
	if (m_pIXMLDoc != NULL)
		return S_FALSE;

	if (lpXMLIn == NULL)
		return E_POINTER;

	HRESULT hr = S_OK;
	hr = ::CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
       IID_IXMLDOMDocument, reinterpret_cast<void**>(&m_pIXMLDoc));
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->put_async(VARIANT_FALSE);
	if (hr != S_OK)
		return hr;

	VARIANT_BOOL varbResult = VARIANT_FALSE;
	hr = m_pIXMLDoc->loadXML(CComBSTR(lpXMLIn), &varbResult);
	if (hr != S_OK)
		return hr;

	if (ppIXMLRoot != NULL)
	{
        hr = m_pIXMLDoc->get_documentElement(ppIXMLRoot);
		if (hr != S_OK)
			return hr;
	}

	return S_OK;
}

HRESULT CXMLHelper::CreateXmlDom(	LPCTSTR lpProcessingInstruction,
									LPCTSTR lpRootName,
									IXMLDOMElement** ppIXMLRoot)
{
	if (lpRootName == NULL)
		return E_POINTER;

	CString strProcessingInstruction;
	if(lpProcessingInstruction)
		strProcessingInstruction = lpProcessingInstruction;
	else
		strProcessingInstruction = _T("version=\"1.0\" encoding=\"utf-8\"");

	HRESULT hr = S_OK;
	hr = ::CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, 
							IID_IXMLDOMDocument, reinterpret_cast<void**>(&m_pIXMLDoc));
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->put_async(VARIANT_FALSE);
	if (hr != S_OK)
		return hr;

	VARIANT_BOOL varbResult = VARIANT_FALSE;
	CComPtr<IXMLDOMProcessingInstruction> pInstruction;
	hr = m_pIXMLDoc->createProcessingInstruction(L"xml", CComBSTR(strProcessingInstruction), &pInstruction);
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->appendChild(pInstruction, NULL);
	if (hr != S_OK)
		return hr;

	CComPtr<IXMLDOMNode> pAddNode;  
	hr = m_pIXMLDoc->createElement(CComBSTR(lpRootName), ppIXMLRoot);
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->appendChild(*ppIXMLRoot, NULL);
	if (hr != S_OK)
		return hr;

	return hr;
}

HRESULT CXMLHelper::GetXMLString(CString& strXMLOut)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	CComBSTR bstrXMLOut;
	HRESULT hr = m_pIXMLDoc->get_xml(&bstrXMLOut);
	if (hr != S_OK)
		return hr;

	COLE2T lpXMLOutT(bstrXMLOut);
	strXMLOut = lpXMLOutT;
	return S_OK;
}

HRESULT CXMLHelper::Close()
{
	if (m_pIXMLDoc == NULL)
		return S_FALSE;

	m_pIXMLDoc.Release();
	m_pIXMLDoc = NULL;

	return S_OK;
}

HRESULT CXMLHelper::GetXMLRoot(IXMLDOMElement** ppIXMLRoot)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (ppIXMLRoot == NULL)
		return E_POINTER;

	HRESULT hr = m_pIXMLDoc->get_documentElement(ppIXMLRoot);
	if (hr != S_OK)
		return hr;

	return S_OK;
}

HRESULT CXMLHelper::GetChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, IXMLDOMNode** ppIChildNode)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pIParentNode == NULL || lpNodeName == NULL || ppIChildNode == NULL)
		return E_POINTER;

	HRESULT hr;
	hr = pIParentNode->selectSingleNode(CComBSTR(lpNodeName), ppIChildNode);
	if (hr != S_OK)
		return hr;

	return S_OK;
}

HRESULT CXMLHelper::GetChildNodeList(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, IXMLDOMNodeList** ppIChildNodeList, long* plListItemCount)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pIParentNode == NULL || lpNodeName == NULL || ppIChildNodeList == NULL)
		return E_POINTER;

	HRESULT hr;
	hr = pIParentNode->selectNodes(CComBSTR(lpNodeName), ppIChildNodeList);
	if (hr != S_OK)
		return hr;

	if (plListItemCount != NULL)
	{
        hr = (*ppIChildNodeList)->get_length(plListItemCount);
		if (hr != S_OK)
			return hr;
	}

	return S_OK;
}

HRESULT CXMLHelper::RemoveChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpChildName)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pIParentNode == NULL || lpChildName == NULL)
		return E_POINTER;

	HRESULT hr;
	CComPtr<IXMLDOMNode> pIChildNode;
	hr = GetChildNode(pIParentNode, lpChildName, &pIChildNode);
	if (hr != S_OK)
		return hr;

	hr = pIParentNode->removeChild(pIChildNode, NULL);
	if (hr != S_OK)
		return hr;

	pIChildNode.Release();

	return S_OK;
}

HRESULT CXMLHelper::RemoveChildNodeList(IXMLDOMNode* pIParentNode, LPCTSTR lpChildName)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pIParentNode == NULL || lpChildName == NULL)
		return E_POINTER;

	HRESULT hr;
	CComPtr<IXMLDOMNodeList> pIChildNodeList;
	long lListItemCount = 0;
	hr = GetChildNodeList(pIParentNode, lpChildName, &pIChildNodeList, &lListItemCount);
	if (hr != S_OK || lListItemCount == 0)
		return hr;

	CComPtr<IXMLDOMNode> pIChildNode;
	for (long l = 0; l < lListItemCount; l++)
	{
		hr = pIChildNodeList->get_item(l, &pIChildNode);
		if (hr != S_OK)
			return hr;

        hr = pIParentNode->removeChild(pIChildNode, NULL);
		if (hr != S_OK)
			return hr;

		pIChildNode.Release();
	}

	pIChildNodeList.Release();

	return S_OK;
}

HRESULT CXMLHelper::InsertChildNodeImpl(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, BSTR bstrValue, IXMLDOMNode** ppIChildNode)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pIParentNode == NULL || lpNodeName == NULL)
		return E_POINTER;

	HRESULT hr;
	CComPtr<IXMLDOMNode> pICreateNode = NULL;

	AppendIndent(pIParentNode);

	hr = m_pIXMLDoc->createNode(CComVariant(1), CComBSTR(lpNodeName), CComBSTR(lpNameSpace == NULL ? _T("") : lpNameSpace), &pICreateNode);
	if (hr != S_OK)
		return hr;

	hr = pICreateNode->put_text(bstrValue);
	if (hr != S_OK)
		return hr;

	hr = pIParentNode->appendChild(pICreateNode, NULL);
	if (hr != S_OK)
		return hr;

	if (ppIChildNode != NULL)
	{
		*ppIChildNode = pICreateNode;
		pICreateNode.Detach();
	}

	return S_OK;
}

HRESULT CXMLHelper::GetChildValueImpl(IXMLDOMNode* pICurrentNode, LPCTSTR lpNodeName, CComBSTR& bstrValue)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pICurrentNode == NULL || lpNodeName == NULL)
		return E_POINTER;

	HRESULT hr;
	CComPtr<IXMLDOMNode> pIChildNode = NULL;

	hr = pICurrentNode->selectSingleNode(CComBSTR(lpNodeName), &pIChildNode);
	if (hr != S_OK)
		return hr;

	hr = pIChildNode->get_text(&bstrValue);
	if (hr != S_OK)
		return hr;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValueImpl(IXMLDOMNode* pICurrentNode, CComBSTR& bstrValue)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pICurrentNode == NULL)
		return E_POINTER;

	HRESULT hr;
	hr = pICurrentNode->get_text(&bstrValue);
	if (hr != S_OK)
		return hr;

	return S_OK;
}

HRESULT CXMLHelper::SetNodeValueImpl(IXMLDOMNode* pICurrentNode, BSTR bstrValue)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pICurrentNode == NULL)
		return E_POINTER;

	HRESULT hr;
	hr = pICurrentNode->put_text(bstrValue);
	if (hr != S_OK)
		return hr;

	return S_OK;
}

HRESULT CXMLHelper::InsertAttributeImpl(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, BSTR bstrValue, IXMLDOMAttribute** ppIAttribute)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pICurrentNode == NULL || lpAttriName == NULL)
		return E_POINTER;

	HRESULT hr;
	CComPtr<IXMLDOMNamedNodeMap> pINodeAttrs = NULL;
	CComPtr<IXMLDOMAttribute> pINodeAttr = NULL;

	hr = pICurrentNode->get_attributes(&pINodeAttrs);
	if (hr != S_OK)
		return hr;

	hr = m_pIXMLDoc->createAttribute(CComBSTR(lpAttriName), &pINodeAttr);
	if (hr != S_OK)
		return hr;

	hr = pINodeAttr->put_nodeValue(CComVariant(bstrValue));
	if (hr != S_OK)
		return hr;

	hr = pINodeAttrs->setNamedItem(pINodeAttr, NULL);
	if (hr != S_OK)
		return hr;

	if (ppIAttribute != NULL)
	{
		*ppIAttribute = pINodeAttr;
		pINodeAttr.Detach();
	}

	return S_OK;
}

HRESULT CXMLHelper::GetAttributeImpl(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, CComBSTR& bstrValue)
{
	if (m_pIXMLDoc == NULL)
		return E_FAIL;

	if (pICurrentNode == NULL || lpAttriName == NULL)
		return E_POINTER;

	HRESULT hr;
	CComPtr<IXMLDOMNamedNodeMap> pINodeAttrs = NULL;
	CComPtr<IXMLDOMAttribute> pINodeAttr = NULL;

	hr = pICurrentNode->get_attributes(&pINodeAttrs);
	if (hr != S_OK)
		return hr;

	hr = pINodeAttrs->getNamedItem(CComBSTR(lpAttriName), reinterpret_cast<IXMLDOMNode**>(&pINodeAttr));
	if (hr != S_OK)
		return hr;

	CComVariant varValue;
	hr = pINodeAttr->get_nodeValue(&varValue);
	if (hr != S_OK)
		return hr;

	bstrValue = varValue.bstrVal;

	return S_OK;
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, LPCTSTR lpValue, IXMLDOMNode** ppIChildNode)
{
	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, (lpValue == NULL) ? CComBSTR(L"") : CComBSTR(lpValue), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, int nValue, IXMLDOMNode** ppIChildNode)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%d"), nValue);

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strValue), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, DWORD dwValue, int nRadix, IXMLDOMNode** ppIChildNode)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	switch (nRadix)
	{
	case RADIX_FORMAT_OCT:
		::_sntprintf(strValue, 64, _T("0%o"), dwValue);
		break;
	case RADIX_FORMAT_DEC:
		::_sntprintf(strValue, 64, _T("%u"), dwValue);
		break;
	case RADIX_FORMAT_HEX:
		::_sntprintf(strValue, 64, _T("0x%08X"), dwValue);
		break;
	}

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strValue), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, bool fValue, IXMLDOMNode** ppIChildNode)
{
	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, fValue ? CComBSTR(_T("true")) : CComBSTR(_T("false")), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, double dValue, IXMLDOMNode** ppIChildNode)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%Lf"), dValue);

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strValue), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, float fValue, IXMLDOMNode** ppIChildNode)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%f"), fValue);

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strValue), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, const GUID & idValue, IXMLDOMNode** ppIChildNode)
{
	CString strGuid = Guid2String(idValue);

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strGuid), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, const FILETIME &ftValue, IXMLDOMNode** ppIChildNode)
{
	SYSTEMTIME systime;
	FileTimeToSystemTime(&ftValue, &systime);

	CString strTime;
	SystemTime2String(systime, strTime);

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strTime), ppIChildNode);
}

HRESULT CXMLHelper::InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, const __int64 &__int64Value, IXMLDOMNode** ppIChildNode)
{	
	CString strValue;
	strValue.Format(_T("%I64d"), __int64Value);

	return InsertChildNodeImpl(pIParentNode, lpNodeName, lpNameSpace, CComBSTR(strValue), ppIChildNode);
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, CString& strValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	strValue = lpConvert;

	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, GUID &idValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	idValue = String2Guid(lpConvert);

	return S_OK;
}


HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, int& nValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%d"), &nValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, DWORD& dwValue, int nRadix)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	switch (nRadix)
	{
	case RADIX_FORMAT_OCT:
		if (_stscanf(lpConvert, _T("0%o"), &dwValue) != 1)
			return E_FAIL;
		break;
	case RADIX_FORMAT_DEC:
		if (_stscanf(lpConvert, _T("%u"), &dwValue) != 1)
			return E_FAIL;
		break;
	case RADIX_FORMAT_HEX:
		if (_stscanf(lpConvert, _T("0x%08X"), &dwValue) != 1)
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, bool& fValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_tcscmp(lpConvert, _T("true")) == 0)
		fValue = true;
	else if (_tcscmp(lpConvert, _T("false")) == 0)
		fValue = false;
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, double& dValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%Lf"), &dValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, float& fValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%f"), &fValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, FILETIME &ftValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);

	SYSTEMTIME systime;
	if(!String2SystemTime(lpConvert, systime))
		return E_FAIL;

	SystemTimeToFileTime(&systime, &ftValue);
	
	return S_OK;
}

HRESULT CXMLHelper::GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, __int64 &__int64Value)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetChildValueImpl(pICurrentNode, lpChildName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%I64d"), &__int64Value) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, CString& strValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	strValue = lpConvert;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, int& nValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%d"), &nValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, DWORD& dwValue, int nRadix)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	switch (nRadix)
	{
	case RADIX_FORMAT_OCT:
		if (_stscanf(lpConvert, _T("0%o"), &dwValue) != 1)
			return E_FAIL;
		break;
	case RADIX_FORMAT_DEC:
		if (_stscanf(lpConvert, _T("%u"), &dwValue) != 1)
			return E_FAIL;
		break;
	case RADIX_FORMAT_HEX:
		if (_stscanf(lpConvert, _T("0x%08X"), &dwValue) != 1)
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, bool& fValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_tcscmp(lpConvert, _T("true")) == 0)
		fValue = true;
	else if (_tcscmp(lpConvert, _T("false")) == 0)
		fValue = false;
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, double& dValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%Lf"), &dValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, float& fValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%f"), &fValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, GUID &idValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	idValue = String2Guid(lpConvert);

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, FILETIME &ftValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);

	SYSTEMTIME systime;
	if(!String2SystemTime(lpConvert, systime))
		return E_FAIL;

	SystemTimeToFileTime(&systime, &ftValue);

	return S_OK;
}

HRESULT CXMLHelper::GetNodeValue(IXMLDOMNode* pICurrentNode, __int64 &__int64Value)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetNodeValueImpl(pICurrentNode, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);

	if (_stscanf(lpConvert, _T("%I64d"), &__int64Value) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpValue)
{
	return SetNodeValueImpl(pICurrentNode, (lpValue == NULL) ? CComBSTR(L"") : CComBSTR(lpValue));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, int nValue)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%d"), nValue);

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strValue));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, DWORD dwValue, int nRadix)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	switch (nRadix)
	{
	case RADIX_FORMAT_OCT:
		::_sntprintf(strValue, 64, _T("0%o"), dwValue);
		break;
	case RADIX_FORMAT_DEC:
		::_sntprintf(strValue, 64, _T("%u"), dwValue);
		break;
	case RADIX_FORMAT_HEX:
		::_sntprintf(strValue, 64, _T("0x%08X"), dwValue);
		break;
	}

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strValue));

}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, bool fValue)
{
	return SetNodeValueImpl(pICurrentNode, fValue ? CComBSTR(_T("true")) : CComBSTR(_T("false")));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, double dValue)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%Lf"), dValue);

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strValue));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, float fValue)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%f"), fValue);

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strValue));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, const GUID &idValue)
{
	CString strGuid = Guid2String(idValue);
// 	TCHAR strValue[64];
// 	::memset(strValue, 0, sizeof(TCHAR) * 64);
// 	::_sntprintf(strValue, 64, _T("%Lf"), dValue);

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strGuid));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, const FILETIME &ftValue)
{
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&ftValue, &sysTime);

	CString strTime;
	SystemTime2String(sysTime, strTime);

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strTime));
}

HRESULT CXMLHelper::SetNodeValue(IXMLDOMNode* pICurrentNode, const __int64 &__int64Value)
{
	CString strValue;
	strValue.Format(_T("%I64d"), __int64Value);

	return SetNodeValueImpl(pICurrentNode, CComBSTR(strValue));
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, LPCTSTR lpValue, IXMLDOMAttribute** ppIAttribute)
{
	return InsertAttributeImpl(pICurrentNode, lpAttriName, (lpValue == NULL) ? CComBSTR(L"") : CComBSTR(lpValue), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, int nValue, IXMLDOMAttribute** ppIAttribute)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%d"), nValue);

	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strValue), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, DWORD dwValue, int nRadix, IXMLDOMAttribute** ppIAttribute)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	switch (nRadix)
	{
	case RADIX_FORMAT_OCT:
		::_sntprintf(strValue, 64, _T("0%o"), dwValue);
		break;
	case RADIX_FORMAT_DEC:
		::_sntprintf(strValue, 64, _T("%u"), dwValue);
		break;
	case RADIX_FORMAT_HEX:
		::_sntprintf(strValue, 64, _T("0x%08X"), dwValue);
		break;
	}

	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strValue), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, bool fValue, IXMLDOMAttribute** ppIAttribute)
{
	return InsertAttributeImpl(pICurrentNode, lpAttriName, fValue ? CComBSTR(_T("true")) : CComBSTR(_T("false")), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, double dValue, IXMLDOMAttribute** ppIAttribute)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%Lf"), dValue);

	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strValue), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, float fValue, IXMLDOMAttribute** ppIAttribute)
{
	TCHAR strValue[64];
	::memset(strValue, 0, sizeof(TCHAR) * 64);
	::_sntprintf(strValue, 64, _T("%f"), fValue);

	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strValue), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, const GUID &idValue, IXMLDOMAttribute** ppIAttribute)
{
	CString strGuid = Guid2String(idValue);
	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strGuid), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, const FILETIME &ftValue, IXMLDOMAttribute** ppIAttribute)
{
	SYSTEMTIME sysTime;
	FileTimeToSystemTime(&ftValue, &sysTime);

	CString strTime;
	SystemTime2String(sysTime, strTime);

	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strTime), ppIAttribute);
}

HRESULT CXMLHelper::InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, const __int64 &__int64Value, IXMLDOMAttribute** ppIAttribute)
{
	CString strValue;
	strValue.Format(_T("%I64d"), __int64Value);

	return InsertAttributeImpl(pICurrentNode, lpAttriName, CComBSTR(strValue), ppIAttribute);
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, CString& strValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	strValue = lpConvert;

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, int& nValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%d"), &nValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, DWORD& dwValue, int nRadix)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	switch (nRadix)
	{
	case RADIX_FORMAT_OCT:
		if (_stscanf(lpConvert, _T("0%o"), &dwValue) != 1)
			return E_FAIL;
		break;
	case RADIX_FORMAT_DEC:
		if (_stscanf(lpConvert, _T("%u"), &dwValue) != 1)
			return E_FAIL;
		break;
	case RADIX_FORMAT_HEX:
		if (_stscanf(lpConvert, _T("0x%08X"), &dwValue) != 1)
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, bool& fValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_tcscmp(lpConvert, _T("true")) == 0)
		fValue = true;
	else if (_tcscmp(lpConvert, _T("false")) == 0)
		fValue = false;
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, double& dValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%Lf"), &dValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, float& fValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	if (_stscanf(lpConvert, _T("%f"), &fValue) != 1)
		return E_FAIL;

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, GUID &idValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);
	idValue = String2Guid(lpConvert);

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, FILETIME &ftValue)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);

	SYSTEMTIME systime;
	if(!String2SystemTime(lpConvert, systime))
		return E_FAIL;

	SystemTimeToFileTime(&systime, &ftValue);

	return S_OK;
}

HRESULT CXMLHelper::GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, __int64 &__int64Value)
{
	HRESULT hr;
	CComBSTR bstrValue;
	hr = GetAttributeImpl(pICurrentNode, lpAttriName, bstrValue);
	if (hr != S_OK)
		return hr;

	COLE2T lpConvert(bstrValue);

	CString strValue;
	strValue.Format(_T("%I64d"), __int64Value);

	return S_OK;
}

HRESULT	CXMLHelper::AppendIndent(IXMLDOMNode* pIParentNode)
{
	HRESULT hr = NOERROR;
// 	CComPtr<IXMLDOMElement> pXMLDOMElement;
// 	hr = m_pIXMLDoc->get_documentElement(&pXMLDOMElement);
// 	if(FAILED(hr))	return hr;

	CString strIndent = _T("\r\n");

	CComPtr<IXMLDOMNode> pCur;
	pCur = pIParentNode;
	while(TRUE) 
	{
		CComPtr<IXMLDOMNode> pParent;
		hr = pCur->get_parentNode(&pParent);
		if(pParent != NULL)
			strIndent += _T("\t");
		else
			break;

		pCur = pParent;
	}

	CComPtr<IXMLDOMText> pXMLDOMText;
	hr = m_pIXMLDoc->createTextNode(CComBSTR(strIndent), &pXMLDOMText);
	if(FAILED(hr))	return hr;

	hr = pIParentNode->appendChild(pXMLDOMText, NULL);
	if(FAILED(hr))	return hr;

	return hr;
}

HRESULT	CXMLHelper::EndAddChild(IXMLDOMNode* pIParentNode)
{
	HRESULT hr = NOERROR;
// 	CComPtr<IXMLDOMElement> pXMLDOMElement;
// 	hr = m_pIXMLDoc->get_documentElement(&pXMLDOMElement);
// 	if(FAILED(hr))	return hr;

	CString strIndent = _T("\r\n");

	BOOL bFirst = TRUE;
	CComPtr<IXMLDOMNode> pCur;
	pCur = pIParentNode;
	while(TRUE) 
	{
		CComPtr<IXMLDOMNode> pParent;
		hr = pCur->get_parentNode(&pParent);
		if(pParent != NULL)
		{
			if(!bFirst)
				strIndent += _T("\t");

			bFirst = FALSE;
		}
		else
			break;

		pCur = pParent;
	}

	CComPtr<IXMLDOMText> pXMLDOMText;
	hr = m_pIXMLDoc->createTextNode(CComBSTR(strIndent), &pXMLDOMText);
	if(FAILED(hr))	return hr;

	hr = pIParentNode->appendChild(pXMLDOMText, NULL);
	if(FAILED(hr))	return hr;

	return hr;
}

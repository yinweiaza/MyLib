#pragma once

#ifdef NOUSE_XMLHELPER_IMPORT_EXPORT
	#define IP_EP_XMLHELPER 
#else
	#define IP_EP_XMLHELPER T9_EXT_CLASS
#endif

class IP_EP_XMLHELPER CXMLHelper
{
public:
	CXMLHelper(void);
	~CXMLHelper(void);

	HRESULT Load(LPCTSTR lpFileName, IXMLDOMElement** ppIXMLRoot = NULL);
	HRESULT Save(LPCTSTR lpFileName);
	HRESULT CreateXmlDom(	LPCTSTR lpProcessingInstruction,
							LPCTSTR lpRootName, 
							IXMLDOMElement** ppIXMLRoot);
	HRESULT LoadXMLString(LPCTSTR lpXMLIn, IXMLDOMElement** ppIXMLRoot = NULL);
	HRESULT GetXMLString(CString& strXMLOut);
	HRESULT Close();

	HRESULT GetXMLRoot(IXMLDOMElement** ppIXMLRoot);
	HRESULT GetChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, IXMLDOMNode** ppIChildNode);
	HRESULT GetChildNodeList(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, IXMLDOMNodeList** ppIChildNodeList, long* plListItemCount = NULL);

	HRESULT RemoveChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpChildName);
	HRESULT RemoveChildNodeList(IXMLDOMNode* pIParentNode, LPCTSTR lpChildName);

	enum {
		RADIX_FORMAT_OCT,
		RADIX_FORMAT_DEC,
		RADIX_FORMAT_HEX
	};

	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, LPCTSTR lpValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, int nValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, DWORD dwValue, int nRadix = RADIX_FORMAT_OCT, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, bool fValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, double dValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, float fValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, const GUID &idValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, const FILETIME &ftValue, IXMLDOMNode** ppIChildNode = NULL);
	HRESULT InsertChildNode(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, const __int64 &__int64Value, IXMLDOMNode** ppIChildNode = NULL);

	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, CString& strValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, int& nValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, DWORD& dwValue, int nRadix = RADIX_FORMAT_OCT);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, bool& fValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, double& dValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, float& fValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, GUID &idValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, FILETIME &ftValue);
	HRESULT GetChildValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpChildName, __int64 &__int64Value);

	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, CString& strValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, int& nValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, DWORD& dwValue, int nRadix);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, bool& fValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, double& dValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, float& fValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, GUID &idValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, FILETIME &ftValue);
	HRESULT GetNodeValue(IXMLDOMNode* pICurrentNode, __int64 &__int64Value);

	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, LPCTSTR lpValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, int nValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, DWORD dwValue, int nRadix);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, bool fValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, double dValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, float fValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, const GUID &idValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, const FILETIME &ftValue);
	HRESULT SetNodeValue(IXMLDOMNode* pICurrentNode, const __int64 &__int64Value);

	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, LPCTSTR lpValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, int nValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, DWORD dwValue, int nRadix, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, bool fValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, double dValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, float fValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, const GUID &idValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, const FILETIME &ftValue, IXMLDOMAttribute** ppIAttribute = NULL);
	HRESULT InsertAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, const __int64 &__int64Value, IXMLDOMAttribute** ppIAttribute = NULL);

	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, CString& strValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, int& nValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, DWORD& dwValue, int nRadix);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, bool& fValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, double& dValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, float& fValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, GUID &idValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, FILETIME &ftValue);
	HRESULT GetAttribute(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, __int64 &__int64Value);

	HRESULT	EndAddChild(IXMLDOMNode* pIParentNode);

protected:
	HRESULT InsertChildNodeImpl(IXMLDOMNode* pIParentNode, LPCTSTR lpNodeName, LPCTSTR lpNameSpace, BSTR bstrValue, IXMLDOMNode** ppIChildNode);
	HRESULT GetChildValueImpl(IXMLDOMNode* pICurrentNode, LPCTSTR lpNodeName, CComBSTR& bstrValue);
	HRESULT GetNodeValueImpl(IXMLDOMNode* pICurrentNode, CComBSTR& bstrValue);
	HRESULT SetNodeValueImpl(IXMLDOMNode* pICurrentNode, BSTR bstrValue);
	HRESULT InsertAttributeImpl(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, BSTR bstrValue, IXMLDOMAttribute** ppIAttribute);
	HRESULT GetAttributeImpl(IXMLDOMNode* pICurrentNode, LPCTSTR lpAttriName, CComBSTR& bstrValue);
	HRESULT	AppendIndent(IXMLDOMNode* pIParentNode);

protected:
	LPBYTE					 m_pByte;
	CComPtr<IXMLDOMDocument> m_pIXMLDoc;
};

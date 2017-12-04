#pragma once

#include <wincrypt.h>

class T9_EXT_CLASS CBacCrypt
{
public:
	CBacCrypt(void);
	virtual ~CBacCrypt(void);

	HRESULT			Init(BYTE* pbPassword, DWORD dwLength);
	HRESULT			Uninit();

	HRESULT			Encrypt(BYTE* pbData, DWORD dwDataLen);
	HRESULT			Decrypt(BYTE* pbData, DWORD dwDataLen);

	static HRESULT Test();
protected:
	HCRYPTPROV		m_hProv;
	HCRYPTKEY		m_hKey;
	HCRYPTHASH		m_hHash;
};

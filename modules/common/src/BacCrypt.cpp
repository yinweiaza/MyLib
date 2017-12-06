#include "StdAfx.h"
#include ".\baccrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBacCrypt::CBacCrypt(void)
{
	m_hProv = 0;
	m_hKey = 0;
	m_hHash = 0;
}

CBacCrypt::~CBacCrypt(void)
{
}

HRESULT	CBacCrypt::Init(BYTE* pbPassword, DWORD dwLength)
{
	// Get a handle to user default provider.
	if(!CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, 0)) 
	{
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			if (!CryptAcquireContext(&m_hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				TRACE(_T("Error %x during CryptAcquireContext!\n"), GetLastError());
				goto done; 
			}
		}
		else
		{
			TRACE(_T("Error %x during CryptAcquireContext!\n"), GetLastError());
			goto done;
		}
	}

	// Create a hash object.
	if(!CryptCreateHash(m_hProv, CALG_MD5, 0, 0, &m_hHash)) {
		TRACE(_T("Error %x during CryptCreateHash!\n"), GetLastError());
		goto done;
	}

	// Hash the password string.
	if(!CryptHashData(m_hHash, pbPassword, dwLength, 0)) {
		TRACE(_T("Error %x during CryptHashData!\n"), GetLastError());
		goto done;
	}

	// Create a block cipher session key based on the hash of the password.
	if(!CryptDeriveKey(m_hProv, CALG_RC4, m_hHash, CRYPT_EXPORTABLE, &m_hKey)) {
		TRACE(_T("Error %x during CryptDeriveKey!\n"), GetLastError());
		goto done;
	}

	return S_OK;

done:

	// Destroy the hash object.
	if(m_hHash != 0) CryptDestroyHash(m_hHash);

	// Destroy the session key.
	if(m_hKey != 0) CryptDestroyKey(m_hKey);

	// Release the provider handle.
	if(m_hProv != 0) CryptReleaseContext(m_hProv, 0);

	return E_FAIL;
}

HRESULT	CBacCrypt::Uninit()
{
	// Destroy the hash object.
	if(m_hHash != 0) CryptDestroyHash(m_hHash);

	// Destroy the session key.
	if(m_hKey != 0) CryptDestroyKey(m_hKey);

	// Release the provider handle.
	if(m_hProv != 0) CryptReleaseContext(m_hProv, 0);

	return S_OK;
}

HRESULT	CBacCrypt::Encrypt(BYTE* pbData, DWORD dwDataLen)
{
	BOOL bResult = CryptEncrypt(
		m_hKey,            // 之前获得的密鈅对象
		0,               // 不散列数据
		TRUE,            // 最后的还是缓冲的数据
		0,               // 必须置0
		pbData,         // 数据缓冲区
		&dwDataLen,         // 数据尺寸
		dwDataLen);         // 数据块尺寸

	return bResult?S_OK:E_FAIL;
}

HRESULT	CBacCrypt::Decrypt(BYTE* pbData, DWORD dwDataLen)
{
	BOOL bResult = CryptDecrypt(
		m_hKey,            // 之前获得的密鈅对象
		0,               // 不散列数据
		TRUE,            // 最后的还是缓冲的数据
		0,               // 必须置0
		pbData,         // 数据缓冲区
		&dwDataLen);         // 数据尺寸

	return bResult?S_OK:E_FAIL;
}
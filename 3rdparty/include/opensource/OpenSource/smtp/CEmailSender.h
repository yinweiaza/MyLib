
#ifndef _MYSMTP
#define _MYSMTP
#include <string>
using namespace std;

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define COPYRIGHT "Smtp Client By LEDnet"	// ��Ȩ��Ϣ
#define BOUNDARY "www.oopfans.com"				// �߽��ַ���
//#define for if(0); else for		// ����������oge

// #ifdef WIN32
// #define uint8_t  unsigned __int8
// #define uint16_t unsigned __int16
// #define uint32_t unsigned __int32
// #define uint64_t unsigned __int64
// 
// #define int8_t  __int8
// #define int16_t __int16
// #define int32_t __int32
// #endif

//��������С,���ֵ������57��������!
#define BUFFER_READ 11400
enum smtpAnswerCode
{
	smtp_OK = 0,					//�ɹ�
	smtp_UnOpenSMTP,				//_T("δ��ͨSMTP����");
	smtp_ValidationPassed,			//_T("�����֤ͨ��");
	smtp_SMTPConnectionFailed,		//_T("(���ӷ�����ʧ��)");
	smtp_SenderAddrError,			//_T("���ͷ���������������");
	smtp_RcvAddrError				//_T("���շ����䲻����");
};
class CEmailSender  
{
public:

	CEmailSender();
	virtual ~CEmailSender();

private:

	SOCKET		m_SOCKET;		
	WSADATA		m_WSADATA;
	HOSTENT		*m_HOSTENT;
	SOCKADDR_IN m_SOCKADDR_IN;
public:
// 	string		m_strSmtpAddr;//SMTP������
// 	int			m_nPort;//�˿�
// 	string		m_strSenderAddr;//�˺�
// 	string		m_strSenderName;//���ͷ�����
// 	string		m_strPassword;//����
// 	string		m_strTitle;//�ʼ�����
//  string		m_strRecvAddr;//���շ���ַ
//  string		m_strContent;//�ʼ�����
// 	unsigned	short m_usSmtpPort;

	CString		m_strSmtp;
	int			m_nPort;
	CString		m_strSender;
	CString		m_strSenderName;
	CString		m_strSenderPsd;
	CString		m_strReceiver;
	CString		m_strTitle;
	CString		m_strContent;
	unsigned short	m_usSmtpPort;


private:

	bool	CreateSocket();		// ����Socket
	void	ReleaseSocket();	// �ͷ�Socket
	string	Encode(const unsigned char* Data,int DataByte);
	string    Decode(const char* Data,int DataByte,int& OutByte);
	/*
	����:
		��֤�ӷ��������ص�ǰ��λ����ʹ��ݽ����Ĳ����Ƿ�һ��
	*/
	bool	CheckResponse(const char* RecvCode);
	bool	GetResponseCode(CString &StrCode);
	//����smtp������
	/*
		SmtpAddr    smtp ��������ַ
		Port        �˿ںţ�SMTP�˿ں�Ϊ25��
	*/
	bool	Connect(const CString SmtpAddr,const int Port);

	//��֤�û���������
	/*
	Username     �˺�
	Password     ����
	*/
	bool	Validate(const CString Username,const CString Password,DWORD &ErrorCode);
public:
   
	//��������

	bool	SendData(DWORD &ErrorCode);
	bool	SendSmtpData(DWORD &ErrorCode);
	int		unicode_to_utf8(WCHAR	*in, int insize, CHAR *out);
};

#endif

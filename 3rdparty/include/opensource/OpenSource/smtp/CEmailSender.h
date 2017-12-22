
#ifndef _MYSMTP
#define _MYSMTP
#include <string>
using namespace std;

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define COPYRIGHT "Smtp Client By LEDnet"	// 版权信息
#define BOUNDARY "www.oopfans.com"				// 边界字符串
//#define for if(0); else for		// 变量作用域oge

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

//缓冲区大小,这个值必须是57的整数倍!
#define BUFFER_READ 11400
enum smtpAnswerCode
{
	smtp_OK = 0,					//成功
	smtp_UnOpenSMTP,				//_T("未开通SMTP服务");
	smtp_ValidationPassed,			//_T("身份验证通过");
	smtp_SMTPConnectionFailed,		//_T("(连接服务器失败)");
	smtp_SenderAddrError,			//_T("发送方邮箱或者密码错误");
	smtp_RcvAddrError				//_T("接收方邮箱不存在");
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
// 	string		m_strSmtpAddr;//SMTP服务器
// 	int			m_nPort;//端口
// 	string		m_strSenderAddr;//账号
// 	string		m_strSenderName;//发送方姓名
// 	string		m_strPassword;//密码
// 	string		m_strTitle;//邮件主题
//  string		m_strRecvAddr;//接收方地址
//  string		m_strContent;//邮件内容
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

	bool	CreateSocket();		// 创建Socket
	void	ReleaseSocket();	// 释放Socket
	string	Encode(const unsigned char* Data,int DataByte);
	string    Decode(const char* Data,int DataByte,int& OutByte);
	/*
	功能:
		验证从服务器返回的前三位代码和传递进来的参数是否一样
	*/
	bool	CheckResponse(const char* RecvCode);
	bool	GetResponseCode(CString &StrCode);
	//连接smtp服务器
	/*
		SmtpAddr    smtp 服务器地址
		Port        端口号（SMTP端口号为25）
	*/
	bool	Connect(const CString SmtpAddr,const int Port);

	//验证用户名和密码
	/*
	Username     账号
	Password     密码
	*/
	bool	Validate(const CString Username,const CString Password,DWORD &ErrorCode);
public:
   
	//发送数据

	bool	SendData(DWORD &ErrorCode);
	bool	SendSmtpData(DWORD &ErrorCode);
	int		unicode_to_utf8(WCHAR	*in, int insize, CHAR *out);
};

#endif

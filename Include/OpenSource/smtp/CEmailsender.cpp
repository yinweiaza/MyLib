#include "stdafx.h"
#include "CEmailSender.h"
#include "CSmtp.h"
CEmailSender::CEmailSender()
{
// 	m_strSmtpAddr    = "";
// 	m_usSmtpPort	 = 25;
// 	m_nPort			 = 25;
// 	m_strSenderAddr  = "";
// 	m_strSenderName  = "";
// 	m_strPassword	 = "";
// 	m_strRecvAddr    = "";
// 	m_strContent     = "";
	m_nPort=25;
	m_usSmtpPort=25;
	m_strTitle=_T("");
}

CEmailSender::~CEmailSender()
{

}

bool CEmailSender::CreateSocket()
{
	if(WSAStartup(MAKEWORD(2,2),&m_WSADATA) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return FALSE;
	}
	
	if((m_SOCKET = socket(AF_INET,SOCK_STREAM,0)) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return FALSE;
	}

//	int time = 10000;
//	if(setsockopt(m_SOCKET,SOL_SOCKET,SO_RCVTIMEO,(char *)&time,sizeof(int)) == SOCKET_ERROR)
//	{
//		ReleaseSocket();
//		return FALSE;
//	}

	return TRUE;
}

void CEmailSender::ReleaseSocket()
{
	shutdown(m_SOCKET,SD_BOTH);
	closesocket(m_SOCKET);
	WSACleanup();
}
bool	CEmailSender::GetResponseCode(CString &StrCode)
{
	try
	{
		char Buf[1024]={0};
		if(recv(m_SOCKET,Buf,1024,0) == SOCKET_ERROR) return false;
		StrCode = Buf;
		StrCode = StrCode.Left(3);
	
		return true;
	}
	catch(...)
	{
		return false;
	}
}

bool CEmailSender::CheckResponse(const char* RecvCode)
{
	try
	{
		char Buf[1024]={0};
		if(recv(m_SOCKET,Buf,1024,0) == SOCKET_ERROR)
			return false;
		//AfxMessageBox(Buf);
		return RecvCode[0] == Buf[0] && \
			   RecvCode[1] == Buf[1] && \
			   RecvCode[2] == Buf[2] ? true : false;
	}
	catch(...)
	{
		return false;
	}
}

bool CEmailSender::Connect(const CString SmtpAddr,const int Port)
{
	if(!CreateSocket())
		return false;

	CHAR cSmtpAddr[256];
	WideCharToMultiByte(CP_ACP, 0, SmtpAddr, -1, cSmtpAddr, 256, NULL, NULL);
	if((m_HOSTENT=gethostbyname(cSmtpAddr))==NULL)
	{
		ReleaseSocket();
		return false;	
	}

	if(m_HOSTENT->h_addr_list[0] == NULL)
	{
		ReleaseSocket();
		return false;
	}

	memset(&m_SOCKADDR_IN,0,sizeof(m_SOCKADDR_IN));
	m_SOCKADDR_IN.sin_family = AF_INET;
	m_SOCKADDR_IN.sin_port   = htons(Port);
	m_SOCKADDR_IN.sin_addr.S_un.S_addr = *(ULONG *)m_HOSTENT->h_addr_list[0];

	//连接服务器
	if(connect(m_SOCKET,(sockaddr *)&m_SOCKADDR_IN,sizeof(m_SOCKADDR_IN)) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("220")) 
		return false;

	//向服务器发送"HELO "+服务器名
	CHAR strTmp[270]="HELO ";
	strcat( strTmp, cSmtpAddr);
	strcat( strTmp, "\r\n");
	if(send(m_SOCKET,strTmp,strlen(strTmp),0) == SOCKET_ERROR)	
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) 
		return false;

	return true;
}

bool CEmailSender::Validate(const CString Username,const CString Password,DWORD &ErrorCode)
{
	CHAR	cUserName[256], cUserPsd[256];
	WideCharToMultiByte( CP_ACP, 0, Username, -1, cUserName, 256, NULL, NULL);
	WideCharToMultiByte( CP_ACP, 0, Password, -1, cUserPsd, 256, NULL, NULL);
	//发送"AUTH LOGIN"
	if(send(m_SOCKET,"AUTH LOGIN\r\n",strlen("AUTH LOGIN\r\n"),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) 
		return false;

	//发送经base64编码的用户名
	string strUserName=Encode((unsigned char *)cUserName,strlen(cUserName))+"\r\n";
	if(send(m_SOCKET,strUserName.c_str(),strUserName.length(),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) 
	{
		ErrorCode = smtp_UnOpenSMTP;//_T("发送方邮箱地址错误或未开通SMTP服务");
		return false;
	}

	//发送经base64编码的密码
	string strPassword=Encode((unsigned char *)cUserPsd,strlen(cUserPsd))+"\r\n";
	if(send(m_SOCKET,strPassword.c_str(),strPassword.length(),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	CString strCode;
	if (GetResponseCode(strCode))
	{
		if (strCode == _T("235"))
		{
			ErrorCode = smtp_ValidationPassed;//_T("身份验证通过");
			return true;
		}
		else if (strCode == _T("454"))
		{
			ErrorCode = smtp_UnOpenSMTP;//_T("请先开通SMTP服务");
			return false;
		}
		else
		{
			ErrorCode = smtp_SenderAddrError;//_T("连接服务器失败");
			return false;
		}
	}
	else
	{
		ErrorCode = smtp_SenderAddrError;//_T("发送方邮箱或者密码错误");
		return false;
	}

// 	if(!CheckResponse("235")) 
// 		//454  请先开通SMTP服务
// 		//535 身份验证失败
// 		return false;

	return true;
}

bool CEmailSender::SendSmtpData(DWORD &ErrorCode)
{
	CHAR	cSMTP[256];
	WideCharToMultiByte( CP_ACP, 0, m_strSmtp, -1, cSMTP, 256, NULL, NULL);
	CHAR	cSenderEMail[256];
	WideCharToMultiByte( CP_ACP, 0, m_strSender, -1, cSenderEMail, 256, NULL, NULL);
	CHAR	cSenderPsd[256];
	WideCharToMultiByte( CP_ACP, 0, m_strSenderPsd, -1, cSenderPsd, 256, NULL, NULL);
	CHAR	cSenderUser[256];
	WideCharToMultiByte( CP_ACP, 0, m_strSenderName, -1, cSenderUser, 256, NULL, NULL);
	CHAR	cTitle[256];
	WideCharToMultiByte( CP_ACP, 0, m_strTitle, -1, cTitle, 256, NULL, NULL);
	CHAR	cContent[2000];
	WideCharToMultiByte( CP_ACP, 0, m_strContent, -1, cContent, 2000, NULL, NULL);

	CHAR	cReceiver[256];
	WideCharToMultiByte( CP_ACP, 0, m_strReceiver, -1, cReceiver, 256, NULL, NULL);

	return SendSmtpOneEMail(cSMTP, m_usSmtpPort, cSenderEMail, cSenderPsd, cSenderUser, cReceiver, cTitle, cContent);
}
bool CEmailSender::SendData(DWORD &ErrorCode)						
{
	//连接服务器
	if( m_usSmtpPort!=0 && m_usSmtpPort!= m_nPort)
	{
		return SendSmtpData(ErrorCode);
	}
	else
	{
		m_nPort = m_usSmtpPort;
	}
	if(!Connect(m_strSmtp,m_nPort))
	{
		ErrorCode = smtp_SMTPConnectionFailed;//_T("(连接服务器失败)"); 
		return false;	
	}

	//验证用户名密码

	if(!Validate(m_strSender,m_strSenderPsd,ErrorCode))
	{
		ErrorCode = smtp_SenderAddrError;//_T("(发送箱验证失败)"); 
		return false;	
	}

// 	if(SendFrom.empty()) 
// 		return false;

	CHAR	strTmp[20000]="MAIL FROM:<";
	CHAR	cSender[256];
	WideCharToMultiByte( CP_ACP, 0, m_strSender, -1, cSender, 256, NULL, NULL);
	CHAR	cTitle[256];
	WideCharToMultiByte( CP_ACP, 0, m_strTitle, -1, cTitle, 256, NULL, NULL);
	CHAR	cSendername[256];
	WideCharToMultiByte( CP_ACP, 0, m_strSenderName, -1, cSendername, 256, NULL, NULL);
	CHAR	cReceiver[256];
	WideCharToMultiByte( CP_ACP, 0, m_strReceiver, -1, cReceiver, 256, NULL, NULL);

	//发送MAIL FROM:<abc@xyz.com>
	strcpy(strTmp, "MAIL FROM:<");
	strcat(strTmp, cSender);
	strcat(strTmp, ">\r\n");
	TRACE("\n%s", strTmp);
	if(send(m_SOCKET,strTmp,strlen(strTmp),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) 
		return false;

	//发送RCPT To:<abc@xyz.com>
	strcpy(strTmp, "RCPT To:<");
	strcat(strTmp, cReceiver);
	strcat(strTmp, ">\r\n");
	TRACE("\n%s", strTmp);
	if(send(m_SOCKET,strTmp,strlen(strTmp),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) 
	{
		ErrorCode = smtp_RcvAddrError;//_T("接收方邮箱不存在");
		return false;
	}

	//发送"DATA\r\n"
	TRACE("\n%s", strTmp);
	if(send(m_SOCKET,"DATA\r\n",strlen("DATA\r\n"),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("354")) 
		return false;

	//"Mail From:SenderName<xxx@mail.com>\r\n"
	strcpy(strTmp, "From:");
	strcat(strTmp, cSendername);
	strcat(strTmp, "<");
	strcat(strTmp, cSender);
	strcat(strTmp, ">\r\n");

	//"Subject: 邮件主题\r\n"
	strcat(strTmp, "Subject:");
	strcat(strTmp, cTitle);
	strcat(cTitle, "\r\n");

	//"MIME_Version:1.0\r\n"
	strcat(strTmp, "\r\n");

	strcat( strTmp, "X-Mailer:");
	strcat(strTmp, COPYRIGHT); 
	strcat(strTmp, "\r\n");
	strcat(strTmp, "MIME_Version:1.0\r\n");

	//"Content-type:multipart/mixed;Boundary=xxx\r\n\r\n";
	strcat(strTmp, "Content-type:multipart/mixed;Boundary=");
	strcat(strTmp, BOUNDARY);
	strcat(strTmp, "\r\n\r\n");

	//先将HEADER部分发送过去
	TRACE("\n%s", strTmp);
	if(send(m_SOCKET,strTmp,strlen(strTmp),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;	
	}

	//邮件主体
	strcpy(strTmp, "--");
	strcat(strTmp, BOUNDARY);
	strcat(strTmp, "\r\n");
	if( 0 )
	{//GB2312
		strcat(strTmp, "Content-type:text/plain;Charset=gb2312\r\n");
		strcat(strTmp, "Content-Transfer-Encoding:8bit\r\n\r\n");
		//邮件内容
		CHAR	*p_cContent;
		int		iContentLen;
		iContentLen = m_strContent.GetLength()*2+1;
		p_cContent = new CHAR[iContentLen];
		WideCharToMultiByte( CP_ACP, 0, m_strContent, -1, p_cContent, iContentLen, NULL, NULL);
		strcat(strTmp, p_cContent);
		delete	p_cContent;
	}
	else
	{//UTF-8
		strcat(strTmp, "Content-type:text/plain;Charset=utf-8\r\n");
		strcat(strTmp, "Content-Transfer-Encoding:8bit\r\n\r\n");
		//邮件内容
		CHAR	*p_cContent;
		int		iContentLen;
		int	charCount = m_strContent.GetLength();

		p_cContent = new CHAR[charCount*3+1];
		ZeroMemory(p_cContent, charCount*3+1);
		unicode_to_utf8( m_strContent.GetBuffer(charCount), charCount*sizeof(TCHAR), p_cContent);

		m_strContent.ReleaseBuffer();
		strcat(strTmp, p_cContent);
		delete	p_cContent;
	}	
	strcat(strTmp, "\r\n\r\n");
	
	//将邮件内容发送出去
	TRACE("\n%s", strTmp);
	if(send(m_SOCKET,strTmp,strlen(strTmp),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;	
	}

	strcpy(strTmp, "--");
	strcat(strTmp, BOUNDARY);
	strcat(strTmp, "--\r\n.\r\n");
	TRACE("\n%s", strTmp);
 	if(send(m_SOCKET,strTmp,strlen(strTmp),0) == SOCKET_ERROR)
 	{
 		ReleaseSocket();
 		return false;
 	}
 	if(!CheckResponse("250")) 
		return false;

	//退出
	TRACE("\n%s", strTmp);
	if(send(m_SOCKET,"QUIT\r\n",strlen("QUIT\r\n"),0) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("221")) 
		return false;

	ReleaseSocket();
	return true;
}

int CEmailSender::unicode_to_utf8(WCHAR *in, int insize, CHAR *out)
{
	int i = 0;
	int outsize = 0;
	int charscount = 0;
	CHAR *tmp = NULL;

	charscount = insize / sizeof(WCHAR);
	tmp = out;

	for (i = 0; i < charscount; i++)
	{
		WCHAR unicode = in[i];

		if (unicode >= 0x0000 && unicode <= 0x007f)
		{
			*tmp = (CHAR)unicode;
			tmp += 1;
			outsize += 1;
		}
		else if (unicode >= 0x0080 && unicode <= 0x07ff)
		{
			*tmp = 0xc0 | (unicode >> 6);
			tmp += 1;
			*tmp = 0x80 | (unicode & 0x3F);
			tmp += 1;
			outsize += 2;
		}
		else if (unicode >= 0x0800 && unicode <= 0xffff)
		{
			*tmp = (0xe0 | (unicode >> 12));//
			tmp += 1;
			*tmp = 0x80 | ((unicode >> 6) & 0x003F);
			tmp += 1;
			*tmp = 0x80 | (unicode & 0x3F);
			tmp += 1;
			outsize += 3;
		}

	}

	*tmp = '\0';

	return 0;
}
string CEmailSender::Encode(const unsigned char* Data,int DataByte)
{
	//编码表
	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	//返回值
	string strEncode;

	unsigned char Tmp[3]={0};
	int LineLength=0;

	for(int i=0;i<(int)(DataByte / 3);i++)
	{
		Tmp[0] = *Data++;
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;

		strEncode+= EncodeTable[Tmp[0] >> 2];
		strEncode+= EncodeTable[((Tmp[0] << 4) | (Tmp[1] >> 4)) & 0x3F];
		strEncode+= EncodeTable[((Tmp[1] << 2) | (Tmp[2] >> 6)) & 0x3F];
		strEncode+= EncodeTable[Tmp[2] & 0x3F];
		if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
	}

	//对剩余数据进行编码
	int Mod=DataByte % 3;
	if(Mod==1)
	{
		Tmp[0] = *Data++;
		strEncode+= EncodeTable[(Tmp[0] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[0] & 0x03) << 4)];
		strEncode+= "==";
	}
	else if(Mod==2)
	{
		Tmp[0] = *Data++;
		Tmp[1] = *Data++;
		strEncode+= EncodeTable[(Tmp[0] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[0] & 0x03) << 4) | ((Tmp[1] & 0xF0) >> 4)];
		strEncode+= EncodeTable[((Tmp[1] & 0x0F) << 2)];
		strEncode+= "=";
	}

	return strEncode;
}
string CEmailSender::Decode(const char* Data,int DataByte,int& OutByte)
{
	//解码表
	const char DecodeTable[] =
	{
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		62, // '+'
		0, 0, 0,
		63, // '/'
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
		0, 0, 0, 0, 0, 0, 0,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
		0, 0, 0, 0, 0, 0,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
	};

	//返回值
	string strDecode;

	int nValue;
	int i= 0;

	while (i < DataByte)
	{
		if (*Data != '\r' && *Data!='\n')
		{
			nValue = DecodeTable[*Data++] << 18;
			nValue += DecodeTable[*Data++] << 12;
			strDecode+=(nValue & 0x00FF0000) >> 16;
			OutByte++;

			if (*Data != '=')
			{
				nValue += DecodeTable[*Data++] << 6;
				strDecode+=(nValue & 0x0000FF00) >> 8;
				OutByte++;

				if (*Data != '=')
				{
					nValue += DecodeTable[*Data++];
					strDecode+=nValue & 0x000000FF;
					OutByte++;
				}
			}
			i += 4;
		}
		else// 回车换行,跳过
		{
			Data++;
			i++;
		}
	}
	return strDecode;
}

/**
 * @file WxApp.cpp
 * @Synopsis  wxWidgets  test
 * @author yin  wei
 * @version 
 * @date 2017-12-24
 */

#include "wx/wx.h"
//����Ӧ�ó�����
class	MyApp:public wxApp
{
	virtual bool OnInit();
}

DECLARE_APP(MyApp)

//ָ���ĸ�ΪӦ�ó�����
IMPLEMENT_APP(MyApp)

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  ��������
 */
/* ----------------------------------------------------------------------------*/
class MyFrame: public wxFrame
{
	public:
		MyFrame(const wxString &title);

		//event handler
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
	private:
		//event table
		DECLARE_EVENT_TABLE()
}


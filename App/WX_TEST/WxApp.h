/**
 * @file WxApp.cpp
 * @Synopsis  wxWidgets  test
 * @author yin  wei
 * @version 
 * @date 2017-12-24
 */

#include "wx/wx.h"
//定义应用程序类
class	MyApp:public wxApp
{
	virtual bool OnInit();
}

DECLARE_APP(MyApp)

//指明哪个为应用程序类
IMPLEMENT_APP(MyApp)

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  主窗口类
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


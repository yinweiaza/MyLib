#include "WxApp.h"

bool MyApp::OnInit()
{
	MyFrame *pFrame = new MyFrame(wxT("my first wxWidgets gui"));
	pFrame->Show(true);
	return true;
}

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
	EVT_MENU(wxID_ABOUT,MyFrame::OnAbout)
	EVT_MENU(wxID_EXIT,MyFrame::OnQuit)
END_EVENT_TABLE()

MyFrame::MyFrame(const wxString &title)
	:wxFrame(NULL,wxID_ANY,title)
{
	//创建菜单项
	wxMenu *pFileMenu = new wxMenu;
	wxMenu *pHelpMenu = new wxMenu;
	pFileMenu->Append(wxID_EXIT,wxT("E&xit\tAlt-X"),wxT("QUIT this program"));

	pHelpMenu->Append(wxID_ABOUT,wxT("&About...\tF1"),wxT("Show about dialog"));
	wxMenuBar *pMenuBar = new wxMenuBar();
	pMenuBar->Append(pFileMenu,wxT("&File"));
	pMenuBar->Append(pHelpMenu,wxT("&Help"));

	//把菜单条放置到主窗口上
	SetMenuBar(pMenuBar);
	
	//create status
	CreateStatusBar(2);
	SetStatusText(wxT("Welcome to wxWidgets!"));
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  about function
 *
 * @Param event
 */
/* ----------------------------------------------------------------------------*/
void MyFrame::OnAbout(wxCommandEvent& event)
{
	 
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  exit handler fun
 *
 * @Param event
 */
/* ----------------------------------------------------------------------------*/
void MyFrame::OnQuit(wxCommandEvent& event)
{
	 
}

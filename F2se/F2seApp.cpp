/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#include "F2seApp.h"
#include "DataModel.h"
#include "MemBufferEX.h"

#include <exception>
#include <csignal>

//(*AppHeaders
#include "F2seMain.h"
#include <wx/image.h>
//*)
#include <wx/msgdlg.h>

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#endif // _MSC_VER

//IMPLEMENT_APP(F2seApp);
IMPLEMENT_APP_NO_MAIN(F2seApp);

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif // _MSC_VER

bool F2seApp::OnInit()
    {
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	F12seFrame* frame = new F12seFrame(0);
    	frame->Show();
    	SetTopWindow(frame);
    }
    //*)
    return wxsOK;
    }

#ifndef _MSC_VER
int main(int argc, char** argv)
    {
    try
        {
        return wxEntry(argc, argv);
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, wxCmdLineArgType /*lpCmdLine*/, int nCmdShow)
    {
    try
        {
        return wxEntry(hInstance, hPrevInstance, NULL, nCmdShow);
#endif
        }
    catch (std::exception& e)
        {
        wxMessageBox(wxString("Exception:") + e.what(), "F12se crash", wxOK|wxCENTRE|wxICON_ERROR);
        }
    catch (...)
        {
        wxMessageBox("Uknown exception!", "F12se crash", wxOK|wxCENTRE|wxICON_ERROR);
        }
    return 1;
    }

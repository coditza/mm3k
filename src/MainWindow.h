#pragma once
#include "stdafx.h"
#include "GameObject.h"
#include <wx/dcbuffer.h>

class CMainWindow: public wxFrame
{
public:

    CMainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, long style);
    ~CMainWindow();
    void SetupMenu();
    void ChangeMenu();
    
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    void OnPauseGame(wxCommandEvent& event);
    void OnRunGame(wxCommandEvent& event);
    void OnStopGame(wxCommandEvent& event);
    
    void OnRandomize(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    
    void OnPaint(wxPaintEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    
    void OnProgressTimer(wxTimerEvent& event);
    
    void OnMouseDoubleClick(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    
    DECLARE_EVENT_TABLE()



private:
    enum
    {
        ID_Quit = 1,
        ID_About,
        ID_Game_Pause,
        ID_Game_Stop,
        ID_Game_Start,
        ID_Randomize,
        ID_Refresh,
        ID_Timer,
    };

    CGameObject m_gameObject;
    wxMenu *m_menuFile;
    wxMenu *m_menuGame;
    wxMenuBar *m_menuBar;  

private:
    void DrawMemoryPanel(wxBufferedPaintDC &dc);
    void DrawInfoPanel(wxBufferedPaintDC &dc);
    void DrawQueuePanel(wxBufferedPaintDC &dc);
    void DrawProgramSquare(wxBufferedPaintDC &dc, wxBitmap bitmap,wxPoint src_offset, wxSize src_size, wxPoint target);
    void DrawUserPanel(wxBufferedPaintDC &dc);
    void DrawScorePanel(wxBufferedPaintDC &dc);
    
    wxPoint ExtractPoint(int x, int y, int &page, int &byte);
        
    wxBitmap m_infoBitmap;
    wxBitmap m_boxesBitmap;
    wxBitmap m_smilesBitmap;
    wxBitmap m_commandsBitmap;
    
    wxTimer *m_timer;
    time_t m_seconds;
    wxRect m_selectedRect;
};
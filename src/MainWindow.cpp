#include "MainWindow.h"

BEGIN_EVENT_TABLE(CMainWindow, wxFrame)
    // menu
    EVT_MENU(ID_Quit,  CMainWindow::OnQuit)
    EVT_MENU(ID_About, CMainWindow::OnAbout)
    
    // game menu
    EVT_MENU(ID_Game_Stop, CMainWindow::OnStopGame)
    EVT_MENU(ID_Game_Start, CMainWindow::OnRunGame)
    
    EVT_LEFT_DCLICK(CMainWindow::OnMouseDoubleClick)
    EVT_MOTION(CMainWindow::OnMouseMove)
    
    // timer
    EVT_TIMER(ID_Timer, CMainWindow::OnProgressTimer)
    
    // paint
    EVT_PAINT(CMainWindow::OnPaint)
    EVT_ERASE_BACKGROUND(CMainWindow::OnEraseBackground)

END_EVENT_TABLE()

#define MATRIX_PANEL_WIDTH  426
#define MATRIX_PANEL_HEIGHT 342 
#define INFO_PANEL_WIDTH    133
#define INFO_PANEL_HEIGHT   86
#define QUEUE_PANEL_WIDTH   133
#define QUEUE_PANEL_HEIGHT  106
#define USER_PANEL_WIDTH    133
#define USER_PANEL_HEIGHT   133

#define SCORE_PANEL_WIDTH   133
#define SCORE_PANEL_HEIGHT  100


CMainWindow::CMainWindow(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, -1, title, pos, size, style)
{
    SetBackgroundColour( wxColour( 255, 255, 255 ) );
    //*
    CreateStatusBar();
    SetStatusText( _("Welcome to wxWidgets!") );
    //*/
    m_gameObject.SetGameState(GS_STOPPED);
    SetupMenu();
    
    // load bitmaps
    m_infoBitmap = wxBitmap(_("info.bmp"),wxBITMAP_TYPE_BMP);
    m_boxesBitmap = wxBitmap(_("allboxes.bmp"),wxBITMAP_TYPE_BMP);
    m_smilesBitmap = wxBitmap(_("smiles.bmp"),wxBITMAP_TYPE_BMP);
    m_commandsBitmap = wxBitmap(_("addremove.bmp"),wxBITMAP_TYPE_BMP);
    
    m_timer = new wxTimer(this, ID_Timer);
    m_seconds = time(NULL);
    
    m_selectedRect = wxRect(-1,-1,-1,-1);
}

CMainWindow::~CMainWindow()
{
    if (m_timer)
    {
        m_timer->Stop();
        delete m_timer;
        m_timer = NULL;
    }
}

void CMainWindow::SetupMenu()
{
    m_menuFile = new wxMenu;

    m_menuFile->Append( ID_About, _("&About...") );
    m_menuFile->AppendSeparator();
    m_menuFile->Append( ID_Quit, _("E&xit") );
    
    // game stuff
    m_menuGame = new wxMenu;
    m_menuGame->Append(ID_Game_Stop, _("&Stop"));
    m_menuGame->Append(ID_Game_Start, _("&Start"));
    
    // menu bar
    m_menuBar = new wxMenuBar;
    m_menuBar->Append( m_menuFile, _("&File") );
    m_menuBar->Append( m_menuGame, _("&Game") );

    SetMenuBar( m_menuBar );
    
    ChangeMenu();
}

void CMainWindow::ChangeMenu()
{
    wxString str;
    wxMenuItem *pos;
    int gameState = m_gameObject.GetGameState();
    switch(gameState)
    {
        case GS_RUNNING:
            //remove start item
            pos = m_menuGame->FindItem(ID_Game_Start);
            if (pos)
            {
                // remove it
                m_menuGame->Delete(ID_Game_Start);
            }
            
            // add pause/stop
            pos = m_menuGame->FindItem(ID_Game_Stop);
            if (!pos)
            {
                m_menuGame->Append(ID_Game_Stop, _("&Stop"));
            }
            str.Append(_("Game State: running"));
            break;
        case GS_STOPPED:
        default:
            //remove stop and pause
            pos = m_menuGame->FindItem(ID_Game_Stop);
            if (pos)
            {
                // remove it
                m_menuGame->Delete(ID_Game_Stop);
            }
            
            // add start
            pos = m_menuGame->FindItem(ID_Game_Start);
            if (!pos)
            {
                m_menuGame->Append(ID_Game_Start, _("&Start"));
            }
            str.Append(_("Game State: stopped"));
            break;            
    }
    SetStatusText( str );
}

void CMainWindow::OnRandomize(wxCommandEvent& WXUNUSED(event))
{
    m_gameObject.RandomizeMatrix();
    this->Refresh(true);
}

void CMainWindow::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
    this->Refresh(true);
}

void CMainWindow::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void CMainWindow::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _("This is a wxWidgets Hello world sample"),
                  _("About Hello World"), 
                  wxOK | wxICON_INFORMATION, this );
}

void CMainWindow::OnPauseGame(wxCommandEvent& WXUNUSED(event))
{
    m_gameObject.SetGameState(GS_PAUSED);
    ChangeMenu();
    m_timer->Stop();
}

void CMainWindow::OnRunGame(wxCommandEvent& WXUNUSED(event))
{
    m_gameObject.Reset();
    m_gameObject.SetGameState(GS_RUNNING);
    ChangeMenu();
    m_timer->Start(500);
}

void CMainWindow::OnStopGame(wxCommandEvent& WXUNUSED(event))
{
    m_gameObject.SetGameState(GS_STOPPED);
    ChangeMenu();
    m_timer->Stop();
}

void CMainWindow::OnProgressTimer(wxTimerEvent& event)
{
    m_gameObject.Simulate();
    if (m_gameObject.GetGameState()==GS_GAME_OVER)
    {
        // show game over message/ stop everything
        m_gameObject.SetGameState(GS_STOPPED);
        wxString s = _("OMG you killed THE USER! You bastard!\n");
        s += wxString::Format(_("You kept him alive for %d seconds."),m_gameObject.GetRunTime());
        wxMessageDialog dialog(this,s,_("Game Over"));
        
        m_gameObject.Reset();
        m_timer->Stop();
        dialog.ShowModal();
        ChangeMenu();
    }
    this->Refresh(true);
}


// paint
void CMainWindow::OnPaint(wxPaintEvent& event)
{
    // clear screen
    wxBufferedPaintDC myDC(this);   
    PrepareDC(myDC);
    
    wxRect windowRect(wxPoint(0, 0), GetClientSize());
    
    myDC.SetBrush(wxBrush(*wxWHITE_BRUSH));
    myDC.SetPen(wxPen(*wxWHITE_PEN));
    myDC.DrawRectangle(windowRect);
    
    // memory panel
    DrawMemoryPanel(myDC);
    // info panel
    DrawInfoPanel(myDC);
    // queue panel
    DrawQueuePanel(myDC);
    // user panel
    DrawUserPanel(myDC);
    // draw score
    //DrawScorePanel(myDC);
    
    //wxString s = wxString::Format(_("User mood: %d"),m_gameObject.GetUserMood());;
   
}

void CMainWindow::DrawScorePanel(wxBufferedPaintDC &dc)
{
    int width = SCORE_PANEL_WIDTH;
    int height = SCORE_PANEL_HEIGHT;
    
    wxPoint offset = wxPoint(5+MATRIX_PANEL_WIDTH+5+USER_PANEL_WIDTH+5, 5);
    wxRect panelRect( offset , wxSize(width+2,height+2));
    
    dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
    dc.SetPen(wxPen(*wxBLACK_PEN));
    dc.DrawRectangle(panelRect);
    
    wxString s = _("Current time:\n");
    int runTime = m_gameObject.GetRunTime();
    int hours = runTime/3600;
    if (hours > 99) {
        s += _("ARE YOU NUTS!?");
    } else {
        int minutes = (runTime - hours*3600)/60;
        int seconds = runTime - hours*3600 - minutes*60;
        s += wxString::Format(_("%02d:%02d:%02d"),hours,minutes,seconds);
    }
    
    dc.DrawText(s,offset.x+5, offset.y+5);
}

void CMainWindow::DrawMemoryPanel(wxBufferedPaintDC &dc)
{
    wxSize memPanelSize(MATRIX_PANEL_WIDTH,MATRIX_PANEL_HEIGHT);
    wxPoint offset(5,5);
    int boxSize = 16;
    
    // panel
    wxRect windowRect(wxPoint(0, 0), GetClientSize());
    wxRect panelRect(offset, memPanelSize);
    
    dc.SetBrush(wxBrush(wxColour(200,200,200)));
    dc.SetPen(wxPen(*wxBLACK_PEN));
    dc.DrawRectangle(panelRect);
    
    int lineOffsetY = offset.y+1+boxSize;
    for (int c=1;c<20;c++)
    {
        dc.DrawLine(offset.x+1,lineOffsetY,offset.x+memPanelSize.GetWidth()-1,lineOffsetY);
        lineOffsetY = lineOffsetY+1+boxSize;
    }
    
    int lineOffsetX = offset.x+1+boxSize;
    for (int c=1;c<25;c++)
    {
        dc.DrawLine(lineOffsetX,offset.y+1,lineOffsetX,offset.y+memPanelSize.GetHeight()-1);
        lineOffsetX = lineOffsetX+1+boxSize;
    }
    
    
    // test boxes    
    int boxOffsetY = offset.y+1;
    for (int c=0;c<20;c++)
    {
        int boxOffsetX = offset.x+1;
        for (int f=0;f<25;f++) 
        {
            int val = m_gameObject.GetMemoryLocation(c,f);
            wxPoint sprite;
            switch(val)
            {
                case 1:
                    sprite = wxPoint(0,0);
                    break;
                case 2:
                    sprite = wxPoint(16,0);
                    break;
                case 3:
                    sprite = wxPoint(32,0);
                    break;
                case 4:
                    sprite = wxPoint(48,0);
                    break;
                case 5:
                    sprite = wxPoint(64,0);
                    break;
            }
            if (val) {
                //DrawProgramSquare(dc,m_boxesBitmap,wxPoint(0,0),wxSize(16,16),wxPoint(boxOffsetX,boxOffsetY));
                DrawProgramSquare(dc,m_boxesBitmap,sprite,wxSize(16,16),wxPoint(boxOffsetX,boxOffsetY));
            }
            boxOffsetX = boxOffsetX + 1 + boxSize;
        }    
        boxOffsetY = boxOffsetY+1+boxSize;
    }
    
    if (m_selectedRect.GetWidth() != -1 && m_gameObject.GetNextOp()) 
    {
        wxRect tempRect = m_selectedRect;
        dc.SetBrush(wxBrush(wxColour(0,150,0)));
        dc.SetPen(wxPen(wxColour(0,255,0),1));
        
        for (int i=0;i<m_gameObject.GetNextOp()->lenght;i++)
        {
            dc.DrawRectangle(tempRect);
            tempRect.Offset(boxSize+1,0);
        }
    }
    
}

void CMainWindow::DrawInfoPanel(wxBufferedPaintDC &dc)
{
    wxSize windowSize = GetClientSize();
    
    int width = INFO_PANEL_WIDTH;
    int height = INFO_PANEL_HEIGHT;
    //wxPoint offset = wxPoint(windowSize.GetWidth() - 5 - width, windowSize.GetHeight()-height-5);
    wxPoint offset = wxPoint(5+MATRIX_PANEL_WIDTH+5, 5+USER_PANEL_HEIGHT+5+QUEUE_PANEL_HEIGHT+5);
    
    wxRect panelRect( offset , wxSize(width+2,height+2));
    
    dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
    dc.SetPen(wxPen(*wxBLACK_PEN));
    dc.DrawRectangle(panelRect);
    dc.DrawBitmap(m_infoBitmap, offset.x+1,offset.y+1);
}

void CMainWindow::DrawProgramSquare(wxBufferedPaintDC &dc, wxBitmap bitmap,wxPoint src_offset, wxSize src_size, wxPoint target)
{
    wxRect subBmpRect(src_offset.x, src_offset.y, src_size.GetWidth(), src_size.GetHeight());
    dc.DrawBitmap(bitmap.GetSubBitmap(subBmpRect),target.x, target.y);
}

void CMainWindow::OnEraseBackground(wxEraseEvent& event)
{
}

void CMainWindow::DrawQueuePanel(wxBufferedPaintDC &dc)
{
    wxSize windowSize = GetClientSize();
    
    int width = QUEUE_PANEL_WIDTH;
    int height = QUEUE_PANEL_HEIGHT;
    //wxPoint offset = wxPoint(windowSize.GetWidth() - 5 - width, windowSize.GetHeight()-height-5-INFO_PANEL_HEIGHT-5 );
    wxPoint offset = wxPoint(5+MATRIX_PANEL_WIDTH+5, 5+USER_PANEL_HEIGHT+5);
    
    wxRect panelRect( offset , wxSize(width+2,height+2));
    
    dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
    dc.SetPen(wxPen(*wxBLACK_PEN));
    dc.DrawRectangle(panelRect);
    
    // draw the squares
    std::vector<OP_QUEUE_ITEM *> programsQueue = m_gameObject.GetQueue();
    std::vector<OP_QUEUE_ITEM *>::const_iterator iter;
    
    int yOffset = offset.y+3+1;
    
    for (iter = programsQueue.begin(); iter != programsQueue.end(); iter++)
    {
        int xOffset = offset.x+5;
        int val = (*iter)->program;
        wxPoint sprite;
        switch(val)
        {
            case 1:
                sprite = wxPoint(0,0);
                break;
            case 2:
                sprite = wxPoint(16,0);
                break;
            case 3:
                sprite = wxPoint(32,0);
                break;
            case 4:
                sprite = wxPoint(48,0);
                break;
            case 5:
                sprite = wxPoint(64,0);
                break;
        }
        
        // draw the command
        wxPoint cmdSprite;
        cmdSprite.y = 0;
        if ((*iter)->op == OP_INSERT)
        {
            cmdSprite.x = 0;
        } else {
            cmdSprite.x = 16;
        }
        DrawProgramSquare(dc,m_commandsBitmap,cmdSprite,wxSize(16,16),wxPoint(xOffset,yOffset));
        xOffset += 16+2;
        
        for (int i = 0; i < (*iter)->lenght; i++)
        {
            DrawProgramSquare(dc,m_boxesBitmap,sprite,wxSize(16,16),wxPoint(xOffset,yOffset));
            xOffset += 16;
        }
        yOffset += 16+1;
    }
}
void CMainWindow::DrawUserPanel(wxBufferedPaintDC &dc)
{
    wxSize windowSize = GetClientSize();
    
    int width = USER_PANEL_WIDTH;
    int height = USER_PANEL_HEIGHT;
    wxPoint offset = wxPoint(5+MATRIX_PANEL_WIDTH+5, 5);
    
    wxRect panelRect( offset , wxSize(width+2,height+2));
    
    dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
    dc.SetPen(wxPen(*wxBLACK_PEN));
    dc.DrawRectangle(panelRect);
    
    int val = m_gameObject.GetUserMood()/2;
    wxPoint sprite;
    switch(val)
    {
        case 5:
            sprite = wxPoint(0,0);
            break;
        case 4:
            sprite = wxPoint(120,0);
            break;
        case 3:
            sprite = wxPoint(240,0);
            break;
        case 2:
            sprite = wxPoint(360,0);
            break;
        case 1:
            sprite = wxPoint(480,0);
            break;
    }
    wxRect subBmpRect(sprite.x, sprite.y, 120, 120);
    wxPoint target;
    target.x = offset.x + ((width-120)/2);
    target.y = offset.y + ((height-120)/2);
    dc.DrawBitmap(m_smilesBitmap.GetSubBitmap(subBmpRect),target.x, target.y);
}

void CMainWindow::OnMouseDoubleClick(wxMouseEvent& event)
{
    wxSize memPanelSize(MATRIX_PANEL_WIDTH,MATRIX_PANEL_HEIGHT);
    wxPoint offset(5,5);
        
    // panel
    wxRect windowRect(wxPoint(0, 0), GetClientSize());
    wxRect panelRect(offset, memPanelSize);
    
    if (panelRect.Contains(event.m_x,event.m_y) && m_gameObject.GetGameState() == GS_RUNNING)
    {
        // we are in the matrix
        int page = 0;
        int byte = 0;
        wxPoint tPoint = ExtractPoint(event.m_x, event.m_y, page, byte);
        if (tPoint.x != -1)
        {
            m_gameObject.OpApply(page, byte);
        }
    }
}

void CMainWindow::OnMouseMove(wxMouseEvent& event)
{
    wxSize memPanelSize(MATRIX_PANEL_WIDTH,MATRIX_PANEL_HEIGHT);
    wxPoint offset(5,5);
    int boxSize = 16;
    wxRect tempRect;
    // panel
    wxRect windowRect(wxPoint(0, 0), GetClientSize());
    wxRect panelRect(offset, memPanelSize);
    if (panelRect.Contains(event.m_x,event.m_y) && m_gameObject.GetGameState() == GS_RUNNING)
    {
        // we are in the matrix
        int page = 0;
        int byte = 0;
        m_selectedRect = wxRect(-1,-1,-1,-1);
        wxPoint tPoint = ExtractPoint(event.m_x, event.m_y, page, byte);
        
        if (tPoint.x != -1 && m_gameObject.OpFits(page, byte)) {
            m_selectedRect = wxRect(tPoint.x, tPoint.y, 16, 16);
        }
    } 
}


wxPoint CMainWindow::ExtractPoint(int x, int y, int &page, int &byte)
{
    wxSize memPanelSize(MATRIX_PANEL_WIDTH,MATRIX_PANEL_HEIGHT);
    wxPoint offset(5,5);
    int boxSize = 16;
    wxRect tempRect;
    // panel
    wxRect windowRect(wxPoint(0, 0), GetClientSize());
    wxRect panelRect(offset, memPanelSize);
    
    wxPoint ret(-1,-1);
    
    // lame but I can't seem to think straight
    int boxOffsetY = offset.y+1;
    for (int c=0;c<20;c++)
    {
        int boxOffsetX = offset.x+1;
        for (int f=0;f<25;f++) 
        {
            tempRect = wxRect(boxOffsetX,boxOffsetY,16,16);
            if (tempRect.Contains(x,y))
            {
                ret = wxPoint(boxOffsetX,boxOffsetY);
                page = c;
                byte = f;
                break;
            }
            
            boxOffsetX = boxOffsetX + 1 + boxSize;
        }  
        if (ret.x != -1)  
        {
            break;
        }
        boxOffsetY = boxOffsetY+1+boxSize;
    }
    
    return ret;
}

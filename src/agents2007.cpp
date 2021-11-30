/*******************************************************************************
**                          A G E N T S   2 0 0 7                              *
********************************************************************************
**  projekt:      agents2007                                                   *
**  autori:       xsusma00, xsmejk13, xnagye00, xtrnko00                       *
*******************************************************************************/

#include "agents2007.h"
#include <iostream>
//#include <math.h>
//#include <wx/wx.h>
//#include "/images/info.xpm"
/*==============================STRING CONVERSION=============================*/

std::string wx2std(const wxString& input, wxMBConv* conv) { 
        if (input.empty()) { 
                return ""; 
        } 

        if (conv == NULL) { 
                conv = wxConvCurrent; 
        } 

        return std::string(input.mb_str(*conv)); 
} 

wxString std2wx(const std::string& input, wxMBConv* conv) { 
        if (input.empty()) { 
                return wxEmptyString; 
        } 

        if (conv == NULL) { 
                conv = wxConvCurrent; 
        } 

        return wxString(input.c_str(), *conv); 
}

/*==============================STRING CONVERSION=============================*/

/*===================================MainApp::BEGIN===========================*/

bool MainApp::OnInit() {

    AgentsFrm *MainWin = new AgentsFrm((wxWindow *) NULL);

    MainWin->SetTitle(wxT("Agents2007"));
    //MainWin->SetIcon("agents.ico");
    //MainWin->SetSize(8,8,853,516);
	MainWin->Center();
    MainWin-> Show(TRUE);
    SetTopWindow(MainWin);
    return TRUE;
}

/*===================================MainApp::END=============================*/


/*===================================PaintWindow::BEGIN=======================*/

PaintWindow::PaintWindow(wxWindow* parent, wxPoint pos, wxSize size)
   : wxScrolledWindow(parent, -1, pos, size/*, wxALWAYS_SHOW_SB*/) {

    zoom=DEFAULT_ZOOM;
    x=0, y=0;
    
    scene = new SCENA((int)(492/21), (int)(492/21));
    loadscena(&(this->scene), "saves/default.sc.xml");
    //scene->printscena();

    // This code will load picture (bmp, gif, ..) into bit and set mask on pink
    wxImage *img;   // pointer to load image
    img = new wxImage(wxT("images/agent.bmp"), wxBITMAP_TYPE_ANY, 0);
    agentU = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentR = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentD = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentL = new wxBitmap(*img);

    wxMask *maskaU = new wxMask(*agentU, wxColour(255,0,255));
    wxMask *maskaR = new wxMask(*agentR, wxColour(255,0,255));
    wxMask *maskaD = new wxMask(*agentD, wxColour(255,0,255));
    wxMask *maskaL = new wxMask(*agentL, wxColour(255,0,255));

    agentU->SetMask(maskaU);
    agentR->SetMask(maskaR);
    agentD->SetMask(maskaD);
    agentL->SetMask(maskaL);

    img = new wxImage(wxT("images/agentcase.bmp"), wxBITMAP_TYPE_ANY, 0);
    agentcaseU = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentcaseR = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentcaseD = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentcaseL = new wxBitmap(*img);

    maskaU = new wxMask(*agentcaseU, wxColour(255,0,255));
    maskaR = new wxMask(*agentcaseR, wxColour(255,0,255));
    maskaD = new wxMask(*agentcaseD, wxColour(255,0,255));
    maskaL = new wxMask(*agentcaseL, wxColour(255,0,255));

    agentcaseU->SetMask(maskaU);
    agentcaseR->SetMask(maskaR);
    agentcaseD->SetMask(maskaD);
    agentcaseL->SetMask(maskaL);

    img = new wxImage(wxT("images/agentoncase.bmp"), wxBITMAP_TYPE_ANY, 0);
    agentoncaseU = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentoncaseR = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentoncaseD = new wxBitmap(*img);
    *img = img->Rotate90(true);
    agentoncaseL = new wxBitmap(*img);

    maskaU = new wxMask(*agentoncaseU, wxColour(255,0,255));
    maskaR = new wxMask(*agentoncaseR, wxColour(255,0,255));
    maskaD = new wxMask(*agentoncaseD, wxColour(255,0,255));
    maskaL = new wxMask(*agentoncaseL, wxColour(255,0,255));

    agentoncaseU->SetMask(maskaU);
    agentoncaseR->SetMask(maskaR);
    agentoncaseD->SetMask(maskaD);
    agentoncaseL->SetMask(maskaL);

    img = new wxImage(wxT("images/case.bmp"), wxBITMAP_TYPE_ANY, 0);
    suitcase = new wxBitmap(*img);

    maskaU = new wxMask(*suitcase, wxColour(255,0,255));

    suitcase->SetMask(maskaU);

    bmpsize = wxSize(suitcase->GetWidth(),suitcase->GetHeight());
    
    img = new wxImage(wxT("images/wall.bmp"), wxBITMAP_TYPE_ANY, 0);
    wall = new wxBitmap(*img);
    //wxGridSizer *gsizer = new wxGridSizer(0,0);
    //gsizer->Add( new wxStaticBitmap(this,*img), 10, wxALIGN_RIGHT);
    //SetSizer(gsizer);
}

void PaintWindow::OnPaint(wxPaintEvent& event) {


    //wxBufferedPaintDC dc(this);
    wxPaintDC dc (this);
    
    //PrepareDC(dc);
    //DoPrepareDC(dc);
    //dc.BeginDrawing();
    dc.SetUserScale(zoom,zoom);
    //cout << "OnPaint: " << zoom << endl;
    
    GetViewStart(&x,&y);
    x=x*(-10); y=y*(-10);
    dc.SetDeviceOrigin(x,y);
    //wxInitAllImageHandlers();
    //ClearBackground();
    
    //DrawWalls(&dc, wxSize(492,492));
    DrawScene(&dc, true);
    //dc.EndDrawing();
}

void PaintWindow::OnDraw(){

    wxClientDC dc (this);
    dc.SetUserScale(zoom,zoom);
    GetViewStart(&x,&y);
    x=x*(-10); y=y*(-10);
    dc.SetDeviceOrigin(x,y);
    DrawScene(&dc, false);
}

void PaintWindow::DrawTest(wxDC *dc) {
    
    DrawGround(dc, wxSize(492,492));
    wxPoint pos;
    for(int i=15; i < 492/21; i+=1)
        for(int j=15; j < 492/21; j+=1) {
            pos = GetObjectPosition(i, j);
            DrawObject(dc, suitcase, pos, pos);
        }
}

void PaintWindow::DrawPosition(wxDC *dc, const wxPoint& pos_scene) {

    wxPoint pos_pixel;
    
    pos_pixel = GetObjectPosition(pos_scene.x, pos_scene.y);    
    
    switch(scene->cojetam(pos_scene.x, pos_scene.y)){
        case(VOLNO): 
            if(scene->cotambolo(pos_scene.x, pos_scene.y) == STENA) EraseWall(dc, pos_pixel); 
            else EraseObject(dc, pos_pixel); break;
        case(STENA): 
            dc->DrawBitmap(*wall, pos_pixel.x-1, pos_pixel.y-1, true); break;
        case(PD): 
            DrawObject(dc, suitcase, pos_pixel, pos_pixel); break;
        case(AU): 
            DrawObject(dc, agentU, pos_pixel, pos_pixel); break;
        case(AD): 
            DrawObject(dc, agentD, pos_pixel, pos_pixel); break;
        case(AL): 
            DrawObject(dc, agentL, pos_pixel, pos_pixel); break;
        case(AR): 
            DrawObject(dc, agentR, pos_pixel, pos_pixel); break;
        case(AUPD): 
            DrawObject(dc, agentoncaseU, pos_pixel, pos_pixel); break;
        case(ADPD): 
            DrawObject(dc, agentoncaseD, pos_pixel, pos_pixel); break;
        case(ALPD): 
            DrawObject(dc, agentoncaseL, pos_pixel, pos_pixel); break;
        case(ARPD): 
            DrawObject(dc, agentoncaseR, pos_pixel, pos_pixel); break;
        case(AUPH): 
            DrawObject(dc, agentcaseU, pos_pixel, pos_pixel); break;
        case(ADPH): 
            DrawObject(dc, agentcaseD, pos_pixel, pos_pixel); break;
        case(ALPH): 
            DrawObject(dc, agentcaseL, pos_pixel, pos_pixel); break;
        case(ARPH): 
            DrawObject(dc, agentcaseR, pos_pixel, pos_pixel); break;
    }
}

void PaintWindow::DrawScene(wxDC *dc, bool flag) {

    wxPoint pos_scene;
    for(int i=0; i < scene->rozmery; i++)
        for(int j=0; j < scene->rozmerx; j++) {
        
            if(scene->pole[scene->rozmerx*i+j].nova != 
               scene->pole[scene->rozmerx*i+j].stara || flag){
               
                pos_scene.x=j; pos_scene.y=i;            
                DrawPosition(dc, pos_scene);
          
            }
        }
      
}
        
void PaintWindow::DrawGround(wxDC *dc, wxSize size) {

    /*dc->SetBrush(wxBrush(wxColour(55,50,120)));
    dc->SetPen(wxPen(wxColour(180,180,180),RECTWIDTH));
    //wxSize xxx = GetClientSize();
    //wxSize xx = GetSize();
    //int x1 = xxx.GetWidth(), x2 = xxx.GetHeight() ,xx1 = xx.GetWidth(),xx2 = xx.GetHeight();
    //cout << endl << "GetClientSize: " << x1 << " " << x2
    //     << endl << "GetSize      : " << xx1 << " " << xx2 << endl;
    wxRect windowRect(wxPoint(0,0), size);

    //windowRect.x -= 2; windowRect.y -= 2;
    //windowRect.width += 4; windowRect.height += 4;
    dc->DrawRectangle(windowRect);*/
}

void PaintWindow::DrawWalls(wxDC *dc, wxSize size) {
    int Width = size.GetWidth()/21;
    int Height = size.GetHeight()/21;
    wxPoint pos;
    for(int i=0; i < Width; i++)
        for(int j=0; j < Height; j++) {
            if(i == 0 || j == 0 || i == Width-1 || j == Height-1) {
                pos = GetObjectPosition(i, j);
                //pos.x-=1; pos.y-=1;        
                //DrawObject(dc, wall, pos, wxPoint(pos.x-1,pos.y-1));
                dc->DrawBitmap(*wall, pos.x-1, pos.y-1, true);
            }
        }    
}

void PaintWindow::EraseObject(wxDC *dc, const wxPoint& pos) {

    //dc->SetPen(*wxTRANSPARENT_PEN);
    //dc->SetBrush(dc->GetBackground());
    dc->SetBrush(wxBrush(wxColour(55,50,120),0));
    dc->SetPen(/**wxWHITE*/wxPen(wxColour(55,50,120),0,0));
    wxRect objectRect(pos, wxSize(bmpsize.GetWidth(), bmpsize.GetHeight()));
    dc->DrawRectangle(objectRect);
}

void PaintWindow::EraseWall(wxDC *dc, const wxPoint& pos) {

    dc->SetBrush(wxBrush(wxColour(55,50,120),0));
    dc->SetPen(/**wxWHITE*/wxPen(wxColour(55,50,120),0,0));
    wxRect objectRect(wxPoint(pos.x-1, pos.y-1), wxSize(wall->GetWidth(), wall->GetHeight()));
    dc->DrawRectangle(objectRect);
}

void PaintWindow::DrawObject(wxDC *dc, wxBitmap *bit,const wxPoint& oldpos,
                               const wxPoint& newpos) {

    EraseObject(dc, oldpos);
    dc->DrawBitmap(*bit, newpos.x, newpos.y, true);

}

// Input scene position , returns position in pixels.
wxPoint PaintWindow::GetObjectPosition(int x, int y) {

    return wxPoint(x*(bmpsize.GetWidth()+GRIDSPACE)+RECTWIDTH,
                   y*(bmpsize.GetHeight()+GRIDSPACE)+RECTWIDTH);
}

// Input position in pixels, returns position in scene.
wxPoint PaintWindow::GetScenePosition(const wxPoint& pos) {

    int width = bmpsize.GetWidth()+GRIDSPACE, 
        height = bmpsize.GetHeight()+GRIDSPACE;
        
    if(width == 0 || height == 0) // We can not divide by zero.
        return wxPoint(-1,-1);
        
    wxPoint pos_pom =  wxPoint((pos.x-RECTWIDTH)/width ,
                               (pos.y-RECTWIDTH)/height);
                               
    if(pos_pom.x >= scene->rozmerx || pos_pom.y >= scene->rozmery) 
        return wxPoint(-1,-1);
    return pos_pom;
}

// Overriding event.
void PaintWindow::OnEraseBackground(wxEraseEvent &event) {

    event.Skip();
}

// Overriding event.
void AgentsFrm::OnSize(wxSizeEvent& event)
{

   event.Skip();
}

// Overriding event.
void PaintWindow::OnScroll( wxScrollWinEvent &event ) {

    event.Skip();
}

// Overriding event.
void PaintWindow::OnMouseWheel(wxMouseEvent& event) {

    event.Skip();
}

// Handles painting in scene editor mode.
void PaintWindow::OnMouse(wxMouseEvent& event) {

    wxClientDC dc (this);
    dc.SetUserScale(zoom,zoom);
    GetViewStart(&x,&y);
    x=x*(-10); y=y*(-10);
    dc.SetDeviceOrigin(x,y);
    wxPoint pos = event.GetLogicalPosition(dc);
    pos = GetScenePosition(pos);

    ((AgentsFrm *) GetParent())->GetStatusBar()->
    PushStatusText(wxString::Format(wxT("Cursor: %d:%d"), pos.x, pos.y), 4);

    if(((AgentsFrm *) GetParent())->b_running) return;
   
    if(event.RightDown()) {
        
        scene->odlozscenu();
        scene->otocojedno(pos.x, pos.y);
        OnDraw();
        return;
    }
    
    if (event.LeftIsDown()) {
        //wxClientDC dc (this);
        //dc.SetUserScale(zoom,zoom);
        //GetViewStart(&x,&y);
        //x=x*(-10); y=y*(-10);
        //dc.SetDeviceOrigin(x,y);
        wxPoint pos_pom;
        wxPoint pos = event.GetLogicalPosition(dc);
        pos = GetScenePosition(pos);
        if(pos.x < 0 || pos.y < 0) return;
            scene->odlozscenu();
            switch(radio_choice){
                case(0):
                    scene->zmazpoziciu(pos.x ,pos.y);
                    
                    if (pos.x < scene->rozmerx-1 && pos.x > 0 && 
                        pos.y < scene->rozmery-1 && pos.y > 0 && 
                        scene->cotambolo(pos.x, pos.y) == STENA) {
                        DrawPosition(&dc, pos);
                        pos_pom.x = pos.x-1; pos_pom.y = pos.y;
                        DrawPosition(&dc, pos_pom);
                        pos_pom.x = pos.x+1; pos_pom.y = pos.y;
                        DrawPosition(&dc, pos_pom);
                        pos_pom.x = pos.x; pos_pom.y = pos.y-1;
                        DrawPosition(&dc, pos_pom);
                        pos_pom.x = pos.x; pos_pom.y = pos.y+1;
                        DrawPosition(&dc, pos_pom);
                        return;
                    }
                    break;
                case(1): 
                    scene->vlozstenu(pos.x ,pos.y); break;
                case(2): 
                    scene->vlozpredmet(pos.x ,pos.y); break;
                case(3): 
                    scene->vlozagenta(AGENT(pos.x ,pos.y, 'U', '-', scene, 
                    wx2std(((AgentsFrm *) GetParent())->choice->agscript, 
                                      wxConvUI))); break;
            }
        OnDraw();
        return;
    }
    //Refresh();
    //cout << "helloworld" << endl;
    
}


/*===================================PaintWindow::END=========================*/

/*===================================MyChoice::BEGIN==========================*/
MyChoice::MyChoice(wxWindow* parent, wxWindowID id, const wxPoint& pos , 
                   const wxSize& size, int n, const wxString choices[], 
                   long style, const wxValidator& validator, 
                   const wxString& name) 
                   : wxChoice(parent, id, pos, size, n, NULL, style, validator, 
                              name) {
    
    b_focus=false;
    //cout << "seting b_focus: " << b_focus << endl;
    
}

void MyChoice::OnChoice(wxCommandEvent& event) {
    
    //if(((AgentsFrm *) GetParent())->b_running) return;
    //cout << "choicebar clicked" << endl;
    //wxChoice *choice = event.GetId() == ID_CHOICE ? m_choice
    //                                              : m_choiceSorted;
    this->Clear();
    //wxString f = wxFindFirstFile("saves/*.ag.xml");
    //wxString choices[] = new *wxString();
    SearchForFiles();
    
    //choice-> Update();
    //choice-> Refresh();
    
    //const int sel = this->GetSelection();

    //wxClientData *dataEvt = event.GetClientObject(),
    //             *dataCtrl = choice->GetClientObject(sel);

    

    /*wxLogMessage(_T("EVT_CHOICE: item %d/%d (event/control), ")
                 _T("string \"%s\"/\"%s\", ")
                 _T("data \"%s\"/\"%s\""),
                 choice_activ = (int)event.GetInt(),
                 sel,
                 event.GetString().c_str(),
                 choice->GetStringSelection().c_str(),
                 GetDataString(dataEvt),
                 GetDataString(dataCtrl));*/
    //wxString str = this->GetStringSelection();
    //cout << "GetSelection: " << sel << endl;
    SetSelection((int)event.GetInt());
    //cout << GetStringSelection() << endl;
    agscript = GetStringSelection();
    cout << agscript << endl;
    //cout << "GetStrSelection: " << this->GetStringSelection().c_str() << endl;
}

/*void MyChoice::OnFocusGot(wxFocusEvent& event) {


    //cout << "B_FOCUS: " << b_focus << endl;
    //if(((AgentsFrm *) GetParent())->b_running) return;    
    //if (b_focus == true) return;
    //else b_focus=true;
    
    //SearchForFiles();
    //cout << "hi hou" << endl;
    //this->Update();
    //this->Refresh();
    
}

void MyChoice::OnFocusLost(wxFocusEvent& event) {

    //if(wxNOT_FOUND != this->GetSelection())
    //    this->SetStringSelection("saves/default.ag.xml"); // default selection
    //else 
    //return;
}*/

void MyChoice::SearchForFiles() {
    
    this->Clear();
    wxString f = wxFindFirstFile(wxT("saves/*.ag.xml"));
    while (!f.empty()) {
        this->Append(f);
        f = wxFindNextFile();        
    }
}
/*===================================MyChoice::END============================*/

/*===================================AgentsFrm::BEGIN=========================*/

AgentsFrm::AgentsFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size)
: wxFrame(parent, id, title, position, size) {
    
    wxIcon *ic = new wxIcon(wxT("images/agent.bmp"), wxBITMAP_TYPE_ANY, -1, -1);
    wxMask *maskaicon = new wxMask(*ic, wxColour(255,0,255));
    ic->SetMask(maskaicon);
    
    SetIcon(*ic);

    b_running = false;
    timer_interval = 500;
    b_radio = false;
    scene_width = DEFAULT_SCENE_WIDTH;
    scene_height = DEFAULT_SCENE_HEIGHT;
    scene_width_pix = (20+GRIDSPACE)*scene_width+RECTWIDTH;
    scene_height_pix = (20+GRIDSPACE)*scene_height+RECTWIDTH;
    wxInitAllImageHandlers();
    CreateGUIControls();
    SetMinSize(wxSize(400, 300));
    //sidepanel->SetMinSize(wxSize(-1, -1));
    pwindow = new PaintWindow(this, wxPoint(-1,-1), wxSize(500,500));
    
    CreateStatusBar(6);
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxT("Edit MODE"), 0);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Field: %dx%d"), 
                                                    pwindow->scene->rozmerx, 
                                                    pwindow->scene->rozmery),1);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Zoom: %1.1fx"), pwindow->zoom), 2);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Delay: %dms"), timer_interval), 3);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Day: %d"), 
                                                    pwindow->scene->pocetkol), 
                                                    5);
    timer = new wxTimer(this, ID_Timer);
    pwindow->radio_choice = 0;
    
    pwindow->SetScrollRate( 10, 10 );
    pwindow->SetVirtualSize(scene_width_pix, scene_height_pix);
    pwindow->SetBackgroundColour(wxColour(55,50,120));
    
    //pwindow->SetMinSize(wxSize(300, 300));
    //pwindow->SetSize(500, 500);
    //pwindow->SetVirtualSizeHints(100, 100, 500, 500);    
    //this->SetBackgroundColour(wxColour(55,50,120));

    wxBoxSizer *boxsizer = new wxBoxSizer(wxHORIZONTAL);
    boxsizer->Add(pwindow, 3, wxEXPAND, 0);
    boxsizer->Add(sidepanel, 1, wxEXPAND, 0);        
    boxsizer->Fit(this);
    //boxsizer->SetItemMinSize(pwindow,200,200);
    //boxsizer->SetItemMinSize(sidepanel,200,200);
    boxsizer->SetSizeHints(this);
        
    this->SetSizer(boxsizer);

}

AgentsFrm::~AgentsFrm() {}

void AgentsFrm::CreateGUIControls() {
	
    wxMenu *fileMenu = new wxMenu();
    wxMenu *helpMenu = new wxMenu();
    
        helpMenu->Append(ID_MENU_Help,
		_("&Help"), _("help"));
		helpMenu->AppendSeparator();
		helpMenu->Append(ID_MENU_About,
		_("&About"), _("about"));
		
        fileMenu->Append(ID_MENU_Paint,
		_("&Paint"), _("Paint on frame"));
		fileMenu->AppendSeparator();
        fileMenu->Append(ID_MENU_Close,
		_("&Quit"), _("Quit the editor"));
		
		wxMenuBar *menuBar = new wxMenuBar();
        menuBar->Append(fileMenu, _T("&Game"));
        menuBar->Append(helpMenu, _T("&Help"));
        
    SetMenuBar(menuBar);
    
    //wxStatusBar *statusBar = new wxStatusBar(this, ID_StatusBar, wxST_SIZEGRIP,
     //                                        wxT("StatusBar"));
    //statusBar->Show();
    
    //Create(this, ID_StatusBar, wxT("statusBar"), wxPoint(-1,-1), wxSize(-1,-1) , wxST_SIZEGRIP);
    //GetStatusBar()->Show();
    //PositionStatusBar();

    this->CreateToolBar(wxNO_BORDER|wxHORIZONTAL|wxTB_FLAT, ID_Toolbar);
    //MainWin->GetToolBar()->SetMargins( 2, 2 );
    this->InitToolbar(this->GetToolBar());
    this->InitPanel(sidepanel);
    
    wxBitmapButton *script_open = new wxBitmapButton(sidepanel, 
        ID_ScriptOpenButton,  wxBitmap(wxT("images/open.xpm"),wxBITMAP_TYPE_XPM), 
        wxPoint(1,162));
    wxBitmapButton *script_edit = new wxBitmapButton(sidepanel, 
        ID_ScriptSaveButton,  wxBitmap(wxT("images/save.xpm"),wxBITMAP_TYPE_XPM), 
        wxPoint(26,162));
    //wxBitmapButton *script_delete = new wxBitmapButton(sidepanel, 
    //    ID_ScriptDeleteButton,  wxBitmap(wxT("images/open.xpm"),wxBITMAP_TYPE_XPM), 
    //    wxPoint(51,162));
    
    textwindow = new wxTextCtrl(this->sidepanel, ID_TextWindow, 
                                            wxT(""), wxPoint(-1,184), 
                                            wxSize(200,200), wxTE_MULTILINE | wxBOTTOM, 
                                            wxDefaultValidator, wxT("Script editor"));
    //textwindow->SetSize();
    
    wxBoxSizer *boxsizerV = new wxBoxSizer(wxVERTICAL);
    boxsizerV->Add(radio, 0, 0, 0);
    boxsizerV->Add(choice, 0, 0, 0); 
        wxBoxSizer *boxsizerH = new wxBoxSizer(wxHORIZONTAL);
        boxsizerH->Add(script_open, 0, 0, 0);
        boxsizerH->Add(script_edit, 0, 0, 0);
        //boxsizerH->Add(script_delete, 0, 0, 0);
    boxsizerV->Add(boxsizerH, 0, 0, 0);
    boxsizerV->Add(textwindow, 1, wxEXPAND, 1);
    //boxsizer->RecalcSizes();
    boxsizerH->Fit(sidepanel);
    boxsizerV->Fit(sidepanel);
    
    //boxsizer->SetItemMinSize(pwindow,200,200);
    //boxsizer->SetItemMinSize(sidepanel,200,200);
    boxsizerV->SetSizeHints(sidepanel);
        
    this->SetSizer(boxsizerV);
      
}

/*void AgentsFrm::UpdateStatusBar() {

    wxString mode;
    if(b_running) mode = wxT("Sim MODE");
    else mode = wxT("Edit MODE");
    GetStatusBar()->PushStatusText(mode, 0);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Field: %dx%d"), 
                                                    pwindow->scene->rozmerx, 
                                                    pwindow->scene->rozmery),1);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Zoom: %1.1fx"), pwindow->zoom), 2);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Delay: %dms"), timer_interval), 3);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Day: %d"), 
                                                    pwindow->scene->pocetkol), 
                                                    5);
}*/

void AgentsFrm::InitToolbar(wxToolBar* ToolBar) {
	ToolBar->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false,
	                    wxT("Tahoma")));

    wxBitmap new_BITMAP (wxT("images/reset.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap new_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_New, wxT(""), new_BITMAP,
	                    new_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));

	wxBitmap open_BITMAP (wxT("images/open.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap open_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Open, wxT(""), open_BITMAP,
	                    open_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));
	                    
    wxBitmap save_BITMAP (wxT("images/save.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap save_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Save, wxT(""), save_BITMAP,
	                    save_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));


	/*wxBitmap bmp_save (AgentsFrm_WxToolButton2_XPM);
	wxBitmap bmp_save_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Save, wxT(""), bmp_save,
	                    bmp_save_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));*/

	ToolBar->AddSeparator();

	wxBitmap play_BITMAP (wxT("images/play.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap play_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Start, wxT(""), play_BITMAP,
	                    play_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));

	wxBitmap stop_BITMAP (wxT("images/stop.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap stop_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Stop, wxT(""), stop_BITMAP,
	                    stop_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));
	                    
	wxBitmap step_BITMAP (wxT("images/step.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap step_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Step, wxT(""), step_BITMAP,
	                    step_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));

	ToolBar->AddSeparator();

	wxBitmap zoomin_BITMAP (wxT("images/zoomin.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap zoomin_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_ZoomIn, wxT(""), zoomin_BITMAP,
	                    zoomin_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));

	wxBitmap zoomout_BITMAP (wxT("images/zoomout.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap zoomout_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_ZoomOut, wxT(""), zoomout_BITMAP,
	                    zoomout_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));
	                    
	wxBitmap zoom_BITMAP (wxT("images/zoom.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap zoom_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Zoom, wxT(""), zoom_BITMAP,
	                    zoom_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));

	ToolBar->AddSeparator();

	timerSlider = new wxSlider(ToolBar, ID_TimerSlider, 50, 1, 100, wxPoint(159,0),
	                         wxSize(150,22), wxSL_HORIZONTAL /*| wxSL_LABELS*/, 
	                         wxDefaultValidator, wxT("Speed"));
	timerSlider->SetTickFreq(10, wxSL_BOTTOM);
	
	                    
	//timerSlider->SetTick                        
	//timerSlider->SetRange(10,0);
	//timerSlider->SetValue(5);
	//timerSlider->SetFont(wxFont(8, wxSWISS, wxNORMAL,wxNORMAL, false,
	//                   wxT("Tahoma")));
	                   
	ToolBar->AddControl(timerSlider);

	ToolBar->AddSeparator();
	
	wxBitmap info_BITMAP (wxT("images/info.xpm"),wxBITMAP_TYPE_XPM);
	wxBitmap info_DISABLE_BITMAP (wxNullBitmap);
	ToolBar->AddTool(ID_Info, wxT(""), info_BITMAP,
	                    info_DISABLE_BITMAP, wxITEM_NORMAL, wxT(""),
	                    wxT(""));


	ToolBar->SetToolBitmapSize(wxSize(20,20));
    ToolBar->Update();
    ToolBar->Refresh();
	ToolBar->Realize();
	this->SetToolBar(ToolBar);
}
void AgentsFrm::InitPanel( wxPanel* panel) {

    sidepanel = new wxPanel(this, ID_SidePanel, wxPoint(-1,-1), wxSize(140,204));
    sidepanel->SetBackgroundColour(wxColour(192,192,192));
    
    wxString *items = new wxString[4];
    
    items[0] = wxT("Erase");
    items[1] = wxT("Wall");
    items[2] = wxT("Object");
    items[3] = wxT("Agent");

    radio = new wxRadioBox(sidepanel, ID_Radio,
                             wxT("Editor"),
                             wxDefaultPosition, wxDefaultSize,
                             4, items,
                             1,
                             wxRA_HORIZONTAL | wxALIGN_CENTER);

    delete [] items;
       
    //wxString *items = new wxString[]
    choice = new MyChoice(sidepanel, ID_Choice, wxPoint(-1,130), 
                          wxSize(200,-1), 0, NULL, 0, wxDefaultValidator,
                          wxT("choice"));
    
    choice->SearchForFiles();
    choice->agscript = wxT("saves/default.ag.xml"); // default script
    choice->SetStringSelection(wxT("saves/default.ag.xml")); // default selection
}

void AgentsFrm::OnOpen(wxCommandEvent& WXUNUSED(event)) {

    OnStop();
    wxFileDialog dialog(this, _T("Load scene dialog"), wxEmptyString, 
                        wxEmptyString, _T("SC.XML File (*.sc.xml)|*.sc.xml"));
    //std::string converted_str = "saves";
    //dialog.SetDirectory(wxT("saves"));
    dialog.SetDirectory((dialog.GetDirectory()+wxT("/saves")));
    dialog.CentreOnParent();
    
    if (dialog.ShowModal() == wxID_OK) {
        delete pwindow->scene;
        loadscena(&(pwindow->scene), wx2std(dialog.GetPath(), wxConvUI));
        //pwindow->scene->printscena();
        scene_width_pix = (20+GRIDSPACE)*pwindow->scene->rozmerx +RECTWIDTH;
        scene_height_pix = (20+GRIDSPACE)*pwindow->scene->rozmery+RECTWIDTH;
        pwindow->SetVirtualSize((int)(scene_width_pix*pwindow->zoom), 
                              (int)(scene_height_pix*pwindow->zoom));
        pwindow->Refresh();
    }
}

void AgentsFrm::OnSave(wxCommandEvent& WXUNUSED(event)) {

    OnStop();
    wxFileDialog dialog(this, _T("Save scene dialog"), wxEmptyString,
                        _T("scena.sc.xml"), 
                        _T("SC.XML File (*.sc.xml)|*.sc.xml"),
                        wxSAVE | wxOVERWRITE_PROMPT);
    //const char* ascii_str = "saves\0";
    //wxString string(ascii_str, wxConvUTF8);
    //dialog.SetDirectory(string);
    dialog.SetDirectory((dialog.GetDirectory()+wxT("/saves")));
    if (dialog.ShowModal() == wxID_OK) {
        savescena(pwindow->scene, wx2std(dialog.GetPath(), wxConvUI));
    }
}

void AgentsFrm::ZoomIn(wxCommandEvent& event) {

    if(pwindow->zoom <= 4.9) {
        pwindow->zoom+=0.1;
        //pwindow->SetScrollRate((int)(10*pwindow->zoom), (int)(10*pwindow->zoom));
    pwindow->SetVirtualSize((int)(scene_width_pix*pwindow->zoom), 
                            (int)(scene_height_pix*pwindow->zoom));
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Zoom: %1.1fx"), 
                                   pwindow->zoom), 2);
    Refresh();
    }
    else event.Skip();
}

void AgentsFrm::ZoomOut(wxCommandEvent& event) {

    if(pwindow->zoom >= 0.6) {
        pwindow->zoom-=0.1;
        //pwindow->SetScrollRate((int)(10*pwindow->zoom), (int)(10*pwindow->zoom));
    pwindow->SetVirtualSize((int)(scene_width_pix*pwindow->zoom), 
                            (int)(scene_height_pix*pwindow->zoom));
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Zoom: %1.1fx"), 
                                   pwindow->zoom), 2);
    Refresh();
    }
    else event.Skip();
}

void AgentsFrm::Zoom(wxCommandEvent& event) {

    pwindow->zoom=DEFAULT_ZOOM;
    //pwindow->SetScrollRate((int)(10*pwindow->zoom), (int)(10*pwindow->zoom));
    pwindow->SetVirtualSize((int)(DEFAULT_SCENE_WIDTH*pwindow->zoom), 
                            (int)(DEFAULT_SCENE_HEIGHT*pwindow->zoom));
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Zoom: %1.1fx"), 
                                   pwindow->zoom), 2);
    Refresh();
}
// Works only on UNIX sofar
void AgentsFrm::Info(wxCommandEvent& event) {

    OnStop();
    int answer = wxMessageBox(wxT("Launch browser?"), wxT("Confirm"),
                            wxYES_NO, this, 1, 1);
    if (answer == wxYES) {
        wxString help_path = wxT("file://");
        help_path += wxGetCwd();
        help_path += wxT("/doc/help.html");
        cout << help_path << endl;
        wxLaunchDefaultBrowser(help_path, wxBROWSER_NEW_WINDOW);
    }
}

void AgentsFrm::Start(wxCommandEvent& event) {

    //sidepanel->Hide();
    //pwindow->Fit();
    //ClearBackground();
    //pwindow->Update();
    //pwindow->Refresh();
    b_running = true;
    radio->Enable(false);
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxT("Sim MODE"), 0);
    OnStart();
}

void AgentsFrm::Stop(wxCommandEvent& event) {

    //sidepanel->Show();
    //pwindow->FitInside();
    //ClearBackground();
    //Update();
    //Refresh();
    b_running = false;
    radio->Enable(true);
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxT("Edit MODE"), 0);
    OnStop();
}

void AgentsFrm::Step(wxCommandEvent& event) {

    if(!b_running) OnStep();
    else event.Skip();
}

void AgentsFrm::OnTimer(wxTimerEvent& event) {

    OnStep();
}

void AgentsFrm::OnTimerSlider(wxScrollEvent& event) {

    timer_interval = event.GetPosition() * 10;
    if(b_running) {
        OnStop();
        OnStart();
    }
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Delay: %dms"), 
                                   timer_interval), 3);
    
}

void AgentsFrm::OnRadioBox(wxCommandEvent& event) {

    if (b_radio == false) b_radio = true;
    pwindow->radio_choice = event.GetSelection();
    //cout << pwindow->radio_choice << endl;
}

void AgentsFrm::OpenScript(wxCommandEvent& event) {
    
    textwindow->Clear();
    std::string load_str;
    nacitajSkript(load_str, wx2std(choice->agscript, wxConvUI));
    //cout << load_str << endl;
    textwindow->WriteText(std2wx(load_str, wxConvUI));
    textwindow->Update();
    textwindow->Refresh();
    //cout << choice->agscript << endl;
    
}

void AgentsFrm::SaveScript(wxCommandEvent& event){
    
    
    wxString save_wxstr;
    textwindow->SetSelection(-1,-1);
    save_wxstr = textwindow->GetValue();
    //cout << wx2std(save_wxstr, wxConvUI) << endl;
    //textwindow->Copy();
    
    
    //textwindow->SetSelection(0,100);
    //cout << textwindow->GetStringSelection() << endl;
    std::string save_str = wx2std(save_wxstr, wxConvUI);
    //cout << wx2std(choice->agscript, wxConvUI) << endl;
    ulozSkript(save_str, wx2std(choice->agscript, wxConvUI));
    
}

void AgentsFrm::DeleteScript(wxCommandEvent& event){
    
}

void AgentsFrm::OnStart() {

    timer->Start(timer_interval);
    
}

void AgentsFrm::OnStop() {

    timer->Stop();
}

void AgentsFrm::OnStep() {

    pwindow->scene->odlozscenu();
    pwindow->scene->kolo();
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Day: %d"), 
                                                    pwindow->scene->pocetkol), 
                                                    5);
    pwindow->OnDraw();
}

void AgentsFrm::Paint(wxCommandEvent& WXUNUSED(event)) {

    Refresh();
}

void AgentsFrm::Close(wxCommandEvent& WXUNUSED(event)) {

    int answer = wxMessageBox(wxT("Are you sure?"), wxT("Confirm"),
                            wxYES_NO, this, 1, 1);
    if (answer == wxYES)
        answer = wxMessageBox(wxT("Are you realy realy sure??"), wxT("Confirm"),
                            wxYES_NO, this, 1, 1);
    
    if (answer == wxYES)
        Destroy();
}

void AgentsFrm::OnNew(wxCommandEvent& event) {

    // Stoping simulation and showing sidepanel
    //radio->Enable(true); 
    //sidepanel->Show();
    //pwindow->Fit();
    //Update();
    //Refresh();
    OnStop();
    
    long new_width, new_height;
    new_width = wxGetNumberFromUser(wxT("Range (1-100)"), wxT("Width:"), 
                                    wxT("New scene"), 23, 0, 100, NULL, 
                                    wxPoint(1,1));
    new_height = wxGetNumberFromUser(wxT("Range (1-100)"), wxT("Height:"), 
                                    wxT("New scene"), 23, 0, 100, NULL, 
                                    wxPoint(1,1));
    if(new_width == -1 || new_height == -1) return;                                
    
    delete pwindow->scene;    
    scene_width_pix = (20+GRIDSPACE)*new_width+RECTWIDTH;
    scene_height_pix = (20+GRIDSPACE)*new_height+RECTWIDTH;
    pwindow->SetVirtualSize((int)(scene_width_pix*pwindow->zoom), 
                            (int)(scene_height_pix*pwindow->zoom));    
    pwindow->scene = new SCENA(new_width, new_height);
    //UpdateStatusBar();
    GetStatusBar()->PushStatusText(wxT("Edit MODE"), 0);
    GetStatusBar()->PushStatusText(wxString::Format(wxT("Field: %dx%d"), 
                                                    pwindow->scene->rozmerx, 
                                                    pwindow->scene->rozmery),1);
    pwindow->Refresh();
}

void AgentsFrm::About(wxCommandEvent& WXUNUSED(event)) {

    wxMessageBox(wxT("Agents2007 version 0.9"), wxT("About"), wxOK, this, 1, 1);
}

/*===================================AgentsFrm::END===========================*/


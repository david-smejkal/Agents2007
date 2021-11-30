/*******************************************************************************
**                          A G E N T S   2 0 0 7                              *
********************************************************************************
**  projekt:      agents2007                                                   *
**  autori:       xsusma00, xsmejk13, xnagye00, xtrnko00                       *
*******************************************************************************/

#ifndef __AGENTS_H__
#define __AGENTS_H__

#ifndef WX_PRECOMP
        #include <wx/wx.h>
        #include <wx/frame.h>
#else
        #include <wx/wxprec.h>
#endif

#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/dc.h>
#include <wx/choice.h>
#include <wx/numdlg.h> // Needed by wxGetNumberFromUser
//#include <wx/dcbuffer.h>

#define GRIDSPACE 1 // Space between objects
#define RECTWIDTH 5 // Width of rectangle around scene
#define DEFAULT_ZOOM 1.0
#define DEFAULT_SCENE_WIDTH 23
#define DEFAULT_SCENE_HEIGHT 23

#include "engine.h"

using namespace std;


// Declaration of IDs which will be asigned to items in application.
enum {
    ID_MENU_Paint = wxID_HIGHEST + 1,
    ID_MENU_Close,
    ID_MENU_Help,
    ID_MENU_About,
    ID_New,    
    ID_Open,
    ID_Save,
    ID_Timer,
    ID_ZoomIn,
    ID_ZoomOut,
    ID_Zoom,
    ID_Info,
    ID_SidePanel,
    ID_TextWindow,
    ID_TextFrame,
    ID_Radio,
    ID_Choice,
    ID_StatusBar,
    ID_ScriptOpenButton,
    ID_ScriptSaveButton,
    ID_ScriptDeleteButton,
    ID_TimerSlider,
    ID_Start,
    ID_Stop,
    ID_Step,
    ID_Toolbar
};

/* Main class, application has only constructor OnInit, its like main in other
cpp codes, this method is called when program starts. */
class MainApp: public wxApp { 

    public:
        virtual bool OnInit();

};

IMPLEMENT_APP(MainApp)

/* This class is a window in frame on which the whole scene is painted. */
class PaintWindow : public wxScrolledWindow {

    public:
        // Constructor
        PaintWindow(wxWindow* parent, wxPoint pos, wxSize size);
        // Drawing methods
        void OnPaint(wxPaintEvent& event);
        void OnDraw();
        void DrawTest(wxDC *dc);
        void DrawPosition(wxDC *dc, const wxPoint& pos_scene);
        void DrawScene(wxDC *dc, bool flag);// if flag == true -> repainting all
        void DrawGround(wxDC *dc, wxSize size);
        void DrawWalls(wxDC *dc, wxSize size);
        void EraseObject(wxDC *dc, const wxPoint& pos);
        void EraseWall(wxDC *dc, const wxPoint& pos);
        void DrawObject(wxDC *dc, wxBitmap *bit, const wxPoint& oldpos,
                        const wxPoint& newpos);
        wxPoint GetObjectPosition(int x, int y);
        wxPoint GetScenePosition(const wxPoint& pos);
        
        // Grafics handling methods
        void OnEraseBackground(wxEraseEvent &event);
        void OnScroll(wxScrollWinEvent &event);
        void OnMouseWheel(wxMouseEvent& event);
        void OnMouse(wxMouseEvent& event);
        // public DATA
        double zoom; // frame needs it (zoom slider)
        SCENA *scene; // making scene accesible for frame
        int radio_choice; // window needs to know what will be painted 
                          // and frame what is selected        
    private:
        // private DATA
        // some bitmaps rotated facing UP, LEFT, DOWN, RIGHT
        wxBitmap *agentU, *agentL, *agentD, *agentR,
                 *agentcaseU, *agentcaseL, *agentcaseD, *agentcaseR,
                 *agentoncaseU, *agentoncaseL, *agentoncaseD, *agentoncaseR,
                 *suitcase , *wall;
        wxSize bmpsize;
        int x,y; // window origin position (needed to be known when scrolling)
        
        // any class wishing to process wxWidgets events must use this macro
        DECLARE_EVENT_TABLE()
};

class MyChoice : public wxChoice {

    public:
        MyChoice(wxWindow* parent, wxWindowID id, const wxPoint& pos , 
                   const wxSize& size, int n, const wxString choices[], 
                   long style, const wxValidator& validator, 
                   const wxString& name);
        //void OnFocusGot(wxFocusEvent& event);
        //void OnFocusLost(wxFocusEvent& event);
        void OnChoice(wxCommandEvent& event);
        void SearchForFiles();
        wxString agscript;
    private:
        bool b_focus;
        DECLARE_EVENT_TABLE()

};

/*class TextWindow : public {

    public:
        void OnText(wxTextEvent& event);
    private:
        DECLARE_EVENT_TABLE()
}*/

class AgentsFrm : public wxFrame {

    public:
        // Constructor
        AgentsFrm(wxWindow *parent, wxWindowID id = -1,
                  const wxString &title = wxT("Agents2007"),
                  const wxPoint& pos = wxPoint(100,100),
                  const wxSize& size = wxSize(500,500));
        // Destructor
        virtual ~AgentsFrm();
        // Methods
        void CreateGUIControls();
        //void UpdateStatusBar();
        void InitToolbar(wxToolBar* toolBar);
        void InitPanel( wxPanel* panel);
        void OnNew(wxCommandEvent& event);
        void OnOpen(wxCommandEvent& event);
        void OnSave(wxCommandEvent& event);
        void ZoomIn(wxCommandEvent& event);
        void ZoomOut(wxCommandEvent& event);
        void Zoom(wxCommandEvent& event);
        void Info(wxCommandEvent& event);
        void About(wxCommandEvent& WXUNUSED(event));
        void Start(wxCommandEvent& event);
        void Stop(wxCommandEvent& event);
        void Step(wxCommandEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnTimerSlider(wxScrollEvent& event);
        void OnRadioBox(wxCommandEvent& event);
        //void OnFocusGot(wxFocusEvent& evnet);
        void OpenScript(wxCommandEvent& event);
        void SaveScript(wxCommandEvent& event);
        void DeleteScript(wxCommandEvent& event);
        void OnChoice(wxCommandEvent& evnet);
        void OnStart();
        void OnStop();
        void OnStep();
        void OnSize(wxSizeEvent &event);
        void Paint(wxCommandEvent& event);
        void Close(wxCommandEvent& event);
                    
        // public DATA      
        bool b_running;
        MyChoice* choice;
        PaintWindow* pwindow;
    private:
        // private DATA
        wxPanel *sidepanel;
        wxComboBox *WxComboBox1;
        wxSlider *timerSlider;
        wxToolBar *ToolBar;
        wxTextCtrl *textwindow;
        wxTimer* timer;
        wxRadioBox *radio;
        
        int scene_width;
        int scene_height;
        int scene_width_pix;
        int scene_height_pix;
        
        bool b_radio;
        long timer_interval;
            
        DECLARE_EVENT_TABLE()

};

//Event tables

BEGIN_EVENT_TABLE(PaintWindow, wxWindow)

    EVT_PAINT(PaintWindow::OnPaint)
    EVT_ERASE_BACKGROUND(PaintWindow::OnEraseBackground)
    EVT_SCROLLWIN( PaintWindow::OnScroll )
    EVT_MOUSEWHEEL(PaintWindow::OnMouseWheel)
    EVT_LEFT_DOWN(PaintWindow::OnMouse)
    EVT_MOTION(PaintWindow::OnMouse)
    EVT_RIGHT_DOWN(PaintWindow::OnMouse)
    
END_EVENT_TABLE()


BEGIN_EVENT_TABLE (MyChoice, wxChoice)

    EVT_CHOICE(ID_Choice, MyChoice::OnChoice)
    //EVT_SET_FOCUS(MyChoice::OnFocusGot)
    //EVT_KILL_FOCUS(MyChoice::OnFocusLost)
    
END_EVENT_TABLE()


BEGIN_EVENT_TABLE (AgentsFrm, wxFrame)

    EVT_MENU(ID_MENU_Paint, AgentsFrm::Paint)
    EVT_MENU(ID_MENU_Close, AgentsFrm::Close)
    EVT_MENU(ID_Open, AgentsFrm::OnOpen)
    EVT_MENU(ID_Save, AgentsFrm::OnSave)
    EVT_MENU(ID_MENU_About, AgentsFrm::About)
    EVT_MENU(ID_MENU_Help, AgentsFrm::Info)
    
    EVT_TOOL(ID_New, AgentsFrm::OnNew)
    EVT_TOOL(ID_Start, AgentsFrm::Start)
    EVT_TOOL(ID_Stop, AgentsFrm::Stop)
    EVT_TOOL(ID_Step, AgentsFrm::Step)
    EVT_TOOL(ID_ZoomIn, AgentsFrm::ZoomIn)
    EVT_TOOL(ID_ZoomOut, AgentsFrm::ZoomOut)
    EVT_TOOL(ID_Zoom, AgentsFrm::Zoom)
    EVT_TOOL(ID_Info, AgentsFrm::Info)  

    EVT_TIMER(ID_Timer, AgentsFrm::OnTimer)
    EVT_COMMAND_SCROLL(ID_TimerSlider, AgentsFrm::OnTimerSlider)
    EVT_SIZE(AgentsFrm::OnSize)
    EVT_RADIOBOX(ID_Radio, AgentsFrm::OnRadioBox)
    EVT_BUTTON(ID_ScriptOpenButton, AgentsFrm::OpenScript)
    EVT_BUTTON(ID_ScriptSaveButton, AgentsFrm::SaveScript)
    EVT_BUTTON(ID_ScriptDeleteButton, AgentsFrm::DeleteScript)
    
END_EVENT_TABLE()

#endif

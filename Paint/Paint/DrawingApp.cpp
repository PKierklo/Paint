#include <wx/wx.h>
#include <wx/image.h>
#include "MainFrame.h"
#include "DrawingModel.h"

class DrawingApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        wxImage::AddHandler(new wxPNGHandler);

        IDrawingModel* model = new DrawingModel();
        MainFrame* frame = new MainFrame(model);
        frame->Show(true);

        return true;
    }
};

wxIMPLEMENT_APP(DrawingApp);

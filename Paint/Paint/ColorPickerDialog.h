#ifndef COLOR_PICKER_DIALOG_H
#define COLOR_PICKER_DIALOG_H

#include <wx/wx.h>
#include <wx/slider.h>
#include <wx/panel.h>

class ColorPickerDialog : public wxDialog {
public:
    ColorPickerDialog(wxWindow* parent);
    wxColour GetSelectedColor() const;

private:
    wxSlider* sliderR;
    wxSlider* sliderG;
    wxSlider* sliderB;
    wxTextCtrl* textR;
    wxTextCtrl* textG;
    wxTextCtrl* textB;
    wxTextCtrl* textC;
    wxTextCtrl* textM;
    wxTextCtrl* textY;
    wxTextCtrl* textK;
    wxTextCtrl* textH;
    wxTextCtrl* textS;
    wxTextCtrl* textV;
    wxPanel* colorPreviewPanel;

    wxColour selectedColor;

    void OnSliderUpdate(wxCommandEvent& event);
    void OnTextUpdate(wxCommandEvent& event);
    void UpdateColor();

    void RGBtoCMYK(int r, int g, int b, double& c, double& m, double& y, double& k);
    void RGBtoHSV(int r, int g, int b, double& h, double& s, double& v);
};

#endif

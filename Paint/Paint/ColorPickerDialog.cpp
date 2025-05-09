#include "ColorPickerDialog.h"

ColorPickerDialog::ColorPickerDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Choose Color", wxDefaultPosition, wxSize(400, 400))
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    sliderR = new wxSlider(this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxSize(250, -1), wxSL_HORIZONTAL);
    sliderG = new wxSlider(this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxSize(250, -1), wxSL_HORIZONTAL);
    sliderB = new wxSlider(this, wxID_ANY, 0, 0, 255, wxDefaultPosition, wxSize(250, -1), wxSL_HORIZONTAL);

    textR = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textG = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textB = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));

    wxBoxSizer* rowR = new wxBoxSizer(wxHORIZONTAL);
    rowR->Add(new wxStaticText(this, wxID_ANY, "R: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    rowR->Add(sliderR, 1, wxALL, 5);
    rowR->Add(textR, 0, wxALL, 5);

    wxBoxSizer* rowG = new wxBoxSizer(wxHORIZONTAL);
    rowG->Add(new wxStaticText(this, wxID_ANY, "G: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    rowG->Add(sliderG, 1, wxALL, 5);
    rowG->Add(textG, 0, wxALL, 5);

    wxBoxSizer* rowB = new wxBoxSizer(wxHORIZONTAL);
    rowB->Add(new wxStaticText(this, wxID_ANY, "B: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    rowB->Add(sliderB, 1, wxALL, 5);
    rowB->Add(textB, 0, wxALL, 5);

    mainSizer->Add(rowR, 0, wxEXPAND);
    mainSizer->Add(rowG, 0, wxEXPAND);
    mainSizer->Add(rowB, 0, wxEXPAND);

    colorPreviewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 50));
    mainSizer->Add(colorPreviewPanel, 0, wxALIGN_CENTER | wxALL, 10);

    wxBoxSizer* cmykSizer = new wxBoxSizer(wxHORIZONTAL);
    textC = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textM = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textY = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textK = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    cmykSizer->Add(new wxStaticText(this, wxID_ANY, "C: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    cmykSizer->Add(textC, 0, wxALL, 5);
    cmykSizer->Add(new wxStaticText(this, wxID_ANY, "M: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    cmykSizer->Add(textM, 0, wxALL, 5);
    cmykSizer->Add(new wxStaticText(this, wxID_ANY, "Y: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    cmykSizer->Add(textY, 0, wxALL, 5);
    cmykSizer->Add(new wxStaticText(this, wxID_ANY, "K: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    cmykSizer->Add(textK, 0, wxALL, 5);
    mainSizer->Add(cmykSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer* hsvSizer = new wxBoxSizer(wxHORIZONTAL);
    textH = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textS = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    textV = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxSize(50, -1));
    hsvSizer->Add(new wxStaticText(this, wxID_ANY, "H: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    hsvSizer->Add(textH, 0, wxALL, 5);
    hsvSizer->Add(new wxStaticText(this, wxID_ANY, "S: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    hsvSizer->Add(textS, 0, wxALL, 5);
    hsvSizer->Add(new wxStaticText(this, wxID_ANY, "V: "), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    hsvSizer->Add(textV, 0, wxALL, 5);
    mainSizer->Add(hsvSizer, 0, wxEXPAND | wxALL, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALL, 5);
    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

    SetSizerAndFit(mainSizer);

    sliderR->Bind(wxEVT_SLIDER, &ColorPickerDialog::OnSliderUpdate, this);
    sliderG->Bind(wxEVT_SLIDER, &ColorPickerDialog::OnSliderUpdate, this);
    sliderB->Bind(wxEVT_SLIDER, &ColorPickerDialog::OnSliderUpdate, this);

    textR->Bind(wxEVT_TEXT, &ColorPickerDialog::OnTextUpdate, this);
    textG->Bind(wxEVT_TEXT, &ColorPickerDialog::OnTextUpdate, this);
    textB->Bind(wxEVT_TEXT, &ColorPickerDialog::OnTextUpdate, this);
}

void ColorPickerDialog::OnSliderUpdate(wxCommandEvent& event) {
    textR->ChangeValue(std::to_string(sliderR->GetValue()));
    textG->ChangeValue(std::to_string(sliderG->GetValue()));
    textB->ChangeValue(std::to_string(sliderB->GetValue()));
    UpdateColor();
}

void ColorPickerDialog::OnTextUpdate(wxCommandEvent& event) {
    long r, g, b;
    textR->GetValue().ToLong(&r);
    textG->GetValue().ToLong(&g);
    textB->GetValue().ToLong(&b);

    sliderR->SetValue(static_cast<int>(r));
    sliderG->SetValue(static_cast<int>(g));
    sliderB->SetValue(static_cast<int>(b));
    UpdateColor();
}

void ColorPickerDialog::UpdateColor() {
    int r = sliderR->GetValue();
    int g = sliderG->GetValue();
    int b = sliderB->GetValue();
    selectedColor = wxColour(r, g, b);


    colorPreviewPanel->SetBackgroundColour(selectedColor);
    colorPreviewPanel->Refresh();


    double c, m, y, k, h, s, v;
    RGBtoCMYK(r, g, b, c, m, y, k);
    RGBtoHSV(r, g, b, h, s, v);


    textC->SetValue(wxString::Format("%.2f", c * 100));
    textM->SetValue(wxString::Format("%.2f", m * 100));
    textY->SetValue(wxString::Format("%.2f", y * 100));
    textK->SetValue(wxString::Format("%.2f", k * 100));

    textH->SetValue(wxString::Format("%.1f", h));
    textS->SetValue(wxString::Format("%.1f", s * 100));
    textV->SetValue(wxString::Format("%.1f", v * 100));
}

wxColour ColorPickerDialog::GetSelectedColor() const {
    return selectedColor;
}


void ColorPickerDialog::RGBtoCMYK(int r, int g, int b, double& c, double& m, double& y, double& k) {
    double rr = r / 255.0;
    double gg = g / 255.0;
    double bb = b / 255.0;

    k = 1.0 - std::max({ rr, gg, bb });
    if (k < 1.0) {
        c = (1.0 - rr - k) / (1.0 - k);
        m = (1.0 - gg - k) / (1.0 - k);
        y = (1.0 - bb - k) / (1.0 - k);
    }
    else {
        c = m = y = 0.0;
    }
}

void ColorPickerDialog::RGBtoHSV(int r, int g, int b, double& h, double& s, double& v) {
    double rr = r / 255.0;
    double gg = g / 255.0;
    double bb = b / 255.0;

    double max = std::max({ rr, gg, bb });
    double min = std::min({ rr, gg, bb });
    v = max;

    double delta = max - min;
    if (max != 0.0)
        s = delta / max;
    else {
        s = 0.0;
        h = -1.0;
        return;
    }

    if (rr == max)
        h = (gg - bb) / delta;
    else if (gg == max)
        h = 2.0 + (bb - rr) / delta;
    else
        h = 4.0 + (rr - gg) / delta;

    h *= 60.0;
    if (h < 0.0)
        h += 360.0;
}

#include <wx/wx.h>
#include <wx/notebook.h>
#include "../bridge/ConverterBridge_macOS.h"

class ConverterApp : public wxApp {
public:
    bool OnInit() override;
};

class ConverterFrame : public wxFrame {
public:
    ConverterFrame(const wxString& title);
private:
    void CreateGUI();
    void OnConvert(wxCommandEvent& event);
    
    // Decimal <-> Binary
    wxTextCtrl* m_decBinDecInput;
    wxTextCtrl* m_decBinDecOutput;
    wxTextCtrl* m_decBinBinInput;
    wxTextCtrl* m_decBinBinOutput;
    
    // Decimal <-> Hex
    wxTextCtrl* m_decHexDecInput;
    wxTextCtrl* m_decHexDecOutput;
    wxTextCtrl* m_decHexHexInput;
    wxTextCtrl* m_decHexHexOutput;
    
    // Binary <-> Hex
    wxTextCtrl* m_binHexBinInput;
    wxTextCtrl* m_binHexBinOutput;
    wxTextCtrl* m_binHexHexInput;
    wxTextCtrl* m_binHexHexOutput;
};

wxIMPLEMENT_APP(ConverterApp);

bool ConverterApp::OnInit()
{
    ConverterFrame* frame = new ConverterFrame("Number Converter");
    frame->Show();
    return true;
}

ConverterFrame::ConverterFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 500))
{
    CreateGUI();
}

void ConverterFrame::CreateGUI()
{
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);
    
    // Tab 1: Decimal <-> Binary
    wxPanel* panel1 = new wxPanel(notebook);
    wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
    
    sizer1->Add(new wxStaticText(panel1, wxID_ANY, "Decimal to Binary:"), 0, wxALL, 5);
    m_decBinDecInput = new wxTextCtrl(panel1, wxID_ANY);
    sizer1->Add(m_decBinDecInput, 0, wxEXPAND | wxALL, 5);
    
    wxButton* btn1 = new wxButton(panel1, wxID_ANY, "Convert D→B");
    btn1->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        std::string result = ConvertDecToBin(m_decBinDecInput->GetValue().ToStdString());
        m_decBinBinOutput->SetValue(result);
    });
    sizer1->Add(btn1, 0, wxEXPAND | wxALL, 5);
    m_decBinBinOutput = new wxTextCtrl(panel1, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    sizer1->Add(m_decBinBinOutput, 0, wxEXPAND | wxALL, 5);
    
    sizer1->Add(new wxStaticText(panel1, wxID_ANY, "Binary to Decimal:"), 0, wxALL, 5);
    m_decBinBinInput = new wxTextCtrl(panel1, wxID_ANY);
    sizer1->Add(m_decBinBinInput, 0, wxEXPAND | wxALL, 5);
    
    wxButton* btn2 = new wxButton(panel1, wxID_ANY, "Convert B→D");
    btn2->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        std::string result = ConvertBinToDec(m_decBinBinInput->GetValue().ToStdString());
        m_decBinDecOutput->SetValue(result);
    });
    sizer1->Add(btn2, 0, wxEXPAND | wxALL, 5);
    m_decBinDecOutput = new wxTextCtrl(panel1, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    sizer1->Add(m_decBinDecOutput, 0, wxEXPAND | wxALL, 5);
    
    panel1->SetSizer(sizer1);
    notebook->AddPage(panel1, "Decimal ↔ Binary");
    
    // Tab 2: Decimal <-> Hex
    wxPanel* panel2 = new wxPanel(notebook);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    
    sizer2->Add(new wxStaticText(panel2, wxID_ANY, "Decimal to Hex:"), 0, wxALL, 5);
    m_decHexDecInput = new wxTextCtrl(panel2, wxID_ANY);
    sizer2->Add(m_decHexDecInput, 0, wxEXPAND | wxALL, 5);
    
    wxButton* btn3 = new wxButton(panel2, wxID_ANY, "Convert D→H");
    btn3->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        std::string result = ConvertDecToHex(m_decHexDecInput->GetValue().ToStdString());
        m_decHexHexOutput->SetValue(result);
    });
    sizer2->Add(btn3, 0, wxEXPAND | wxALL, 5);
    m_decHexHexOutput = new wxTextCtrl(panel2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    sizer2->Add(m_decHexHexOutput, 0, wxEXPAND | wxALL, 5);
    
    sizer2->Add(new wxStaticText(panel2, wxID_ANY, "Hex to Decimal:"), 0, wxALL, 5);
    m_decHexHexInput = new wxTextCtrl(panel2, wxID_ANY);
    sizer2->Add(m_decHexHexInput, 0, wxEXPAND | wxALL, 5);
    
    wxButton* btn4 = new wxButton(panel2, wxID_ANY, "Convert H→D");
    btn4->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        std::string result = ConvertHexToDec(m_decHexHexInput->GetValue().ToStdString());
        m_decHexDecOutput->SetValue(result);
    });
    sizer2->Add(btn4, 0, wxEXPAND | wxALL, 5);
    m_decHexDecOutput = new wxTextCtrl(panel2, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    sizer2->Add(m_decHexDecOutput, 0, wxEXPAND | wxALL, 5);
    
    panel2->SetSizer(sizer2);
    notebook->AddPage(panel2, "Decimal ↔ Hex");
    
    // Tab 3: Binary <-> Hex
    wxPanel* panel3 = new wxPanel(notebook);
    wxBoxSizer* sizer3 = new wxBoxSizer(wxVERTICAL);
    
    sizer3->Add(new wxStaticText(panel3, wxID_ANY, "Binary to Hex:"), 0, wxALL, 5);
    m_binHexBinInput = new wxTextCtrl(panel3, wxID_ANY);
    sizer3->Add(m_binHexBinInput, 0, wxEXPAND | wxALL, 5);
    
    wxButton* btn5 = new wxButton(panel3, wxID_ANY, "Convert B→H");
    btn5->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        std::string result = ConvertBinToHex(m_binHexBinInput->GetValue().ToStdString());
        m_binHexHexOutput->SetValue(result);
    });
    sizer3->Add(btn5, 0, wxEXPAND | wxALL, 5);
    m_binHexHexOutput = new wxTextCtrl(panel3, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    sizer3->Add(m_binHexHexOutput, 0, wxEXPAND | wxALL, 5);
    
    sizer3->Add(new wxStaticText(panel3, wxID_ANY, "Hex to Binary:"), 0, wxALL, 5);
    m_binHexHexInput = new wxTextCtrl(panel3, wxID_ANY);
    sizer3->Add(m_binHexHexInput, 0, wxEXPAND | wxALL, 5);
    
    wxButton* btn6 = new wxButton(panel3, wxID_ANY, "Convert H→B");
    btn6->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        std::string result = ConvertHexToBin(m_binHexHexInput->GetValue().ToStdString());
        m_binHexBinOutput->SetValue(result);
    });
    sizer3->Add(btn6, 0, wxEXPAND | wxALL, 5);
    m_binHexBinOutput = new wxTextCtrl(panel3, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    sizer3->Add(m_binHexBinOutput, 0, wxEXPAND | wxALL, 5);
    
    panel3->SetSizer(sizer3);
    notebook->AddPage(panel3, "Binary ↔ Hex");
}

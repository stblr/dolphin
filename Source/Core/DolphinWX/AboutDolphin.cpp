// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <wx/bitmap.h>
#include <wx/defs.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/hyperlink.h>
#include <wx/image.h>
#include <wx/mstream.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/translation.h>
#include <wx/windowid.h>
#include <wx/generic/statbmpg.h>

#ifdef __APPLE__
#import <AppKit/AppKit.h>
#endif

#include "Common/Common.h"
#include "DolphinWX/AboutDolphin.h"
#include "DolphinWX/resources/dolphin_logo.cpp"

static void BanishBackground(wxTextCtrl* ctrl)
{
#ifdef __APPLE__
       NSTextField* tf = (NSTextField*)ctrl->GetHandle();
       tf.drawsBackground = NO;
#endif
}

AboutDolphin::AboutDolphin(wxWindow *parent, wxWindowID id,
		const wxString &title, const wxPoint &position,
		const wxSize& size, long style)
	: wxDialog(parent, id, title, position, size, style)
{
	const unsigned char* dolphin_logo_bin;
	size_t dolphin_logo_size;
	double scaleFactor = 1.0;
	if (GetContentScaleFactor() >= 2)
	{
		dolphin_logo_bin = dolphin_logo_2x_png;
		dolphin_logo_size = sizeof dolphin_logo_2x_png;
		scaleFactor = 2.0;
	}
	else
	{
		dolphin_logo_bin = dolphin_logo_png;
		dolphin_logo_size = sizeof dolphin_logo_png;
	}
	wxMemoryInputStream istream(dolphin_logo_bin, dolphin_logo_size);
	wxImage iDolphinLogo(istream, wxBITMAP_TYPE_PNG);
	wxGenericStaticBitmap* const sbDolphinLogo = new wxGenericStaticBitmap(this, wxID_ANY,
			wxBitmap(iDolphinLogo, -1, scaleFactor));

	const wxString DolphinText = _("Dolphin");
	const wxString RevisionText = scm_desc_str;
	const wxString CopyrightText = _("(c) 2003-2015+ Dolphin Team. \"GameCube\" and \"Wii\" are trademarks of Nintendo. Dolphin is not affiliated with Nintendo in any way.");
	const wxString BranchText = wxString::Format(_("Branch: %s"), scm_branch_str);
	const wxString BranchRevText = wxString::Format(_("Revision: %s"), scm_rev_git_str);
	const wxString CompiledText = wxString::Format(_("Compiled: %s @ %s"), __DATE__, __TIME__);
	const wxString CheckUpdateText = _("Check for updates: ");
	const wxString Text = _("\n"
		"Dolphin is a free and open-source GameCube and Wii emulator.\n"
		"\n"
		"This software should not be used to play games you do not legally own.\n");
	const wxString LicenseText = _("License");
	const wxString AuthorsText = _("Authors");
	const wxString SupportText = _("Support");

	wxStaticText* const Dolphin = new wxStaticText(this, wxID_ANY, DolphinText);
	wxTextCtrl* const Revision = new wxTextCtrl(this, wxID_ANY, RevisionText, wxDefaultPosition, wxDefaultSize, wxNO_BORDER | wxTE_NO_VSCROLL);
	BanishBackground(Revision);
	wxStaticText* const Copyright = new wxStaticText(this, wxID_ANY, CopyrightText);
	wxTextCtrl* const Branch = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, 50), wxNO_BORDER | wxTE_NO_VSCROLL);
	BanishBackground(Branch);
	wxStaticText* const Message = new wxStaticText(this, wxID_ANY, Text);
	wxStaticText* const Update = new wxStaticText(this, wxID_ANY, CheckUpdateText);
	wxStaticText* const FirstSpacer = new wxStaticText(this, wxID_ANY, wxString("  |  "));
	wxStaticText* const SecondSpacer = new wxStaticText(this, wxID_ANY, wxString("  |  "));
	wxHyperlinkCtrl* const Download = new wxHyperlinkCtrl(this, wxID_ANY, "dolphin-emu.org/download", "https://dolphin-emu.org/download/");
	wxHyperlinkCtrl* const License = new wxHyperlinkCtrl(this, wxID_ANY, LicenseText, "https://github.com/dolphin-emu/dolphin/blob/master/license.txt");
	wxHyperlinkCtrl* const Authors = new wxHyperlinkCtrl(this, wxID_ANY, AuthorsText, "https://github.com/dolphin-emu/dolphin/graphs/contributors");
	wxHyperlinkCtrl* const Support = new wxHyperlinkCtrl(this, wxID_ANY, SupportText, "https://forums.dolphin-emu.org/");

	wxFont DolphinFont = Dolphin->GetFont();
	wxFont RevisionFont = Revision->GetFont();
	wxFont CopyrightFont = Copyright->GetFont();
	wxFont BranchFont = Branch->GetFont();

	DolphinFont.SetPointSize(36);
	Dolphin->SetFont(DolphinFont);

	RevisionFont.SetWeight(wxFONTWEIGHT_BOLD);
	Revision->SetFont(RevisionFont);

	Revision->SetEditable(false);
	Revision->SetWindowStyle(wxNO_BORDER);

	BranchFont.SetPointSize(7);
	Branch->SetFont(BranchFont);

	Branch->SetEditable(false);
	Branch->AppendText(BranchText + "\n");
	Branch->AppendText(BranchRevText + "\n");
	Branch->AppendText(CompiledText);

	CopyrightFont.SetPointSize(7);
	Copyright->SetFont(CopyrightFont);
	Copyright->SetFocus();

	wxBoxSizer* const sCheckUpdates = new wxBoxSizer(wxHORIZONTAL);
	sCheckUpdates->Add(Update);
	sCheckUpdates->Add(Download);

	wxBoxSizer* const sLinks = new wxBoxSizer(wxHORIZONTAL);
	sLinks->Add(License);
	sLinks->Add(FirstSpacer);
	sLinks->Add(Authors);
	sLinks->Add(SecondSpacer);
	sLinks->Add(Support);

	wxBoxSizer* const sInfo = new wxBoxSizer(wxVERTICAL);
	sInfo->Add(Dolphin);
	sInfo->AddSpacer(5);
	sInfo->Add(Revision);
	sInfo->AddSpacer(10);
	sInfo->Add(Branch);
	sInfo->Add(sCheckUpdates);
	sInfo->Add(Message);
	sInfo->Add(sLinks);

	wxBoxSizer* const sLogo = new wxBoxSizer(wxVERTICAL);
	sLogo->AddSpacer(75);
	sLogo->Add(sbDolphinLogo);
	sLogo->AddSpacer(40);

	wxBoxSizer* const sMainHor = new wxBoxSizer(wxHORIZONTAL);
	sMainHor->AddSpacer(30);
	sMainHor->Add(sLogo);
	sMainHor->AddSpacer(30);
	sMainHor->Add(sInfo);
	sMainHor->AddSpacer(30);

	wxBoxSizer* const sFooter = new wxBoxSizer(wxVERTICAL);
	sFooter->AddSpacer(15);
	sFooter->Add(Copyright, 0, wxALIGN_BOTTOM | wxALIGN_CENTER);
	sFooter->AddSpacer(5);

	wxBoxSizer* const sMain = new wxBoxSizer(wxVERTICAL);
	sMain->Add(sMainHor, 1, wxEXPAND);
	sMain->Add(sFooter, 0, wxEXPAND);

	SetSizerAndFit(sMain);
	Center();
	SetFocus();
}

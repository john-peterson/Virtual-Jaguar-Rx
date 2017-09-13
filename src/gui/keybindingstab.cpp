//
// keybindingstab.cpp: Key Bindings tab on the settings dialog
//
// Part of the Virtual Jaguar Project
//
// JPM = Jean-Paul Mari <djipi.mari@gmail.com>
//
// WHO  WHEN        WHAT
// ---  ----------  ------------------------------------------------------------
// JPM  09/10/2017  Created this file

#include "keybindingstab.h"
#include "settings.h"


//
KeyBindings KeyBindingsTable[KB_END] =	{
											{	"KB_Quit", "Quit", "Quit keybinding", "Ctrl+Q", NULL, NULL	},
											{	"KB_PickFile", "Pick file", "Pick file keybinding", "Ctrl+I", NULL, NULL	},
											{	"KB_Configure", "Configure", "Configure keybinding", "Ctrl+C", NULL, NULL	},
											{	"KB_EmuStatus", "Emulator Status", "Emulator status keybinding", "Ctrl+S", NULL, NULL	},
											{	"KB_Pause", "Pause", "Pause keybinding", "Esc", NULL, NULL	},
											{	"KB_FrameAdvance", "Frame Advance", "Frame advance keybinding", "F7", NULL, NULL	},
											{	"KB_Restart", "Restart", "Restart keybinding", "Ctrl+Shift+F5", NULL, NULL	},
											{	"KB_StepInto", "Step Into", "Step into keybinding", "F11", NULL, NULL	},
											{	"KB_StepOver", "Step Over", "Step over kebinding", "F10", NULL, NULL	},
											{	"KB_FullScreen", "Full Screen", "Full screen kebinding", "F9", NULL, NULL	}
										};


//
#define	NBKEYBINDINGS	sizeof(KeyBindingsTable)/sizeof(KeyBindings)


// 
KeyBindingsTab::KeyBindingsTab(QWidget * parent/*= 0*/): QWidget(parent)
{
	size_t i;

	QVBoxLayout *layout1 = new QVBoxLayout;
	QVBoxLayout *layout2 = new QVBoxLayout;

	// Initialisation for each layout line
	for (i = 0; i < NBKEYBINDINGS; i++)
	{
		layout1->addWidget(KeyBindingsTable[i].KBLabel = new QLabel(KeyBindingsTable[i].KBNameLabel));
		layout2->addWidget(KeyBindingsTable[i].KBLineEdit = new QLineEdit(""));
		KeyBindingsTable[i].KBLineEdit->setMaxLength(30);
		KeyBindingsTable[i].KBLineEdit->setPlaceholderText(KeyBindingsTable[i].KBPlaceholderText);
	}

	// Layouts setup
	QHBoxLayout *layout3 = new QHBoxLayout;
	layout3->addLayout(layout1);
	layout3->addLayout(layout2);
	QVBoxLayout *layout4 = new QVBoxLayout;
	layout4->addLayout(layout3);
	setLayout(layout4);
}


KeyBindingsTab::~KeyBindingsTab()
{
}


// Load / Update the tabs dialog from the settings
void KeyBindingsTab::GetSettings(void)
{
	size_t i;

	for (i = 0; i < NBKEYBINDINGS; i++)
	{
		KeyBindingsTable[i].KBLineEdit->setText(vjs.KBContent[i].KBSettingValue);
	}
}


// Save / Update the settings from the tabs dialog
void KeyBindingsTab::SetSettings(void)
{
	size_t i;

	for (i = 0; i < NBKEYBINDINGS; i++)
	{
		//strcpy(vjs.KBContent[i].KBSettingName, KeyBindingsTable[i].KBNameSetting);
		strcpy(vjs.KBContent[i].KBSettingValue, KeyBindingsTable[i].KBLineEdit->text().toUtf8().data());
	}
}

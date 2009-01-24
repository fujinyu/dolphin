//////////////////////////////////////////////////////////////////////////////////////////
// Project description
// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
// Name: nJoy 
// Description: A Dolphin Compatible Input Plugin
//
// Author: Falcon4ever (nJoy@falcon4ever.com)
// Site: www.multigesture.net
// Copyright (C) 2003-2008 Dolphin Project.
//
//////////////////////////////////////////////////////////////////////////////////////////
//
// Licensetype: GNU General Public License (GPL)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.
//
// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/
//
// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/
//
//////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
// Include
// ŻŻŻŻŻŻŻŻŻ
#include "nJoy.h"
#include "Common.h"

Config g_Config;
extern ConfigBox* m_frame;

//////////////////////////////////


// Run when created
// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
Config::Config()
{
	// Clear the memory
	//memset(this, 0, sizeof(Config));
}


// Enable output log
// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
void DEBUG_INIT()
{
	if (pFile)
		return;

	#ifdef _WIN32
	char dateStr [9]; 
	_strdate( dateStr);
	char timeStr [9]; 
	_strtime( timeStr );
	#endif

	pFile = fopen ("nJoy-debug.txt","wt");
	fprintf(pFile, "nJoy v"INPUT_VERSION" Debug\n");
	#ifdef _WIN32
	fprintf(pFile, "Date: %s\nTime: %s\n", dateStr, timeStr);
	#endif
	fprintf(pFile, "ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ\n");
}

// Disable output log
// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
void DEBUG_QUIT()
{
	if (!pFile)
		return;

	#ifdef _WIN32
	char timeStr [9]; 
	_strtime(timeStr);

	fprintf(pFile, "_______________\n");
	fprintf(pFile, "Time: %s", timeStr);
	#endif
	fclose(pFile);
}


/////////////////////////////////////////////////////////////////////////////////////
// Save settings to file
// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
void Config::Save(int Slot)
{
	// Load ini file
	IniFile file;
	file.Load("nJoy.ini");

	// ==================================================================
	// Global settings
	// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
	file.Set("General", "ShowAdvanced", g_Config.bShowAdvanced);
	file.Set("General", "SaveByID", g_Config.bSaveByID);
	// ========================

	for (int i = 0; i < 4; i++)
	{
		// Should we save this slot?
		if (Slot != -1 && Slot != i) continue;

		// ==================================================================
		// Slot specific settings only
		// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
		std::string SectionName = StringFromFormat("PAD%i", i+1);
		file.Set(SectionName.c_str(), "enabled", PadMapping[i].enabled);

		// Save the physical device ID
		file.Set(SectionName.c_str(), "joy_id", PadMapping[i].ID);
		// ===================
		
		// ==================================================================
		// Joypad or slot specific settings
		// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
			// Current joypad device ID: PadMapping[i].ID
			// Current joypad name: joyinfo[PadMapping[i].ID].Name		
		if(g_Config.bSaveByID)
		{
			/* Save joypad specific settings. Check for "PadMapping[i].ID < SDL_NumJoysticks()" to
			   avoid reading a joyinfo that does't exist */
			if(PadMapping[i].ID >= SDL_NumJoysticks()) continue;

			// Create a new section name after the joypad name
			SectionName = joyinfo[PadMapping[i].ID].Name;
		}

		file.Set(SectionName.c_str(), "l_shoulder", PadMapping[i].buttons[CTL_L_SHOULDER]);
		file.Set(SectionName.c_str(), "r_shoulder", PadMapping[i].buttons[CTL_R_SHOULDER]);
		file.Set(SectionName.c_str(), "a_button", PadMapping[i].buttons[CTL_A_BUTTON]);
		file.Set(SectionName.c_str(), "b_button", PadMapping[i].buttons[CTL_B_BUTTON]);
		file.Set(SectionName.c_str(), "x_button", PadMapping[i].buttons[CTL_X_BUTTON]);
		file.Set(SectionName.c_str(), "y_button", PadMapping[i].buttons[CTL_Y_BUTTON]);
		file.Set(SectionName.c_str(), "z_trigger", PadMapping[i].buttons[CTL_Z_TRIGGER]);
		file.Set(SectionName.c_str(), "start_button", PadMapping[i].buttons[CTL_START]);
		file.Set(SectionName.c_str(), "dpad", PadMapping[i].dpad);	
		file.Set(SectionName.c_str(), "dpad_up", PadMapping[i].dpad2[CTL_D_PAD_UP]);
		file.Set(SectionName.c_str(), "dpad_down", PadMapping[i].dpad2[CTL_D_PAD_DOWN]);
		file.Set(SectionName.c_str(), "dpad_left", PadMapping[i].dpad2[CTL_D_PAD_LEFT]);
		file.Set(SectionName.c_str(), "dpad_right", PadMapping[i].dpad2[CTL_D_PAD_RIGHT]);
		file.Set(SectionName.c_str(), "main_x", PadMapping[i].axis[CTL_MAIN_X]);
		file.Set(SectionName.c_str(), "main_y", PadMapping[i].axis[CTL_MAIN_Y]);
		file.Set(SectionName.c_str(), "sub_x", PadMapping[i].axis[CTL_SUB_X]);
		file.Set(SectionName.c_str(), "sub_y", PadMapping[i].axis[CTL_SUB_Y]);
		
		file.Set(SectionName.c_str(), "deadzone", PadMapping[i].deadzone);
		file.Set(SectionName.c_str(), "halfpress", PadMapping[i].halfpress);
		
		file.Set(SectionName.c_str(), "controllertype", PadMapping[i].controllertype);
		file.Set(SectionName.c_str(), "TriggerType", PadMapping[i].triggertype);
		file.Set(SectionName.c_str(), "eventnum", PadMapping[i].eventnum);

		file.Set(SectionName.c_str(), "Diagonal", PadMapping[i].SDiagonal);
		file.Set(SectionName.c_str(), "SquareToCircle", PadMapping[i].bSquareToCircle);
		// ======================================

		// Debugging
		//if(m_frame) m_frame->LogMsg("Saved: %s %i\n", SectionName.c_str(), PadMapping[i].triggertype);
	}

	file.Save("nJoy.ini");
}

// Load settings from file
// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
void Config::Load(bool ChangePad, bool ChangeSaveByID)
{
	// Load file
	IniFile file;
	file.Load("nJoy.ini");
	bool Tmp; // Tmp storage

	// ==================================================================
	// Global settings
	// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
	file.Get("General", "ShowAdvanced", &g_Config.bShowAdvanced, false);
	if(!ChangeSaveByID)
	{
		file.Get("General", "SaveByID", &Tmp, false); g_Config.bSaveByID = Tmp;
	}
	// =============

	for (int i = 0; i < 4; i++)
	{
		std::string SectionName = StringFromFormat("PAD%i", i+1);

		// Don't update this when we are loading settings from the ConfigBox
		if(!ChangePad)
		{
			file.Get(SectionName.c_str(), "joy_id", &PadMapping[i].ID, 0);
			file.Get(SectionName.c_str(), "enabled", &PadMapping[i].enabled, 1);
		}

		// ==================================================================
		// Joypad or slot specific settings
		// ŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻŻ
			// Current joypad device ID: PadMapping[i].ID
			// Current joypad name: joyinfo[PadMapping[i].ID].Name
		if(g_Config.bSaveByID)
		{
			/* Prevent a crash from illegal access to joyinfo that will only have values for
			   the current amount of connected PadMapping */
			if(PadMapping[i].ID >= SDL_NumJoysticks()) continue;

			//PanicAlert("%i  %i",PadMapping[i].ID, SDL_NumJoysticks());
			//PanicAlert("%s", joyinfo[PadMapping[i].ID].Name);

			// Create a section name			
			SectionName = joyinfo[PadMapping[i].ID].Name;
		}

		file.Get(SectionName.c_str(), "l_shoulder", &PadMapping[i].buttons[CTL_L_SHOULDER], 4);
		file.Get(SectionName.c_str(), "r_shoulder", &PadMapping[i].buttons[CTL_R_SHOULDER], 5);
		file.Get(SectionName.c_str(), "a_button", &PadMapping[i].buttons[CTL_A_BUTTON], 0);
		file.Get(SectionName.c_str(), "b_button", &PadMapping[i].buttons[CTL_B_BUTTON], 1);	
		file.Get(SectionName.c_str(), "x_button", &PadMapping[i].buttons[CTL_X_BUTTON], 3);
		file.Get(SectionName.c_str(), "y_button", &PadMapping[i].buttons[CTL_Y_BUTTON], 2);	
		file.Get(SectionName.c_str(), "z_trigger", &PadMapping[i].buttons[CTL_Z_TRIGGER], 7);
		file.Get(SectionName.c_str(), "start_button", &PadMapping[i].buttons[CTL_START], 9);	
		file.Get(SectionName.c_str(), "dpad", &PadMapping[i].dpad, 0);	
		file.Get(SectionName.c_str(), "dpad_up", &PadMapping[i].dpad2[CTL_D_PAD_UP], 0);
		file.Get(SectionName.c_str(), "dpad_down", &PadMapping[i].dpad2[CTL_D_PAD_DOWN], 0);
		file.Get(SectionName.c_str(), "dpad_left", &PadMapping[i].dpad2[CTL_D_PAD_LEFT], 0);
		file.Get(SectionName.c_str(), "dpad_right", &PadMapping[i].dpad2[CTL_D_PAD_RIGHT], 0);
		file.Get(SectionName.c_str(), "main_x", &PadMapping[i].axis[CTL_MAIN_X], 0);	
		file.Get(SectionName.c_str(), "main_y", &PadMapping[i].axis[CTL_MAIN_Y], 1);	
		file.Get(SectionName.c_str(), "sub_x", &PadMapping[i].axis[CTL_SUB_X], 2);	
		file.Get(SectionName.c_str(), "sub_y", &PadMapping[i].axis[CTL_SUB_Y], 3);

		file.Get(SectionName.c_str(), "deadzone", &PadMapping[i].deadzone, 9);
		file.Get(SectionName.c_str(), "halfpress", &PadMapping[i].halfpress, -1);	
		file.Get(SectionName.c_str(), "controllertype", &PadMapping[i].controllertype, 0);
		file.Get(SectionName.c_str(), "TriggerType", &PadMapping[i].triggertype, 0);
		file.Get(SectionName.c_str(), "eventnum", &PadMapping[i].eventnum, 0);

		file.Get(SectionName.c_str(), "Diagonal", &PadMapping[i].SDiagonal, "100%");
		file.Get(SectionName.c_str(), "SquareToCircle", &Tmp, false); PadMapping[i].bSquareToCircle = Tmp;
		// =============================

		// Debugging
		//if(m_frame) m_frame->LogMsg("%i: Load triggertype: %s %i\n", i, SectionName.c_str(), PadMapping[i].triggertype);
	}
}


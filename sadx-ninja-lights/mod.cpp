#include "stdafx.h"

#include <SADXModLoader.h>
#include <shellapi.h>	// for CommandLineToArgvW
#include <string>
#include <unordered_map>
#include <algorithm>
#include "Stuff.h"
#include <Trampoline.h>
#include "teapot.h"
#include "lightvalues.h"

NJS_LIGHT   simple; //SIMPLE light
NJS_LIGHT   lighte; //ENV light
NJS_LIGHT   lightd; //DIR light

const char* modename[] = {"TWO LIGHTS", "SIMPLE", "EASY"};

char selection_y[] = { //Y coordinates for cursor
	7, 8, 9,    //XYZ for light 1
	11, 12, 13, //RGB for light 1
	15, 16, 17, //ADS for light 1
	22, 23, 24, //XYZ for light 2
	26, 27, 28, //RGB for light 2
	30, 31, 32  //ADS for light 2
};

Uint8 mode = 0;
Sint8 selection = 0;
Sint32  xx, yy, zz = 0;
bool show_true_dir = false;
bool hide_text = false;
int PrevX = 0;
int PrevY = 0;

void UpdateLighting()
{
    return;
    /*
    //Clean up all lights
    njDeleteLight(&lighte);
    njDeleteLight(&lightd);
    njDeleteLight(&simple);
    //Two lights mode
    if (mode == 0)
    {
        //ENV light
        njCreateLight(&lighte, 0x13);
        njSetLightColor(&lighte, lighte_r, lighte_g, lighte_b);
        njSetLightIntensity(&lighte, lighte_s, lighte_d, lighte_a);
        njSetLightDirection(&lighte, lighte_x, lighte_y, lighte_z);
        //DIR light
        njCreateLight(&lightd, 0x13);
        njSetLightColor(&lightd, lightd_r, lightd_g, lightd_b);
        njSetLightIntensity(&lightd, lightd_s, lightd_d, lightd_a);
        njSetLightDirection(&lightd, lightd_x, lightd_y, lightd_z);
    }
    //Simple mode
    else if (mode == 1)
    {
        njCreateLight(&simple, 0x13);
        njSetLightColor(&simple, simple_r, simple_g, simple_b);
        njSetLightIntensity(&simple, simple_s, simple_d, simple_a);
        njSetLightDirection(&simple, simple_x, simple_y, simple_z);
    }
    */
}


void DrawBGGradient(float x, float y, float z, float width, float height)
{
	NJS_COLOR v5; // edi
	NJS_COLOR v6; // esi
	double v7; // st7
	double v8; // st6
	float a3; // ST08_4
	NJS_COLOR col[4]; // [esp+10h] [ebp-40h]
	NJS_POINT2COL point2col; // [esp+20h] [ebp-30h]
	NJS_POINT2 point2[4]; // [esp+30h] [ebp-20h]

	v7 = y + height;
	point2col.p = point2;
	point2[1].y = v7;
	v8 = x + width;
	point2[2].x = v8;
	point2[3].x = v8;
	point2col.col = col;
	point2[3].y = v7;
	point2col.tex = 0;
	point2col.num = 4;
	a3 = -z;
	point2[0].x = x;
	point2[0].y = y;
	point2[1].x = x;
	point2[2].y = y;
	col[0].color = 0xFF000000;
	col[1].color = 0xFF0000FF;
	col[2].color = 0xFF000000;
	col[3].color = 0xFF0000FF;
	njDrawTriangle2D_SomeOtherVersion_(&point2col, 4, a3, NJD_FILL);
}

void Main(ObjectMaster* a1)
{
    DrawBGGradient(0, 0, 0, 640 * HorizontalStretch, 480 * VerticalStretch);
	Direct3D_SetNearFarPlanes(-1.0f, -20000.0f);
	njColorBlendingMode(0, NJD_COLOR_BLENDING_SRCALPHA);
	njColorBlendingMode(NJD_DESTINATION_COLOR, NJD_COLOR_BLENDING_ONE);
	njPushMatrix(0);
	njTranslate(0, 0, 0, -12.0f);
	njRotateXYZ(NULL, xx, yy, zz);
    /*
    if (mode == 0)
    {
        //ENV light
        njSetLightColor(&lighte, lighte_r, lighte_g, lighte_b);
        njSetLightIntensity(&lighte, lighte_s, lighte_d, lighte_a);
        njSetLightDirection(&lighte, lighte_x, lighte_y, lighte_z);
        //DIR light
        njSetLightColor(&lightd, lightd_r, lightd_g, lightd_b);
        njSetLightIntensity(&lightd, lightd_s, lightd_d, lightd_a);
        njSetLightDirection(&lightd, lightd_x, lightd_y, lightd_z);
    }
    else if (mode == 1)
    {
        //Simple light
        njSetLightColor(&simple, simple_r, simple_g, simple_b);
        njSetLightIntensity(&simple, simple_s, simple_d, simple_a);
        njSetLightDirection(&simple, simple_x, simple_y, simple_z);
    }
    */
    Direct3D_PerformLighting(0);
    if (!mode) DrawModel_ResetRenderFlags(&attach_00013B68); //Two lights mode
    else if (mode == 1) DrawModel_ResetRenderFlags(&attach_00013B68); //SIMPLE mode
    else //EASY mode
    {
        //njSetEasyLight(easy_x, easy_y, easy_z);
        //njSetEasyLightColor(easy_r, easy_g, easy_b);
        //njSetEasyLightIntensity(easy_d, easy_a);
        DrawModel_ResetRenderFlags(&attach_00013B68);
    }
	njPopMatrix(1u);
	ClampGlobalColorThing_Thing();
}

extern "C"
{
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		WriteJump((void*)0x510390, (void*)0x50E270);
		WriteJump((void*)0x50E230, Main);
		WriteData((int*)0x0040C10C, 11);
	}

    __declspec(dllexport) void __cdecl OnFrame()
    {
        if (!hide_text)
        {
            SetDebugFontSize(12 * HorizontalStretch);
            SetDebugFontColor(0xCC30F030);
            DisplayDebugStringFormatted(NJM_LOCATION(0, selection_y[selection]), ">");
            SetDebugFontColor(0xCCC0C0C0);
            DisplayDebugStringFormatted(NJM_LOCATION(14, 1), "-- NINJA LIGHT TEST --");
            DisplayDebugStringFormatted(NJM_LOCATION(1, 34), "Controls: ANALOG to rotate model,");
            DisplayDebugStringFormatted(NJM_LOCATION(1, 35), "TRIGGERS to show LIGHT struct fields,");
            DisplayDebugStringFormatted(NJM_LOCATION(1, 36), "START to change mode, UP / DOWN to move cursor,");
            DisplayDebugStringFormatted(NJM_LOCATION(1, 37), "A / B / LEFT / RIGHT to change values");
            DisplayDebugStringFormatted(NJM_LOCATION(1, 38), "X / Y to hide all text");
            DisplayDebugStringFormatted(NJM_LOCATION(1, 3), "MODE: %s", modename[mode]);
            //Two lights mode
            if (mode == 0)
            {
                //ENV light
                DisplayDebugString(NJM_LOCATION(1, 5), "LIGHT 1");
                if (!show_true_dir)
                {
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 7), "X  : %f", lighte_x);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 8), "Y  : %f", lighte_y);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 9), "Z  : %f", lighte_z);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 11), "R  : %f", lighte_r);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 12), "G  : %f", lighte_g);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 13), "B  : %f", lighte_b);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 15), "AMB: %f", lighte_a);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 16), "DIF: %f", lighte_d);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 17), "SPC: %f", lighte_s);
                }
                else
                {
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 7), "X  : %f", lighte.vctr.x);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 8), "Y  : %f", lighte.vctr.y);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 9), "Z  : %f", lighte.vctr.z);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 11), "R  : %f", lighte.attr.argb.r);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 12), "G  : %f", lighte.attr.argb.g);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 13), "B  : %f", lighte.attr.argb.b);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 15), "AMB: %f", lighte.attr.iamb);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 16), "DIF: %f", lighte.attr.idif);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 17), "SPC: %f", lighte.attr.ispc);
                }

                //DIR light
                DisplayDebugString(NJM_LOCATION(1, 20), "LIGHT 2");
                if (!show_true_dir)
                {
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 22), "X  : %f", lightd_x);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 23), "Y  : %f", lightd_y);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 24), "Z  : %f", lightd_z);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 26), "R  : %f", lightd_r);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 27), "G  : %f", lightd_g);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 28), "B  : %f", lightd_b);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 30), "AMB: %f", lightd_a);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 31), "DIF: %f", lightd_d);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 32), "SPC: %f", lightd_s);
                }
                else
                {
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 22), "X  : %f", lightd.vctr.x);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 23), "Y  : %f", lightd.vctr.y);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 24), "Z  : %f", lightd.vctr.z);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 26), "R  : %f", lightd.attr.argb.r);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 27), "G  : %f", lightd.attr.argb.g);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 28), "B  : %f", lightd.attr.argb.b);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 30), "AMB: %f", lightd.attr.iamb);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 31), "DIF: %f", lightd.attr.idif);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 32), "SPC: %f", lightd.attr.ispc);
                }
               
            }
            //Simple mode
            else if (mode == 1)
            {
                if (!show_true_dir)
                {
                    DisplayDebugString(NJM_LOCATION(1, 5), "SIMPLE LIGHT");
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 7), "X  : %f", simple_x);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 8), "Y  : %f", simple_y);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 9), "Z  : %f", simple_z);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 11), "AMB: %f", simple_a);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 12), "DIF: %f", simple_d);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 13), "SPC: %f", simple_s);
                }
                else
                {
                    DisplayDebugString(NJM_LOCATION(1, 5), "SIMPLE LIGHT");
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 7), "X  : %f", simple.vctr.x);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 8), "Y  : %f", simple.vctr.y);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 9), "Z  : %f", simple.vctr.z);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 11), "AMB: %f", simple.attr.iamb);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 12), "DIF: %f", simple.attr.idif);
                    DisplayDebugStringFormatted(NJM_LOCATION(1, 13), "SPC: %f", simple.attr.ispc);
                }
            }
            //Easy mode
            else if (mode == 2)
            {
                DisplayDebugString(NJM_LOCATION(1, 5), "EASY LIGHT");
                DisplayDebugStringFormatted(NJM_LOCATION(1, 7), "X  : %f", easy_x);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 8), "Y  : %f", easy_y);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 9), "Z  : %f", easy_z);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 11), "R  : %f", easy_r);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 12), "G  : %f", easy_g);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 13), "B  : %f", easy_b);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 15), "AMB: %f", easy_a);
                DisplayDebugStringFormatted(NJM_LOCATION(1, 16), "DIF: %f", easy_d);
            }
        }
    }

    __declspec(dllexport) void __cdecl OnInput()
    {
        ControllerData *pad = ControllerPointers[0];
        //Hack for second stick to imitate the D-Pad
        if (pad->RightStickY > 0 && PrevY == 0)
        {
            pad->PressedButtons |= Buttons_Down;
            PrevY = pad->RightStickY;
        }
        if (pad->RightStickY < 0 && PrevY == 0)
        {
            pad->PressedButtons |= Buttons_Up;
            PrevY = pad->RightStickY;
        }
        if (pad->RightStickX > 0 && PrevX == 0)
        {
            pad->PressedButtons |= Buttons_Right;
            PrevX = pad->RightStickX;
        }
        if (pad->RightStickX < 0 && PrevX == 0)
        {
            pad->PressedButtons |= Buttons_Left;
            PrevX = pad->RightStickX;
        }
        if (pad->RightStickY == 0) PrevY = 0;
        if (pad->RightStickX == 0) PrevX = 0;
        float valueadd = 0.0f;
        //Show or hide text
        if (pad->PressedButtons & Buttons_X || pad->PressedButtons & Buttons_Y) hide_text = !hide_text;
        //Show njVector-ed direction
        if (pad->PressedButtons & Buttons_L || pad->PressedButtons & Buttons_R) show_true_dir = !show_true_dir;
        //Switch modes
        if (pad->PressedButtons & Buttons_Start)
        {
            mode++;
            if (mode > 2) mode = 0;
            UpdateLighting();
        }
        //Move teapot
        xx += pad->LeftStickY * -4;
        yy += pad->LeftStickX * 4;
        //Move cursor
        if (pad->PressedButtons & Buttons_Up) selection++;
        if (pad->PressedButtons & Buttons_Down) selection--;
        if (mode == 0)
        {
            if (selection < 0) selection = 17;
            if (selection > 17) selection = 0;
        }
        else if (mode == 1)
        {
            if (selection < 0) selection = 5;
            if (selection > 5) selection = 0;
        }
        else if (mode == 2)
        {
            if (selection < 0) selection = 7;
            if (selection > 7) selection = 0;
        }
        //Increase or decrease values
        if (pad->PressedButtons & Buttons_Right || pad->PressedButtons & Buttons_Left || pad->HeldButtons & Buttons_A || pad->HeldButtons & Buttons_B)
        {
            if (pad->PressedButtons & Buttons_Right) valueadd = 0.1f;
            else if (pad->PressedButtons & Buttons_Left) valueadd = -0.1f;
            else if (pad->HeldButtons & Buttons_A) valueadd = 0.01f;
            else if (pad->HeldButtons & Buttons_B) valueadd = -0.01f;
            if (mode == 0)
            {
                if (selection == 0)       lighte_x += valueadd;
                else if (selection == 1)  lighte_y += valueadd;
                else if (selection == 2)  lighte_z += valueadd;
                else if (selection == 3)  lighte_r += valueadd;
                else if (selection == 4)  lighte_g += valueadd;
                else if (selection == 5)  lighte_b += valueadd;
                else if (selection == 6)  lighte_a += valueadd;
                else if (selection == 7)  lighte_d += valueadd;
                else if (selection == 8)  lighte_s += valueadd;
                else if (selection == 9)  lightd_x += valueadd;
                else if (selection == 10) lightd_y += valueadd;
                else if (selection == 11) lightd_z += valueadd;
                else if (selection == 12) lightd_r += valueadd;
                else if (selection == 13) lightd_g += valueadd;
                else if (selection == 14) lightd_b += valueadd;
                else if (selection == 15) lightd_a += valueadd;
                else if (selection == 16) lightd_d += valueadd;
                else if (selection == 17) lightd_s += valueadd;
                if (lighte_r < 0) lighte_r = 1;
                if (lighte_g < 0) lighte_g = 1;
                if (lighte_b < 0) lighte_b = 1;
                if (lighte_a < 0) lighte_a = 1;
                if (lighte_d < 0) lighte_d = 1;
                if (lighte_s < 0) lighte_s = 1;
                if (lightd_r < 0) lightd_r = 1;
                if (lightd_g < 0) lightd_g = 1;
                if (lightd_b < 0) lightd_b = 1;
                if (lightd_a < 0) lightd_a = 1;
                if (lightd_d < 0) lightd_d = 1;
                if (lightd_s < 0) lightd_s = 1;
            }
            else if (mode == 1)
            {
                if (selection == 0)  simple_x += valueadd;
                else if (selection == 1)  simple_y += valueadd;
                else if (selection == 2)  simple_z += valueadd;
                else if (selection == 3)  simple_a += valueadd;
                else if (selection == 4)  simple_d += valueadd;
                else if (selection == 5)  simple_s += valueadd;
                if (simple_a < 0) simple_a = 1;
                if (simple_d < 0) simple_d = 1;
                if (simple_s < 0) simple_s = 1;
            }
            else if (mode == 2)
            {
                if (selection == 0)       easy_x += valueadd;
                else if (selection == 1)  easy_y += valueadd;
                else if (selection == 2)  easy_z += valueadd;
                else if (selection == 3)  easy_r += valueadd;
                else if (selection == 4)  easy_g += valueadd;
                else if (selection == 5)  easy_b += valueadd;
                else if (selection == 6)  easy_a += valueadd;
                else if (selection == 7)  easy_d += valueadd;
                if (easy_r < 0) easy_r = 1;
                if (easy_g < 0) easy_g = 1;
                if (easy_b < 0) easy_b = 1;
                if (easy_a < 0) easy_a = 1;
                if (easy_d < 0) easy_d = 1;
            }
            valueadd = 0;
        }
    }
}
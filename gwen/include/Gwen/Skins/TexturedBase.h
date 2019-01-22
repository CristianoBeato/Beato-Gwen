/*
===========================================================================
GWEN

Copyright (c) 2010 Facepunch Studios
Copyright (c) 2017-2019 Cristiano Beato

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===========================================================================
*/

#ifndef GWEN_SKINS_TEXTUREDBASE_H
#define GWEN_SKINS_TEXTUREDBASE_H

#include "Skin.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/texture/Texture.h"
#include "Texturing.h"

namespace Gwen
{
	namespace Skin
	{
		class TexturedBase : public Gwen::Skin::Base
		{
			public:

				TexturedBase(Gwen::Renderer::BaseRender* renderer);

				~TexturedBase(void);

				Texture m_Texture;

				struct Textures_t
				{
					Texturing::Bordered StatusBar;
					Texturing::Bordered Selection;
					Texturing::Bordered	Shadow;
					Texturing::Bordered	Tooltip;

					struct Panel_t
					{
						Texturing::Bordered Normal;
						Texturing::Bordered Bright;
						Texturing::Bordered Dark;
						Texturing::Bordered Highlight;

					} Panel;

					struct Window_t
					{
						Texturing::Bordered Normal;
						Texturing::Bordered Inactive;
						Texturing::Single Close;
						Texturing::Single Close_Hover;
						Texturing::Single Close_Down;
						Texturing::Single Maxi;
						Texturing::Single Maxi_Hover;
						Texturing::Single Maxi_Down;
						Texturing::Single Mini;
						Texturing::Single Mini_Hover;
						Texturing::Single Mini_Down;
						Texturing::Single Restore;
						Texturing::Single Restore_Hover;
						Texturing::Single Restore_Down;

					} Window;

					struct Checkbox_t
					{
						struct  Active_t
						{
							Texturing::Single Normal;
							Texturing::Single Checked;

						} Active;

						struct  Disabled_t
						{
							Texturing::Single Normal;
							Texturing::Single Checked;

						} Disabled;

					} Checkbox;

					struct  RadioButton_t
					{
						struct  Active_t
						{
							Texturing::Single Normal;
							Texturing::Single Checked;

						} Active;

						struct  Disabled_t
						{
							Texturing::Single Normal;
							Texturing::Single Checked;

						} Disabled;

					} RadioButton;

					struct  TextBox_t
					{
						Texturing::Bordered Normal;
						Texturing::Bordered Focus;
						Texturing::Bordered Disabled;

					} TextBox;

					struct  Tree_t
					{
						Texturing::Bordered Background;
						Texturing::Single Minus;
						Texturing::Single Plus;

					} Tree;


					struct  ProgressBar_t
					{
						Texturing::Bordered Back;
						Texturing::Bordered Front;

					} ProgressBar;

					struct  Scroller_t
					{
						Texturing::Bordered TrackV;
						Texturing::Bordered ButtonV_Normal;
						Texturing::Bordered ButtonV_Hover;
						Texturing::Bordered ButtonV_Down;
						Texturing::Bordered ButtonV_Disabled;
						Texturing::Bordered TrackH;
						Texturing::Bordered ButtonH_Normal;
						Texturing::Bordered ButtonH_Hover;
						Texturing::Bordered ButtonH_Down;
						Texturing::Bordered ButtonH_Disabled;

						struct  Button_t
						{
							Texturing::Bordered	Normal[4];
							Texturing::Bordered	Hover[4];
							Texturing::Bordered	Down[4];
							Texturing::Bordered	Disabled[4];

						} Button;

					} Scroller;

					struct  Menu_t
					{
						Texturing::Single RightArrow;
						Texturing::Single Check;

						Texturing::Bordered Strip;
						Texturing::Bordered Background;
						Texturing::Bordered BackgroundWithMargin;
						Texturing::Bordered Hover;

					} Menu;

					struct  Input_t
					{
						struct  Button_t
						{
							Texturing::Bordered Normal;
							Texturing::Bordered Hovered;
							Texturing::Bordered Disabled;
							Texturing::Bordered Pressed;

						} Button;

						struct  ListBox_t
						{
							Texturing::Bordered Background;
							Texturing::Bordered Hovered;

							Texturing::Bordered EvenLine;
							Texturing::Bordered OddLine;
							Texturing::Bordered EvenLineSelected;
							Texturing::Bordered OddLineSelected;

						} ListBox;

						struct  UpDown_t
						{
							struct  Up_t
							{
								Texturing::Single Normal;
								Texturing::Single Hover;
								Texturing::Single Down;
								Texturing::Single Disabled;

							} Up;

							struct  Down_t
							{
								Texturing::Single Normal;
								Texturing::Single Hover;
								Texturing::Single Down;
								Texturing::Single Disabled;

							} Down;

						} UpDown;

						struct  ComboBox_t
						{
							Texturing::Bordered Normal;
							Texturing::Bordered Hover;
							Texturing::Bordered Down;
							Texturing::Bordered Disabled;

							struct  Button_t
							{
								Texturing::Single Normal;
								Texturing::Single Hover;
								Texturing::Single Down;
								Texturing::Single Disabled;

							} Button;

						} ComboBox;

						struct  Slider_t
						{
							struct  H_t
							{
								Texturing::Single Normal;
								Texturing::Single Hover;
								Texturing::Single Down;
								Texturing::Single Disabled;
							} H;

							struct  V_t
							{
								Texturing::Single Normal;
								Texturing::Single Hover;
								Texturing::Single Down;
								Texturing::Single Disabled;
							} V;

						} Slider;

					} Input;

					struct  Tab_t
					{
						struct  Bottom_t
						{
							Texturing::Bordered Active;
							Texturing::Bordered Inactive;
						} Bottom;

						struct  Top_t
						{
							Texturing::Bordered Active;
							Texturing::Bordered Inactive;
						} Top;

						struct  Left_t
						{
							Texturing::Bordered Active;
							Texturing::Bordered Inactive;
						} Left;

						struct  Right_t
						{
							Texturing::Bordered Active;
							Texturing::Bordered Inactive;
						} Right;

						Texturing::Bordered Control;
						Texturing::Bordered HeaderBar;

					} Tab;

					struct  CategoryList_t
					{
						Texturing::Bordered Outer;
						Texturing::Bordered Inner;
						Texturing::Bordered Header;

					} CategoryList;

					Texturing::Bordered GroupBox;

				} Textures;

				virtual void Init(const TextObject & TextureName);
				virtual void DrawButton(Gwen::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled);
				virtual void DrawMenuItem(Gwen::Controls::Base* control, bool bSubmenuOpen, bool bChecked);
				virtual void DrawMenuStrip(Gwen::Controls::Base* control);
				virtual void DrawMenu(Gwen::Controls::Base* control, bool bPaddingDisabled);
				virtual void DrawMenuRightArrow(Controls::Base* control);
				virtual void DrawShadow(Gwen::Controls::Base* control);
				virtual void DrawRadioButton(Gwen::Controls::Base* control, bool bSelected, bool bDepressed);
				virtual void DrawCheckBox(Gwen::Controls::Base* control, bool bSelected, bool bDepressed);
				virtual void DrawGroupBox(Gwen::Controls::Base* control, int textStart, int textHeight, int textWidth);
				virtual void DrawTextBox(Gwen::Controls::Base* control);
				virtual void DrawActiveTabButton(Gwen::Controls::Base* control, int dir);
				virtual void DrawTabButton(Gwen::Controls::Base* control, bool bActive, int dir);
				virtual void DrawTabControl(Gwen::Controls::Base* control);
				virtual void DrawTabTitleBar(Gwen::Controls::Base* control);
				virtual void DrawGenericPanel(Controls::Base* control);
				virtual void DrawWindow(Gwen::Controls::Base* control, int topHeight, bool inFocus);
				virtual void DrawHighlight(Gwen::Controls::Base* control);
				virtual void DrawScrollBar(Gwen::Controls::Base* control, bool isHorizontal, bool bDepressed);
				virtual void DrawScrollBarBar(Controls::Base* control, bool bDepressed, bool isHovered, bool isHorizontal);
				virtual void DrawProgressBar(Gwen::Controls::Base* control, bool isHorizontal, float progress);
				virtual void DrawListBox(Gwen::Controls::Base* control);
				virtual void DrawListBoxLine(Gwen::Controls::Base* control, bool bSelected, bool bEven);
				void DrawSliderNotchesH(Gwen::Rect rect, int numNotches, int dist);
				void DrawSliderNotchesV(Gwen::Rect rect, int numNotches, int dist);
				virtual void DrawSlider(Gwen::Controls::Base* control, bool bIsHorizontal, int numNotches, int barSize);
				virtual void DrawComboBox(Gwen::Controls::Base* control, bool bDown, bool bMenuOpen);
				virtual void DrawKeyboardHighlight(Gwen::Controls::Base* control, const Gwen::Rect & r, int iOffset);
				virtual void DrawToolTip(Gwen::Controls::Base* control);
				virtual void DrawScrollButton(Gwen::Controls::Base* control, int iDirection, bool bDepressed, bool bHovered, bool bDisabled);
				virtual void DrawComboDownArrow(Gwen::Controls::Base* control, bool bHovered, bool bDown, bool bMenuOpen, bool bDisabled);
				virtual void DrawNumericUpDownButton(Gwen::Controls::Base* control, bool bDepressed, bool bUp);
				virtual void DrawStatusBar(Controls::Base* control);
				virtual void DrawTreeButton(Controls::Base* control, bool bOpen);
				void DrawColorDisplay(Controls::Base* control, Gwen::Color color);
				virtual void DrawModalControl(Controls::Base* control);
				virtual void DrawMenuDivider(Controls::Base* control);
				virtual void DrawTreeControl(Controls::Base* control);
				virtual void DrawWindowCloseButton(Gwen::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled);
				virtual void DrawWindowMaximizeButton(Gwen::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled, bool bMaximized);
				virtual void DrawWindowMinimizeButton(Gwen::Controls::Base* control, bool bDepressed, bool bHovered, bool bDisabled);
				virtual void DrawSlideButton(Gwen::Controls::Base* control, bool bDepressed, bool bHorizontal);
				void DrawTreeNode(Controls::Base* ctrl, bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot);
				void DrawCategoryHolder(Controls::Base* ctrl);
				void DrawCategoryInner(Controls::Base* ctrl, bool bCollapsed);
		};
	}
}
#endif

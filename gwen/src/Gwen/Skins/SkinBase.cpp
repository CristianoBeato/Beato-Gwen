/*
===========================================================================
GWEN

Copyright (c) 2010 Facepunch Studios
Copyright (c) 2017-2018 Cristiano Beato

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

#include "precompiled.h"
#pragma hdrstop

#include "Gwen/Skins/SkinBase.h"
#include "tinyxml2.h"


Gwen::Skin::SkinBase::SkinBase(Gwen::Renderer::Base * renderer) : Gwen::Skin::Base(renderer)
{
}

Gwen::Skin::SkinBase::~SkinBase(void)
{
	Clear();
}

inline bool Gwen::Skin::SkinBase::Init(const TextObject & SkinXml)
{ 
	tinyxml2::XMLElement* skinElement;
	tinyxml2::XMLDocument *skinDocument = NULL;
	char * docSrc = NULL;
	//file strteam from the src xml
	Platform::Stream	*srcFile = new Platform::FileStream;

	//Try Open the file to read
	if (!srcFile->open(SkinXml.c_str(), "r"))
	{
		//use defalt set
		SetDefaltSkin();

		delete srcFile;
		return false;
	}


	//Read src
	size_t fileSize = srcFile->size();
	docSrc = (char*)SDL_malloc(fileSize);
	srcFile->read(docSrc, fileSize);

	//can free the file handler
	delete srcFile;

	//create the XML interation, and 
	skinDocument = new tinyxml2::XMLDocument;
	skinDocument->Parse(docSrc);

	//check for error
	if (skinDocument->ErrorID() != tinyxml2::XML_SUCCESS)
	{
		//use defalt set
		SetDefaltSkin();

		delete skinDocument;
		//TODO: handler error here
		return false;
	}

	//star get the documents elements
	skinElement = skinDocument->FirstChildElement("GWENSkin");
	if (skinElement != NULL)
	{
		tinyxml2::XMLElement* titleElement = skinElement->FirstChildElement();
		while (titleElement != NULL)
		{
			TextObject className = titleElement->Name();
			/*
			====================
			Load Skins Textures
			====================
			*/
			if (className == TextObject("Textures"))
			{
				tinyxml2::XMLElement* textureElement = skinElement->FirstChildElement("texture");
				while (textureElement != NULL)
				{
					const char *textureName = textureElement->Attribute("name");
					const char *texturePath = textureElement->Attribute("image");
					
					//
					if (textureName != nullptr && textureName != nullptr)
						LoadTexture(textureName, textureName);

					textureElement = textureElement->NextSiblingElement("texture");
				}
			}

			/*
			====================
			Load Classes Skins definitions
			====================
			*/
			else if (className == TextObject("Window"))
			{
				tinyxml2::XMLElement* Active = titleElement->FirstChildElement("Active");
				if (Active)
				{
					setColor(Colors.Window.TitleActive, Active);
					setTexture(Textures.Window.Normal, Active);
				}

				tinyxml2::XMLElement* Inactive = titleElement->FirstChildElement("Inactive");
				if (Inactive)
				{
					setColor(Colors.Window.TitleInactive, Inactive);
					setTexture(Textures.Window.Inactive, Inactive);
				}
			}
			else if (className == TextObject("Button"))
			{

			}
			else if (className == TextObject("Tab"))
			{

			}
			else if (className == TextObject("Label"))
			{

			}
			else if (className == TextObject("Tree"))
			{

			}
			else if (className == TextObject("Properties"))
			{

			}
			else if (className == TextObject("Category"))
			{

			}
			titleElement = titleElement->NextSiblingElement();
		}
	}
	else
	{
		//use defalt set
		SetDefaltSkin();

		delete skinDocument;
		//TODO: handler error here
		return false;
	}
	
	SDL_free(docSrc);

	return true;
}

void Gwen::Skin::SkinBase::Clear(void)
{
	//clear textures from render
	for (std::pair<Uint16, Texture*> texture : m_skinTextures)
	{
		//free and delete texture reference
		texture.second->Release(m_Render);
		delete texture.second;
	}

	//clear texture list
	m_skinTextures.clear();
}

inline void Gwen::Skin::SkinBase::DrawButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled)
{
	if (bDisabled) 
		return Textures.Input.Button.Disabled.Draw(GetRender(), control->GetRenderBounds());

	if (bDepressed) 
		return Textures.Input.Button.Pressed.Draw(GetRender(), control->GetRenderBounds());

	if (bHovered)
		return Textures.Input.Button.Hovered.Draw(GetRender(), control->GetRenderBounds());

	Textures.Input.Button.Normal.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawMenuItem(Gwen::Controls::Base * control, bool bSubmenuOpen, bool bChecked)
{
	const Gwen::Rect & rect = control->GetRenderBounds();

	if (bSubmenuOpen || control->IsHovered())
		Textures.Menu.Hover.Draw(GetRender(), rect);

	if (bChecked)
		Textures.Menu.Check.Draw(GetRender(), Gwen::Rect(rect.x + 4, rect.y + 3, 15, 15));
}

inline void Gwen::Skin::SkinBase::DrawMenuStrip(Gwen::Controls::Base * control)
{
	Textures.Menu.Strip.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawMenu(Gwen::Controls::Base * control, bool bPaddingDisabled)
{
	if (!bPaddingDisabled)
		return Textures.Menu.BackgroundWithMargin.Draw(GetRender(), control->GetRenderBounds());

	Textures.Menu.Background.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawMenuRightArrow(Controls::Base * control)
{
	Textures.Menu.RightArrow.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawShadow(Gwen::Controls::Base * control)
{
	Gwen::Rect r = control->GetRenderBounds();
	r.x -= 4;
	r.y -= 4;
	r.w += 10;
	r.h += 10;
	Textures.Shadow.Draw(GetRender(), r);
}

inline void Gwen::Skin::SkinBase::DrawRadioButton(Gwen::Controls::Base * control, bool bSelected, bool bDepressed)
{
	if (bSelected)
	{
		if (control->IsDisabled())
			Textures.RadioButton.Disabled.Checked.Draw(GetRender(), control->GetRenderBounds());
		else
			Textures.RadioButton.Active.Checked.Draw(GetRender(), control->GetRenderBounds());
	}
	else
	{
		if (control->IsDisabled())
			Textures.RadioButton.Disabled.Normal.Draw(GetRender(), control->GetRenderBounds());
		else
			Textures.RadioButton.Active.Normal.Draw(GetRender(), control->GetRenderBounds());
	}
}

inline void Gwen::Skin::SkinBase::DrawCheckBox(Gwen::Controls::Base * control, bool bSelected, bool bDepressed)
{
	if (bSelected)
	{
		if (control->IsDisabled())
			Textures.Checkbox.Disabled.Checked.Draw(GetRender(), control->GetRenderBounds());
		else
			Textures.Checkbox.Active.Checked.Draw(GetRender(), control->GetRenderBounds());
	}
	else
	{
		if (control->IsDisabled())
			Textures.Checkbox.Disabled.Normal.Draw(GetRender(), control->GetRenderBounds());
		else
			Textures.Checkbox.Active.Normal.Draw(GetRender(), control->GetRenderBounds());
	}
}

inline void Gwen::Skin::SkinBase::DrawGroupBox(Gwen::Controls::Base * control, int textStart, int textHeight, int textWidth)
{
	Gwen::Rect rect = control->GetRenderBounds();
	rect.y += textHeight * 0.5f;
	rect.h -= textHeight * 0.5f;
	Textures.GroupBox.Draw(GetRender(), rect, Gwen::Colors::White, true, false);
	rect.x += textStart + textWidth - 4;
	rect.w -= textStart + textWidth - 4;
	Textures.GroupBox.Draw(GetRender(), rect, Gwen::Colors::White, false, true, false, false, false, false, false, false, false);
}

inline void Gwen::Skin::SkinBase::DrawTextBox(Gwen::Controls::Base * control)
{
	if (control->IsDisabled())
	{
		return Textures.TextBox.Disabled.Draw(GetRender(), control->GetRenderBounds());
	}

	if (control->HasFocus())
	{
		Textures.TextBox.Focus.Draw(GetRender(), control->GetRenderBounds());
	}
	else
	{
		Textures.TextBox.Normal.Draw(GetRender(), control->GetRenderBounds());
	}
}

inline void Gwen::Skin::SkinBase::DrawActiveTabButton(Gwen::Controls::Base * control, int dir)
{
	if (dir == Pos::Bottom) { return Textures.Tab.Bottom.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(0, -8, 0, 8)); }

	if (dir == Pos::Top) { return Textures.Tab.Top.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(0, 0, 0, 8)); }

	if (dir == Pos::Left) { return Textures.Tab.Left.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(0, 0, 8, 0)); }

	if (dir == Pos::Right) { return Textures.Tab.Right.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(-8, 0, 8, 0)); }
}

inline void Gwen::Skin::SkinBase::DrawTabButton(Gwen::Controls::Base * control, bool bActive, int dir)
{
	if (bActive)
	{
		return DrawActiveTabButton(control, dir);
	}

	if (dir == Pos::Bottom) { return Textures.Tab.Bottom.Inactive.Draw(GetRender(), control->GetRenderBounds()); }

	if (dir == Pos::Top) { return Textures.Tab.Top.Inactive.Draw(GetRender(), control->GetRenderBounds()); }

	if (dir == Pos::Left) { return Textures.Tab.Left.Inactive.Draw(GetRender(), control->GetRenderBounds()); }

	if (dir == Pos::Right) { return Textures.Tab.Right.Inactive.Draw(GetRender(), control->GetRenderBounds()); }
}

inline void Gwen::Skin::SkinBase::DrawTabControl(Gwen::Controls::Base * control)
{
	Textures.Tab.Control.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawTabTitleBar(Gwen::Controls::Base * control)
{
	Textures.Tab.HeaderBar.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawGenericPanel(Controls::Base * control)
{
	Textures.Panel.Normal.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawWindow(Gwen::Controls::Base * control, int topHeight, bool inFocus)
{
	if (inFocus) { Textures.Window.Normal.Draw(GetRender(), control->GetRenderBounds()); }
	else { Textures.Window.Inactive.Draw(GetRender(), control->GetRenderBounds()); }
}

inline void Gwen::Skin::SkinBase::DrawHighlight(Gwen::Controls::Base * control)
{
	Gwen::Rect rect = control->GetRenderBounds();
	GetRender()->SetDrawColor(Gwen::Color(255, 100, 255, 255));
	GetRender()->DrawFilledRect(rect);
}

inline void Gwen::Skin::SkinBase::DrawScrollBar(Gwen::Controls::Base * control, bool isHorizontal, bool bDepressed)
{
	if (isHorizontal)
	{
		Textures.Scroller.TrackH.Draw(GetRender(), control->GetRenderBounds());
	}
	else
	{
		Textures.Scroller.TrackV.Draw(GetRender(), control->GetRenderBounds());
	}
}

inline void Gwen::Skin::SkinBase::DrawScrollBarBar(Controls::Base * control, bool bDepressed, bool isHovered, bool isHorizontal)
{
	if (!isHorizontal)
	{
		if (control->IsDisabled())
		{
			return Textures.Scroller.ButtonV_Disabled.Draw(GetRender(), control->GetRenderBounds());
		}

		if (bDepressed)
		{
			return Textures.Scroller.ButtonV_Down.Draw(GetRender(), control->GetRenderBounds());
		}

		if (isHovered)
		{
			return Textures.Scroller.ButtonV_Hover.Draw(GetRender(), control->GetRenderBounds());
		}

		return Textures.Scroller.ButtonV_Normal.Draw(GetRender(), control->GetRenderBounds());
	}

	if (control->IsDisabled())
	{
		return Textures.Scroller.ButtonH_Disabled.Draw(GetRender(), control->GetRenderBounds());
	}

	if (bDepressed)
	{
		return Textures.Scroller.ButtonH_Down.Draw(GetRender(), control->GetRenderBounds());
	}

	if (isHovered)
	{
		return Textures.Scroller.ButtonH_Hover.Draw(GetRender(), control->GetRenderBounds());
	}

	return Textures.Scroller.ButtonH_Normal.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawProgressBar(Gwen::Controls::Base * control, bool isHorizontal, float progress)
{
	Gwen::Rect rect = control->GetRenderBounds();
	Gwen::Color FillColour(0, 211, 40, 255);

	if (isHorizontal)
	{
		Textures.ProgressBar.Back.Draw(GetRender(), rect);
		rect.w *= progress;

		if (rect.w > 0)
		{
			Textures.ProgressBar.Front.Draw(GetRender(), rect);
		}
	}
	else
	{
		Textures.ProgressBar.Back.Draw(GetRender(), rect);
		int inv_progress = rect.h * (1 - progress);
		rect.y += inv_progress;
		rect.h -= inv_progress;
		Textures.ProgressBar.Front.Draw(GetRender(), rect);
	}
}

inline void Gwen::Skin::SkinBase::DrawListBox(Gwen::Controls::Base * control)
{
	return Textures.Input.ListBox.Background.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawListBoxLine(Gwen::Controls::Base * control, bool bSelected, bool bEven)
{
	if (bSelected)
	{
		if (bEven)
		{
			return Textures.Input.ListBox.EvenLineSelected.Draw(GetRender(), control->GetRenderBounds());
		}
		else
		{
			return Textures.Input.ListBox.OddLineSelected.Draw(GetRender(), control->GetRenderBounds());
		}
	}

	if (control->IsHovered())
	{
		return Textures.Input.ListBox.Hovered.Draw(GetRender(), control->GetRenderBounds());
	}

	if (bEven)
	{
		return Textures.Input.ListBox.EvenLine.Draw(GetRender(), control->GetRenderBounds());
	}

	return Textures.Input.ListBox.OddLine.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawSliderNotchesH(Gwen::Rect rect, int numNotches, int dist)
{
	if (numNotches == 0) { return; }

	float iSpacing = (float)rect.w / (float)numNotches;

	for (int i = 0; i < numNotches + 1; i++)
	{
		GetRender()->DrawFilledRect(Gwen::Rect(rect.x + iSpacing * i, rect.y + dist - 2, 1, 5));
	}
}

inline void Gwen::Skin::SkinBase::DrawSliderNotchesV(Gwen::Rect rect, int numNotches, int dist)
{
	if (numNotches == 0) { return; }

	float iSpacing = (float)rect.h / (float)numNotches;

	for (int i = 0; i < numNotches + 1; i++)
	{
		GetRender()->DrawFilledRect(Gwen::Rect(rect.x + dist - 1, rect.y + iSpacing * i, 5, 1));
	}
}

inline void Gwen::Skin::SkinBase::DrawSlider(Gwen::Controls::Base * control, bool bIsHorizontal, int numNotches, int barSize)
{
	if (bIsHorizontal)
	{
		Gwen::Rect rect = control->GetRenderBounds();
		rect.x += barSize * 0.5;
		rect.w -= barSize;
		rect.y += rect.h * 0.5 - 1;
		rect.h = 1;
		GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 100));
		DrawSliderNotchesH(rect, numNotches, barSize * 0.5);
		return GetRender()->DrawFilledRect(rect);
	}

	Gwen::Rect rect = control->GetRenderBounds();
	rect.y += barSize * 0.5;
	rect.h -= barSize;
	rect.x += rect.w * 0.5 - 1;
	rect.w = 1;
	GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 100));
	DrawSliderNotchesV(rect, numNotches, barSize * 0.4);
	return GetRender()->DrawFilledRect(rect);
}

inline void Gwen::Skin::SkinBase::DrawComboBox(Gwen::Controls::Base * control, bool bDown, bool bMenuOpen)
{
	if (control->IsDisabled())
	{
		return Textures.Input.ComboBox.Disabled.Draw(GetRender(), control->GetRenderBounds());
	}

	if (bDown || bMenuOpen)
	{
		return Textures.Input.ComboBox.Down.Draw(GetRender(), control->GetRenderBounds());
	}

	if (control->IsHovered())
	{
		return Textures.Input.ComboBox.Hover.Draw(GetRender(), control->GetRenderBounds());
	}

	Textures.Input.ComboBox.Normal.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawKeyboardHighlight(Gwen::Controls::Base * control, const Gwen::Rect & r, int iOffset)
{
	Gwen::Rect rect = r;
	rect.x += iOffset;
	rect.y += iOffset;
	rect.w -= iOffset * 2;
	rect.h -= iOffset * 2;
	//draw the top and bottom
	bool skip = true;

	for (int i = 0; i < rect.w * 0.5; i++)
	{
		m_Render->SetDrawColor(Gwen::Color(0, 0, 0, 255));

		if (!skip)
		{
			GetRender()->DrawPixel(rect.x + (i * 2), rect.y);
			GetRender()->DrawPixel(rect.x + (i * 2), rect.y + rect.h - 1);
		}
		else
		{
			skip = !skip;
		}
	}

	skip = false;

	for (int i = 0; i < rect.h * 0.5; i++)
	{
		GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 255));

		if (!skip)
		{
			GetRender()->DrawPixel(rect.x, rect.y + i * 2);
			GetRender()->DrawPixel(rect.x + rect.w - 1, rect.y + i * 2);
		}
		else
		{
			skip = !skip;
		}
	}
}

inline void Gwen::Skin::SkinBase::DrawToolTip(Gwen::Controls::Base * control)
{
	return Textures.Tooltip.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawScrollButton(Gwen::Controls::Base * control, int iDirection, bool bDepressed, bool bHovered, bool bDisabled)
{
	int i = 0;

	if (iDirection == Pos::Top) { i = 1; }

	if (iDirection == Pos::Right) { i = 2; }

	if (iDirection == Pos::Bottom) { i = 3; }

	if (bDisabled)
	{
		return Textures.Scroller.Button.Disabled[i].Draw(GetRender(), control->GetRenderBounds());
	}

	if (bDepressed)
	{
		return Textures.Scroller.Button.Down[i].Draw(GetRender(), control->GetRenderBounds());
	}

	if (bHovered)
	{
		return Textures.Scroller.Button.Hover[i].Draw(GetRender(), control->GetRenderBounds());
	}

	return Textures.Scroller.Button.Normal[i].Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawComboDownArrow(Gwen::Controls::Base * control, bool bHovered, bool bDown, bool bMenuOpen, bool bDisabled)
{
	if (bDisabled)
	{
		return Textures.Input.ComboBox.Button.Disabled.Draw(GetRender(), control->GetRenderBounds());
	}

	if (bDown || bMenuOpen)
	{
		return Textures.Input.ComboBox.Button.Down.Draw(GetRender(), control->GetRenderBounds());
	}

	if (bHovered)
	{
		return Textures.Input.ComboBox.Button.Hover.Draw(GetRender(), control->GetRenderBounds());
	}

	Textures.Input.ComboBox.Button.Normal.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawNumericUpDownButton(Gwen::Controls::Base * control, bool bDepressed, bool bUp)
{
	if (bUp)
	{
		if (control->IsDisabled()) { return Textures.Input.UpDown.Up.Disabled.DrawCenter(GetRender(), control->GetRenderBounds()); }

		if (bDepressed) { return Textures.Input.UpDown.Up.Down.DrawCenter(GetRender(), control->GetRenderBounds()); }

		if (control->IsHovered()) { return Textures.Input.UpDown.Up.Hover.DrawCenter(GetRender(), control->GetRenderBounds()); }

		return Textures.Input.UpDown.Up.Normal.DrawCenter(GetRender(), control->GetRenderBounds());
	}

	if (control->IsDisabled()) { return Textures.Input.UpDown.Down.Disabled.DrawCenter(GetRender(), control->GetRenderBounds()); }

	if (bDepressed) { return Textures.Input.UpDown.Down.Down.DrawCenter(GetRender(), control->GetRenderBounds()); }

	if (control->IsHovered()) { return Textures.Input.UpDown.Down.Hover.DrawCenter(GetRender(), control->GetRenderBounds()); }

	return Textures.Input.UpDown.Down.Normal.DrawCenter(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawStatusBar(Controls::Base * control)
{
	Textures.StatusBar.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawTreeButton(Controls::Base * control, bool bOpen)
{
	Gwen::Rect rect = control->GetRenderBounds();

	if (bOpen)
	{
		Textures.Tree.Minus.Draw(GetRender(), rect);
	}
	else
	{
		Textures.Tree.Plus.Draw(GetRender(), rect);
	}
}

inline void Gwen::Skin::SkinBase::DrawColorDisplay(Controls::Base * control, Gwen::Color color)
{
	Gwen::Rect rect = control->GetRenderBounds();

	if (color.a != 255)
	{
		GetRender()->SetDrawColor(Gwen::Color(255, 255, 255, 255));
		GetRender()->DrawFilledRect(rect);
		GetRender()->SetDrawColor(Gwen::Color(128, 128, 128, 128));
		GetRender()->DrawFilledRect(Gwen::Rect(0, 0, rect.w * 0.5, rect.h * 0.5));
		GetRender()->DrawFilledRect(Gwen::Rect(rect.w * 0.5, rect.h * 0.5, rect.w * 0.5, rect.h * 0.5));
	}

	GetRender()->SetDrawColor(color);
	GetRender()->DrawFilledRect(rect);
	GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 255));
	GetRender()->DrawLinedRect(rect);
}

inline void Gwen::Skin::SkinBase::DrawModalControl(Controls::Base * control)
{
	if (!control->ShouldDrawBackground()) { return; }

	Gwen::Rect rect = control->GetRenderBounds();
	GetRender()->SetDrawColor(Colors.ModalBackground);
	GetRender()->DrawFilledRect(rect);
}

inline void Gwen::Skin::SkinBase::DrawMenuDivider(Controls::Base * control)
{
	Gwen::Rect rect = control->GetRenderBounds();
	GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 100));
	GetRender()->DrawFilledRect(rect);
}

inline void Gwen::Skin::SkinBase::DrawTreeControl(Controls::Base * control)
{
	Textures.Tree.Background.Draw(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawWindowCloseButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled)
{
	Gwen::Rect r = Gwen::Rect(control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31);

	if (bDisabled) { return Textures.Window.Close.Draw(GetRender(), r, Gwen::Color(255, 255, 255, 50)); }

	if (bDepressed) { return Textures.Window.Close_Down.Draw(GetRender(), r); }

	if (bHovered) { return Textures.Window.Close_Hover.Draw(GetRender(), r); }

	Textures.Window.Close.Draw(GetRender(), r);
}

inline void Gwen::Skin::SkinBase::DrawWindowMaximizeButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled, bool bMaximized)
{
	Gwen::Rect r = Gwen::Rect(control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31);

	if (!bMaximized)
	{
		if (bDisabled) { return Textures.Window.Maxi.Draw(GetRender(), r, Gwen::Color(255, 255, 255, 50)); }

		if (bDepressed) { return Textures.Window.Maxi_Down.Draw(GetRender(), r); }

		if (bHovered) { return Textures.Window.Maxi_Hover.Draw(GetRender(), r); }

		return Textures.Window.Maxi.Draw(GetRender(), r);
	}

	if (bDisabled) { return Textures.Window.Restore.Draw(GetRender(), r, Gwen::Color(255, 255, 255, 50)); }

	if (bDepressed) { return Textures.Window.Restore_Down.Draw(GetRender(), r); }

	if (bHovered) { return Textures.Window.Restore_Hover.Draw(GetRender(), r); }

	return Textures.Window.Restore.Draw(GetRender(), r);
}

inline void Gwen::Skin::SkinBase::DrawWindowMinimizeButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled)
{
	Gwen::Rect r = Gwen::Rect(control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31);

	if (bDisabled) { return Textures.Window.Mini.Draw(GetRender(), r, Gwen::Color(255, 255, 255, 100)); }

	if (bDepressed) { return Textures.Window.Mini_Down.Draw(GetRender(), r); }

	if (bHovered) { return Textures.Window.Mini_Hover.Draw(GetRender(), r); }

	Textures.Window.Mini.Draw(GetRender(), r);
}

inline void Gwen::Skin::SkinBase::DrawSlideButton(Gwen::Controls::Base * control, bool bDepressed, bool bHorizontal)
{
	if (!bHorizontal)
	{
		if (control->IsDisabled()) { return Textures.Input.Slider.V.Disabled.DrawCenter(GetRender(), control->GetRenderBounds()); }

		if (bDepressed) { return Textures.Input.Slider.V.Down.DrawCenter(GetRender(), control->GetRenderBounds()); }

		if (control->IsHovered()) { return Textures.Input.Slider.V.Hover.DrawCenter(GetRender(), control->GetRenderBounds()); }

		return Textures.Input.Slider.V.Normal.DrawCenter(GetRender(), control->GetRenderBounds());
	}

	if (control->IsDisabled()) { return Textures.Input.Slider.H.Disabled.DrawCenter(GetRender(), control->GetRenderBounds()); }

	if (bDepressed) { return Textures.Input.Slider.H.Down.DrawCenter(GetRender(), control->GetRenderBounds()); }

	if (control->IsHovered()) { return Textures.Input.Slider.H.Hover.DrawCenter(GetRender(), control->GetRenderBounds()); }

	Textures.Input.Slider.H.Normal.DrawCenter(GetRender(), control->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawTreeNode(Controls::Base * ctrl, bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot)
{
	if (bSelected)
	{
		Textures.Selection.Draw(GetRender(), Gwen::Rect(17, 0, iLabelWidth + 2, iLabelHeight - 1));
	}

	Base::DrawTreeNode(ctrl, bOpen, bSelected, iLabelHeight, iLabelWidth, iHalfWay, iLastBranch, bIsRoot);
}

inline void Gwen::Skin::SkinBase::DrawCategoryHolder(Controls::Base * ctrl)
{
	Textures.CategoryList.Outer.Draw(GetRender(), ctrl->GetRenderBounds());
}

inline void Gwen::Skin::SkinBase::DrawCategoryInner(Controls::Base * ctrl, bool bCollapsed)
{
	if (bCollapsed)
	{
		return Textures.CategoryList.Header.Draw(GetRender(), ctrl->GetRenderBounds());
	}

	Textures.CategoryList.Inner.Draw(GetRender(), ctrl->GetRenderBounds());
}

void Gwen::Skin::SkinBase::LoadTexture(const String textName, String textPath)
{
	Uint16 imgHash = 0;
	std::hash<Gwen::String>	hashstring;

	//reserve texture memory
	Gwen::Texture *newTexture = new Texture;
	newTexture->Load(textName, GetRender());

	//check for fail
	if (newTexture->FailedToLoad())
	{
		delete newTexture;
		return;
	}

	//get the name hash
	imgHash = hashstring(textName);

	//put texture in the list using the name hash
	m_skinTextures[imgHash] = newTexture;
}

Gwen::Texture* Gwen::Skin::SkinBase::GetTexture(String textName)const
{
	Uint16 imgHash = 0;
	std::hash<Gwen::String>	hashstring;

	//get the name hash
	imgHash = hashstring(textName);

	std::map<Uint16, Texture*>::const_iterator it;
	it = m_skinTextures.find(imgHash);

	//check if find it
	if (it != m_skinTextures.end())
		return it->second;

	//return a null texture
	return nullptr;
}

void Gwen::Skin::SkinBase::setTexture(Texturing::Bordered & texture, const tinyxml2::XMLElement * baseElement)
{
	String textureName;
	float x = 0.0f, y = 0.0f, whidth = 0.0f, heigh = 0.0f;
	unsigned int left = 0, top = 0, right = 0, bottom = 0;
	//TODO: Assert Here
	if (baseElement == nullptr)
		return;

	const tinyxml2::XMLElement* textureElement = baseElement->FirstChildElement("color");
	if (textureElement)
		return;

	//get the texture reference
	textureName = textureElement->Attribute("name");

	//get skin position in texture
	x = textureElement->FloatAttribute("x");
	y = textureElement->FloatAttribute("y");

	//get skin size
	whidth = textureElement->FloatAttribute("width");
	heigh = textureElement->FloatAttribute("height");

	//get the margin
	left = textureElement->UnsignedAttribute("left");
	top = textureElement->UnsignedAttribute("top"); 
	right = textureElement->UnsignedAttribute("right");
	bottom = textureElement->UnsignedAttribute("bottom");

	texture.Init(GetTexture(textureName), x, y, whidth, heigh, Margin(left, top, right, bottom));
}

void Gwen::Skin::SkinBase::setTexture(Texturing::Single & texture, const tinyxml2::XMLElement * baseElement)
{
	String textureName;
	float x = 0.0f, y = 0.0f, whidth = 0.0f, heigh = 0.0f;
	//TODO: Assert Here
	if (baseElement == nullptr)
		return;

	const tinyxml2::XMLElement* textureElement = baseElement->FirstChildElement("color");
	if (textureElement)
		return;

	//get the texture reference
	textureName = textureElement->Attribute("name");

	//get skin position in texture
	x = textureElement->FloatAttribute("x");
	y = textureElement->FloatAttribute("y");

	//get skin size
	whidth = textureElement->FloatAttribute("width");
	heigh = textureElement->FloatAttribute("height");

	//init the texture
	texture.Init(GetTexture(textureName), x, y, whidth, heigh);
}

void Gwen::Skin::SkinBase::setColor(Gwen::Color & color, const tinyxml2::XMLElement * baseElement)
{
	//TODO: Assert Here
	if (baseElement == nullptr)
		return;

	const tinyxml2::XMLElement* colorElement = baseElement->FirstChildElement("color");
	if (colorElement)
		return;


}

void Gwen::Skin::SkinBase::SetDefaltSkin(void)
{
	//setup the defalt skin set
	m_DefaultFont.facename = L"OpenSans";
	m_DefaultFont.size = 11;

	//m_Texture.Load(L"DefaultSkin", GetRender());
	LoadTexture("DefaultSkin", "DefaultSkin");

	Colors.Window.TitleActive = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 0, 508, Color(255, 0, 0));
	Colors.Window.TitleInactive = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 1, 508, Color(255, 255, 0));
	Colors.Button.Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 2, 508, Color(255, 255, 0));
	Colors.Button.Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 3, 508, Color(255, 255, 0));
	Colors.Button.Down = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 2, 500, Color(255, 255, 0));
	Colors.Button.Disabled = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 3, 500, Color(255, 255, 0));
	Colors.Tab.Active.Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 4, 508, Color(255, 255, 0));
	Colors.Tab.Active.Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 5, 508, Color(255, 255, 0));
	Colors.Tab.Active.Down = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 4, 500, Color(255, 255, 0));
	Colors.Tab.Active.Disabled = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 5, 500, Color(255, 255, 0));
	Colors.Tab.Inactive.Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 6, 508, Color(255, 255, 0));
	Colors.Tab.Inactive.Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 7, 508, Color(255, 255, 0));
	Colors.Tab.Inactive.Down = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 6, 500, Color(255, 255, 0));
	Colors.Tab.Inactive.Disabled = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 7, 500, Color(255, 255, 0));
	Colors.Label.Default = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 8, 508, Color(255, 255, 0));
	Colors.Label.Bright = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 9, 508, Color(255, 255, 0));
	Colors.Label.Dark = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 8, 500, Color(255, 255, 0));
	Colors.Label.Highlight = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 9, 500, Color(255, 255, 0));
	Colors.Tree.Lines = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 10, 508, Color(255, 255, 0));
	Colors.Tree.Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 11, 508, Color(255, 255, 0));
	Colors.Tree.Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 10, 500, Color(255, 255, 0));
	Colors.Tree.Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 11, 500, Color(255, 255, 0));
	Colors.Properties.Line_Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 12, 508, Color(255, 255, 0));
	Colors.Properties.Line_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 13, 508, Color(255, 255, 0));
	Colors.Properties.Line_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 12, 500, Color(255, 255, 0));
	Colors.Properties.Title = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 13, 500, Color(255, 255, 0));
	Colors.Properties.Column_Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 14, 508, Color(255, 255, 0));
	Colors.Properties.Column_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 15, 508, Color(255, 255, 0));
	Colors.Properties.Column_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 14, 500, Color(255, 255, 0));
	Colors.Properties.Border = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 15, 500, Color(255, 255, 0));
	Colors.Properties.Label_Normal = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 16, 508, Color(255, 255, 0));
	Colors.Properties.Label_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 17, 508, Color(255, 255, 0));
	Colors.Properties.Label_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 16, 500, Color(255, 255, 0));
	Colors.ModalBackground = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 18, 508, Color(255, 255, 0));
	Colors.TooltipText = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 19, 508, Color(255, 255, 0));
	Colors.Category.Header = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 18, 500, Color(255, 255, 0));
	Colors.Category.Header_Closed = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 19, 500, Color(255, 255, 0));
	Colors.Category.Line.Text = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 20, 508, Color(255, 255, 0));
	Colors.Category.Line.Text_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 21, 508, Color(255, 255, 0));
	Colors.Category.Line.Text_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 20, 500, Color(255, 255, 0));
	Colors.Category.Line.Button = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 21, 500, Color(255, 255, 0));
	Colors.Category.Line.Button_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 22, 508, Color(255, 255, 0));
	Colors.Category.Line.Button_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 23, 508, Color(255, 255, 0));
	Colors.Category.LineAlt.Text = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 22, 500, Color(255, 255, 0));
	Colors.Category.LineAlt.Text_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 23, 500, Color(255, 255, 0));
	Colors.Category.LineAlt.Text_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 24, 508, Color(255, 255, 0));
	Colors.Category.LineAlt.Button = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 25, 508, Color(255, 255, 0));
	Colors.Category.LineAlt.Button_Hover = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 24, 500, Color(255, 255, 0));
	Colors.Category.LineAlt.Button_Selected = GetRender()->PixelColour(GetTexture("DefaultSkin"), 4 + 8 * 25, 500, Color(255, 255, 0));
	Textures.Shadow.Init(GetTexture("DefaultSkin"), 448, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.Tooltip.Init(GetTexture("DefaultSkin"), 128, 320, 127, 31, Margin(8, 8, 8, 8));
	Textures.StatusBar.Init(GetTexture("DefaultSkin"), 128, 288, 127, 31, Margin(8, 8, 8, 8));
	Textures.Selection.Init(GetTexture("DefaultSkin"), 384, 32, 31, 31, Margin(4, 4, 4, 4));
	Textures.Panel.Normal.Init(GetTexture("DefaultSkin"), 256, 0, 63, 63, Margin(16, 16, 16, 16));
	Textures.Panel.Bright.Init(GetTexture("DefaultSkin"), 256 + 64, 0, 63, 63, Margin(16, 16, 16, 16));
	Textures.Panel.Dark.Init(GetTexture("DefaultSkin"), 256, 64, 63, 63, Margin(16, 16, 16, 16));
	Textures.Panel.Highlight.Init(GetTexture("DefaultSkin"), 256 + 64, 64, 63, 63, Margin(16, 16, 16, 16));
	Textures.Window.Normal.Init(GetTexture("DefaultSkin"), 0, 0, 127, 127, Margin(8, 32, 8, 8));
	Textures.Window.Inactive.Init(GetTexture("DefaultSkin"), 128, 0, 127, 127, Margin(8, 32, 8, 8));
	Textures.Checkbox.Active.Checked.Init(GetTexture("DefaultSkin"), 448, 32, 15, 15);
	Textures.Checkbox.Active.Normal.Init(GetTexture("DefaultSkin"), 464, 32, 15, 15);
	Textures.Checkbox.Disabled.Checked.Init(GetTexture("DefaultSkin"), 448, 48, 15, 15);
	Textures.Checkbox.Disabled.Normal.Init(GetTexture("DefaultSkin"), 464, 48, 15, 15);
	Textures.RadioButton.Active.Checked.Init(GetTexture("DefaultSkin"), 448, 64, 15, 15);
	Textures.RadioButton.Active.Normal.Init(GetTexture("DefaultSkin"), 464, 64, 15, 15);
	Textures.RadioButton.Disabled.Checked.Init(GetTexture("DefaultSkin"), 448, 80, 15, 15);
	Textures.RadioButton.Disabled.Normal.Init(GetTexture("DefaultSkin"), 464, 80, 15, 15);
	Textures.TextBox.Normal.Init(GetTexture("DefaultSkin"), 0, 150, 127, 21, Margin(4, 4, 4, 4));
	Textures.TextBox.Focus.Init(GetTexture("DefaultSkin"), 0, 172, 127, 21, Margin(4, 4, 4, 4));
	Textures.TextBox.Disabled.Init(GetTexture("DefaultSkin"), 0, 193, 127, 21, Margin(4, 4, 4, 4));
	Textures.Menu.Strip.Init(GetTexture("DefaultSkin"), 0, 128, 127, 21, Margin(1, 1, 1, 1));
	Textures.Menu.BackgroundWithMargin.Init(GetTexture("DefaultSkin"), 128, 128, 127, 63, Margin(24, 8, 8, 8));
	Textures.Menu.Background.Init(GetTexture("DefaultSkin"), 128, 192, 127, 63, Margin(8, 8, 8, 8));
	Textures.Menu.Hover.Init(GetTexture("DefaultSkin"), 128, 256, 127, 31, Margin(8, 8, 8, 8));
	Textures.Menu.RightArrow.Init(GetTexture("DefaultSkin"), 464, 112, 15, 15);
	Textures.Menu.Check.Init(GetTexture("DefaultSkin"), 448, 112, 15, 15);
	Textures.Tab.Control.Init(GetTexture("DefaultSkin"), 0, 256, 127, 127, Margin(8, 8, 8, 8));
	Textures.Tab.Bottom.Active.Init(GetTexture("DefaultSkin"), 0, 416, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Bottom.Inactive.Init(GetTexture("DefaultSkin"), 0 + 128, 416, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Top.Active.Init(GetTexture("DefaultSkin"), 0, 384, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Top.Inactive.Init(GetTexture("DefaultSkin"), 0 + 128, 384, 63, 31, Margin(8, 8, 8, 8));
	Textures.Tab.Left.Active.Init(GetTexture("DefaultSkin"), 64, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Left.Inactive.Init(GetTexture("DefaultSkin"), 64 + 128, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Right.Active.Init(GetTexture("DefaultSkin"), 96, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.Right.Inactive.Init(GetTexture("DefaultSkin"), 96 + 128, 384, 31, 63, Margin(8, 8, 8, 8));
	Textures.Tab.HeaderBar.Init(GetTexture("DefaultSkin"), 128, 352, 127, 31, Margin(4, 4, 4, 4));
	Textures.Window.Close.Init(GetTexture("DefaultSkin"), 32, 448, 31, 31);
	Textures.Window.Close_Hover.Init(GetTexture("DefaultSkin"), 64, 448, 31, 31);
	Textures.Window.Close_Down.Init(GetTexture("DefaultSkin"), 96, 448, 31, 31);
	Textures.Window.Maxi.Init(GetTexture("DefaultSkin"), 32 + 96 * 2, 448, 31, 31);
	Textures.Window.Maxi_Hover.Init(GetTexture("DefaultSkin"), 64 + 96 * 2, 448, 31, 31);
	Textures.Window.Maxi_Down.Init(GetTexture("DefaultSkin"), 96 + 96 * 2, 448, 31, 31);
	Textures.Window.Restore.Init(GetTexture("DefaultSkin"), 32 + 96 * 2, 448 + 32, 31, 31);
	Textures.Window.Restore_Hover.Init(GetTexture("DefaultSkin"), 64 + 96 * 2, 448 + 32, 31, 31);
	Textures.Window.Restore_Down.Init(GetTexture("DefaultSkin"), 96 + 96 * 2, 448 + 32, 31, 31);
	Textures.Window.Mini.Init(GetTexture("DefaultSkin"), 32 + 96, 448, 31, 31);
	Textures.Window.Mini_Hover.Init(GetTexture("DefaultSkin"), 64 + 96, 448, 31, 31);
	Textures.Window.Mini_Down.Init(GetTexture("DefaultSkin"), 96 + 96, 448, 31, 31);
	Textures.Tree.Background.Init(GetTexture("DefaultSkin"), 256, 128, 127, 127, Margin(16, 16, 16, 16));
	Textures.Tree.Plus.Init(GetTexture("DefaultSkin"), 448, 96, 15, 15);
	Textures.Tree.Minus.Init(GetTexture("DefaultSkin"), 464, 96, 15, 15);
	Textures.Input.Button.Normal.Init(GetTexture("DefaultSkin"), 480, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Button.Hovered.Init(GetTexture("DefaultSkin"), 480, 32, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Button.Disabled.Init(GetTexture("DefaultSkin"), 480, 64, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Button.Pressed.Init(GetTexture("DefaultSkin"), 480, 96, 31, 31, Margin(8, 8, 8, 8));

	for (int i = 0; i < 4; i++)
	{
		Textures.Scroller.Button.Normal[i].Init(GetTexture("DefaultSkin"), 464 + 0, 208 + i * 16, 15, 15, Margin(2, 2, 2, 2));
		Textures.Scroller.Button.Hover[i].Init(GetTexture("DefaultSkin"), 480, 208 + i * 16, 15, 15, Margin(2, 2, 2, 2));
		Textures.Scroller.Button.Down[i].Init(GetTexture("DefaultSkin"), 464, 272 + i * 16, 15, 15, Margin(2, 2, 2, 2));
		Textures.Scroller.Button.Disabled[i].Init(GetTexture("DefaultSkin"), 480 + 48, 272 + i * 16, 15, 15, Margin(2, 2, 2, 2));
	}

	Textures.Scroller.TrackV.Init(GetTexture("DefaultSkin"), 384, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Normal.Init(GetTexture("DefaultSkin"), 384 + 16, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Hover.Init(GetTexture("DefaultSkin"), 384 + 32, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Down.Init(GetTexture("DefaultSkin"), 384 + 48, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonV_Disabled.Init(GetTexture("DefaultSkin"), 384 + 64, 208, 15, 127, Margin(4, 4, 4, 4));
	Textures.Scroller.TrackH.Init(GetTexture("DefaultSkin"), 384, 128, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Normal.Init(GetTexture("DefaultSkin"), 384, 128 + 16, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Hover.Init(GetTexture("DefaultSkin"), 384, 128 + 32, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Down.Init(GetTexture("DefaultSkin"), 384, 128 + 48, 127, 15, Margin(4, 4, 4, 4));
	Textures.Scroller.ButtonH_Disabled.Init(GetTexture("DefaultSkin"), 384, 128 + 64, 127, 15, Margin(4, 4, 4, 4));
	Textures.Input.ListBox.Background.Init(GetTexture("DefaultSkin"), 256, 256, 63, 127, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.Hovered.Init(GetTexture("DefaultSkin"), 320, 320, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.EvenLine.Init(GetTexture("DefaultSkin"), 352, 256, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.OddLine.Init(GetTexture("DefaultSkin"), 352, 288, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.EvenLineSelected.Init(GetTexture("DefaultSkin"), 320, 256, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ListBox.OddLineSelected.Init(GetTexture("DefaultSkin"), 320, 288, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.ComboBox.Normal.Init(GetTexture("DefaultSkin"), 384, 336, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Hover.Init(GetTexture("DefaultSkin"), 384, 336 + 32, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Down.Init(GetTexture("DefaultSkin"), 384, 336 + 64, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Disabled.Init(GetTexture("DefaultSkin"), 384, 336 + 96, 127, 31, Margin(8, 8, 32, 8));
	Textures.Input.ComboBox.Button.Normal.Init(GetTexture("DefaultSkin"), 496, 272, 15, 15);
	Textures.Input.ComboBox.Button.Hover.Init(GetTexture("DefaultSkin"), 496, 272 + 16, 15, 15);
	Textures.Input.ComboBox.Button.Down.Init(GetTexture("DefaultSkin"), 496, 272 + 32, 15, 15);
	Textures.Input.ComboBox.Button.Disabled.Init(GetTexture("DefaultSkin"), 496, 272 + 48, 15, 15);
	Textures.Input.UpDown.Up.Normal.Init(GetTexture("DefaultSkin"), 384, 112, 7, 7);
	Textures.Input.UpDown.Up.Hover.Init(GetTexture("DefaultSkin"), 384 + 8, 112, 7, 7);
	Textures.Input.UpDown.Up.Down.Init(GetTexture("DefaultSkin"), 384 + 16, 112, 7, 7);
	Textures.Input.UpDown.Up.Disabled.Init(GetTexture("DefaultSkin"), 384 + 24, 112, 7, 7);
	Textures.Input.UpDown.Down.Normal.Init(GetTexture("DefaultSkin"), 384, 120, 7, 7);
	Textures.Input.UpDown.Down.Hover.Init(GetTexture("DefaultSkin"), 384 + 8, 120, 7, 7);
	Textures.Input.UpDown.Down.Down.Init(GetTexture("DefaultSkin"), 384 + 16, 120, 7, 7);
	Textures.Input.UpDown.Down.Disabled.Init(GetTexture("DefaultSkin"), 384 + 24, 120, 7, 7);
	Textures.ProgressBar.Back.Init(GetTexture("DefaultSkin"), 384, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.ProgressBar.Front.Init(GetTexture("DefaultSkin"), 384 + 32, 0, 31, 31, Margin(8, 8, 8, 8));
	Textures.Input.Slider.H.Normal.Init(GetTexture("DefaultSkin"), 416, 32, 15, 15);
	Textures.Input.Slider.H.Hover.Init(GetTexture("DefaultSkin"), 416, 32 + 16, 15, 15);
	Textures.Input.Slider.H.Down.Init(GetTexture("DefaultSkin"), 416, 32 + 32, 15, 15);
	Textures.Input.Slider.H.Disabled.Init(GetTexture("DefaultSkin"), 416, 32 + 48, 15, 15);
	Textures.Input.Slider.V.Normal.Init(GetTexture("DefaultSkin"), 416 + 16, 32, 15, 15);
	Textures.Input.Slider.V.Hover.Init(GetTexture("DefaultSkin"), 416 + 16, 32 + 16, 15, 15);
	Textures.Input.Slider.V.Down.Init(GetTexture("DefaultSkin"), 416 + 16, 32 + 32, 15, 15);
	Textures.Input.Slider.V.Disabled.Init(GetTexture("DefaultSkin"), 416 + 16, 32 + 48, 15, 15);
	Textures.CategoryList.Outer.Init(GetTexture("DefaultSkin"), 256, 384, 63, 63, Margin(8, 8, 8, 8));
	Textures.CategoryList.Inner.Init(GetTexture("DefaultSkin"), 256 + 64, 384, 63, 63, Margin(8, 21, 8, 8));
	Textures.CategoryList.Header.Init(GetTexture("DefaultSkin"), 320, 352, 63, 31, Margin(8, 8, 8, 8));
	Textures.GroupBox.Init(GetTexture("DefaultSkin"), 0, 448, 31, 31, Margin(8, 8, 8, 8));
}

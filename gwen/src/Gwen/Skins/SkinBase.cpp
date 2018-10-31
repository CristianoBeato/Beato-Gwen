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
#include "missimg.h"

Gwen::Texture* Gwen::Skin::SkinBase::m_missingTexture = NULL;

Gwen::Skin::SkinBase::SkinBase(Gwen::Renderer::Base * renderer) : Gwen::Skin::Base(renderer)
{
}

Gwen::Skin::SkinBase::~SkinBase(void)
{
	Clear();
}

bool Gwen::Skin::SkinBase::Init(const TextObject & SkinXml)
{
	char * buff = NULL;

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
	buff = (char*)SDL_malloc(fileSize);
	srcFile->read(buff, fileSize);

	//can free the file handler
	delete srcFile;

	//parse the src
	bool ret = Init(buff, fileSize);

	//free the src ptr
	SDL_free(buff);

	return ret;
}

bool Gwen::Skin::SkinBase::Init(const char * buff, size_t size)
{ 
	tinyxml2::XMLElement* skinElement;
	tinyxml2::XMLDocument *skinDocument = NULL;

	//create the XML interation, and 
	skinDocument = new tinyxml2::XMLDocument;
	skinDocument->Parse(buff, size);

	//create the defalt texture
	if (m_missingTexture != NULL)
		delete m_missingTexture;

	
	m_missingTexture = new Texture;
	m_missingTexture->Load(TextObject("missing"),
		missing_image.width,
		missing_image.height,
		missing_image.bytes_per_pixel,
		(char*)missing_image.pixel_data,
		GetRender());

	//check for fail
	if (m_missingTexture->FailedToLoad())
		m_missingTexture = NULL;

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
				tinyxml2::XMLElement* textureElement = titleElement->FirstChildElement("texture");
				while (textureElement != NULL)
				{
					const char *textureName = textureElement->Attribute("name");
					const char *texturePath = textureElement->Attribute("image");
					
					//
					if (textureName != nullptr && texturePath != nullptr)
						LoadTexture(textureName, texturePath);

					textureElement = textureElement->NextSiblingElement("texture");
				}
			}

			/*
			====================
			Load Classes Skins definitions
			====================
			*/
			else if (className == TextObject("Panel"))
			{
				tinyxml2::XMLElement* Normal = titleElement->FirstChildElement("Normal");
				if (Normal)	setTexture(Textures.Panel.Normal, Normal);

				tinyxml2::XMLElement* Bright = titleElement->FirstChildElement("Bright");
				if (Bright)	setTexture(Textures.Panel.Bright, Bright);

				tinyxml2::XMLElement* Dark = titleElement->FirstChildElement("Dark");
				if (Dark) setTexture(Textures.Panel.Dark, Dark);

				tinyxml2::XMLElement* Highlight = titleElement->FirstChildElement("Highlight");
				if (Highlight)	setTexture(Textures.Panel.Highlight, Highlight);
			}
			else if (className == TextObject("Window"))
			{
				//the ative title color
				tinyxml2::XMLElement* TitleActive = titleElement->FirstChildElement("TitleActive");
				if (TitleActive) setColor(Colors.Window.TitleActive, TitleActive);

				//the inative title color 
				tinyxml2::XMLElement* TitleInactive = titleElement->FirstChildElement("TitleInactive");
				if (TitleInactive)	setColor(Colors.Window.TitleInactive, TitleInactive);

				//the close button
				tinyxml2::XMLElement* Close = titleElement->FirstChildElement("Close");
				if (Close) setTexture(Textures.Window.Close, Close);

				//the close button hovered
				tinyxml2::XMLElement* CloseHover = titleElement->FirstChildElement("CloseHover");
				if (CloseHover) setTexture(Textures.Window.Close_Hover, CloseHover);

				//the close button down 
				tinyxml2::XMLElement* CloseDown = titleElement->FirstChildElement("CloseDown");
				if (CloseDown)	setTexture(Textures.Window.Close_Down, CloseDown);

				//the maximise button 
				tinyxml2::XMLElement* Maxi = titleElement->FirstChildElement("Maxi");
				if (Maxi) setTexture(Textures.Window.Maxi, Maxi);

				//the maximise button hovered
				tinyxml2::XMLElement* MaxiHover = titleElement->FirstChildElement("MaxiHover");
				if (MaxiHover) setTexture(Textures.Window.Maxi_Hover, MaxiHover);

				//the maximise button down
				tinyxml2::XMLElement* MaxiDown = titleElement->FirstChildElement("MaxiDown");
				if (MaxiDown) setTexture(Textures.Window.Maxi_Down, MaxiDown);

				//the minimise button
				tinyxml2::XMLElement* Mini = titleElement->FirstChildElement("Mini");
				if (Mini) setTexture(Textures.Window.Mini, Mini);

				//the minimise button hovered
				tinyxml2::XMLElement* MiniHover = titleElement->FirstChildElement("MiniHover");
				if (MiniHover) setTexture(Textures.Window.Mini_Hover, MiniHover);

				//the minimise button down
				tinyxml2::XMLElement* MiniDown = titleElement->FirstChildElement("MiniDown");
				if (MiniDown) setTexture(Textures.Window.Mini_Down, MiniDown);

				//the restore button 
				tinyxml2::XMLElement* Restore = titleElement->FirstChildElement("Restore");
				if (Restore) setTexture(Textures.Window.Restore, Restore);

				//the restore button hovered
				tinyxml2::XMLElement* RestoreHover = titleElement->FirstChildElement("RestoreHover");
				if (RestoreHover) setTexture(Textures.Window.Restore_Hover, RestoreHover);

				//the restore button hovered
				tinyxml2::XMLElement* RestoreDown = titleElement->FirstChildElement("RestoreDown");
				if (RestoreDown) setTexture(Textures.Window.Restore_Down, RestoreDown);

				//the restore button hovered
				tinyxml2::XMLElement* Normal = titleElement->FirstChildElement("Normal");
				if (Normal) setTexture(Textures.Window.Normal, Normal);

				//the restore button hovered
				tinyxml2::XMLElement* Inactive = titleElement->FirstChildElement("Inactive");
				if (Inactive) setTexture(Textures.Window.Inactive, Inactive);
			}
			else if (className == TextObject("Tab"))
			{
				tinyxml2::XMLElement* Active = titleElement->FirstChildElement("Active");
				if (Active)
				{
					//normal active tab
					tinyxml2::XMLElement* Normal = Active->FirstChildElement("Normal");
					if (Normal) setColor(Colors.Tab.Active.Normal, Normal);

					//hovered active tab
					tinyxml2::XMLElement* Hover = Active->FirstChildElement("Hover");
					if (Hover) setColor(Colors.Tab.Active.Hover, Hover);

					//down active tab
					tinyxml2::XMLElement* Down = Active->FirstChildElement("Down");
					if (Down) setColor(Colors.Tab.Active.Down, Down);

					//disable active tab
					tinyxml2::XMLElement* Disabled = Active->FirstChildElement("Disabled");
					if (Disabled) setColor(Colors.Tab.Active.Disabled, Disabled);
				}

				tinyxml2::XMLElement* Inactive = titleElement->FirstChildElement("Inactive");
				if (Inactive)
				{
					//normal inactive tab
					tinyxml2::XMLElement* Normal = Inactive->FirstChildElement("Normal");
					if (Normal) setColor(Colors.Tab.Inactive.Normal, Normal);

					//hovered inactive tab
					tinyxml2::XMLElement* Hover = Inactive->FirstChildElement("Hover");
					if (Hover) setColor(Colors.Tab.Inactive.Hover, Hover);

					//down inactive tab
					tinyxml2::XMLElement* Down = Inactive->FirstChildElement("Down");
					if (Down) setColor(Colors.Tab.Inactive.Down, Down);

					//disable inactive tab
					tinyxml2::XMLElement* Disabled = Inactive->FirstChildElement("Disabled");
					if (Disabled) setColor(Colors.Tab.Inactive.Disabled, Disabled);
				}

				tinyxml2::XMLElement* Control = titleElement->FirstChildElement("Control");
				if (Control) setTexture(Textures.Tab.Control, Control);

				tinyxml2::XMLElement* HeaderBar = titleElement->FirstChildElement("HeaderBar");
				if (HeaderBar) setTexture(Textures.Tab.HeaderBar, HeaderBar);

				tinyxml2::XMLElement* Bottom = titleElement->FirstChildElement("Bottom");
				if (Bottom)
				{
					//tab button active
					tinyxml2::XMLElement* Active = Bottom->FirstChildElement("Active");
					if (Active) setTexture(Textures.Tab.Bottom.Active, Active);

					//tab buttom inactive
					tinyxml2::XMLElement* Inactive = Bottom->FirstChildElement("Inactive");
					if (Inactive) setTexture(Textures.Tab.Bottom.Inactive, Inactive);
				}

				tinyxml2::XMLElement* Top = titleElement->FirstChildElement("Top");
				if (Top)
				{
					//tab top active
					tinyxml2::XMLElement* Active = Top->FirstChildElement("Active");
					if (Active) setTexture(Textures.Tab.Top.Active, Active);

					//tab top inactive
					tinyxml2::XMLElement* Inactive = Top->FirstChildElement("Inactive");
					if (Inactive) setTexture(Textures.Tab.Top.Inactive, Inactive);
				}

				tinyxml2::XMLElement* Left = titleElement->FirstChildElement("Left");
				if (Left)
				{
					//tab left active
					tinyxml2::XMLElement* Active = Left->FirstChildElement("Active");
					if (Active) setTexture(Textures.Tab.Left.Active, Active);

					//tab left inactive
					tinyxml2::XMLElement* Inactive = Left->FirstChildElement("Inactive");
					if (Inactive) setTexture(Textures.Tab.Left.Inactive, Inactive);
				}

				tinyxml2::XMLElement* Right = titleElement->FirstChildElement("Right");
				if (Right)
				{
					//tab left active
					tinyxml2::XMLElement* Active = Right->FirstChildElement("Active");
					if (Active) setTexture(Textures.Tab.Right.Active, Active);

					//tab left inactive
					tinyxml2::XMLElement* Inactive = Right->FirstChildElement("Inactive");
					if (Inactive) setTexture(Textures.Tab.Right.Inactive, Inactive);
				}
			}
			else if (className == TextObject("Label"))
			{
				//set label defalt color
				tinyxml2::XMLElement* Default = titleElement->FirstChildElement("Default");
				if (Default) setColor(Colors.Label.Default, Default);

				//set label bright color
				tinyxml2::XMLElement* Bright = titleElement->FirstChildElement("Bright");
				if (Bright) setColor(Colors.Label.Bright, Bright);

				//set label dark color
				tinyxml2::XMLElement* Dark = titleElement->FirstChildElement("Dark");
				if (Dark) setColor(Colors.Label.Dark, Dark);

				//set label highlight color
				tinyxml2::XMLElement* Highlight = titleElement->FirstChildElement("Highlight");
				if (Highlight) setColor(Colors.Label.Highlight, Highlight);

			}
			else if (className == TextObject("Tree"))
			{
				//set tree lines color
				tinyxml2::XMLElement* Lines = titleElement->FirstChildElement("Lines");
				if (Lines) setColor(Colors.Tree.Lines, Lines);

				//set tree normal color
				tinyxml2::XMLElement* Normal = titleElement->FirstChildElement("Normal");
				if (Normal) setColor(Colors.Tree.Normal, Normal);

				//set tree hovered color
				tinyxml2::XMLElement* Hover = titleElement->FirstChildElement("Hover");
				if (Hover) setColor(Colors.Tree.Hover, Hover);

				//set tree selected color
				tinyxml2::XMLElement* Selected = titleElement->FirstChildElement("Selected");
				if (Selected) setColor(Colors.Tree.Selected, Selected);

				//set tree backgroud
				tinyxml2::XMLElement* Background = titleElement->FirstChildElement("Background");
				if (Background) setTexture(Textures.Tree.Background, Background);

				//set tree plus button
				tinyxml2::XMLElement* Plus = titleElement->FirstChildElement("Plus");
				if (Plus) setTexture(Textures.Tree.Plus, Plus);

				//set tree minus button
				tinyxml2::XMLElement* Minus = titleElement->FirstChildElement("Minus");
				if (Minus) setTexture(Textures.Tree.Minus, Minus);

			}
			else if (className == TextObject("Properties"))
			{
				//set properties normal line color
				tinyxml2::XMLElement* LineNormal = titleElement->FirstChildElement("LineNormal");
				if (LineNormal) setColor(Colors.Properties.Line_Normal, LineNormal);

				//set properties selected line color
				tinyxml2::XMLElement* LineSelected = titleElement->FirstChildElement("LineSelected");
				if (LineSelected) setColor(Colors.Properties.Line_Selected, LineSelected);

				//set properties hovered line color
				tinyxml2::XMLElement* LineHover = titleElement->FirstChildElement("LineHover");
				if (LineHover) setColor(Colors.Properties.Line_Hover, LineHover);

				//set properties colun normal color
				tinyxml2::XMLElement* ColumnNormal = titleElement->FirstChildElement("ColumnNormal");
				if (ColumnNormal) setColor(Colors.Properties.Column_Normal, ColumnNormal);

				//set properties selected colun color
				tinyxml2::XMLElement* ColumnSelected = titleElement->FirstChildElement("ColumnSelected");
				if (ColumnSelected) setColor(Colors.Properties.Column_Selected, ColumnSelected);

				//set properties hovered colunm color
				tinyxml2::XMLElement* ColumnHover = titleElement->FirstChildElement("ColumnHover");
				if (ColumnHover) setColor(Colors.Properties.Column_Hover, ColumnHover);

				//set properties label normal color
				tinyxml2::XMLElement* LabelNormal = titleElement->FirstChildElement("LabelNormal");
				if (LabelNormal) setColor(Colors.Properties.Label_Normal, LabelNormal);

				//set properties selected label color
				tinyxml2::XMLElement* LabelSelected = titleElement->FirstChildElement("LabelSelected");
				if (LabelSelected) setColor(Colors.Properties.Label_Selected, LabelSelected);

				//set properties hovered label color
				tinyxml2::XMLElement* LabelHover = titleElement->FirstChildElement("LabelHover");
				if (LabelHover) setColor(Colors.Properties.Label_Hover, LabelHover);

				//set properties title color
				tinyxml2::XMLElement* Title = titleElement->FirstChildElement("Title");
				if (Title) setColor(Colors.Properties.Title, Title);

				//set properties border color
				tinyxml2::XMLElement* Border = titleElement->FirstChildElement("Border");
				if (Border) setColor(Colors.Properties.Border, Border);

			}
			else if (className == TextObject("ModalBackground"))
			{
				setColor(Colors.ModalBackground, titleElement);
			}
			else if (className == TextObject("TooltipText"))
			{
				setColor(Colors.TooltipText, titleElement);
			}
			else if (className == TextObject("Category"))
			{
				//set categoty header color
				tinyxml2::XMLElement* Header = titleElement->FirstChildElement("Header");
				if (Header) setColor(Colors.Category.Header, Header);

				//set properties border color
				tinyxml2::XMLElement* HeaderClosed = titleElement->FirstChildElement("HeaderClosed");
				if (HeaderClosed) setColor(Colors.Category.Header_Closed, HeaderClosed);

				//line
				tinyxml2::XMLElement* Line = titleElement->FirstChildElement("Line");
				if (Line)
				{
					//set Category line text color
					tinyxml2::XMLElement* Text = Line->FirstChildElement("Text");
					if (Text) setColor(Colors.Category.Line.Text, Text);

					//set Category line text hovered color
					tinyxml2::XMLElement* TextHover = Line->FirstChildElement("TextHover");
					if (TextHover) setColor(Colors.Category.Line.Text_Hover, TextHover);

					//set Category line text hovered color
					tinyxml2::XMLElement* TextSelected = Line->FirstChildElement("TextSelected");
					if (TextSelected) setColor(Colors.Category.Line.Text_Selected, TextSelected);

					//set Category line button color
					tinyxml2::XMLElement* Button = Line->FirstChildElement("Button");
					if (Button) setColor(Colors.Category.Line.Button, Button);

					//set Category line button hovered color
					tinyxml2::XMLElement* ButtonHover = Line->FirstChildElement("ButtonHover");
					if (ButtonHover) setColor(Colors.Category.Line.Button_Hover, ButtonHover);

					//set Category line button hovered color
					tinyxml2::XMLElement* ButtonSelected = Line->FirstChildElement("ButtonSelected");
					if (ButtonSelected) setColor(Colors.Category.Line.Button_Selected, ButtonSelected);
				}
				//LineAlt
				tinyxml2::XMLElement* LineAlt = titleElement->FirstChildElement("LineAlt");
				if (LineAlt)
				{
					//set Category line text color
					tinyxml2::XMLElement* Text = Line->FirstChildElement("Text");
					if (Text) setColor(Colors.Category.LineAlt.Text, Text);

					//set Category line text hovered color
					tinyxml2::XMLElement* TextHover = Line->FirstChildElement("TextHover");
					if (TextHover) setColor(Colors.Category.LineAlt.Text_Hover, TextHover);

					//set Category line text hovered color
					tinyxml2::XMLElement* TextSelected = Line->FirstChildElement("TextSelected");
					if (TextSelected) setColor(Colors.Category.LineAlt.Text_Selected, TextSelected);

					//set Category line button color
					tinyxml2::XMLElement* Button = Line->FirstChildElement("Button");
					if (Button) setColor(Colors.Category.LineAlt.Button, Button);

					//set Category line button hovered color
					tinyxml2::XMLElement* ButtonHover = Line->FirstChildElement("ButtonHover");
					if (ButtonHover) setColor(Colors.Category.LineAlt.Button_Hover, ButtonHover);

					//set Category line button hovered color
					tinyxml2::XMLElement* ButtonSelected = Line->FirstChildElement("ButtonSelected");
					if (ButtonSelected) setColor(Colors.Category.LineAlt.Button_Selected, ButtonSelected);
				}
			}
			else if (className == TextObject("CategoryList"))
			{
				//set the category list outer color list
				tinyxml2::XMLElement* Outer = titleElement->FirstChildElement("Outer");
				if (Outer) setTexture(Textures.CategoryList.Outer, Outer);

				//set the category list Inner color list
				tinyxml2::XMLElement* Inner = titleElement->FirstChildElement("Inner");
				if (Inner) setTexture(Textures.CategoryList.Inner, Inner);

				//set the category list outer color list
				tinyxml2::XMLElement* Header = titleElement->FirstChildElement("Header");
				if (Header) setTexture(Textures.CategoryList.Header, Header);

			}
			else if (className == TextObject("Shadow"))
			{
				setTexture(Textures.Shadow, titleElement);
			}
			else if (className == TextObject("Tooltip"))
			{
				setTexture(Textures.Tooltip, titleElement);
			}
			else if (className == TextObject("StatusBar"))
			{
				setTexture(Textures.StatusBar, titleElement);
			}
			else if (className == TextObject("Selection"))
			{
				setTexture(Textures.Selection, titleElement);
			}
			else if (className == TextObject("Checkbox"))
			{
				tinyxml2::XMLElement* Active = titleElement->FirstChildElement("Active");
				if (Active)
				{
					//the active, set checkbox
					tinyxml2::XMLElement* Checked = Active->FirstChildElement("Checked");
					if (Checked) setTexture(Textures.Checkbox.Active.Checked, Checked);

					//the active, clear checkbox
					tinyxml2::XMLElement* Normal = Active->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Checkbox.Active.Normal, Normal);

				}

				tinyxml2::XMLElement* Disabled = titleElement->FirstChildElement("Disabled");
				if (Disabled)
				{
					//the active, set checkbox
					tinyxml2::XMLElement* Checked = Disabled->FirstChildElement("Checked");
					if (Checked) setTexture(Textures.Checkbox.Disabled.Checked, Checked);

					//the active, clear checkbox
					tinyxml2::XMLElement* Normal = Disabled->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Checkbox.Disabled.Normal, Normal);

				}
			}
			else if (className == TextObject("RadioButton"))
			{
				tinyxml2::XMLElement* Active = titleElement->FirstChildElement("Active");
				if (Active)
				{
					//the active, set checkbox
					tinyxml2::XMLElement* Checked = Active->FirstChildElement("Checked");
					if (Checked) setTexture(Textures.RadioButton.Active.Checked, Checked);

					//the active, clear checkbox
					tinyxml2::XMLElement* Normal = Active->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.RadioButton.Active.Normal, Normal);

				}

				tinyxml2::XMLElement* Disabled = titleElement->FirstChildElement("Disabled");
				if (Disabled)
				{
					//the active, set checkbox
					tinyxml2::XMLElement* Checked = Disabled->FirstChildElement("Checked");
					if (Checked) setTexture(Textures.RadioButton.Disabled.Checked, Checked);

					//the active, clear checkbox
					tinyxml2::XMLElement* Normal = Disabled->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.RadioButton.Disabled.Normal, Normal);

				}
			}
			else if (className == TextObject("TextBox"))
			{
				//normal text box texture 
				tinyxml2::XMLElement* Normal = titleElement->FirstChildElement("Normal");
				if (Normal) setTexture(Textures.TextBox.Normal, Normal);

				//normal text box texture  
				tinyxml2::XMLElement* Focus = titleElement->FirstChildElement("Focus");
				if (Focus) setTexture(Textures.TextBox.Focus, Focus);

				//normal text box color 
				tinyxml2::XMLElement* Disabled = titleElement->FirstChildElement("Disabled");
				if (Disabled) setTexture(Textures.TextBox.Disabled, Disabled);
			}
			else if (className == TextObject("Menu"))
			{
				//menu strip texture
				tinyxml2::XMLElement* Strip = titleElement->FirstChildElement("Strip");
				if (Strip) setTexture(Textures.Menu.Strip, Strip);

				//menu background with margin texture
				tinyxml2::XMLElement* BackgroundWithMargin = titleElement->FirstChildElement("BackgroundWithMargin");
				if (BackgroundWithMargin) setTexture(Textures.Menu.BackgroundWithMargin, BackgroundWithMargin);

				//menu Background texture
				tinyxml2::XMLElement* Background = titleElement->FirstChildElement("Background");
				if (Background) setTexture(Textures.Menu.Background, Background);

				//menu Hover texture
				tinyxml2::XMLElement* Hover = titleElement->FirstChildElement("Hover");
				if (Hover) setTexture(Textures.Menu.Hover, Hover);

				//menu RightArrow texture
				tinyxml2::XMLElement* RightArrow = titleElement->FirstChildElement("RightArrow");
				if (RightArrow) setTexture(Textures.Menu.RightArrow, RightArrow);

				//menu RightArrow texture
				tinyxml2::XMLElement* Check = titleElement->FirstChildElement("Check");
				if (Check) setTexture(Textures.Menu.Check, Check);
			}
			else if (className == TextObject("Button"))
			{
				//button normal texture
				tinyxml2::XMLElement* Normal = titleElement->FirstChildElement("Normal");
				if (Normal) setTexture(Textures.Input.Button.Normal, Normal);

				//button Hovered texture
				tinyxml2::XMLElement* Hovered = titleElement->FirstChildElement("Hovered");
				if (Hovered) setTexture(Textures.Input.Button.Hovered, Hovered);

				//button Disabled texture
				tinyxml2::XMLElement* Disabled = titleElement->FirstChildElement("Disabled");
				if (Disabled) setTexture(Textures.Input.Button.Disabled, Disabled);

				//button Pressed texture
				tinyxml2::XMLElement* Pressed = titleElement->FirstChildElement("Pressed");
				if (Pressed) setTexture(Textures.Input.Button.Pressed, Pressed);
			}
			else if (className == TextObject("ListBox"))
			{
				//list box background texture
				tinyxml2::XMLElement* Background = titleElement->FirstChildElement("Background");
				if (Background) setTexture(Textures.Input.ListBox.Background, Background);

				//list box Hovered texture
				tinyxml2::XMLElement* Hovered = titleElement->FirstChildElement("Hovered");
				if (Hovered) setTexture(Textures.Input.ListBox.Hovered, Hovered);

				//list box EvenLine texture
				tinyxml2::XMLElement* EvenLine = titleElement->FirstChildElement("EvenLine");
				if (EvenLine) setTexture(Textures.Input.ListBox.EvenLine, EvenLine);

				//list box OddLine texture
				tinyxml2::XMLElement* OddLine = titleElement->FirstChildElement("OddLine");
				if (OddLine) setTexture(Textures.Input.ListBox.OddLine, OddLine);

				//list box EvenLineSelected texture
				tinyxml2::XMLElement* EvenLineSelected = titleElement->FirstChildElement("EvenLineSelected");
				if (EvenLineSelected) setTexture(Textures.Input.ListBox.EvenLineSelected, EvenLineSelected);

				//list box OddLineSelected texture
				tinyxml2::XMLElement* OddLineSelected = titleElement->FirstChildElement("OddLineSelected");
				if (OddLineSelected) setTexture(Textures.Input.ListBox.OddLineSelected, OddLineSelected);
			}
			else if (className == TextObject("ComboBox"))
			{
				//ComboBox Normal texture
				tinyxml2::XMLElement* Normal = titleElement->FirstChildElement("Normal");
				if (Normal) setTexture(Textures.Input.ComboBox.Normal, Normal);

				//ComboBox Hover texture
				tinyxml2::XMLElement* Hover = titleElement->FirstChildElement("Hover");
				if (Hover) setTexture(Textures.Input.ComboBox.Hover, Hover);

				//ComboBox Down texture
				tinyxml2::XMLElement* Down = titleElement->FirstChildElement("Down");
				if (Down) setTexture(Textures.Input.ComboBox.Down, Down);

				//ComboBox Disabled texture
				tinyxml2::XMLElement* Disabled = titleElement->FirstChildElement("Disabled");
				if (Disabled) setTexture(Textures.Input.ComboBox.Disabled, Disabled);

				//ComboBox button set
				tinyxml2::XMLElement* Button = titleElement->FirstChildElement("Button");
				if (Button)
				{
					//ComboBox buton Normal texture
					tinyxml2::XMLElement* Normal = Button->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Input.ComboBox.Button.Normal, Normal);

					//ComboBox buton Hover texture
					tinyxml2::XMLElement* Hover = Button->FirstChildElement("Hover");
					if (Hover) setTexture(Textures.Input.ComboBox.Button.Hover, Hover);

					//ComboBox buton Down texture
					tinyxml2::XMLElement* Down = Button->FirstChildElement("Down");
					if (Down) setTexture(Textures.Input.ComboBox.Button.Down, Down);

					//ComboBox buton Down texture
					tinyxml2::XMLElement* Disabled = Button->FirstChildElement("Disabled");
					if (Disabled) setTexture(Textures.Input.ComboBox.Button.Disabled, Disabled);
				}
			}
			else if (className == TextObject("UpDown"))
			{

				//Up button set
				tinyxml2::XMLElement* Up = titleElement->FirstChildElement("Up");
				if (Up)
				{
					//Normal
					tinyxml2::XMLElement* Normal = Up->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Input.UpDown.Up.Normal, Normal);

					//Hover
					tinyxml2::XMLElement* Hover = Up->FirstChildElement("Hover");
					if (Hover) setTexture(Textures.Input.UpDown.Up.Hover, Hover);

					//Down
					tinyxml2::XMLElement* Down = Up->FirstChildElement("Down");
					if (Down) setTexture(Textures.Input.UpDown.Up.Down, Down);

					//Down
					tinyxml2::XMLElement* Disabled = Up->FirstChildElement("Disabled");
					if (Disabled) setTexture(Textures.Input.UpDown.Up.Disabled, Disabled);
				}

				//Down button set
				tinyxml2::XMLElement* Down = titleElement->FirstChildElement("Down");
				if (Down)
				{
					//Normal
					tinyxml2::XMLElement* Normal = Up->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Input.UpDown.Up.Normal, Normal);

					//Hover
					tinyxml2::XMLElement* Hover = Up->FirstChildElement("Hover");
					if (Hover) setTexture(Textures.Input.UpDown.Up.Hover, Hover);

					//Down
					tinyxml2::XMLElement* Down = Up->FirstChildElement("Down");
					if (Down) setTexture(Textures.Input.UpDown.Up.Down, Down);

					//Down
					tinyxml2::XMLElement* Disabled = Up->FirstChildElement("Disabled");
					if (Disabled) setTexture(Textures.Input.UpDown.Up.Disabled, Disabled);
				}
			}
			else if (className == TextObject("Slider"))
			{
				//Down button set
				tinyxml2::XMLElement* Vertical = titleElement->FirstChildElement("Vertical");
				if (Vertical)
				{
					//Normal
					tinyxml2::XMLElement* Normal = Vertical->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Input.Slider.H.Normal, Normal);

					//Hover
					tinyxml2::XMLElement* Hover = Vertical->FirstChildElement("Hover");
					if (Hover) setTexture(Textures.Input.Slider.H.Hover, Hover);

					//Down
					tinyxml2::XMLElement* Down = Vertical->FirstChildElement("Down");
					if (Down) setTexture(Textures.Input.Slider.H.Down, Down);

					//Down
					tinyxml2::XMLElement* Disabled = Vertical->FirstChildElement("Disabled");
					if (Disabled) setTexture(Textures.Input.Slider.H.Disabled, Disabled);
				}

				//Down button set
				tinyxml2::XMLElement* Horisontal = titleElement->FirstChildElement("Horisontal");
				if (Horisontal)
				{
					//Normal
					tinyxml2::XMLElement* Normal = Vertical->FirstChildElement("Normal");
					if (Normal) setTexture(Textures.Input.Slider.V.Normal, Normal);

					//Hover
					tinyxml2::XMLElement* Hover = Vertical->FirstChildElement("Hover");
					if (Hover) setTexture(Textures.Input.Slider.V.Hover, Hover);

					//Down
					tinyxml2::XMLElement* Down = Vertical->FirstChildElement("Down");
					if (Down) setTexture(Textures.Input.Slider.V.Down, Down);

					//Down
					tinyxml2::XMLElement* Disabled = Vertical->FirstChildElement("Disabled");
					if (Disabled) setTexture(Textures.Input.Slider.V.Disabled, Disabled);
				}
			}
			else if (className == TextObject("ProgressBar"))
			{
				//Back
				tinyxml2::XMLElement* Back = titleElement->FirstChildElement("Back");
				if (Back) setTexture(Textures.ProgressBar.Back, Back);

				//Front
				tinyxml2::XMLElement* Front = titleElement->FirstChildElement("Front");
				if (Front) setTexture(Textures.ProgressBar.Front, Front);
			}
			else if (className == TextObject("CategoryList"))
			{
				//Outer
				tinyxml2::XMLElement* Outer = titleElement->FirstChildElement("Outer");
				if (Outer) setTexture(Textures.CategoryList.Outer, Outer);

				//Outer
				tinyxml2::XMLElement* Inner = titleElement->FirstChildElement("Inner");
				if (Inner) setTexture(Textures.CategoryList.Inner, Inner);

				//Outer
				tinyxml2::XMLElement* Header = titleElement->FirstChildElement("Header");
				if (Header) setTexture(Textures.CategoryList.Header, Header);
			}
			else if (className == TextObject("GroupBox"))
			{
				setTexture(Textures.GroupBox, titleElement);
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

	return true;
}

void Gwen::Skin::SkinBase::Clear(void)
{
	delete m_missingTexture;

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

void Gwen::Skin::SkinBase::DrawButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled)
{
	if (bDisabled) 
		return Textures.Input.Button.Disabled.Draw(GetRender(), control->GetRenderBounds());

	if (bDepressed) 
		return Textures.Input.Button.Pressed.Draw(GetRender(), control->GetRenderBounds());

	if (bHovered)
		return Textures.Input.Button.Hovered.Draw(GetRender(), control->GetRenderBounds());

	Textures.Input.Button.Normal.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawMenuItem(Gwen::Controls::Base * control, bool bSubmenuOpen, bool bChecked)
{
	const Gwen::Rect & rect = control->GetRenderBounds();

	if (bSubmenuOpen || control->IsHovered())
		Textures.Menu.Hover.Draw(GetRender(), rect);

	if (bChecked)
		Textures.Menu.Check.Draw(GetRender(), Gwen::Rect(rect.x + 4, rect.y + 3, 15, 15));
}

void Gwen::Skin::SkinBase::DrawMenuStrip(Gwen::Controls::Base * control)
{
	Textures.Menu.Strip.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawMenu(Gwen::Controls::Base * control, bool bPaddingDisabled)
{
	if (!bPaddingDisabled)
		return Textures.Menu.BackgroundWithMargin.Draw(GetRender(), control->GetRenderBounds());

	Textures.Menu.Background.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawMenuRightArrow(Controls::Base * control)
{
	Textures.Menu.RightArrow.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawShadow(Gwen::Controls::Base * control)
{
	Gwen::Rect r = control->GetRenderBounds();
	r.x -= 4;
	r.y -= 4;
	r.w += 10;
	r.h += 10;
	Textures.Shadow.Draw(GetRender(), r);
}

void Gwen::Skin::SkinBase::DrawRadioButton(Gwen::Controls::Base * control, bool bSelected, bool bDepressed)
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

void Gwen::Skin::SkinBase::DrawCheckBox(Gwen::Controls::Base * control, bool bSelected, bool bDepressed)
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

void Gwen::Skin::SkinBase::DrawGroupBox(Gwen::Controls::Base * control, int textStart, int textHeight, int textWidth)
{
	Gwen::Rect rect = control->GetRenderBounds();
	rect.y += textHeight * 0.5f;
	rect.h -= textHeight * 0.5f;
	Textures.GroupBox.Draw(GetRender(), rect, Gwen::Colors::White, true, false);
	rect.x += textStart + textWidth - 4;
	rect.w -= textStart + textWidth - 4;
	Textures.GroupBox.Draw(GetRender(), rect, Gwen::Colors::White, false, true, false, false, false, false, false, false, false);
}

void Gwen::Skin::SkinBase::DrawTextBox(Gwen::Controls::Base * control)
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

void Gwen::Skin::SkinBase::DrawActiveTabButton(Gwen::Controls::Base * control, int dir)
{
	if (dir == Pos::Bottom) { return Textures.Tab.Bottom.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(0, -8, 0, 8)); }

	if (dir == Pos::Top) { return Textures.Tab.Top.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(0, 0, 0, 8)); }

	if (dir == Pos::Left) { return Textures.Tab.Left.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(0, 0, 8, 0)); }

	if (dir == Pos::Right) { return Textures.Tab.Right.Active.Draw(GetRender(), control->GetRenderBounds() + Rect(-8, 0, 8, 0)); }
}

void Gwen::Skin::SkinBase::DrawTabButton(Gwen::Controls::Base * control, bool bActive, int dir)
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

void Gwen::Skin::SkinBase::DrawTabControl(Gwen::Controls::Base * control)
{
	Textures.Tab.Control.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawTabTitleBar(Gwen::Controls::Base * control)
{
	Textures.Tab.HeaderBar.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawGenericPanel(Controls::Base * control)
{
	Textures.Panel.Normal.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawWindow(Gwen::Controls::Base * control, int topHeight, bool inFocus)
{
	if (inFocus) { Textures.Window.Normal.Draw(GetRender(), control->GetRenderBounds()); }
	else { Textures.Window.Inactive.Draw(GetRender(), control->GetRenderBounds()); }
}

void Gwen::Skin::SkinBase::DrawHighlight(Gwen::Controls::Base * control)
{
	Gwen::Rect rect = control->GetRenderBounds();
	GetRender()->SetDrawColor(Gwen::Color(255, 100, 255, 255));
	GetRender()->DrawFilledRect(rect);
}

void Gwen::Skin::SkinBase::DrawScrollBar(Gwen::Controls::Base * control, bool isHorizontal, bool bDepressed)
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

void Gwen::Skin::SkinBase::DrawScrollBarBar(Controls::Base * control, bool bDepressed, bool isHovered, bool isHorizontal)
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

void Gwen::Skin::SkinBase::DrawProgressBar(Gwen::Controls::Base * control, bool isHorizontal, float progress)
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

void Gwen::Skin::SkinBase::DrawListBox(Gwen::Controls::Base * control)
{
	return Textures.Input.ListBox.Background.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawListBoxLine(Gwen::Controls::Base * control, bool bSelected, bool bEven)
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

void Gwen::Skin::SkinBase::DrawSliderNotchesH(Gwen::Rect rect, int numNotches, int dist)
{
	if (numNotches == 0) { return; }

	float iSpacing = (float)rect.w / (float)numNotches;

	for (int i = 0; i < numNotches + 1; i++)
	{
		GetRender()->DrawFilledRect(Gwen::Rect(rect.x + iSpacing * i, rect.y + dist - 2, 1, 5));
	}
}

void Gwen::Skin::SkinBase::DrawSliderNotchesV(Gwen::Rect rect, int numNotches, int dist)
{
	if (numNotches == 0) { return; }

	float iSpacing = (float)rect.h / (float)numNotches;

	for (int i = 0; i < numNotches + 1; i++)
	{
		GetRender()->DrawFilledRect(Gwen::Rect(rect.x + dist - 1, rect.y + iSpacing * i, 5, 1));
	}
}

void Gwen::Skin::SkinBase::DrawSlider(Gwen::Controls::Base * control, bool bIsHorizontal, int numNotches, int barSize)
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

void Gwen::Skin::SkinBase::DrawComboBox(Gwen::Controls::Base * control, bool bDown, bool bMenuOpen)
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

void Gwen::Skin::SkinBase::DrawKeyboardHighlight(Gwen::Controls::Base * control, const Gwen::Rect & r, int iOffset)
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

void Gwen::Skin::SkinBase::DrawToolTip(Gwen::Controls::Base * control)
{
	return Textures.Tooltip.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawScrollButton(Gwen::Controls::Base * control, int iDirection, bool bDepressed, bool bHovered, bool bDisabled)
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

void Gwen::Skin::SkinBase::DrawComboDownArrow(Gwen::Controls::Base * control, bool bHovered, bool bDown, bool bMenuOpen, bool bDisabled)
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

void Gwen::Skin::SkinBase::DrawNumericUpDownButton(Gwen::Controls::Base * control, bool bDepressed, bool bUp)
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

void Gwen::Skin::SkinBase::DrawStatusBar(Controls::Base * control)
{
	Textures.StatusBar.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawTreeButton(Controls::Base * control, bool bOpen)
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

void Gwen::Skin::SkinBase::DrawColorDisplay(Controls::Base * control, Gwen::Color color)
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

void Gwen::Skin::SkinBase::DrawModalControl(Controls::Base * control)
{
	if (!control->ShouldDrawBackground()) { return; }

	Gwen::Rect rect = control->GetRenderBounds();
	GetRender()->SetDrawColor(Colors.ModalBackground);
	GetRender()->DrawFilledRect(rect);
}

void Gwen::Skin::SkinBase::DrawMenuDivider(Controls::Base * control)
{
	Gwen::Rect rect = control->GetRenderBounds();
	GetRender()->SetDrawColor(Gwen::Color(0, 0, 0, 100));
	GetRender()->DrawFilledRect(rect);
}

void Gwen::Skin::SkinBase::DrawTreeControl(Controls::Base * control)
{
	Textures.Tree.Background.Draw(GetRender(), control->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawWindowCloseButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled)
{
	Gwen::Rect r = Gwen::Rect(control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31);

	if (bDisabled) { return Textures.Window.Close.Draw(GetRender(), r, Gwen::Color(255, 255, 255, 50)); }

	if (bDepressed) { return Textures.Window.Close_Down.Draw(GetRender(), r); }

	if (bHovered) { return Textures.Window.Close_Hover.Draw(GetRender(), r); }

	Textures.Window.Close.Draw(GetRender(), r);
}

void Gwen::Skin::SkinBase::DrawWindowMaximizeButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled, bool bMaximized)
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

void Gwen::Skin::SkinBase::DrawWindowMinimizeButton(Gwen::Controls::Base * control, bool bDepressed, bool bHovered, bool bDisabled)
{
	Gwen::Rect r = Gwen::Rect(control->GetRenderBounds().x, control->GetRenderBounds().y, 31, 31);

	if (bDisabled) { return Textures.Window.Mini.Draw(GetRender(), r, Gwen::Color(255, 255, 255, 100)); }

	if (bDepressed) { return Textures.Window.Mini_Down.Draw(GetRender(), r); }

	if (bHovered) { return Textures.Window.Mini_Hover.Draw(GetRender(), r); }

	Textures.Window.Mini.Draw(GetRender(), r);
}

void Gwen::Skin::SkinBase::DrawSlideButton(Gwen::Controls::Base * control, bool bDepressed, bool bHorizontal)
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

void Gwen::Skin::SkinBase::DrawTreeNode(Controls::Base * ctrl, bool bOpen, bool bSelected, int iLabelHeight, int iLabelWidth, int iHalfWay, int iLastBranch, bool bIsRoot)
{
	if (bSelected)
	{
		Textures.Selection.Draw(GetRender(), Gwen::Rect(17, 0, iLabelWidth + 2, iLabelHeight - 1));
	}

	Base::DrawTreeNode(ctrl, bOpen, bSelected, iLabelHeight, iLabelWidth, iHalfWay, iLastBranch, bIsRoot);
}

void Gwen::Skin::SkinBase::DrawCategoryHolder(Controls::Base * ctrl)
{
	Textures.CategoryList.Outer.Draw(GetRender(), ctrl->GetRenderBounds());
}

void Gwen::Skin::SkinBase::DrawCategoryInner(Controls::Base * ctrl, bool bCollapsed)
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
	return m_missingTexture;
}

void Gwen::Skin::SkinBase::setTexture(Texturing::Bordered & texture, const tinyxml2::XMLElement * baseElement)
{
	String textureName;
	float x = 0.0f, y = 0.0f, whidth = 0.0f, heigh = 0.0f;
	unsigned int left = 0, top = 0, right = 0, bottom = 0;
	//TODO: Assert Here
	if (baseElement == nullptr)
		return;

	const tinyxml2::XMLElement* textureElement = baseElement->FirstChildElement("texture");
	if (textureElement == NULL)
		return;

	//get the texture reference
#if 1
	textureName = textureElement->Attribute("name");
#else
	const char * text;
	textureElement->QueryStringAttribute("name", &text);
	textureName = text;
#endif

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
	if (!textureElement)
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
	String textureName;
	char r, g, b, a;
	int x, y;

	//TODO: Assert Here
	if (baseElement == nullptr)
		return;

	const tinyxml2::XMLElement* colorElement = baseElement->FirstChildElement("color");
	if (!colorElement)
		return;

	//set color only
	r = colorElement->IntAttribute("r");
	g = colorElement->IntAttribute("g");
	b = colorElement->IntAttribute("b");
	a = colorElement->IntAttribute("a");

	x = colorElement->IntAttribute("x");
	y = colorElement->IntAttribute("y");

	//get the color from image
	if ( x > -1 && y > -1 )
	{
		//get the texture reference
		textureName = colorElement->Attribute("name");
		color = GetRender()->PixelColour(GetTexture(textureName), x, y, Color(r, g, b, a));
	}
	else
		//set the color
		color = GetRender()->PixelColour(NULL, 0, 0, Color(r, g, b, a));
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

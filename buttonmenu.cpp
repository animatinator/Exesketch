// ButtonMenu.cpp


#include "buttonmenu.h"


ButtonMenu::ButtonMenu(vector<GLuint> images, Vector2d pos, int menuDirection, Vector2d menuButtonSize, Vector2d menuButtonOffset, float menuButtonSpacing):
	buttonImages(images),
	pos(pos),
	selection(0),
	direction(menuDirection),
	buttonOffset(menuButtonOffset),
	buttonSize(menuButtonSize),
	buttonSpacing(menuButtonSpacing)
{}

void ButtonMenu::Draw() const
{
	Vector2d size = GetDimensions();
	
	glColor3f(0.7294117647f, 0.7411764706f, 0.7137254902f);
	
	glBegin(GL_QUADS);
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y + size.y);
		glVertex2f(pos.x, pos.y + size.y);
	glEnd();
	
	Vector2d curPos;
	for (int i = 0; i < buttonImages.size(); ++i)
	{
		// Calculate the button's position relative to the menu
		if (direction == HORIZONTAL) curPos.x = buttonOffset.x + ((float)i * buttonSize.x) + ((float)i * buttonSpacing);
		else if (direction == VERTICAL) curPos.y = buttonOffset.y + ((float)i * buttonSize.y) + ((float)i * buttonSpacing);
		
		if (direction == HORIZONTAL) curPos.y = buttonOffset.y;
		else if (direction == VERTICAL) curPos.x = buttonOffset.x;
		
		curPos += pos;  // Add the menu's offset to the button's position
		
		// Make the button lighter when selected
		if (i == selection) glColor3f(1.0f, 1.0f, 1.0f);
		else glColor3f(0.9f, 0.9f, 0.9f);
		
		// Initialise texturing
		glEnable(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		// Bind the current texture
		glBindTexture(GL_TEXTURE_2D, buttonImages.at(i));
		
		// Draw the button
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f(curPos.x, curPos.y);
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f(curPos.x + buttonSize.x, curPos.y);
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(curPos.x + buttonSize.x, curPos.y + buttonSize.y);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(curPos.x, curPos.y + buttonSize.y);
		glEnd();
		
		glDisable(GL_TEXTURE_2D);
	}
}

void ButtonMenu::HandleEvent(Event& event)
{
	if (event.type == EVENT_MOUSE_DOWN_LEFT || event.type == EVENT_MOUSE_DOWN_MIDDLE || event.type == EVENT_MOUSE_DOWN_RIGHT)
	{
		if (MouseInWindow(event.xpos, event.ypos))
		{
			event.TargetFound();
			
			int button = GetSelectedButton(event.xpos, event.ypos);
			
			if (button != NA)
			{
				selection = button;
			}
		}
	}
}

Vector2d ButtonMenu::GetDimensions() const
{
	float width, height;
	if (direction == HORIZONTAL) height = (buttonOffset.y * 2) + buttonSize.y;
	else if (direction == VERTICAL) width = (buttonOffset.x * 2) + buttonSize.x;
	
	if (direction == HORIZONTAL) width = (buttonOffset.x * 2) + (buttonSize.x * (float)buttonImages.size()) + (buttonSpacing * (float)(buttonImages.size() - 1));
	else if (direction == VERTICAL) height = (buttonOffset.y * 2) + (buttonSize.y * (float)buttonImages.size()) + (buttonSpacing * (float)(buttonImages.size() - 1));
	
		return Vector2d(width, height);
}

bool ButtonMenu::MouseInWindow(float x, float y) const
{
	Vector2d size = GetDimensions();
	
		return (x >= pos.x && y >= pos.y && x <= (pos.x + size.x) && y <= (pos.y + size.y));
}

int ButtonMenu::GetSelectedButton(float x, float y) const
{
	// Precalculate the number of buttons
	int numButtons = buttonImages.size();
	
	// Get the mousepos relative to the start of the buttons
	Vector2d mpos(x, y);
	mpos -= (pos + buttonOffset);
	
	// If the mouse is above or to the left of the buttons, return NA
	if (mpos.x < 0 || mpos.y < 0) return NA;
	
	// If the mouse is below or to the right of the buttons, return NA
	if (direction == HORIZONTAL && ((mpos.x > ((numButtons * buttonSize.x) + ((numButtons - 1) * buttonSpacing)) || (mpos.y > buttonSize.y)))) return NA;
	else if (direction == VERTICAL && ((mpos.y > ((numButtons * buttonSize.y) + ((numButtons - 1) * buttonSpacing)) || (mpos.x > buttonSize.x)))) return NA;
	
	// Caluclate the selected button and return its index
	int button;
	if (direction == HORIZONTAL)
	{
		button =  (int)(mpos.x / (float)(buttonSize.x + buttonSpacing));
		
		// If the click lies in the space between two buttons
		if (mpos.x > (((button + 1) * (buttonSize.x + buttonSpacing)) - buttonSpacing)) return NA;
	}
	else if (direction == VERTICAL)
	{
		button =  (int)(mpos.y / (float)(buttonSize.y + buttonSpacing));
		
		// If the click lies in the space between two buttons
		if (mpos.y > (((button + 1) * (buttonSize.y + buttonSpacing)) - buttonSpacing)) return NA;
	}
	
		return button;
}

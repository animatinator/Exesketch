// rect.cpp


#include "rect.h"


Rect::Rect():
	Object(OBJ_RECT),
	width(50.0f),
	height(50.0f),
	fill(true),
	dragHandle(false)
{}

Rect::Rect(const Vector2d& startPos, float startWidth, float startHeight):
	Object(startPos, OBJ_RECT),
	width(startWidth),
	height(startHeight),
	fill(true),
	dragHandle(false)
{}

Rect::Rect(const Rect& other):
	Object(other),
	width(other.width),
	height(other.height),
	fill(other.fill),
	dragHandle(other.dragHandle)
{}

void Rect::Draw()
{
	if (state == EDITING) glColor3f(0.5f, 1.0f, 0.5f);
	else if (selected) glColor3f(0.0f, 0.5f, 1.0f);
	else glColor3f(0.0f, 0.0f, 0.0f);
	
	if (fill) glBegin(GL_QUADS);
	else glBegin(GL_LINE_LOOP);
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + width, pos.y);
		glVertex2f(pos.x + width, pos.y + height);
		glVertex2f(pos.x, pos.y + height);
	glEnd();
	
	if (dragHandle)
	{
		glColor3f(0.5f, 0.5f, 1.0f);
		
		glBegin(GL_LINE_LOOP);
			glVertex2f(oldPos.x, oldPos.y);
			glVertex2f(oldPos.x + oldWidth, oldPos.y);
			glVertex2f(oldPos.x + oldWidth, oldPos.y + oldHeight);
			glVertex2f(oldPos.x, oldPos.y + oldHeight);
		glEnd();
	}
	
	if (state == EDITING)
	{
		glColor3f(0.5f, 0.5f, 0.5f);
		
		DrawHandle(pos);
		DrawHandle(Vector2d(pos.x + width, pos.y));
		DrawHandle(Vector2d(pos.x + width, pos.y + height));
		DrawHandle(Vector2d(pos.x, pos.y + height));
	}
}

void Rect::HandleEvent(Event& event)
{
	if (state == EDITING)
	{
		if (event.type == EVENT_MOUSE_DOWN_LEFT)
		{
			selectedHandle = MouseHitHandle(event.realxpos, event.realypos);
			
			if (selectedHandle != NONE)
			{
				dragHandle = true;
				storedMouse = Vector2d(event.realxpos, event.realypos);
				oldWidth = width;
				oldHeight = height;
				oldPos = pos;
			}
			else dragHandle = false;
		}
		
		else if (event.type == EVENT_MOUSE_UP_LEFT)
		{
			if (dragHandle)
			{
				dragHandle = false;
				EndHandleDrag();
			}
		}
		
		else if (event.type == EVENT_MOUSE_MOTION)
		{
			if (dragHandle)
			{
				switch(selectedHandle)
				{
					case TOP_LEFT:
						pos.x = oldPos.x + (event.realxpos - storedMouse.x);
						width = oldWidth - (event.realxpos - storedMouse.x);
						pos.y = oldPos.y + (event.realypos - storedMouse.y);
						height = oldHeight - (event.realypos - storedMouse.y);
							break;
					
					case TOP_RIGHT:
						width = oldWidth + (event.realxpos - storedMouse.x);
						pos.y = oldPos.y + (event.realypos - storedMouse.y);
						height = oldHeight - (event.realypos - storedMouse.y);
							break;
					
					case BOTTOM_LEFT:
						pos.x = oldPos.x + (event.realxpos - storedMouse.x);
						width = oldWidth - (event.realxpos - storedMouse.x);
						height = oldHeight + (event.realypos - storedMouse.y);
							break;
					
					case BOTTOM_RIGHT:
						width = oldWidth + (event.realxpos - storedMouse.x);
						height = oldHeight + (event.realypos - storedMouse.y);
							break;
					
					default:
						break;
				}
				
				// Preserve aspect ratio when CTRL is held
				if (event.modifiers == GLUT_ACTIVE_CTRL)
				{
					if (fabs((float)width / (float)oldWidth) > fabs((float)height / (float)oldHeight))
					{
						float sign = height / fabs(height);  // Positive or negative?
						height = (int)(sign * ((fabs(width) / oldWidth) * oldHeight));
						
						if (selectedHandle == TOP_LEFT || selectedHandle == TOP_RIGHT)
						{
							pos.y = oldPos.y - (height - oldHeight);
						}
					}
					else
					{
						float sign = width / fabs(width);
						width = (int)(sign * ((fabs(height) / oldHeight) * oldWidth));
						
						if (selectedHandle == TOP_LEFT || selectedHandle == BOTTOM_LEFT)
						{
							pos.x = oldPos.x - (width - oldWidth);
						}
					}
				}
			}
		}
		
		else if (event.type == EVENT_KEYBOARD_K_TAB) state = OBJECT_VIEW;
	}
	
	else Object::HandleEvent(event);
	
	if (event.type == EVENT_KEYBOARD_K_F && selected) fill = !fill;
}

bool Rect::MouseHitObject(float x, float y) const
{
	return ((((x > pos.x) && (x < pos.x + width)) && ((y > pos.y) && (y < pos.y + height)))  // Mouse is inside shape
		|| (MouseHitHandle(x, y) != NONE && state == EDITING));  // Or (when in edit mode) mouse is over a corner handle
}

int Rect::MouseHitHandle(float x, float y) const
{
	if ((Vector2d::Distance(Vector2d(x, y), pos) / pixelSize) < HANDLE_SIZE) return TOP_LEFT;
	else if ((Vector2d::Distance(Vector2d(x, y), Vector2d(pos.x + width, pos.y)) / pixelSize) < HANDLE_SIZE) return TOP_RIGHT;
	else if ((Vector2d::Distance(Vector2d(x, y), Vector2d(pos.x, pos.y + height)) / pixelSize) < HANDLE_SIZE) return BOTTOM_LEFT;
	else if ((Vector2d::Distance(Vector2d(x, y), Vector2d(pos.x + width, pos.y + height)) / pixelSize) < HANDLE_SIZE) return BOTTOM_RIGHT;
	else return NONE;
}

void Rect::EndHandleDrag()
{
	// Fix negative widths and heights
	if (width < 0)
	{
		width *= -1;
		pos.x -= width;
	}
	
	if (height < 0)
	{
		height *= -1;
		pos.y -= height;
	}
}

Vector2d Rect::GetMin() const
{
	return pos;
}

Vector2d Rect::GetMax() const
{
	return (pos + Vector2d(width, height));
}

Object* Rect::CreateCopy()
{
	Object* newObject = new Rect(*this);
	
		return newObject;
}

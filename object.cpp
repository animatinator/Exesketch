// object.cpp

#include "object.h"


float Object::pixelSize = 2.0f;

Object::Object(int obType):
	type(obType),
	pos(Vector2d(0.0f, 0.0f)),
	selected(false),
	dragging(false),
	oldObject(NULL),
	state(OBJECT_VIEW)
{}

Object::Object(const Vector2d& startPos, int obType):
	type(obType),
	pos(startPos),
	selected(false),
	dragging(false),
	oldObject(NULL),
	state(OBJECT_VIEW)
{}

Object::Object(const Object& other):
	type(other.type),
	pos(other.pos),
	selected(other.selected),
	dragging(other.dragging),
	oldObject(other.oldObject),
	state(other.state)
{}

void Object::HandleEvent(Event& event)
{
	if (oldObject) delete oldObject;
	oldObject = CreateCopy();
	
	if (event.type == EVENT_MOUSE_DOWN_LEFT)
	{
		if (MouseHitObject(event.realxpos, event.realypos) && !event.HasFoundTarget())
		{
			// If the object is selected, all other selected objects should also be dragged
			if (selected) event.alwaysDragSelected = true;
			
			selected = true;
			dragging = true;
			oldPos = pos;
			storedMouse = Vector2d(event.realxpos, event.realypos);
			
			event.TargetFound();  // Tell the event that it has found its target
		}
		else
		{
			if (event.alwaysDragSelected && selected)
			{
				dragging = true;
				oldPos = pos;
				storedMouse = Vector2d(event.realxpos, event.realypos);
			}
			else if (state != EDITING && event.modifiers != GLUT_ACTIVE_SHIFT) selected = false;
		}
	}
	
	else if (event.type == EVENT_MOUSE_UP_LEFT)
	{
		dragging = false;
	}
	
	else if (event.type == EVENT_MOUSE_MOTION)
	{
		if (selected && dragging)
		{
			Vector2d currentMouse(event.realxpos, event.realypos);
			pos = oldPos + (currentMouse - storedMouse);
		}
	}
	
	else if (event.type == EVENT_KEYBOARD_K_TAB)
	{
		if (selected && (state != EDITING)) state = EDITING;
		else state = OBJECT_VIEW;
	}
}

void Object::DrawHandle(const Vector2d& pos, bool fill)
{
	float i = pixelSize * HANDLE_SIZE;
	float x = pos.x; float y = pos.y;
	
	if (fill) glBegin(GL_QUADS);
	else glBegin(GL_LINE_LOOP);
		glVertex2f(x - i, y - i);
		glVertex2f(x + i, y - i);
		glVertex2f(x + i, y + i);
		glVertex2f(x - i, y + i);
	glEnd();
}

void Object::DrawBoundingBox()
{
	Vector2d min = GetMin();
	Vector2d max = GetMax();
	
	glColor3f(0.0f, 0.3f, 0.7f);
	
	glBegin(GL_LINE_LOOP);
		glVertex2f(min.x, min.y);
		glVertex2f(max.x, min.y);
		glVertex2f(max.x, max.y);
		glVertex2f(min.x, max.y);
	glEnd();
}

void Object::RevertState()
{
	pos = oldObject->pos;
	selected = oldObject->selected;
	dragging = oldObject->dragging;
}

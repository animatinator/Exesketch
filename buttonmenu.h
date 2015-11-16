// objectmenu.h


#ifndef BUTTONMENU_H
#define BUTTONMENU_H


#include <vector>

#include "vector2d.h"
#include "event.h"


class Event;
typedef unsigned int GLuint;  // Better to define this type than to include the whole of gl.h

enum DIRECTION {HORIZONTAL = 0, VERTICAL};


class ButtonMenu
{
	public:
		ButtonMenu(vector<GLuint> images, Vector2d pos = Vector2d(0.0f, 0.0f), 
		int menuDirection = HORIZONTAL, Vector2d menuButtonSize = Vector2d(64.0f, 64.0f),
		Vector2d menuButtonOffset = Vector2d(5.0f, 5.0f), float menuButtonSpacing = 5.0f);
		
		void Draw() const;
		void HandleEvent(Event& event);
		
		Vector2d GetDimensions() const;
		bool MouseInWindow(float x, float y) const;
		int GetSelectedButton(float x, float y) const;
		
		Vector2d GetPos() const {return pos;}
		void SetPos(Vector2d newPos) {pos = newPos;}
		
		int GetSelection() const {return selection;}
		void SetSelection(int newSelect) {selection = newSelect;}
		
		
		void SetHorizontal() {direction = HORIZONTAL;}
		void SetVertical() {direction = VERTICAL;}
		
		void SetButtonOffset(Vector2d newOffset) {buttonOffset = newOffset;}
		Vector2d GetButtonOffset() const {return buttonOffset;}
		
		void SetButtonSize(Vector2d newSize) {buttonSize = newSize;}
		Vector2d GetButtonSize() const {return buttonSize;}
	
	private:
		int selection;
		
		Vector2d pos;
		int direction;
		
		Vector2d buttonOffset;
		Vector2d buttonSize;
		float buttonSpacing;
		
		vector<GLuint> buttonImages;
};

#endif

// rect.h


#ifndef RECT_H
#define RECT_H

#include "math.h"

#include "object.h"


class Event;


enum HANDLE_TYPE {NONE = 0, TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};

class Rect : public Object
{
	public:
		Rect();
		Rect(const Vector2d& startPos, float startWidth = 25.0f, float startHeight = 25.0f);
		Rect(const Rect& other);
		
		virtual void Draw();
		virtual void HandleEvent(Event& event);
		virtual bool MouseHitObject(float x, float y) const;
		
		int MouseHitHandle(float x, float y) const;
		
		void EndHandleDrag();
		
		virtual Vector2d GetMin() const;
		virtual Vector2d GetMax() const;
		
		float GetWidth() {return width;}
		float GetHeight() {return height;}
		void SetWidth(float newWidth) {width = newWidth;}
		void SetHeight(float newHeight) {height = newHeight;}
		
		bool GetFill() {return fill;}
		void SetFill(bool newFill) {fill = newFill;}
		
		virtual Object* CreateCopy();
	
	protected:
		float width, oldWidth;
		float height, oldHeight;
		
		int selectedHandle;
		bool dragHandle;
		
		bool fill;
};

#endif

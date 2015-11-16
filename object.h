// object.h


#ifndef OBJECT_H
#define OBJECT_H

#include "vector2d.h"
#include "event.h"


class Event;

enum OBJECT {OBJ_RECT = 0, OBJ_POLYGON, OBJ_CIRCLE, OBJ_BEZIER};
const float HANDLE_SIZE = 4.0;


class Object
{
	public:
		Object(int obType);
		Object(const Vector2d& startpos, int obType);
		Object(const Object& other);
		
		bool IsSelected() const {return selected;}
		int GetState() const {return state;}
		void SetState(int newState) {state = newState;}
		
		static void SetPixelSize(float newSize) {pixelSize = newSize;}
		static float GetPixelSize() {return pixelSize;}
		
		virtual void Draw() = 0;
		virtual void HandleEvent(Event& event);
		virtual bool MouseHitObject(float x, float y) const = 0;
		
		static void DrawHandle(const Vector2d& pos, bool fill = false);
		void DrawBoundingBox();
		
		virtual Vector2d GetMin() const = 0;
		virtual Vector2d GetMax() const = 0;
		
		Vector2d GetPos() {return pos;}
		void SetPos(Vector2d newPos) {pos = newPos;}
		
		virtual int GetType() {return type;}
		virtual Object* CreateCopy() = 0;
		
		void Select() {selected = true;}
		void Deselect() {selected = false;}
		
		void RevertState();
	
	protected:
		int type;  // Which object is it?
		Vector2d pos, oldPos;
		int state;
		bool selected, dragging;
		Vector2d storedMouse;
		Object* oldObject;
		
		static float pixelSize;
};

#endif

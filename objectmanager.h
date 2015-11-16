// objectmanager.h


#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <list>

#include "object.h"
#include "event.h"

using namespace std;


class Object;
class Event;
class Rect;


enum STATE {OBJECT_VIEW = 0, EDITING};

class ObjectManager
{
	public:
		ObjectManager(const vector<Object*>& newObjects = vector<Object*>());
		~ObjectManager();
		
		int GetState() const {return state;}
		void GoEdit();
		void GoObjectView();
		int GetNumSelected() const;
		
		void SetAddObject(int objectType) {addObject = objectType;}  // Sets the object to add on right-click
		int GetAddObject() const {return addObject;}
		
		vector<Object*> GetObjects() const {return objects;}
		
		void SetPixelSize(float newSize) const;
		
		void Draw() const;
		void HandleEvent(Event& event);
		
		void AddObject(Object* newObject) {objects.push_back(newObject);}
		bool MouseHitObject(float x, float y) const;
		
		void SelectAll();
		void DeselectAll();
	
	private:
		vector<Object*> objects;
		int state;
		int addObject; // Type of object to add on right-click
};

#endif

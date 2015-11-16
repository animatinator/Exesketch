// ObjectManagerdisplay.h


#ifndef OBJECTDISPLAY_H
#define OBJECTDISPLAY_H

#include "event.h"
#include "objectmanager.h"
#include "vector2d.h"
#include "buttonmenu.h"

#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glut.h>
#include <GL/freeglut.h>

#include <vector>

using namespace std;


class Event;
class ObjectManager;
class ButtonMenu;


class ObjectDisplay
{
	public:
		ObjectDisplay();
		ObjectDisplay(const vector<ObjectManager*>& objects);
		ObjectDisplay(const ObjectDisplay& other);
		
		void DrawObjects() const;
		void AddClient(ObjectManager* const newClient) {clients.push_back(newClient);}
		void AddMenu(ButtonMenu* const newMenu) {menus.push_back(newMenu);}
		
		float GetZoom() const{return zoomFactor;}
		void SetZoom(float newZoom) {zoomFactor = newZoom;}
		Vector2d GetOffset() const {return offset;}
		void SetOffset(const Vector2d& newOffset) {offset = newOffset;}
		
		Vector2d ScreenToReal(Vector2d coords);
		
		void HandleEvent(const Event& event);
		void Update();
	
	private:
		vector<ObjectManager*> clients;
		vector<ButtonMenu*> menus;
		ButtonMenu* objectMenu;
		float zoomFactor, oldZoomFactor;
		Vector2d offset, oldOffset;
		Vector2d storedMouse, realStoredMouse, currentMouse;
		bool dragging;
		bool zooming;
};

#endif

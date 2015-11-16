// Event.h


#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <queue>

#include "objectmanager.h"
#include "objectdisplay.h"
#include "buttonmenu.h"


class Object;
class ObjectDisplay;
class ObjectManager;

using namespace std;


const int NA = 99999;
enum EVENT_TYPE {EVENT_MOUSE_UP_LEFT = 0, EVENT_MOUSE_UP_MIDDLE, EVENT_MOUSE_UP_RIGHT,
	EVENT_MOUSE_DOWN_LEFT, EVENT_MOUSE_DOWN_MIDDLE, EVENT_MOUSE_DOWN_RIGHT,
	EVENT_MOUSE_MOTION,
	
	EVENT_KEYBOARD_K_A, EVENT_KEYBOARD_K_B, EVENT_KEYBOARD_K_C, EVENT_KEYBOARD_K_D,
	EVENT_KEYBOARD_K_E, EVENT_KEYBOARD_K_F, EVENT_KEYBOARD_K_G, EVENT_KEYBOARD_K_H,
	EVENT_KEYBOARD_K_I, EVENT_KEYBOARD_K_J, EVENT_KEYBOARD_K_K, EVENT_KEYBOARD_K_L,
	EVENT_KEYBOARD_K_M, EVENT_KEYBOARD_K_N, EVENT_KEYBOARD_K_O, EVENT_KEYBOARD_K_P,
	EVENT_KEYBOARD_K_Q, EVENT_KEYBOARD_K_R, EVENT_KEYBOARD_K_S, EVENT_KEYBOARD_K_T,
	EVENT_KEYBOARD_K_U, EVENT_KEYBOARD_K_V, EVENT_KEYBOARD_K_W, EVENT_KEYBOARD_K_X,
	EVENT_KEYBOARD_K_Y, EVENT_KEYBOARD_K_Z,
	
	EVENT_KEYBOARD_K_F1, EVENT_KEYBOARD_K_F2, EVENT_KEYBOARD_K_F3, EVENT_KEYBOARD_K_F4,
	EVENT_KEYBOARD_K_F5, EVENT_KEYBOARD_K_F6, EVENT_KEYBOARD_K_F7, EVENT_KEYBOARD_K_F8,
	EVENT_KEYBOARD_K_F9, EVENT_KEYBOARD_K_F10, EVENT_KEYBOARD_K_F11, EVENT_KEYBOARD_K_F12,
	
	EVENT_KEYBOARD_K_UP, EVENT_KEYBOARD_K_DOWN, EVENT_KEYBOARD_K_LEFT, EVENT_KEYBOARD_K_RIGHT,
	EVENT_KEYBOARD_K_PGUP, EVENT_KEYBOARD_K_PGDN, EVENT_KEYBOARD_K_HOME, EVENT_KEYBOARD_K_END,
	
	EVENT_KEYBOARD_K_INSERT, EVENT_KEYBOARD_K_DEL, EVENT_KEYBOARD_K_TAB, NO_EVENT = 99999};


class Event
{
	friend ostream& operator<<(ostream& os, const Event& event);
	
	public:
		Event(int eType = NO_EVENT, int mods = NA, int x = NA, int y = NA);
		Event(const Event& other);
		
		// Target functions - for informing objects whether a mouseclick has hit another object
		void TargetFound() {foundTarget = true;}
		void ForgetTarget() {foundTarget = false;}
		bool HasFoundTarget() const {return foundTarget;}
		
		void SetModifiers(int mods) {modifiers = mods;}  // DO NOT call outside of an event call
		
		int type;
		int xpos, ypos;
		float realxpos, realypos;
		int modifiers;
		bool foundTarget;  // Has the event found a target object?
		bool alwaysDragSelected;  // For when multiple selected objects are being dragged
};


class EventHandler
{
	public:
		EventHandler(ObjectDisplay* screen, const vector<ObjectManager*>& objects = vector<ObjectManager*>());
		
		void RegisterClient(ObjectManager* client);
		void RegisterMenu(ButtonMenu* menu);
		void ProcessQueue();
		queue<Event> GetEvents() const {return oldEventQueue;}
		void EmptyQueue();
		void AddEvent(Event& event);
	
	private:
		ObjectDisplay* display;
		vector<ObjectManager*> clients;
		vector<ButtonMenu*> menus;
		queue<Event> eventQueue, oldEventQueue;
};


#endif


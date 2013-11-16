#include "sprite.cpp"

class State{
	public:
		State();
		void init();
		void update();
		void add(Sprite);
		Sprite objects[1000];
};


State::State(){

}

void State::init(){

}

void State::update(){

}

void State::add(Sprite s){
	objects[objects.length] = s;
}
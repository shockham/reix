#include "core.reix.cpp"

class ThisGame: public Core{
	public:
		static const float TILE_WIDTH = 64.f;
        static const int MAP_HEIGHT = 15;
        static const int MAP_WIDTH = 20;
		void create();
		void update();
		void render();
		int map[MAP_HEIGHT][MAP_WIDTH] = {
	        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,1},
	        {1,0,2,0,0,2,0,0,0,0,0,0,2,0,0,2,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	        {1,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1},
	        {1,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1},
	        {1,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1},
	        {1,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1}
	    };

	    Sprite splash;
	    Sprite title;
	    int splash_timer;

		Sprite player;
		Sprite enemy;
		Sprite background;
		Sprite tiles[2];

		Sound minmin;

};

void ThisGame::create(){
	tiles[0].load_image("stuff/tile1.png");
    tiles[1].load_image("stuff/tile2.png");

    //make the sprites
    background.load_image("stuff/bg.png");
    background.width = SCREEN_WIDTH;
    background.height = SCREEN_HEIGHT;

    splash.load_image("stuff/splash.png");
    splash.width = SCREEN_WIDTH;
    splash.height = SCREEN_HEIGHT;
    splash_timer = 0;

    title.load_image("stuff/title.png");
    title.width = SCREEN_WIDTH/4;
    title.height = SCREEN_HEIGHT/4;

    player.load_image("stuff/cap_man.png");
    player.movement = 5.f;
    player.x = player.width;
    player.y = player.height;

    enemy.load_image("stuff/bad_man.png");
    enemy.x = SCREEN_WIDTH - (enemy.width*2);
    enemy.y = enemy.height;
    enemy.movement = 5.f;

    minmin.load_sound("stuff/minminzemi.wav");
    minmin.play();
    // minmin.stop();
}

void ThisGame::update(){
    //update the other stuff
    // background.update(); // don't think this needs updating
    player.update();
    enemy.update();

    //keys for player one movement
    if(keysHeld[SDLK_d]) player.x += player.movement;
    if(keysHeld[SDLK_w]) player.y -= player.movement;
    if(keysHeld[SDLK_a]) player.x -= player.movement;
    if(keysHeld[SDLK_s]) player.y += player.movement;

    //key for player two movement
    if(keysHeld[SDLK_l]) enemy.x += enemy.movement;
    if(keysHeld[SDLK_i]) enemy.y -= enemy.movement;
    if(keysHeld[SDLK_j]) enemy.x -= enemy.movement;
    if(keysHeld[SDLK_k]) enemy.y += enemy.movement;

    //check the collisions on them
    collide(player, enemy);
}

void ThisGame::render(){
    background.render();

    for (int i = 0; i < MAP_HEIGHT; ++i){
        for (int j = 0; j < MAP_WIDTH; ++j){
            
            switch(map[i][j]){
                case 1:
                    tiles[0].x = TILE_WIDTH * j;
                    tiles[0].y = TILE_WIDTH * i;
                    tiles[0].render();
                    collide(tiles[0], player);
                    collide(tiles[0], enemy);
                    break;
                case 2:
                    tiles[1].x = TILE_WIDTH * j;
                    tiles[1].y = TILE_WIDTH * i;
                    tiles[1].render();
                    collide(tiles[1], player);
                    collide(tiles[1], enemy);
            }
        }
    }

    //render the enemy and the player
    enemy.render();
    player.render();

    if(splash_timer < 60){
    	splash_timer += fps.get_ticks();
    	splash.render();
    }else if(splash_timer < 120){
    	splash_timer += fps.get_ticks();
    	title.render();
    }
}


int main(int argc, char *argv[]){
    ThisGame g;
    g.start();
	return 0;
}

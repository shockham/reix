#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#include "timer.cpp"
#include "sprite.cpp"

class Game{
    private:
        static const int SCREEN_WIDTH = 640;
        static const int SCREEN_HEIGHT = 480;
        static const int SCREEN_BPP = 32;
        static const int FRAMES_PER_SECOND = 60;
        static const float TILE_WIDTH = 64.f;
        static const int MAP_HEIGHT = 10;
        static const int MAP_WIDTH = 10;
        bool initGL();
        bool init();
        void render();
        void update();
        void clean_up();
    public:
        SDL_Event event;
        bool keysHeld[323];
        int map[MAP_HEIGHT][MAP_WIDTH];
        Game();
        bool overlap(Sprite A, Sprite B);
        void collide(Sprite A, Sprite B);

};

Sprite player;
Sprite enemy;
Sprite background;
Sprite tiles[2];

bool Game::initGL(){
    glEnable( GL_TEXTURE_2D );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    glClear( GL_COLOR_BUFFER_BIT );

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Check for error
    GLenum error = glGetError();
    if(error != GL_NO_ERROR){
        // printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        return false;
    }

    return true;
}

bool Game::init(){
    //Initialize SDL
    if(SDL_Init( SDL_INIT_EVERYTHING ) < 0) return false;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    //Create Window
    if(SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL | SDL_RESIZABLE ) == NULL) return false;

    //Enable unicode
    SDL_EnableUNICODE( SDL_TRUE );

    //Initialize OpenGL
    if(initGL() == false) return false;

    //Set caption
    SDL_WM_SetCaption( "Framework", NULL );

    tiles[0].load_image("stuff/tile1.png");
    tiles[1].load_image("stuff/tile2.png");

    background.load_image("stuff/bg.png");
    background.width = SCREEN_WIDTH;
    background.height = SCREEN_HEIGHT;

    player.load_image("stuff/cap_man.png");
    player.movement = 2.f;
    player.x = player.width;
    player.y = player.height;

    enemy.load_image("stuff/bad_man.png");
    enemy.x = SCREEN_WIDTH - (enemy.width*2);
    enemy.y = enemy.height;
    enemy.movement = 2.f;

    return true;
}

bool Game::overlap(Sprite A, Sprite B){
    //The sides of the rectangles
    float leftA, leftB;
    float rightA, rightB;
    float topA, topB;
    float bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = A.x;
    rightA = A.x + A.width;
    topA = A.y;
    bottomA = A.y + A.height;
        
    //Calculate the sides of rect B
    leftB = B.x;
    rightB = B.x + B.width;
    topB = B.y;
    bottomB = B.y + B.height;

     //If any of the sides from A are outside of B
    if( bottomA <= topB ) return false;
    if( topA >= bottomB ) return false;
    if( rightA <= leftB ) return false;
    if( leftA >= rightB ) return false;
    
    //If none of the sides from A are outside B
    return true;
}

void Game::collide(Sprite A, Sprite B){
    if(overlap(A, B)){
        //break apart
    }
}

void Game::update(){
    background.update();
    player.update();
    enemy.update();

    if(keysHeld[SDLK_d]){ 
        player.x += player.movement;
        if(overlap(player, enemy)) player.x -= player.movement;
    }
    if(keysHeld[SDLK_w]){
        player.y -= player.movement;
        if(overlap(player, enemy)) player.y += player.movement;
    }
    if(keysHeld[SDLK_a]){
        player.x -= player.movement;
        if(overlap(player, enemy)) player.x += player.movement;
    }
    if(keysHeld[SDLK_s]){
        player.y += player.movement;
        if(overlap(player, enemy)) player.y -= player.movement;
    }

    if(keysHeld[SDLK_RIGHT]){
        enemy.x += enemy.movement;
        if(overlap(player, enemy)) enemy.x -= player.movement;
    }
    if(keysHeld[SDLK_UP]){
        enemy.y -= enemy.movement;
        if(overlap(player, enemy)) enemy.y += enemy.movement;
    }
    if(keysHeld[SDLK_LEFT]){
        enemy.x -= enemy.movement;
        if(overlap(player, enemy)) enemy.x += enemy.movement;
    }
    if(keysHeld[SDLK_DOWN]){
        enemy.y += enemy.movement;
        if(overlap(player, enemy)) enemy.y -= enemy.movement;
    }

    // if(player.y < 400){
    //     player.y++;
    //     if(overlap(player, enemy)) player.y--;
    // }
    // if(enemy.y < 400){
    //     enemy.y++;
    //     if(overlap(player, enemy)) player.y--;
    // }
}

void Game::render(){
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    background.render();

    for (int i = 0; i < MAP_HEIGHT; ++i){
        for (int j = 0; j < MAP_WIDTH; ++j)
        {
            switch(map[i][j]){
                case 1:
                    tiles[0].x = TILE_WIDTH * j;
                    tiles[0].y = TILE_WIDTH * i;
                    tiles[0].render();
                    break;
                case 2:
                    tiles[1].x = TILE_WIDTH * j;
                    tiles[1].y = TILE_WIDTH * i;
                    tiles[1].render();
            }
        }
    }

    enemy.render();
    player.render();

    //Update screen
    SDL_GL_SwapBuffers();
}

void Game::clean_up(){
    //Quit SDL
    SDL_Quit();
}

Game::Game(){
    //Quit flag
    bool quit = false;

    for (int i = 0; i < 323; ++i) keysHeld[i] = false;

    int map_cpy[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,1},
        {1,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,1}
    };
    memcpy(map, map_cpy, sizeof map);

    //Initialize
    if(init()){
        //The frame rate regulator
        Timer fps;
        
        //Wait for user exit
        while(quit == false){
            //Start the frame timer
            fps.start();
            //While there are events to handle
            while(SDL_PollEvent(&event)){
                if(event.type == SDL_QUIT) quit = true;
                else if( event.type == SDL_KEYDOWN ) keysHeld[event.key.keysym.sym] = true;
                else if( event.type == SDL_KEYUP ) keysHeld[event.key.keysym.sym] = false;
                else if( event.type == SDL_VIDEORESIZE ){
                    // glViewport( 0, 0, event.resize.w, event.resize.h );
                    // glOrtho(0.0f, event.resize.w, event.resize.h, 0.0f, -1.0f, 1.0f);
                }
            }
            //Run frame update
            update();
            //Render frame
            render();
            //Cap the frame rate
            if(fps.get_ticks() < 1000 / FRAMES_PER_SECOND){
                SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
            }
        }
        //Clean up
        clean_up();
    }
}

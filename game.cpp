#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "timer.cpp"
#include "sprite.cpp"

class Game{
    private:
        bool initGL();
        bool init();
        void pre_render();
        void post_render();
        void clean_up();
    public:
        virtual void create() =0;
        virtual void update() =0;
        virtual void render() =0;
        static const int SCREEN_WIDTH = 640;
        static const int SCREEN_HEIGHT = 480;
        static const int SCREEN_BPP = 32;
        static const int FRAMES_PER_SECOND = 60;
        static const float TILE_WIDTH = 64.f;
        static const int MAP_HEIGHT = 10;
        static const int MAP_WIDTH = 10;
        SDL_Event event;
        SDL_Window *window;
        SDL_GLContext glcontext;
        bool keysHeld[323];
        Game();
        void start();
        bool overlap(Sprite A, Sprite B);
        void collide(Sprite& A, Sprite& B);
};

bool Game::initGL(){
    glEnable( GL_TEXTURE_2D );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

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
        printf( "Error initializing OpenGL!" );
        return false;
    }

    return true;
}

bool Game::init(){
    //Initialize SDL
    if(SDL_Init( SDL_INIT_EVERYTHING ) < 0){
        return false;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    //Create Window
    window = SDL_CreateWindow("Reix", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    glcontext = SDL_GL_CreateContext(window);

    //Initialize OpenGL
    if(initGL() == false) return false;

    create();

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

void Game::collide(Sprite& A, Sprite& B){
    if(overlap(A, B)){
        //break apart
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

        float bottom_diff = abs(bottomA - topB);
        float top_diff = abs(topA - bottomB);
        float right_diff = abs(rightA - leftB);
        float left_diff = abs(leftA - rightB);

        if(bottom_diff < top_diff && bottom_diff < right_diff && bottom_diff < left_diff){
            A.y -= bottom_diff;
            B.y += bottom_diff;
        }else if(top_diff < right_diff && top_diff < left_diff){
            A.y += top_diff;
            B.y -= top_diff;
        }else if(right_diff < left_diff){
            A.x -= right_diff;
            B.x += right_diff;
        }else{
            A.x += left_diff;
            B.x -= left_diff;
        }
    }
}

void Game::pre_render(){
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );

    this->render();
    this->post_render();
}
void Game::post_render(){
    //Update screen
    SDL_GL_SwapWindow(window);
}

void Game::clean_up(){
    //Quit SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Game::Game(){}

void Game::start(){
    //Quit flag
    bool quit = false;

    for (int i = 0; i < 323; ++i) keysHeld[i] = false;

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
                if(event.type == SDL_QUIT){
                    quit = true;
                }else if(event.type == SDL_KEYDOWN && event.key.keysym.sym < 323){
                    keysHeld[event.key.keysym.sym] = true;
                }else if(event.type == SDL_KEYUP && event.key.keysym.sym < 323){
                    keysHeld[event.key.keysym.sym] = false;
                }
            }
            //Run frame update
            update();
            //Render frame
            pre_render();
            //Cap the frame rate
            if(fps.get_ticks() < 1000 / FRAMES_PER_SECOND){
                SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
            }
        }
        //Clean up
        clean_up();
    }
}

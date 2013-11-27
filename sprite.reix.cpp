#include "SDL2_image/SDL_image.h"

class Sprite{
	private:
		GLuint texture;         // This is a handle to our texture object
		SDL_Surface *surface;   // This surface will tell us the details of the image
		GLenum texture_format;
		GLint  nOfColors;

	public:
		float x;
		float y;
		float width;
		float height;
		float movement;
		Sprite();
        ~Sprite();
		int load_image(const char* img_path);
		void update();
		void render();
};

Sprite::Sprite(){
	x = 0.f;
	y = 0.f;
	width = 64.f;
	height = 64.f;
	movement = 1.f;
}

Sprite::~Sprite(){
    
}

int Sprite::load_image(const char* img_path){
    if((surface = IMG_Load(img_path))){ 

        if((surface->w & (surface->w - 1)) != 0){ printf("warning: %s image width is not a power of 2\n", img_path); }
    
        if((surface->h & (surface->h - 1)) != 0){ printf("Warning: %s image height not a power of 2 \n", img_path); }
     
        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if(nOfColors == 4){    // contains an alpha channel
            if(surface->format->Rmask == 0x000000ff) texture_format = GL_RGBA;
            else texture_format = GL_BGRA;
        }else if (nOfColors == 3){     // no alpha channel
            if(surface->format->Rmask == 0x000000ff) texture_format = GL_RGB;
            else texture_format = GL_BGR;
        }else{
                printf("warning: the image is not truecolor..  this will probably break\n");
                // this error should not go unhandled
        }
     
        // Have OpenGL generate a texture object handle for us
        glGenTextures(1, &texture);
     
        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture);
     
        // Set the texture's stretching properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     
        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    }else{
        // SDL could not load image
        SDL_Quit();
        return false;
    }    
     
    // Free the SDL_Surface only if it was successfully created
    if(surface){ SDL_FreeSurface(surface); }

    return true;
}

void Sprite::update(){ }

void Sprite::render(){
	glBindTexture(GL_TEXTURE_2D, texture);

    //Render quad
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex3f(x, y, 0.f);

        glTexCoord2i(1, 0);
        glVertex3f(x + width , y, 0.f);

        glTexCoord2i(1, 1);
        glVertex3f(x + width,  y + height, 0.f);

        glTexCoord2i(0, 1);
        glVertex3f(x,  y + height, 0.f);
    glEnd();
}

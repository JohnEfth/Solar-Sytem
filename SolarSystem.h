


//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();


void keyboardDown(unsigned char key, int x, int y);
// when the user presses a normal key, this is called

void specialKeys(int key, int x, int y);
// when the use presses a special key, this is called

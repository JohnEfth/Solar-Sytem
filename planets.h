
// ------------------ Library with objects -----------
#include <vector>

using namespace std;

typedef struct CObjLoader
{
	CObjLoader();
	void Clear();
	int Load(const char *filename);
	vector <float> v;	// verteces
	vector <float> n;	// normals
	vector <float> t;	// triangles

	// bounding box
	float min[3], max[3];
	void DrawSolid(float maxSize);
	void DrawWireframe(float maxSize);
} objLoader;

void drawStars();
void initStars();

void planets(CObjLoader & sphere, double AX,double AY,double EX, double EY ,double EEX, double EEY,double SX, double SY,double PX,double PY, float rot,float coloor,float sunsize);
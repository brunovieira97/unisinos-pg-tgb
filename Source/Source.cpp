using namespace std;

#include <Classes/SceneManager.h>

int main() {
	SceneManager *scene = new SceneManager;
	scene->initialize(800, 600);

	scene->run();

	scene->finish();
	
	return 0;
}
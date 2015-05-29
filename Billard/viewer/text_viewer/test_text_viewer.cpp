#include <iostream>
#include "text_viewer.h"
#include "boule.h"

int main(int argc, const char * argv[]) {
    Boule b(1,2,3,4);
    TextViewer viewer;
    Objet& objet = b;
    
    viewer.dessiner(objet);
    return 0;
}
 
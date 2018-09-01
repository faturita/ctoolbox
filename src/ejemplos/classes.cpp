#include <stdio.h>
#include <stdlib.h>

class fatu
{
    public:
    int a;
    int b;
    int c;
    int main() { return 2; }
    void show() {
	printf("Gil...");
    }
};

int main(int argc, char *argv[])
{
    int a= 2;
    fatu f;
    f.show();
    return 1;
}

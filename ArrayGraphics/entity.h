#pragma once
//maximum array size (width as well as height) for each entity
#define MAX 101

//#define color codes for manipulating console color
#define BLACK        '0'
#define BLUE         '1'
#define GREEN        '2'
#define AQUA         '3'
#define RED          '4'
#define PURPLE       '5'
#define YELLOW       '6'
#define WHITE        '7'
#define GRAY         '8'
#define LIGHT_BLUE   '9'
#define LIGHT_GREEN  'A'
#define LIGHT_AQUA   'B'
#define LIGHT_RED    'C'
#define LIGHT_PURPLE 'D'
#define LIGHT_YELLOW 'E'
#define BRIGHT_WHITE 'F'

//base class of all the entities
class entity
{
public:
    //array of MAX size to be allocated to each entity
    char array[MAX][MAX];
    //store the character for entity border in BOUNDARY
    char BOUNDARY;
    //store the character to fill the space in REGION
    char REGION;
    //store the used size for each entity.
    //WIDTH : stores Horizontal size - Width[number of columns]
    //HEIGHT : store Vertical size    - Height[number of rows]
    int WIDTH, HEIGHT;
    //X, Y positions
    int POS_X, POS_Y;

    //entity constructor
    //takes in rows and columns for usable size for each entity
    //Maximum columns = maximum rows = MAX(defined above)
    entity(int row = 5, int col = 5, int posX = 0, int posY = 0);
    //checks if any entity collides with any other entity
    bool collideWith(entity&);
    //checks if this entity is above argument entity
    bool isAbove(entity&);
    //checks if this entity is below argument entity
    bool isBelow(entity&);
    //checks if this entity is after argument entity
    bool isAfter(entity&);
    //checks if this entity is before argument entity
    bool isBefore(entity&);
    //control the entity wrt another
//    void control(char up = 'W', char down = 'S', char left = 'A', char right = 'D');
    void control(char up, char down, char left, char right);
};

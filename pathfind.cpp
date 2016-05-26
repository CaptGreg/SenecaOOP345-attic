// http://www.cplusplus.com/forum/general/101799/

// GB needs C++11
// g++ -c -std=c++11 pathfind.cpp

/*
  pathFinding is a hard and complicated topic.

  I modified an A* algorithm to accept pretty much any type of map, maybe it'll 
  be of good use to you: it returns a string of movement (0 down right, 1 down, 
  2 down left, 3 left, 4 up left, 5 up, 6, up right, 7 right).  The function 
  accepts a function which returns true upon collision and false when there's 
  no collision.

         4 5 6
         3 * 7
         2 1 0
*/
using namespace std;

// pathFind.hpp
#ifndef PATHFIND_HPP_INCLUDED
#define PATHFIND_HPP_INCLUDED


#include <string>
#include <functional>


std::string pathFind
(
    const int xStart, const int yStart,
    const int xFinish, const int yFinish,
    std::function<bool(const int, const int)> collision
);

#endif // PATHFIND_HPP_INCLUDED 

// pathFind.cpp
// #include "PathFind.hpp"
#include <queue> // Using std::priority_queue
#include <map> // Using std::map
#include <memory> // Using std::unique_ptr
#include <cmath> // Using std::abs and std::sqrt


std::string pathFind
(
    const int xStart, const int yStart,
    const int xFinish, const int yFinish,
    std::function<bool(const int, const int)> collision
)
{
    enum {dir = 8}; // number of possible directions to go at any position

    class node
    {
        // current position
        int xPos;
        int yPos;
        // total distance already travelled to reach the node
        int level;
        // priority=level+remaining distance estimate
        int priority;  // smaller: higher priority

    public:

        node(int xp, int yp, int d, int p)
        :
        xPos(xp),
        yPos(yp),
        level(d),
        priority(p)
        {}

        inline int getxPos() const {return xPos;}
        inline int getyPos() const {return yPos;}
        inline int getLevel() const {return level;}
        inline int getPriority() const {return priority;}

        inline void updatePriority(const int xDest, const int yDest)
        {
             priority = level + estimate(xDest, yDest) * 10; //A*
        }

        // give better priority to going strait instead of diagonally
        inline void nextLevel(const int i) // i: direction
        {
             level += (dir == 8?(i % 2 == 0 ? 10 : 14) : 10);
        }

        // Estimation function for the remaining distance to the goal.
        inline const int estimate(const int xDest, const int yDest) const
        {
            static int xd, yd, d;
            xd = xDest - xPos;
            yd = yDest - yPos;

            // Euclidian Distance
            //d = static_cast<int>(std::sqrt(xd * xd + yd * yd));

            // Manhattan distance
            d = std::abs(xd) + std::abs(yd);

            // Chebyshev distance
            //d = std::max(std::abs(xd), std::abs(yd));

            return d;
        }

        inline bool operator<(const node &rhs) const
        {
            return this->getPriority() > rhs.getPriority();
        }
    };

    const static int dx[dir] = {1, 1, 0, -1, -1, -1, 0, 1};
    const static int dy[dir] = {0, 1, 1, 1, 0, -1, -1, -1};

    std::priority_queue<node> pq[2]; // pqueue of open (not-yet-tried) nodes
    int pqi = 0; // pq index
    std::unique_ptr<node> n0(new node(xStart, yStart, 0, 0)); // mapp
    std::unique_ptr<node> m0;
    int i, j, x, y, xdx, ydy;
    char c;

    std::map<int, std::map<int, int>> closed_nodes_map; // map of closed (tried-out) nodes
    std::map<int, std::map<int, int>> open_nodes_map; // map of open (not-yet-tried) nodes
    std::map<int, std::map<int, int>> dir_map; // map of directions

    // create the start node and push into list of open nodes
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[x][y] = n0->getPriority(); // mark it on the open nodes map

    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0.reset(new node(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority()));

        x = n0->getxPos(); y = n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[x][y] = 0;
        // mark it on the closed nodes map
        closed_nodes_map[x][y] = 1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x == xFinish && y == yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            std::string path = "";
            while(!(x == xStart && y == yStart))
            {
                j = dir_map[x][y];
                c = '0' + (j + dir / 2) % dir;
                path = c + path;
                x += dx[j];
                y += dy[j];
            }

            // empty the leftover nodes
            //while(!pq[pqi].empty()) pq[pqi].pop();
            /// No need to, object is local. Let STD handle it.
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i = 0; i < dir; ++i)
        {
            xdx = x + dx[i]; ydy = y + dy[i];

            if(!(/*xdx < lower_x_bound *//*|| xdx > upper_x_bound - 1 *//*We have unlimited size*//*|| ydy < lower_y_bound *//*|| ydy > upper_y_bound - 1*//*Unlimited size ||*/
             collision(xdx, ydy) == true
                || closed_nodes_map[xdx][ydy] == 1))
            {
                // generate a child node
                m0.reset(new node(xdx, ydy, n0->getLevel(),
                             n0->getPriority()));
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[xdx][ydy] == 0)
                {
                    open_nodes_map[xdx][ydy]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[xdx][ydy] = (i + dir / 2 )%dir;
                }
                else if(open_nodes_map[xdx][ydy]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[xdx][ydy] = m0->getPriority();
                    // update the parent direction info
                    dir_map[xdx][ydy] = (i + dir / 2) % dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getxPos() == xdx &&
                           pq[pqi].top().getyPos() == ydy))
                    {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size() > pq[1 - pqi].size())
                        pqi = 1 - pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi = 1 - pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
            }
        } // Possible child nodes in all directions
    } // while pq is not empty.
    return ""; // no route found
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>  // gluPerspective

#define OPEN 1
#define CLOSED 2
#define UNVISITED 3
#define BLOCKED 4
#define HIGHLIGHT 5
#define ONROUTE 6
#define GOAL 7

#define GSIZE 20 // size of tile grid
#define ISTART 15 // index position of starting tile in grid
#define JSTART 8 //
#define IGOAL 7 // index position of goal tile in grid
#define JGOAL 8

bool endsearch = false ; // flag to indeicate have reached goal
int icurrent = 15 ; // index position of the current tile
int jcurrent = 8 ;
int cursori = 10;
int cursorj = 10;

struct tile
{
  int status ; // Is tile open, closed, unvisited etc.
  int cost ; // accumulated cost from start to this tile
  int iparent ; // index position in grid for tile previous to this one
  int jparent ;
  int h; //heuristic
  int F;
};
tile grid[GSIZE][GSIZE] ; // 2d array of tiles. The main data structure
//Math.abs(start.x-destination.x) + Math.abs(start.y-destination.y))
int costinc = 10; // set cost at 10 for sideways and updown movements
//int heuristic = 10 *((abs(ISTART-IGOAL)/GSIZE) + (abs(JSTART-JGOAL)/GSIZE)) ;

#if 1
// GB May 26, 2016 gluPerspective woes:
// /usr/include/GL/glu.h:327:23: note: candidate 1: void gluPerspective(GLdouble, GLdouble, GLdouble, GLdouble)
// GLAPI void GLAPIENTRY gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

/*GLAPI*/ void GLAPIENTRY gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
// GB GLAPI undefined
{
  // GB pilferred from somewhere
  auto DEG_TO_RAD = [] (GLdouble deg) { return M_PI * deg / 180.; };
  double range = zNear*tan(DEG_TO_RAD(fovy/2));
  glFrustum(-range*aspect,range*aspect,-range,range,zNear,zFar);
  return;
}
#else
void gluPerspective(GLfloat fovy,        
                    GLfloat aspect,
                    GLfloat znear,
                    GLfloat zfar)
{
  // GB pilferred from somewhere
  auto DEG_TO_RAD = [] (GLfloat deg) { return M_PI * deg / 180.; };
  double range = znear*tan(DEG_TO_RAD(fovy/2));
  glFrustum(-range*aspect,range*aspect,-range,range,znear,zfar);
  return;
}
#endif

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    // GB pilferred from EGL
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

//matrix will receive the calculated perspective matrix.
//You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
    // GB pilferred from EGL
    float ymax, xmax;
    // float temp, temp2, temp3, temp4; // GB not used
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    //ymin = -ymax;
    //xmin = -ymax * aspectRatio;
    xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}




void Initialize()
//
// Initialise all tiles to UNVISITED except those around the edges. Edge tiles are
// set to BLOCKED so the search cannot go over outside the grid
// Make the start tile the current OPEN tile with accumulated cost of zero
//
{
  int i, j ;

  for(i=0 ; i< GSIZE ; i++)
  {
    for(j = 0 ; j<GSIZE; j++)
    {
      if((i == 0) || (i==GSIZE-1) ||(j == 0) || (j==GSIZE-1))
      {
        grid[i][j].status = BLOCKED ;
      }
      else
      {
        grid[i][j].status = UNVISITED ;

      }
    }
  }
  grid[ISTART][JSTART].status = OPEN ;
  grid[ISTART][JSTART].cost = 0 ;
  grid[IGOAL][JGOAL].status = GOAL;
}
void getroute()
//
// The goal tile has been reached. Using the iparent, jparent fields trace the path
// back to the start. For each tile on the way back mark its status as ONROUTE. Do
//this until iparent = 0 so have reached start
//
{
  int i = icurrent ;
  int j = jcurrent ;
  int itemp, jtemp ;
  do
  {
    grid[i][j].status = ONROUTE ; // mark tile as on path
    itemp = grid[i][j].iparent ; // dont overwrite i,j until we have finished using them
    jtemp = grid[i][j].jparent ;
    i = itemp ;
    j = jtemp ;
  }
  while(i != 0) ; // reached start
    endsearch = true ; // set finished flag
}
void findcheapest(void)
//
// Cycle through all the tiles(nodes) and look at those that are OPEN
// Find which OPEN tile has the lowest cost. This then becomes the current tile
//(icurrent, jcurrent). If the node selected is also the finishing point then the
// solution has been found so call getroute to list the route.
{
  // arbitrary high initial value for lowcost comparison
  int i, j ;
  //int F = grid[i][j].cost + grid[i][j].h;
  for(i=0 ; i< GSIZE ; i++)
  {
    for(j = 0 ; j<GSIZE; j++)
    {
      if (grid[i][j].status == OPEN)
      {
        if (grid[i][j].cost < grid[i][j].F) // is this the lowest cost so far?
        {
          grid[i][j].F = grid[i][j].cost ; // if yes then make this tile
          icurrent = i ; // the current tile
          jcurrent = j ;
        }
      }
    }
  }
  // have we reached the destination ?. If yes then generate the route
  if((icurrent == IGOAL) && (jcurrent == JGOAL))

  getroute() ;
}
void update(int i, int j)
//
// Converts the node/tile [i][j] into an OPEN node and assigns
// an accumulated cost (parent cost + increment). Stores the path
// to this tile from the current tile in iparent, jparent
//
{
  grid[i][j].status = OPEN ;
  grid[i][j].cost = grid[icurrent][jcurrent].cost+ costinc;
  grid[i][j].h = ((abs(icurrent-IGOAL) + (abs(jcurrent-JGOAL))));
  grid[i][j].F = (grid[i][j].cost + costinc) + grid[i][j].h;
  grid[i][j].iparent = icurrent ;
  grid[i][j].jparent = jcurrent ;
}
void getconnection(int i, int j)
//
// Check if this path is BLOCKED. If it is then return and do nothing - we cannot
// go there. If not BLOCKED then check if it is as yet UNVISITED. If so then convert it to an OPEN
// node/tile. If the node/tile is already OPEN then compare the cost it would have when
// reached via this route against the cost that it already has (having previously been reached
// from a different tile). If the newcost(from current tile) is lower then replace the cost
// value and the previous tile link with the new values
//
{
  int newcost ;
  if (grid[i][j].status != BLOCKED)
  {
    switch (grid[i][j].status)
    {
      case UNVISITED:
        update(i,j) ;
        break ;
      case HIGHLIGHT:
        update(i,j) ;
        break;
      case GOAL:
        update(i,j) ;
        break;

      case OPEN:
        newcost = grid[icurrent][jcurrent].cost + costinc ;
        if (newcost < grid[i][j].cost)
        {
          update(i, j) ;
        }
        break ;
      case CLOSED:
        newcost = grid[icurrent][jcurrent].cost + costinc ;
        if (newcost < grid[i][j].cost)
        {
          update(i, j) ;
        }
        break ;
    }
  }
}

void walk(void)
//
// From the current tile examine all its neighbors for a possible pathway. Looks at
// left, right, up, down neighbours but also diagonal moves. Moving diagonally means
// moving a further in distance than when moving sideways etc. So make the cost higher.
// (change costinc.) A cost of 14 approximates the extra distance whilst avoiding use of floats.
// When we have processed all the possible pathways from the current tile we can then CLOSE it
//
{
  int i = icurrent ;
  int j = jcurrent ;
  costinc = 10 ;
  getconnection(i+1, j) ;
  getconnection(i-1, j) ;
  getconnection(i, j+1) ;
  getconnection(i, j-1) ;
  costinc = 14 ;
  getconnection(i+1, j+1) ;
  getconnection(i+1, j-1) ;
  getconnection(i-1, j+1) ;
  getconnection(i-1, j-1) ;
  grid[i][j].status = CLOSED ;
}

void drawsquares (void) { //draws the entire grid interface and colours the grid

  for (int i = 0; i < GSIZE; i++){
    if(i == 0){
      glTranslatef(-2.0 ,3.0, 0.0);
    }else{
      glTranslatef(-3.0 ,-0.15, 0.0);
    }
    for (int j = 0; j < GSIZE; j++){
      glTranslatef(0.15,0.0,0.0);
      switch (grid[i][j].status)
      {
      case 1:
        glColor3f(0,1,0);
        break;
      case 2:
        glColor3f(1,0,0);
        break;
      case 3:
        glColor3f(0.3,0.3,0.3);
        break;
      case 4:
        glColor3f(1,0,1);
        break;
      case 5:
        glColor3f(1,1,1);
        break;
      case 6:
        glColor3f(0,0,1);
        break;
      case 7:
        glColor3f(1,1,0);
        break;
      }
      if((icurrent == IGOAL) && (jcurrent == JGOAL))
      {
        grid[IGOAL][JGOAL].status = ONROUTE;
      }
      glPushMatrix(); //push the matrix so that our translations only affect this tile
      glTranslatef(j, -i, 0); //translate the tile to where it should belong
      glBegin (GL_QUADS); //begin drawing our quads
      glVertex3f(0.0, 0.0, 0.0); //with our vertices we have to assign a texcoord
      glVertex3f(1.0, 0.0, 0.0); //so that our texture has some points to draw to
      glVertex3f(1.0, 1.0, 0.0);
      glVertex3f(0.0, 1.0, 0.0);
      glEnd();
      glPopMatrix();
    }
  }
}
void display(void)
{
  glClearColor (0.0,0.0,0.0,1.0);
  glClear (GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(-8, 6, -30); //translate back a bit to view the map correctly
  drawsquares();
  glFlush();
}

void reshape (int w, int h) {
  glViewport (0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  gluPerspective ((GLfloat) 60, (GLfloat)w / (GLfloat)h, 1.0, 100.0); // GB was 60, added GLfloat cast 
  glMatrixMode (GL_MODELVIEW);
}


void keyboard(unsigned char key, int i, int j)
{
// int i = cursori;
//int j = cursorj;

  // grid[cursori][cursorj];  // compiler says: no effect
  glColor3f(1,1,1); ;

  switch (key)
  {
  case 'p':
    do
    {
    findcheapest() ;
    walk() ;
    }while (endsearch == false) ;
    break ;
  case 'd':
    cursorj = cursorj + 1;
    i= cursori;
    j= cursorj;
    grid[i][j].status = HIGHLIGHT;

    break ;
  case 'a':
    cursorj = cursorj - 1 ;
    i= cursori;
    j= cursorj;
    grid[i][j].status = HIGHLIGHT;

    break ;
  case 'w':
    cursori = cursori - 1 ;
    i= cursori;
    j= cursorj;
    grid[i][j].status = HIGHLIGHT;

    break ;
  case 's':
    cursori = cursori + 1 ;
    i= cursori;
    j= cursorj;
    grid[i][j].status = HIGHLIGHT;

    break ;
  case 'o':
    i= cursori;
    j= cursorj;
    grid[i][j].status = BLOCKED ;
    break ;
  case 'c':
    i = cursori;
    j = cursorj;
    if( (grid[i][j].status = HIGHLIGHT) ) // warning: suggest parentheses around assignment used as truth value [-Wparentheses]

    {
      grid[i][j].status =UNVISITED;
    }
  break;
  //case 'g':
    //IGOAL = cursori;
    //JGOAL = cursorj;
    //grid[IGOAL][JGOAL].status = GOAL;
    //break;
  case 'l':
    findcheapest();
    walk();
    break;
  }
  display() ;
  // also process cursor keys in here
}

int main(int argc, char **argv)
{
  // int i, j ; // GB - not used
  Initialize() ;
  glutInit(&argc, argv) ;
  glutInitDisplayMode(GLUT_SINGLE) ;
  glutInitWindowSize(GSIZE *32, GSIZE * 32);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Pathfinding") ;
  glutDisplayFunc(display) ;
  glutKeyboardFunc(keyboard);
  glutReshapeFunc (reshape);
  glutMainLoop();
}

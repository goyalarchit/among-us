#include"maze.h"
#include<time.h>
#include<fstream>
#include <algorithm>

int cell::get_x()
{
    return x;
}

int cell::get_y()
{
    return y;
}


void cell::print(){
    std::cout<<"\n x: "<<x <<", y: "<<y;
    std::cout<<"\t Visited : "<<visited;
    std::cout<<"\t walls :";
    for(auto wall:walls)
        std::cout<<"\t "<<wall;
}

void cell::removeWall(char w)
{
    switch(w)
    {
        case 't':   walls[0] = false;
                    break;
        case 'r':   walls[1] = false;
                    break;
        case 'b':   walls[2] = false;
                    break;
        case 'l':   walls[3] = false;
                    break;
    }
}

cell::cell(int x,int y)
{
    this->x=x;
    this->y=y;
    for(int i = 0 ;i<4;i++)
        walls[i] = true;
    this->visited=false;
}


std::string cell::show(int w)
{
    int x_ = x*w;
    int y_ = y*w;
    std::string s;
    std::vector<std::pair<int,int>> coordinates;
    coordinates.push_back(std::make_pair(x_,y_));
    coordinates.push_back(std::make_pair(x_+w,y_));
    coordinates.push_back(std::make_pair(x_+w,y_+w));
    coordinates.push_back(std::make_pair(x_,y_+w));
    for(int i=0;i<4;i++)
    {
        if(walls[i]==true)
        {
            s= s+ "\n<line x1=\""+ std::to_string(coordinates[i%4].first) 
                    + "\" y1=\"" + std::to_string(coordinates[i%4].second)
                    + "\" x2=\"" + std::to_string(coordinates[(i+1)%4].first)
                    + "\" y2=\"" + std::to_string(coordinates[(i+1)%4].second)
                    + "\" stroke=\"black\" stroke-width=\"3\" />"; 
        }
    }
    return s;
}


std::vector<float> cell:: get_wall_coordinates(int w)
{
    int x_ = x*w;
    int y_ = y*w;
    std::vector<float> coords;
    std::vector<std::pair<int,int>> coordinates;
    coordinates.push_back(std::make_pair(x_,y_));
    coordinates.push_back(std::make_pair(x_+w,y_));
    coordinates.push_back(std::make_pair(x_+w,y_+w));
    coordinates.push_back(std::make_pair(x_,y_+w));
    for(int i=0;i<4;i++)
    {
        if(walls[i]==true)
        {
            coords.push_back((float)coordinates[i%4].first); 
            coords.push_back((float)coordinates[i%4].second); 
            coords.push_back(0);
            coords.push_back((float)coordinates[(i+1)%4].first); 
            coords.push_back((float)coordinates[(i+1)%4].second); 
            coords.push_back(0);
        }
    }
    return coords;
}


bool cell::isWalled(char w)
{
    switch(w)
    {
        case 't':   return walls[0];
                    break;
        case 'r':   return walls[1];
                    break;
        case 'b':   return walls[2];
                    break;
        case 'l':   return walls[3];
                    break;
    }
}


maze:: maze(int r,int c){
    this->rows=r;
    this->cols=c;
    for(int j = 0;j<rows;j++)
    {
        for(int i = 0;i<cols;i++)
            {
                cell* c = new cell(i,j);
                grid.push_back(c);
            }
    }
    std::cout<<"Grid Initialised"<<std::endl;
}


int maze::index (int i, int j)
{
    if (i<0|| j<0 || i> cols-1 || j > rows-1 )
        return -1;
    return (i+j*cols);
}

cell* maze::checkNeighbours (cell* c){
    if (c == nullptr)
        return nullptr;
    int x_ = c->get_x();
    int y_ = c->get_y();
    int i[] = {x_-1,x_+1}, j[] = {y_-1,y_+1};
    std::vector<int> neighbour_index;
    std::vector<cell*> neighbours;
    for(auto y : j)
        neighbour_index.push_back(index(x_,y));
    for(auto x : i)
        neighbour_index.push_back(index(x,y_));
    for(auto idx : neighbour_index)
    {
        if ((idx >= 0) && (grid[idx]->visited == false)) 
            neighbours.push_back(grid[idx]);
    }
    // for(auto cel : neighbours)
    //     cel->print();
    if (neighbours.size() > 0){
        return neighbours[rand()%neighbours.size()];
    }
    return nullptr;
}

void maze::gen_maze(cell *cur){
    srand(time(0));
    cell * current = grid[0];
    cell * next;
    std::vector<cell *> stack;
    while(current!=nullptr)
    {
        current->visited = true;
        next = checkNeighbours(current) ;
        if(next){
            stack.push_back(current);
            removeWalls(current,next);
            // current->print();
            current = next;
        }
        else if(!stack.empty()){
            // current->print();
            current = stack.back();
            stack.pop_back();
        }
        else
            {
                // current->print();
                current = nullptr;
            }
    }
    int cnt = 0;
    for(auto c: grid )
        if (c->visited)
            cnt++;

    std::cout<<"Generated Maze visited: "<<cnt<<std::endl;

}

void maze :: removeWalls(cell * c1, cell * c2)
{
    // We are assuming top Left corner to be (0,0) and Bottom Right to be (r,c)
    // To change origin to bottom, left and draw the grid till top right, change conditions with  "Y"
    // _______
    // |c2|c1|
    // '''''''
    if(c1->get_x()==c2->get_x()+1)
    {
        c1->removeWall('l');
        c2->removeWall('r');
    }
    // ___________
    // |_c1_|_c2_|
    else if(c1->get_x()+1==c2->get_x())
    {
        c1->removeWall('r');
        c2->removeWall('l');
    }
    // ______
    // |_c1_|
    // |_c2_|
    else if(c1->get_y()+1==c2->get_y())
    {
        c1->removeWall('b');
        c2->removeWall('t');
    }
    // ______
    // |_c2_|
    // |_c1_|
    else if(c1->get_y()==c2->get_y()+1)
    {
        c1->removeWall('t');
        c2->removeWall('b');
    }
}

void maze::show(int w)
{
    std::string s = "<!DOCTYPE html>\n <title>SVG Line</title>\n <body>\n <h1>Sample SVG Line Image</h1>\n <svg width= \"800 \" height= \"800 \">\n <g>\n ";
    for(auto c: grid)
    {
        s = s+ c->show(w);
    }
    s= s +  "</g>\n" +
            "</svg>\n" +
            "</body>\n" +
            "</html>\n";
    std::ofstream out("mymaze.html");
    out << s;
    out.close();
}


void maze::get_coordinates(int w){
    std::vector<float> coords;
    std::vector<float> cell_coords;
    for(auto c: grid)
    {
        cell_coords = c->get_wall_coordinates(w);
        coords.insert(coords.end(),cell_coords.begin(),cell_coords.end());
    }
    // for(int i=0;i<coords.size();i++)
    // {
    //     if(i%3==0)
    //         std::cout<<std::endl;
    //     std::cout<<"\t"<<coords[i];
    // }
    coordinates = new float[coords.size()];
    std::copy(coords.begin(),coords.end(),coordinates);
    length_coords = coords.size();
}

void maze::init_draw(int w)
{
    get_coordinates(w);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    std::cout<<"\n VAO: "<<VAO <<"\t VBO: "<<VBO<<"\t Size of :"<<sizeof(coordinates);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, length_coords*sizeof(float), coordinates, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    
}

void maze::draw(Shader &shader){
    shader.Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, length_coords/3);

}

bool maze::detect_collision(int x,int y,char d)
{
    int id = index(x,y);
    return (grid[id]->isWalled(d));
}

int maze::distance_between(int x1,int y1,int x2,int y2)
{
    std::cout<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<std::endl;

    int idx1=index(x1,y1),idx2 = index(x2,y2);
    std::cout<<idx1<<"\t"<<idx2<<std::endl;
    if(idx1==-1 ||idx2 == -1)
        return INT_MAX;
    std::vector<int> dist;
    if(idx1==idx2)
        return 0;
    if(grid[idx1]->isWalled('t')==false)
    {
        dist.push_back(1+(distance_between(x1,y1-1,x2,y2)));
    }
    if(grid[idx1]->isWalled('b')==false)
    {
        dist.push_back(1+(distance_between(x1,y1+1,x2,y2)));
    }
    if(grid[idx1]->isWalled('r')==false)
    {
        dist.push_back(1+(distance_between(x1+1,y1,x2,y2)));
    }
    if(grid[idx1]->isWalled('l')==false)
    {
        dist.push_back(1+(distance_between(x1-1,y1,x2,y2)));
    }
    if(dist.size()>0)
        return(*min_element(dist.begin(),dist.end()));
    return 0;
}

// char maze::shortest_path(int x1,int y1,int x2,int y2)
// {
//     std::cout<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<std::endl;
//     std::vector<std::pair<int,char>> dist;
//     int idx1=index(x1,y1),idx2 = index(x2,y2);
//     if(idx1==-1 ||idx2 == -1)
//         return 'a';
//     std::cout<<idx1<<"\t"<<idx2<<std::endl;
//     if(idx1==idx2)
//         return 'a';
//     if(grid[idx1]->isWalled('t')==false)
//     {
//         dist.push_back(std::make_pair(1+(distance_between(x1,y1-1,x2,y2)),'t'));
//     }
//     if(grid[idx1]->isWalled('b')==false)
//     {
//         dist.push_back(std::make_pair(1+(distance_between(x1,y1+1,x2,y2)),'b'));
//     }
//     if(grid[idx1]->isWalled('l')==false)
//     {
//         dist.push_back(std::make_pair(1+(distance_between(x1-1,y1,x2,y2)),'l'));
//     }
//     if(grid[idx1]->isWalled('r')==false)
//     {
//         dist.push_back(std::make_pair(1+(distance_between(x1+1,y1,x2,y2)),'r'));
//     }
    
//     auto result = *std::min_element(dist.cbegin(), dist.cend(), [](const std::pair<int,char>& lhs, const std::pair<int,char>& rhs) {
//         return lhs.first < rhs.first;    
//     });
//     std::cout << result.first << " " << result.second  << std::endl; // 0 3
//     return (result.second);
// }

char maze::shortest_path(int x1,int y1,int x2,int y2)
{
    std::cout<<"\t"<<x1<<"\t"<<y1<<"\t"<<x2<<"\t"<<y2<<std::endl;
    std::vector<char> dir{'t','l','b','r'};
    int idx1=index(x1,y1),idx2 = index(x2,y2);
    int x=x2-x1,y=y2-y1;
    if(x>0 && (grid[idx1]->isWalled('r')==false))
        return 'r';
    else if(x<0 && (grid[idx1]->isWalled('l')==false))
        return 'l';
    else if(y>0 && (grid[idx1]->isWalled('b')==false))
        return 'b';
    else if(y<0 && (grid[idx1]->isWalled('t')==false))
        return 't';
    else 
    {
        for(auto d:dir)
            if(grid[idx1]->isWalled(d)==false)
                return d;
    }
    return 't';

}
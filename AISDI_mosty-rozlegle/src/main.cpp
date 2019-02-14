#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include <ctime>
#include <chrono>
#include <algorithm>

class Graph
{
    std::vector<std::vector<unsigned> > vertices;
    unsigned size;

    bool bfs(unsigned firstRemoved, unsigned secondRemoved)
    {

        std::vector<bool> visited;
        visited.resize(size);

        for(unsigned i = 0; i < visited.size(); ++i)
            visited[i] = false;

        visited[firstRemoved] = true;
        visited[secondRemoved] = true;

        unsigned current; //przy pierwszym przejsciu jest to element startowy, ktory musimy wczesniej znalezc

        for(unsigned i = 0; i < vertices.size(); ++i)
            if(visited[i] == false)
            {
                current = i;
                break;
            }

        std::queue<unsigned> queue;

        queue.push(current);

        while(!queue.empty())
        {
            current = queue.front();

            if(visited[current] == false)
            {
                for(unsigned i = 0; i < vertices[current].size(); ++i)
                    if(visited[vertices[current][i]] == false)
                        queue.push(vertices[current][i]);
                visited[current] = true;
            }

            queue.pop();
        }

        for(unsigned i = 0; i < visited.size(); ++i)
            if(visited[i] == false)
                return true; // niespojny - krawedz jest mostem

        return false; // spojny
    }

public:

    Graph()
    {}

    ~Graph()
    {}

    std::vector<std::pair<unsigned, unsigned> > find()
    {
        std::vector<std::pair<unsigned, unsigned> > result;

        for(unsigned i = 0; i < vertices.size(); ++i)                //sortuje sasiadow kazdego wierzcholka, aby
            std::sort(vertices[i].begin(), vertices[i].end());  //wypisywac potem po kolei jednokrotnie mosty

        unsigned firstRemoved, secondRemoved;

        std::vector<std::vector<unsigned> > backup = vertices;

        for(unsigned i = 0; i < vertices.size(); ++i)
        {
            for(unsigned j = 0; j < vertices[i].size(); ++j)
            {
                if(vertices[i][j] > i) //jesli np 01 jest mostem, to dzieki temu wypiszemy tylko 01, a 10 juz nie
                {
                    firstRemoved = i;
                    secondRemoved = vertices[i][j];
                    vertices[firstRemoved].clear();
                    vertices[secondRemoved].clear();
                    if(bfs(firstRemoved, secondRemoved) == true) //krawedz jest mostem
                            result.push_back(std::make_pair(firstRemoved, secondRemoved));
                    vertices = backup;
                }
            }
        }

        return result;

    }

    void resize(unsigned size)
    {
        this->size = size;
        vertices.resize(size);
    }

    void add(unsigned a, unsigned b)
    {
        vertices[a].push_back(b);
        vertices[b].push_back(a);
    }
};

void read(Graph& graph, char *nameOfFile)
{
    std::fstream file;
    unsigned size = 0;

    file.open(nameOfFile, std::ios::in);
    file >> size;

    graph.resize(size);

    unsigned a, b;
    while(file >> a >> b)
    {
        graph.add(a, b);
    }

    file.close();

}

void write(std::vector<std::pair<unsigned, unsigned> > &vec)
{
    for(unsigned i = 0; i < vec.size(); ++i)
        std::cout << vec[i].first << " " << vec[i].second << std::endl;
}

int main(int argc, char **argv)
{
    char *nameOfFile;

    if(argc != 2)
    {
        std::cout << "Add files!" <<std::endl;
        return 0;
    }

    nameOfFile = argv[1];

    Graph graph;

    read(graph, nameOfFile);

    std::vector<std::pair<unsigned, unsigned> > bridges;

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    bridges = graph.find();

    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Szukanie mostow rozleglych: " << elapsed_seconds.count() << "s" << std::endl;

    write(bridges);

    return 0;
}

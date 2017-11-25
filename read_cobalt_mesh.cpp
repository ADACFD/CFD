#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class NODE {

public:
    NODE() {}
    ~NODE() {}
    double X, Y, Z;
};

class FACE {

public:
    FACE() {}
    ~FACE() {}
    int Cell_Left, Cell_Right;
    int Vertex[4];
};

class BC {

public:
    BC() {}
    ~BC() {}
    int Id;
    string Name;
};

vector<string> convert_string_to_vector(string);

int str_to_int(string);


int main()
{
    ifstream meshFile, meshBcFile ;
    string line;
    int nDimensions, nZones, nBoundaryPatches;
    int nVertices, nFaces, nCells, nVerticesPerFaceMax, nFacesPerCellMax;
    int nFacesPerCell, nNodesPerFace;
    int checkint=0, bc_count=0;
    string mesh_file_name, bc_file_name;
    vector<string> line_vector;
    int nQuad=0, nTri=0;

    mesh_file_name = "cobalt";
    bc_file_name = "cobalt.bc";

    meshFile.open(mesh_file_name, ios::in);

    if (!meshFile.is_open())
    {
        perror(("error while opening file " + mesh_file_name).c_str());
        return 0;
    }
    if (!(meshFile >> nDimensions >> nZones >> nBoundaryPatches) )
        cout << "Error in reading file" + mesh_file_name;
    else
    {
        cout << nDimensions << "\t" << nZones << "\t"  << nBoundaryPatches << "\n" ;
    }
    if (!(meshFile >> nVertices >> nFaces >> nCells >> nVerticesPerFaceMax >> nFacesPerCellMax) )
        cout << "Error in reading file" + mesh_file_name;
    else
    {
        cout << nVertices << "\t" << nFaces << "\t" << nCells << "\t" << nVerticesPerFaceMax << "\t" << nFacesPerCellMax << "\n";
    }

    NODE *nodes = NULL;
    nodes = new NODE[nVertices];
    for (int i=0 ; i < nVertices ; i++)
    {
        if(!(meshFile >> nodes[i].X >> nodes[i].Y >> nodes[i].Z))
            cout << "Error in reading file" + mesh_file_name;

        //cout << nodes[i].X << "\t"  << nodes[i].Y << "\t"  << nodes[i].Z << "\n" ;
    }
    delete[] nodes;
    FACE *faces = NULL;
    faces = new FACE[nFaces];
    for (int i=0 ; i < nFaces ; i++)
    {
        meshFile >> nNodesPerFace;
        if nNodesPerFace==3
            nTri+=1;
        else if nNodesPerFace==4
            nQuad+=1;
        else
            cout << "New Face types found --> Faces have to be either Triangular/Quadrilateral";
        for(int j=0 ; j< nFacesPerCell ; j++)
        {
            meshFile >> faces[i].Vertex[j];
        }

        meshFile >> faces[i].Cell_Left >> faces[i].Cell_Right;
        //cout << faces[i].Vertex[0] << "\t"  << faces[i].Vertex[1] << "\t"  << faces[i].Vertex[2] << "\t"  << faces[i].Vertex[3] << "\t"  << faces[i].Cell_Left << "\t" << faces[i].Cell_Right << "\n" ;

    }
    delete[] faces;
    if (meshFile.bad())
        perror(("error while reading file " + mesh_file_name).c_str());

    meshFile.close();
    
    

    meshBcFile.open(bc_file_name, ios::in);

    if (!meshBcFile.is_open())
    {
        perror(("error while opening file " + bc_file_name).c_str());
        return 0;
    }
    BC *bcs = NULL;
    bcs = new BC[2];

    while(getline(meshBcFile, line))
    {
        line_vector = convert_string_to_vector(line);
        if(line_vector.empty())
            continue;
        checkint = str_to_int( line_vector[0]);
        if(checkint == 0)
            continue;
        else
        {
           bcs[bc_count].Id = checkint;
           getline(meshBcFile, line);
           bcs[bc_count].Name = line;
           cout << bcs[bc_count].Id << "\t" << bcs[bc_count].Name << "\n";
           cout << bc_count << "\n";
           bc_count++;
        }

    }

    meshBcFile.close();


    return 0;
}

vector<string> convert_string_to_vector(string line)
{
    stringstream ss;
    vector<string> stringVector;
    stringVector.clear();
    ss << line;

    getline(ss, line);
    ss.clear();
    size_t prev = 0, pos;
    //cout << line << "\n";
    while ((pos = line.find_first_of(" ()", prev)) != string::npos)
    {
        //cout << prev << "\t" << pos << "\n";
        if (pos > prev)
        {
            //cout << line.substr(prev,pos-prev) << "\t";
            stringVector.push_back(line.substr(prev, pos-prev));
        }
        prev = pos+1;
    }
    //cout << "\n";
    if (prev < line.length())
        stringVector.push_back(line.substr(prev, string::npos));

/*
    for(int j=0; j < stringVector.size(); j++)
    {
        cout << stringVector[j] << "\t";
    }
    cout << "\n";
*/
    return stringVector;
}

int str_to_int(string s)
{
    //cout << s << "\t";
    istringstream temps(s);
    int temp;
    temps >> temp;
    //cout << temp << "\n";
    return temp;
}

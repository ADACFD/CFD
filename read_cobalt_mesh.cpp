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
    int lCell, rCell;
    int Vertex[4];
    int Type;
};

class CELL {
      
public:
    CELL() {}
    ~CELL() {}
    int Vertex[8];
    int Type;
    int Face[6];
};             

class BC {

public:
    BC() {}
    ~BC() {}
    int Id, nSurfFaces;
    string Name, prescBC;
};

vector<string> convert_string_to_vector(string);

int str_to_int(string);


int main()
{
    //ifstream meshFile, meshBcFile ;
    string line;
    int nDimensions, nZones, nBoundaryPatches;
    int nVertices, nFaces, nCells, nVerticesPerFaceMax, nFacesPerCellMax;
    int nFacesPerCell;
    int checkint=0, bc_count=0;
    string mesh_file_name, bc_file_name;
    vector<string> line_vector;
    int nQuad=0, nTri=0;
    int i=0, j=0;
    int lcell, rcell;
    int tri_face, quad_face, null_face, tmpf;
    int nTetra=0, nPyra=0, nPrism=0, nHexa=0;

    mesh_file_name = "cobalt";
    bc_file_name = "cobalt.bc";

    ifstream meshFile(mesh_file_name.c_str());
    ifstream meshBcFile(bc_file_name.c_str());

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
    for ( i=0 ; i < nVertices ; i++)
    {
        if(!(meshFile >> nodes[i].X >> nodes[i].Y >> nodes[i].Z))
            cout << "Error in reading file" + mesh_file_name;
    }
    cout << nodes[0].X << "\t"  << nodes[0].Y << "\t"  << nodes[0].Z << "\n" ;
    cout << nodes[nVertices-1].X << "\t"  << nodes[nVertices-1].Y << "\t"  << nodes[nVertices-1].Z << "\n" ;
    //delete[] nodes;
    FACE *faces = NULL;
    faces = new FACE[nFaces];
    for (int i=0 ; i < nFaces ; i++)
    {
        meshFile >> faces[i].Type;
        if (faces[i].Type==3) nTri+=1;
        else if (faces[i].Type==4) nQuad+=1;
        else cout << "New Face types found --> Faces have to be either Triangular/Quadrilateral";
        
        for(int j=0 ; j< faces[i].Type ; j++)
        {
            meshFile >> faces[i].Vertex[j];
        }

        meshFile >> faces[i].lCell >> faces[i].rCell;
        if (i%10000==0) cout << i << "\t" << faces[i].Vertex[0] << "\t"  << faces[i].Vertex[1] << "\t"  << faces[i].Vertex[2] << "\t"  << faces[i].Vertex[3] << "\t"  << faces[i].lCell << "\t" << faces[i].rCell << "\n" ;
    }
    cout << faces[0].Type << "\t" << faces[0].lCell << "\t" << faces[0].rCell << "\n" ;
    cout << faces[nFaces-1].Type << "\t" << faces[nFaces-1].lCell << "\t" << faces[nFaces-1].rCell << "\n" ;
    cout << "Number of Triangles : " << "\t" << nTri << "\n" ;
    cout << "Number of Quadrilaterals : " << "\t" << nQuad << "\n" ;
    
    if (meshFile.bad())
        perror(("error while reading file " + mesh_file_name).c_str());

    meshFile.close();
    

    //meshBcFile.open(bc_file_name, ios::in);

    if (!meshBcFile.is_open())
    {
        perror(("error while opening file " + bc_file_name).c_str());
        return 0;
    }
    BC *bcs = NULL;
    bcs = new BC[nBoundaryPatches+1];

    /*while(getline(meshBcFile, line))
    {
        cout << "line : " << "\t" << line << "\n";         
        line_vector = convert_string_to_vector(line);
        if(line_vector.empty())
            continue;
        checkint = str_to_int( line_vector[0]);
        cout << "checkint : " << "\t" << checkint << "\n";
        if(checkint == 0)
            continue;
        else
        {
           bcs[bc_count].Id = checkint;
           getline(meshBcFile, line);
           bcs[bc_count].Name = line;
           cout << bc_count << "\t" << bcs[bc_count].Id << "\t" << bcs[bc_count].Name << "\n";
           bcs[bc_count].nSurfFaces=0;
           bc_count++;
        }
    }*/
    
    getline(meshBcFile, line);
    getline(meshBcFile, line);
    getline(meshBcFile, line);
    getline(meshBcFile, line);
    
    for( i=0; i< nBoundaryPatches; i++)
    {
     getline(meshBcFile, line);
     line_vector = convert_string_to_vector(line);
     checkint = str_to_int( line_vector[0]);
     bcs[bc_count].Id = checkint;
     getline(meshBcFile, line);
     bcs[bc_count].Name = line;
     getline(meshBcFile, line);
     bcs[bc_count].prescBC = line;
     bcs[bc_count].nSurfFaces=0;
     bc_count++;
     getline(meshBcFile, line);
     getline(meshBcFile, line);
     getline(meshBcFile, line);
    }

    meshBcFile.close();
    
    for(i=0; i<bc_count; i++) cout << i << "\t" << bcs[i].Id << "\t" << bcs[i].Name << "\t" << bcs[i].nSurfFaces << "\t" << "\t" << bcs[i].prescBC << "\n";

    // Counting number of faces in each surface bc
    
    for (i=0 ; i<nFaces; i++)
    {
        if (faces[i].rCell<0)
           if (abs(faces[i].rCell) <= nBoundaryPatches) bcs[abs(faces[i].rCell)-1].nSurfFaces+=1;
           else cout << "Mesh and bc file mismatch" << "\t" << i << "\t" << faces[i].rCell << "\n" ;
    }
    
    for(i=0; i<bc_count; i++) cout << i+1 << "\t" << bcs[i].Id << "\t" << bcs[i].Name << "\t" << bcs[i].nSurfFaces << "\t" << "\t" << bcs[i].prescBC << "\n";

    // building cell data base
    
    // face numbers forming a particular cell
    
    CELL *cells = NULL;
    cells = new CELL[nCells];

    for ( i=0 ; i < nFaces ; i++)
    {
     lcell=faces[i].lCell - 1;
     for (j=0 ; j < 6 ; j++)
     {
      if (cells[lcell].Face[j]==0)
      {
         cells[lcell].Face[j]=i+1;
         cells[lcell].Type+=1;
         break;
      }
     }
     rcell=faces[i].rCell - 1;
     for (j=0 ; j < 6 ; j++)
     {
      if (rcell >= 0 && cells[rcell].Face[j]==0)
      {
         cells[rcell].Face[j]=i+1;
         cells[rcell].Type+=1;
         break;
      }
     }
    } 

    i=0;
    cout << "cell info :" << "\t" << cells[i].Face[0] << "\t" << cells[i].Face[1] << "\t" << cells[i].Face[2] << "\t" << cells[i].Face[3] << "\t" << cells[i].Face[4] << "\t" << cells[i].Face[5] << "\t" << cells[42543].Face[6] << "\n";
    
    for ( i=0 ; i < nCells ; i++)
    {
     if (cells[i].Type < 4 || cells[i].Type > 6)
     {
      cout << "Cell type computation error" << "\n" ;
     }
     
     tri_face=0;
     quad_face=0;
     null_face=0;
     
     for (j=0 ; j < 6 ; j++)
     {
      tmpf=cells[i].Face[j];
      if (i == 42450) cout << tmpf << "\t" << faces[tmpf-1].Type << "\n" ;
      if (tmpf == 0) null_face += 1;
      else if (faces[tmpf-1].Type == 3) tri_face += 1;
      else if (faces[tmpf-1].Type == 4) quad_face += 1;     
      else cout << " Error in finding the face type" << "\t" << i << "\n";
     }
     if (i==42450) cout << tri_face << quad_face << null_face << "\n";
     if (cells[i].Type == 4)
     {
      if (tri_face == 4 && quad_face == 0 && null_face == 2)
      {
       nTetra+=1;
      }
     }
     else if (cells[i].Type == 5)
     {
      if (tri_face == 4 && quad_face == 1 && null_face == 1)
      {
       nPyra+=1;
      }
      else if (tri_face == 2 && quad_face == 3 && null_face == 1)
      {
       nPrism+=1;
       cells[i].Type = 6;    //manual update to 6
      }
     }
     else if (cells[i].Type == 6)
     {
      if (tri_face == 0 && quad_face == 6 && null_face == 0)
      {
       nHexa+=1;
      }
     }
    }
    
    cout << "Number of Tetrahedra : " << "\t" << nTetra << "\n" ;
    cout << "Number of Pyramid : " << "\t" << nPyra << "\n" ;
    cout << "Number of Prisms : " << "\t" << nPrism << "\n" ;
    cout << "Number of Hexahedra : " << "\t" << nHexa << "\n" ;
            
    delete[] faces;
    
    cin.get();
    //return 0;
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

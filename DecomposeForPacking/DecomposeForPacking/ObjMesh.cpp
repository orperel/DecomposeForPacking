#include "ObjMesh.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

typedef unsigned char byte;

static int version;
static int depth, height, width;
static int size;
static byte *voxels = 0;
static float tx, ty, tz;
static float scale;

const std::wstring BINVOX_FILENAME = L"binvox.exe";
const std::wstring BINVOX_EXTENSION = L".binvox";
const int OBJECT_DIMENSION = 1;


ObjMesh::ObjMesh(std::wstring objPath, int dimension)
{
	// Build object .binvox file path
	std::wstring objBinvoxPath = objPath.substr(0, objPath.rfind(L"."));
	objBinvoxPath += BINVOX_EXTENSION;

	::DeleteFile(objBinvoxPath.c_str());

	createVortexFile(objPath, dimension);

	readBinvox(objBinvoxPath);
}

void ObjMesh::createVortexFile(std::wstring objPath, int dimension) {
	// Build full path to binvox
	TCHAR currentProcssPath[MAX_PATH];
	::GetModuleFileName(NULL, currentProcssPath, MAX_PATH);

	std::wstring fullPath(currentProcssPath);

	fullPath += L"\\..\\" + BINVOX_FILENAME;

	// Run binvox
	STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	std::wstring dimensionStr = std::to_wstring(dimension);
	std::wstring binvoxParams = fullPath + L" -d " + dimensionStr + L" " + objPath;

	// Call binvox process
	if( !::CreateProcess( NULL, &binvoxParams[0], NULL, NULL, false, 0, NULL, NULL, &si, &pi ) ) {
		return;
	}
	
    // Wait until binvox process exits.
	DWORD waitForBinvoxMilliseconds = 1000*60;
    if (WAIT_TIMEOUT == ::WaitForSingleObject( pi.hProcess, waitForBinvoxMilliseconds )) {
		return;
	}

    // Close process and thread handles. 
    ::CloseHandle( pi.hProcess );
    ::CloseHandle( pi.hThread );
}

ObjMesh::~ObjMesh(void)
{
}

int ObjMesh::getIndex(int x, int y, int z)
{
	int wxh = width * height;
	int index = x * wxh + z * width + y;  // wxh = width * height = d * d
	return index;
}

bool ObjMesh::isVoxelExist(int x, int y, int z)
{
	int index = getIndex(x, y, z);
	return m_voxels[index] != '\0';
}

void ObjMesh::readBinvox(std::wstring filespec)
{

  ifstream *input = new ifstream(filespec.c_str(), ios::in | ios::binary);

  //
  // read header
  //
  string line;
  *input >> line;  // #binvox
  if (line.compare("#binvox") != 0) {
    cout << "Error: first line reads [" << line << "] instead of [#binvox]" << endl;
    delete input;
    return;
  }
  *input >> version;
  cout << "reading binvox version " << version << endl;

  depth = -1;
  int done = 0;
  while(input->good() && !done) {
    *input >> line;
    if (line.compare("data") == 0) done = 1;
    else if (line.compare("dim") == 0) {
      *input >> depth >> height >> width;
    }
    else if (line.compare("translate") == 0) {
      *input >> tx >> ty >> tz;
    }
    else if (line.compare("scale") == 0) {
      *input >> scale;
    }
    else {
      cout << "  unrecognized keyword [" << line << "], skipping" << endl;
      char c;
      do {  // skip until end of line
        c = input->get();
      } while(input->good() && (c != '\n'));

    }
  }
  if (!done) {
    cout << "  error reading header" << endl;
    return;
  }
  if (depth == -1) {
    cout << "  missing dimensions in header" << endl;
    return;
  }

  size = width * height * depth;
  voxels = new byte[size];
  if (!voxels) {
    cout << "  error allocating memory" << endl;
    return;
  }

  //
  // read voxel data
  //
  byte value;
  byte count;
  int index = 0;
  int end_index = 0;
  int nr_voxels = 0;
  
  input->unsetf(ios::skipws);  // need to read every byte now (!)
  *input >> value;  // read the linefeed char

  while((end_index < size) && input->good()) {
    *input >> value >> count;

    if (input->good()) {
      end_index = index + count;
      if (end_index > size) return;
      for(int i=index; i < end_index; i++) {
		  voxels[i] = value;
		  m_voxels.push_back(value);
	  }
      
      if (value) nr_voxels += count;
      index = end_index;
    }  // if file still ok
    
  }  // while

  input->close();
  cout << "  read " << nr_voxels << " voxels" << endl;

  m_width = width;
  m_height = height;
  m_depth = depth;
}

int ObjMesh::getWidth()
{
	return m_width;
}

int ObjMesh::getHeight()
{
	return m_height;
}

int ObjMesh::getDepth()
{
	return m_depth;
}

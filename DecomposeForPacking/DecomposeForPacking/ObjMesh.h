#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include <memory>

class ObjMesh
{
public:
	ObjMesh(std::wstring path ,int dimension = 10);
	virtual ~ObjMesh(void);
	bool isVoxelExist(int x, int y, int z);

	int getWidth();
	int getHeight();
	int getDepth();

private:
	void readBinvox(std::wstring path);
	int getIndex(int x, int y, int z);
	void createVortexFile(std::wstring path, int dimension);

	std::vector<byte> m_voxels;
	int m_width;
	int m_height;
	int m_depth;
};

typedef std::shared_ptr<ObjMesh> ObjMeshPtr;

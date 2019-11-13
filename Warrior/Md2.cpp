#include "h\main.h"
#include "h\Md2.h"


CLoadMD2::CLoadMD2()
{
	// Here we initialize our structures to 0
	memset(&m_Header, 0, sizeof(tMd2Header));

	// Set the pointers to null
	m_pSkins=NULL;
	m_pTexCoords=NULL;
	m_pTriangles=NULL;
	m_pFrames=NULL;
}

bool CLoadMD2::ImportMD2(t3DModelMd2 *pModel, char *strFileName, char *strTexture)
{
	char strMessage[255] = {0};

	// Open the MD2 file in binary
	m_FilePointer = fopen(strFileName, "rb");

	// Make sure we have a valid file pointer (we found the file)
	if(!m_FilePointer) 
	{
		// Display an error message and don't load anything if no file was found
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	
	// Read the header data and store it in our m_Header member variable
	fread(&m_Header, 1, sizeof(tMd2Header), m_FilePointer);

	// Make sure the version is this crazy number '8' or else it's a bad egg
	if(m_Header.version != 8)
	{
		// Display an error message for bad file format, then stop loading
		sprintf(strMessage, "Invalid file format (Version not 8): %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// Read in the model and animation data
	ReadMD2Data();
	
	// Here we pass in our model structure to it can store the read Quake data
	// in our own model and object structure data
	ConvertDataStructures(pModel);

	// If there is a valid texture name passed in, we want to set the texture data
	if(strTexture)
	{
		// Create a local material info structure
		tMaterialInfo texture;

		// Copy the name of the file into our texture file name variable
		strcpy(texture.strFile, strTexture);

		// Since there is only one texture for a .Md2 file, the ID is always 0
		texture.texureId = 0;

		// The tile or scale for the UV's is 1 to 1 (but Quake saves off a 0-256 ratio)
		texture.uTile = texture.uTile = 1;

		// We only have 1 material for a model
		pModel->numOfMaterials = 1;

		// Add the local material info to our model's material list
		pModel->pMaterials.push_back(texture);
	}

	// Clean up after everything
	CleanUp();

	// Return a success
	return true;
}


///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function reads in all of the model's data, except the animation frames
/////
///////////////////////////////// READ MD2 DATA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void CLoadMD2::ReadMD2Data()
{
	// Create a larger buffer for the frames of animation (not fully used yet)
	unsigned char buffer[MD2_MAX_FRAMESIZE];

	// Here we allocate all of our memory from the header's information
	m_pSkins     = new tMd2Skin [m_Header.numSkins];
	m_pTexCoords = new tMd2TexCoord [m_Header.numTexCoords];
	m_pTriangles = new tMd2Face [m_Header.numTriangles];
	m_pFrames    = new tMd2Frame [m_Header.numFrames];

	// Next, we start reading in the data by seeking to our skin names offset
	fseek(m_FilePointer, m_Header.offsetSkins, SEEK_SET);
	
	// Depending on the skin count, we read in each skin for this model
	fread(m_pSkins, sizeof(tMd2Skin), m_Header.numSkins, m_FilePointer);
	
	// Move the file pointer to the position in the file for texture coordinates
	fseek(m_FilePointer, m_Header.offsetTexCoords, SEEK_SET);
	
	// Read in all the texture coordinates in one fell swoop
	fread(m_pTexCoords, sizeof(tMd2TexCoord), m_Header.numTexCoords, m_FilePointer);

	// Move the file pointer to the triangles/face data offset
	fseek(m_FilePointer, m_Header.offsetTriangles, SEEK_SET);
	
	// Read in the face data for each triangle (vertex and texCoord indices)
	fread(m_pTriangles, sizeof(tMd2Face), m_Header.numTriangles, m_FilePointer);
			
	// Move the file pointer to the vertices (frames)
	fseek(m_FilePointer, m_Header.offsetFrames, SEEK_SET);


	for (int i=0; i < m_Header.numFrames; i++)
	{
		// Assign our alias frame to our buffer memory
		tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;

		// Allocate the memory for the first frame of animation's vertices
		m_pFrames[i].pVertices = new tMd2Triangle [m_Header.numVertices];

		// Read in the first frame of animation
		fread(pFrame, 1, m_Header.frameSize, m_FilePointer);

		// Copy the name of the animation to our frames array
		strcpy(m_pFrames[i].strName, pFrame->name);
			
		// Store off a vertex array pointer to cut down large lines of code
		tMd2Triangle *pVertices = m_pFrames[i].pVertices;

		// Go through all of the number of vertices and assign the scale and translations.
		// Store the vertices in our current frame's vertex list array, while swapping Y and Z.
		// Notice we also negate the Z axis as well to make the swap correctly.
		for (int j=0; j < m_Header.numVertices; j++)
		{
			pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
			pVertices[j].vertex[2] = -1 * (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]);
			pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
		}
	}
}

void CLoadMD2::ParseAnimations(t3DModelMd2 *pModel)
{
	tAnimationInfo animation;
	string strLastName = "";
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		// Assign the name of this frame of animation to a string object
		string strName  = m_pFrames[i].strName;
		int frameNum = 0;
		
		// Go through and extract the frame numbers and erase them from the name
		for(unsigned int j = 0; j < strName.length(); j++)
		{
			// If the current index is a number and it's one of the last 2 characters of the name
			if( isdigit(strName[j]) && j >= strName.length() - 2)
			{
				// Use a C function to convert the character to a integer.
				// Notice we use the address to pass in the current character and on
				frameNum = atoi(&strName[j]);

				// Erase the frame number from the name so we extract the animation name
				strName.erase(j, strName.length() - j);
				break;
			}
		}

		// Check if this animation name is not the same as the last frame,
		// or if we are on the last frame of animation for this model
		if(strName != strLastName || i == pModel->numOfObjects - 1)
		{
			// If this animation frame is NOT the first frame
			if(strLastName != "")
			{
				// Copy the last animation name into our new animation's name
				strcpy(animation.strName, strLastName.c_str());

				// Set the last frame of this animation to i
				animation.endFrame = i;

				// Add the animation to our list and reset the animation object for next time
				pModel->pAnimations.push_back(animation);
				memset(&animation, 0, sizeof(tAnimationInfo));

				// Increase the number of animations for this model
				pModel->numOfAnimations++;
			}

			// Set the starting frame number to the current frame number we just found,
			// minus 1 (since 0 is the first frame) and add 'i'.
			animation.startFrame = frameNum - 1 + i;
		}

		// Store the current animation name in the strLastName string to check it latter
		strLastName = strName;
	}
}

void CLoadMD2::ConvertDataStructures(t3DModelMd2 *pModel)
{
	int j = 0, i = 0;
	memset(pModel, 0, sizeof(t3DModelMd2));

	pModel->numOfObjects = m_Header.numFrames;

	ParseAnimations(pModel);

	for (i=0; i < pModel->numOfObjects; i++)
	{
		t3DObject currentFrame = {0};
		currentFrame.numOfVerts   = m_Header.numVertices;
		currentFrame.numTexVertex = m_Header.numTexCoords;
		currentFrame.numOfFaces   = m_Header.numTriangles;
		currentFrame.pVerts    = new CVector3 [currentFrame.numOfVerts];
		for (j=0; j < currentFrame.numOfVerts; j++)
		{
			currentFrame.pVerts[j].x = m_pFrames[i].pVertices[j].vertex[0];
			currentFrame.pVerts[j].y = m_pFrames[i].pVertices[j].vertex[1];
			currentFrame.pVerts[j].z = m_pFrames[i].pVertices[j].vertex[2];
		}
		delete m_pFrames[i].pVertices;

		if(i > 0)
		{
			// Here we add the current object (or frame) to our list object list
			pModel->pObject.push_back(currentFrame);
			continue;	// Go on to the next key frame
		}

		currentFrame.pTexVerts = new CVector2 [currentFrame.numTexVertex];
		currentFrame.pFaces    = new tFace [currentFrame.numOfFaces];

		for(j=0; j < currentFrame.numTexVertex; j++)
		{
			currentFrame.pTexVerts[j].x = m_pTexCoords[j].u / float(m_Header.skinWidth);
			currentFrame.pTexVerts[j].y = 1 - m_pTexCoords[j].v / float(m_Header.skinHeight);
		}

		// Go through all of the face data and assign it over to OUR structure
		for(j=0; j < currentFrame.numOfFaces; j++)
		{
			// Assign the vertex indices to our face data
			currentFrame.pFaces[j].vertIndex[0] = m_pTriangles[j].vertexIndices[0];
			currentFrame.pFaces[j].vertIndex[1] = m_pTriangles[j].vertexIndices[1];
			currentFrame.pFaces[j].vertIndex[2] = m_pTriangles[j].vertexIndices[2];

			// Assign the texture coord indices to our face data
			currentFrame.pFaces[j].coordIndex[0] = m_pTriangles[j].textureIndices[0];
			currentFrame.pFaces[j].coordIndex[1] = m_pTriangles[j].textureIndices[1];
			currentFrame.pFaces[j].coordIndex[2] = m_pTriangles[j].textureIndices[2];
		}

		// Here we add the current object (or frame) to our list object list
		pModel->pObject.push_back(currentFrame);
	}
}

void CLoadMD2::CleanUp()
{

	fclose(m_FilePointer);						

	if(m_pSkins)	 delete [] m_pSkins;		
	if(m_pTexCoords) delete m_pTexCoords;		
	if(m_pTriangles) delete m_pTriangles;		
	if(m_pFrames)	 delete m_pFrames;			
}


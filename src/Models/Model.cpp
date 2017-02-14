/**
 * The MIT License (MIT)
 * Copyright (c) 2016-2017 Tuetuopay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 **/

#include "Models/Model.h"

#include <vector>

// Use AssImp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace models {

Model::Model(const std::string &filename) : _vbo(nullptr) {
	// Load model through assimp
	Assimp::Importer importer;

	// This combined with the SortByPType flag will ensure we have only meshes
	// with triangles
	importer.SetPropertyInteger(
		AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT
	);
	const aiScene *scene = importer.ReadFile(
		filename.c_str(),
		aiProcess_JoinIdenticalVertices | aiProcess_Triangulate |
		aiProcess_GenSmoothNormals | aiProcess_SortByPType
	);

	if (scene) {
		// For now, we'll choose the first mesh in the scene
		const aiMesh *mesh = scene->mMeshes[0];

		// First we need to build a proper index buffer
		std::vector<GLuint> idxBuffer;
		idxBuffer.reserve(mesh->mNumFaces * 3);

		for (int i = 0; i < mesh->mNumFaces; i++) {
			const aiFace &face = mesh->mFaces[i];
			idxBuffer[(i * 3) + 0] = face.mIndices[0];
			idxBuffer[(i * 3) + 1] = face.mIndices[1];
			idxBuffer[(i * 3) + 2] = face.mIndices[2];
		}

		// Give null everywhere since we'll manually set buffers one by one
		_vbo = std::shared_ptr<GFX::GL::VBO>(new GFX::GL::VBO(
			nullptr, nullptr, nullptr, nullptr, 0, nullptr, 0
		));

		_vbo->setVertices(&mesh->mVertices[0].x, mesh->mNumVertices);
		if (mesh->HasTextureCoords(0))
			_vbo->setTextures(
				&mesh->mTextureCoords[0][0].x,
				mesh->mNumUVComponents[0], 3 - mesh->mNumUVComponents[0]
			);
		if (mesh->HasNormals())
			_vbo->setNormals(&mesh->mNormals[0].x);
		if (mesh->HasVertexColors(0))
			_vbo->setColors(&mesh->mColors[0][0].r);

		_vbo->setIndexes(&idxBuffer[0], idxBuffer.size());
	}
	else
		std::cerr << "Failed to load mesh '" << filename << "' : " <<
			importer.GetErrorString() << std::endl;
}

}


#pragma once

#include "vertex_buffer.h"
#include "index_buffer.h"

class Mesh
{
private:
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;

    uint32 numVertices;
    uint32 numIndices;

public:
    Mesh(Vertex *vertices, uint32 numVertices, uint32 *indices, uint32 numIndices);
    void bind();
    void unbind();
    VertexBuffer getVertexBuffer();
    IndexBuffer getIndexBuffer();
    uint32 getNumVertices();
    uint32 getNumIndices();
};
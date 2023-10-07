#include "mesh.h"

Mesh::Mesh(Vertex *vertices, uint32 numVertices, uint32 *indices, uint32 numIndices)
: vertexBuffer(vertices, numVertices), indexBuffer(indices, numIndices, sizeof(uint32)), numVertices(numVertices), numIndices(numIndices)
{

}

void Mesh::bind()
{
    vertexBuffer.bind();
    indexBuffer.bind();
}

void Mesh::unbind()
{
    vertexBuffer.unbind();
    indexBuffer.unbind();
}

VertexBuffer Mesh::getVertexBuffer()
{
    return vertexBuffer;
}

IndexBuffer Mesh::getIndexBuffer()
{
    return indexBuffer;
}

uint32 Mesh::getNumVertices()
{
    return numVertices;
}

uint32 Mesh::getNumIndices()
{
    return numIndices;
}
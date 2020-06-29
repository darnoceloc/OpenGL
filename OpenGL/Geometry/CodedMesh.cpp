#include "CodedMesh.h"

CodedMesh::CodedMesh(ShapeVertices::Shape shape)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Give the vertices to OpenGL
    glBufferData(GL_ARRAY_BUFFER, ShapeVertices::GetShapeByteSize(shape), ShapeVertices::GetShapeVertices(shape), GL_STATIC_DRAW);

    //Added code end
    //First attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glVertexAttribPointer
    (
        0,              //attribute 0. No reason 0, but must match layout in shader. 
        3,              //size
        GL_FLOAT,       //type
        GL_FALSE,       //normalized?
        0,              //stride
        (void*)0        //array buffer offset
    );

    glBindVertexArray(0);
}

unsigned int CodedMesh::GetVAO()
{
    return VAO;
}
// ==================================================================
#version 330 core

// ======================= attributes ========================
layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals;
layout(location=2)in vec2 texCoord;
layout(location=3)in vec3 tangents;
layout(location=4)in vec3 bitangents;

// ======================= uniforms ========================
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ======================= out ========================
out vec3 myNormal;
out vec3 FragPos;  // Export our Fragment Position computed in world space
out vec2 v_texCoord;


void main()
{

    gl_Position = projection * view * model * vec4(position, 1.0f);

    myNormal = normals;
    // Transform normal into world space
    FragPos = vec3(model* vec4(position,1.0f));

    // Store the texture coordinaets
    v_texCoord = texCoord;
}
// ==================================================================
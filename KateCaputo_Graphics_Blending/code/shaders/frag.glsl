// ====================================================
#version 330 core

// ======================= uniform ====================
// light info (not currently used)
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientIntensity;
uniform mat4 view;

// texture coords
uniform sampler2D u_DiffuseMap; 

// ======================= IN =========================
in vec3 myNormal; 
in vec2 v_texCoord; 
in vec3 FragPos; 

// ======================= out ========================
// The final output color of each 'fragment' from our fragment shader.
out vec4 FragColor;

// ======================= Globals ====================
float specularStrength = 0.5f;

void main()
{
    // Store our final texture color
    vec4 diffuseColor;
    diffuseColor = texture(u_DiffuseMap, v_texCoord);

    // ======================= IMPORTANT ====================
    // NONE OF THIS CODE FOR LIGHTING IS BEING USED, IT IS HERE
    // IN CASE OF EXPANSION IN FURTHER IMPLEMENTATIONS
    //
    // SEE: line 65 - final color is set to diffuse color, not affect by lighting calcs



    // (1) Compute ambient light
    vec3 ambient = ambientIntensity * lightColor;

    // (2) Compute diffuse light
    vec3 norm = normalize(myNormal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffImpact = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diffImpact * lightColor;

    // (3) Compute Specular lighting
    vec3 viewPos = vec3(0.0,0.0,0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // (4) Compute Lighting
    vec3 Lighting = diffuseLight + ambient + specular;
    //vec4 Lighting2 = (Lighting, 1.0f);

    // FINAL OUTPUT - if using lighting
    //FragColor = vec4(diffuseColor * vec4(Lighting, 1.0f));
    
    // FINAL OUTPUT - not using lighting
    FragColor = diffuseColor;
}
// ==================================================================
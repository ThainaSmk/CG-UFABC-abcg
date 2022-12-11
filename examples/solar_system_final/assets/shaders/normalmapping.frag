#version 300 es

in mediump vec2 fragTexCoord;

in mediump vec3  fragPObj;
in mediump vec3  fragTObj;
in mediump vec3  fragBObj;
in mediump vec3  fragNObj;

in mediump vec3 fragLEye;
in mediump vec3 fragVEye;

in mediump vec4 fragPosition;


uniform mediump mat3 normalMatrix;

// Light properties
uniform mediump vec4 Ia, Id, Is;
uniform mediump vec4 lightPosWorldSpace;
uniform mediump vec4 lightDirWorldSpace;
uniform mediump float lightCutOff;
uniform mediump float lightOuterCutOff;

// Material properties
uniform mediump vec4 Ka, Kd, Ks;
uniform mediump float shininess;

// Diffuse map sampler
uniform sampler2D diffuseTex;

// Normal map sampler
uniform sampler2D normalTex;

// Mapping mode
// 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
uniform int mappingMode;

out mediump vec4 outColor;

// Compute matrix to transform from camera space to tangent space
mediump mat3 ComputeTBN(mediump vec3 TObj, mediump vec3 BObj, mediump vec3 NObj) {
  mediump vec3 TEye = normalMatrix * normalize(TObj);
  mediump vec3 BEye = normalMatrix * normalize(BObj);
  mediump vec3 NEye = normalMatrix * normalize(NObj);
  return mat3(TEye.x, BEye.x, NEye.x, TEye.y, BEye.y, NEye.y, TEye.z, BEye.z,
              NEye.z);
}

// Blinn-Phong reflection model
mediump vec4 BlinnPhong(mediump vec3 N, mediump vec3 L, mediump vec3 V, mediump vec2 texCoord, mediump float intensity, mediump float attenuation) {
  N = normalize(N);
  L = normalize(L);

  // Compute lambertian term
  mediump float lambertian = max(dot(N, L), 0.0);

  // Compute specular term
  mediump float specular = 0.0;
  if (lambertian > 0.0) {
    V = normalize(V);
    mediump vec3 H = normalize(L + V);
    mediump float angle = max(dot(H, N), 0.0);
    specular = pow(angle, shininess);
  }

  mediump vec4 map_Kd = texture(diffuseTex, texCoord);
  mediump vec4 map_Ka = map_Kd;

  mediump vec4 diffuseColor = map_Kd * Kd * Id * lambertian * intensity * attenuation;
  mediump vec4 specularColor = Ks * Is * specular * intensity * attenuation;
  mediump vec4 ambientColor = map_Ka * Ka * Ia * attenuation;

  return ambientColor + diffuseColor + specularColor;
}

// Planar mapping
mediump vec2 PlanarMappingXUV(mediump vec3 P) { return vec2(1.0 - P.z, P.y); }
mediump mat3 PlanarMappingXTBN(mediump vec3 P) {
  mediump vec3 T = vec3(0, 0, -1);
  mediump vec3 N = fragNObj;
  mediump vec3 B = cross(N, T);
  return ComputeTBN(T, B, N);
}

mediump vec2 PlanarMappingYUV(mediump vec3 P) { return vec2(P.x, 1.0 - P.z); }
mediump mat3 PlanarMappingYTBN(mediump vec3 P) {
  mediump vec3 T = vec3(1, 0, 0);
  mediump vec3 N = fragNObj;
  mediump vec3 B = cross(N, T);
  return ComputeTBN(T, B, N);
}

mediump vec2 PlanarMappingZUV(mediump vec3 P) { return P.xy; }
mediump mat3 PlanarMappingZTBN(mediump vec3 P) {
  mediump vec3 T = vec3(1, 0, 0);
  mediump vec3 N = fragNObj;
  mediump vec3 B = cross(N, T);
  return ComputeTBN(T, B, N);
}

#define PI 3.14159265358979323846

// Cylindrical mapping
mediump vec2 CylindricalUV(mediump vec3 P) {
  mediump float longitude = atan(P.x, P.z);
  mediump float height = P.y;

  mediump float u = longitude / (2.0 * PI) + 0.5;  // From [-pi, pi] to [0, 1]
  mediump float v = height - 0.5;                  // Base at y = -0.5

  return vec2(u, v);
}

mediump mat3 CylindricalTBN(mediump vec3 P) {
  mediump vec3 T = vec3(P.z, 0, -P.x);
  mediump vec3 N = fragNObj;
  mediump vec3 B = cross(N, T);
  return ComputeTBN(T, B, N);
}

// Spherical mapping
mediump vec2 SphericalUV(mediump vec3 P) {
  mediump float longitude = atan(P.x, P.z);
  mediump float latitude = asin(P.y / length(P));

  mediump float u = longitude / (2.0 * PI) + 0.5;  // From [-pi, pi] to [0, 1]
  mediump float v = latitude / PI + 0.5;           // From [-pi/2, pi/2] to [0, 1]

  return vec2(u, v);
}

mediump mat3 SphericalTBN(mediump vec3 P) {
  mediump vec3 T = vec3(P.z, 0, -P.x);
  mediump vec3 N = fragNObj;
  mediump vec3 B = cross(N, T);
  return ComputeTBN(T, B, N);
}

void main() {
  mediump vec4 color;

  // Spotlight (soft edges)
  mediump float theta     = dot(normalize(lightDirWorldSpace), normalize(fragPosition - lightPosWorldSpace));
  mediump float epsilon   = lightCutOff - lightOuterCutOff;
  mediump float intensity = clamp((theta - lightOuterCutOff) / epsilon, 0.0, 1.0); 

  // Attenuation
  mediump float distance    = length(fragPosition - lightPosWorldSpace);
  mediump float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032 * (distance * distance));

  if (mappingMode == 0) {
    // Triplanar mapping

    // A offset to center the texture around the origin
    mediump vec3 offset = vec3(-0.5, -0.5, -0.5);

    // Sample with x planar mapping
    mediump vec2 texCoord1 = PlanarMappingXUV(fragPObj + offset);
    mediump mat3 TBN = PlanarMappingXTBN(fragPObj + offset);
    mediump vec3 LTan = TBN * normalize(fragLEye);
    mediump vec3 VTan = TBN * normalize(fragVEye);
    mediump vec3 NTan = texture(normalTex, texCoord1).xyz;
    NTan = normalize(NTan * 2.0 - 1.0);  // From [0, 1] to [-1, 1]
    mediump vec4 color1 = BlinnPhong(NTan, LTan, VTan, texCoord1, intensity, attenuation);

    // Sample with y planar mapping
    mediump vec2 texCoord2 = PlanarMappingYUV(fragPObj + offset);
    TBN = PlanarMappingYTBN(fragPObj + offset);
    LTan = TBN * normalize(fragLEye);
    VTan = TBN * normalize(fragVEye);
    NTan = texture(normalTex, texCoord2).xyz;
    NTan = normalize(NTan * 2.0 - 1.0);  // From [0, 1] to [-1, 1]
    mediump vec4 color2 = BlinnPhong(NTan, LTan, VTan, texCoord2, intensity, attenuation);

    // Sample with z planar mapping
    mediump vec2 texCoord3 = PlanarMappingZUV(fragPObj + offset);
    TBN = PlanarMappingZTBN(fragPObj + offset);
    LTan = TBN * normalize(fragLEye);
    VTan = TBN * normalize(fragVEye);
    NTan = texture(normalTex, texCoord3).xyz;
    NTan = normalize(NTan * 2.0 - 1.0);  // From [0, 1] to [-1, 1]
    mediump vec4 color3 = BlinnPhong(NTan, LTan, VTan, texCoord3, intensity, attenuation);

    // Compute average based on normal
    mediump vec3 weight = abs(normalize(fragNObj));
    color = color1 * weight.x + color2 * weight.y + color3 * weight.z;
  } else {
    mediump vec2 texCoord;
    mediump mat3 TBN;
    if (mappingMode == 1) {
      // Cylindrical mapping
      texCoord = CylindricalUV(fragPObj);
      TBN = CylindricalTBN(fragPObj);
    } else if (mappingMode == 2) {
      // Spherical mapping
      texCoord = SphericalUV(fragPObj);
      TBN = SphericalTBN(fragPObj);
    } else if (mappingMode == 3) {
      // From mesh
      texCoord = fragTexCoord;
      TBN = ComputeTBN(fragTObj, fragBObj, fragNObj);
    }

    // Compute tangent space vectors
    mediump vec3 LTan = TBN * normalize(fragLEye);
    mediump vec3 VTan = TBN * normalize(fragVEye);
    mediump vec3 NTan = texture(normalTex, texCoord).xyz;
    NTan = normalize(NTan * 2.0 - 1.0);  // From [0, 1] to [-1, 1]

    color = BlinnPhong(NTan, LTan, VTan, texCoord, intensity, 1.0);
  }

  outColor = color;
}
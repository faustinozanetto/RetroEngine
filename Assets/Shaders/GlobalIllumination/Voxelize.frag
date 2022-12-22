#version 460

layout(RGBA8) uniform image3D gVoxelizationTexture;
layout(binding = 1) uniform sampler2D gAlbedo;

uniform int u_VoxelSize;

in Vertex_GS {
  vec3 worldPositionFrag;
  vec3 normalFrag;
} gs;

vec3 scaleAndBias(vec3 p) { return 0.5f * p + vec3(0.5f); }

bool isInsideCube(const vec3 p, float e) { return abs(p.x) < 1 + e && abs(p.y) < 1 + e && abs(p.z) < 1 + e; }

void main() {
  vec3 color = vec3(0.25f);
  if(!isInsideCube(gs.worldPositionFrag, 0)) return;

  vec3 voxel = scaleAndBias(gs.worldPositionFrag);
  ivec3 dim = imageSize(gVoxelizationTexture);

  float alpha = pow(1, 4);

  vec4 res = alpha * vec4(vec3(color), 1);
  imageStore(gVoxelizationTexture, ivec3(dim * voxel), res);
}
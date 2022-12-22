#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 u_VoxelizationMatX;
uniform mat4 u_VoxelizationMatY;
uniform mat4 u_VoxelizationMatZ;

in Vertex {
  vec3 worldPosition;
  vec3 normalGeometry;
}vertices[];

out Vertex_GS {
  vec3 worldPositionFrag;
  vec3 normalFrag;
};

void main()
{
  const vec3 p1 = vertices[1].worldPosition - vertices[0].worldPosition;
	const vec3 p2 = vertices[2].worldPosition - vertices[0].worldPosition;

  const vec3 p = abs(cross(p1, p2)); 
	for(uint i = 0; i < 3; ++i){
		worldPositionFrag = vertices[i].worldPosition;
		normalFrag = vertices[i].normalGeometry;
		if(p.z > p.x && p.z > p.y){
			gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.y, 0, 1);
		} else if (p.x > p.y && p.x > p.z){
			gl_Position = vec4(worldPositionFrag.y, worldPositionFrag.z, 0, 1);
		} else {
			gl_Position = vec4(worldPositionFrag.x, worldPositionFrag.z, 0, 1);
		}
		EmitVertex();
	}
  EndPrimitive();
}
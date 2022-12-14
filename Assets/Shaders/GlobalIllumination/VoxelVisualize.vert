#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 0) out vec4 FragColor;

layout( rgba16f, binding = 0 ) uniform image3D u_VoxelizationSampler;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    vec3 u_Position;
} camera;

void main()
{
  int voxel_texture_size = imageSize( u_VoxelizationSampler ).x;
  int x                  = gl_InstanceID % voxel_texture_size;
  int y                  = ( gl_InstanceID / voxel_texture_size ) % voxel_texture_size;
  int z                  = gl_InstanceID / ( voxel_texture_size * voxel_texture_size );
  vec4 color;

  color = imageLoad( u_VoxelizationSampler, ivec3( x, y, z ) );

  if ( color.a < 0.0001 )
  {
    // move the voxel outside the clipping space
    gl_Position = vec4( vec3( -99999 ), 1.0 );
  }
  else
  {
    vec3 WorldCenter = vec3(5,5,5);
    vec3 WorldSize = vec3(15,15,15);
    float worldSize = max( WorldSize.x, max( WorldSize.y, WorldSize.z ) );
    float WorldSizeHalf = worldSize * 0.5;
    float voxel_size = worldSize / float( voxel_texture_size );
    vec3 coord_pos   = vec3( float( x ), float( y ), float( z ) ) /
                      float( voxel_texture_size );         // [0, 1]
    coord_pos           = 2.0 * coord_pos - vec3( 1.0 );  // [-1, 1]
    vec3 offset         = WorldSizeHalf * coord_pos + WorldCenter + 0.5 * vec3( voxel_size );
    vec3 world_position = voxel_size * aPos + offset;
    gl_Position         = camera.u_ViewProjectionMatrix * vec4( world_position, 1.0 );

    FragColor = color;
  }
}